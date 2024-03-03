// ARISE WildThing code for VNH5019 Motor Driver Shield for Arduino
/*
  ARISE - Syracuse, NY
  Main Contact: Connor McGough
  Coders: Bill Smith 2024/02/09
  this version contains Speed Control trials
*/

  #include "src/Debounce.h"
  #include "src/Filter.h"
  #include "PID.h"
  #include "src/DualVNH5019MotorShield.h";
  #include "pinouts.h"; // Use for VNH5019 MotorShield
  #include "Config.h";
  #include "Init.h";
  #include "joyProcessing.h";
  DualVNH5019MotorShield md; //setup vnh5019 method

  int debugPeriod = 100; // time between serialprint debug when running
  unsigned long lastDebugTime = millis(); // only send serialmonitor debug() every debugPeriod ms
  struct joyInput joyInputs_;
  
void setup() {
  Serial.begin(115200); // set communication between computer & Arduino
  md.init(); // initialize VNH5019
  pinMode(JoySwitch_Main,INPUT);
  createJoystickTables(); // Generate mix tables for Left & Right mix based on joystick angle and scale based on radius

  // PID setup
    motorPID_L.setLimits(PID_min, PID_max);
    motorPID_L.setConstants(Kp, Ki, Kd, iForget);
    motorPID_L.setRateLimit(PID_rateLim);
    motorPID_R.setLimits(PID_min, PID_max);
    motorPID_R.setConstants(Kp, Ki, Kd, iForget);
    motorPID_R.setRateLimit(PID_rateLim);

}

void loop() {
  // set timesteps and get time info
  prevTime = currentTime; // previous timestamp (ms)
  currentTime = millis(); // current timestamp (ms)
  timestep = max(0.1, currentTime - prevTime); // last timestep (ms)

  //////////////////////////////////
  // INPUT FILTER AND DIAGNOSTICS //

  // filter joystick inputs
  joyInputs_ = joyProcessing();
  potScale = readPot(PowerLevelPotInput); // Read speed potentiometer and calc potScale
  
  
  
  
  
  
  
  
  if(usingTether){ // using tether
    if (joyInputs_.T.x.health<500 || joyInputs_.T.y.health<500 ) {
      joyInit = false; joyPassed = false; StopMotors = true;} else {StopMotors = false;
      }
  } else {  // using onboard
    if (joyInputs_.O.x.health<500 || joyInputs_.O.y.health<500 ) {
      joyInit = false; joyPassed = false; StopMotors = true;} else {StopMotors = false;}
  }
  if (!StopMotors && !joyInit) {
    // <<<< TODO >>>>  CHANGE AutoCenterJoystick() to InitInputs()
    // Check Joystick and AutoCenter
      AutoCenterJoystick(); // Check Joystick is near center and not moving, return "at Rest" values if pass test
  }
  
  // <<<< TODO >>>> I don't know where selectJoystick() should go - need to do a logic flow to see what makes sense
  selectJoystick(); // Check if we use occupant or tether joystick & Interrupt to switch & Autocenter if it changes
  // << Read the Joystick X and Y positions >>
  if(usingTether){ // using tether
    joyPosX = joyInputs_.T.x.pos;
    joyPosY = joyInputs_.T.y.pos;
  } else {
    joyPosX = joyInputs_.O.x.pos;
    joyPosY = joyInputs_.O.y.pos;
  }

  if (!joyPassed) {
    // <<<< TODO >>>> get rid of joyPassed, just use jotInit
    //setJoystickLearnError();
      //  TBD
    // Rescale to set lowest motor on point to motorDropout <NOTE> Dropout is here instead of in scale lookup table because motor dropout cannot be modified by LMix, potScale, etc
      motorReqVel_L = 0;
      motorReqVel_R = 0;
    if (showDetail) {
      Serial.println("  <<<STOP!!!>>>  "); 
    }
  } else {
    // Calc joyAngle & joyRadius if good
      posX = joyPosX; // calc A2D cartesian position from "at Rest"
      posY = joyPosY; // calc A2D cartesian position from "at Rest"
      getPolarCoordinates(); // convert cartesian posX and posY of joystick into polar coordinates joyAngle & joyRadius

    // Convert Polar Coordinates to Desired Speed & Mix using 2D Joystick Table Lookups 
      int numRows = sizeof(radTable)/sizeof(radTable[0]);
      scale = interpolate(joyRadius, radTable, numRows); // calc scale from joyRadius
      if (usingTether) {scale = scale*TetherDownrate;} else { scale = scale*OccupantDownrate;} // modify scale per customer desired speeds on tether vs onboard joysticks
      numRows = sizeof(mixTable_L)/sizeof(mixTable_L[0]);
      mix_L = interpolate(joyAngle, mixTable_L, numRows); // calc mix_L from joyAngle
      numRows = sizeof(mixTable_R)/sizeof(mixTable_R[0]);
      mix_R = interpolate(joyAngle, mixTable_R, numRows); // calc mix_R from joyAngle
      
    // Calc desired motor velocity as product of Mix (joystick angle), Scale (joystick radius), potScale Pot) and trimFactor (left vs right motor strength)
      motorReqVel_L = min( 1, max( mix_L * scale * potScale * (1 + trimFactor), -1)); // limited between +/-1
      motorReqVel_R = min( 1, max( mix_R * scale * potScale * (1 - trimFactor), -1)); // limited between +/-1

      if(motorReqVel_L != 0){ 
        float minSpeed_ = (motorReqVel_L>0)? minSpeed:-minSpeed;
        float maxSpeed_ = (motorReqVel_L>0)? 1:-1;
        motorReqVel_L = rescale(motorReqVel_L,0,maxSpeed_,minSpeed_,maxSpeed_); // ensure some minimum speed
      }
      if(motorReqVel_R != 0){ 
        float minSpeed_ = (motorReqVel_R>0)? minSpeed:-minSpeed;
        float maxSpeed_ = (motorReqVel_R>0)? 1:-1;
        motorReqVel_R = rescale(motorReqVel_R,0,maxSpeed_,minSpeed_,maxSpeed_); // ensure some minimum speed
      }
  }
  
  // Rate Limit Motor Velocity & Delay in case of zero crossing <<< TODO: Change code so inputs/outputs are not invisible - but haven't taken time to figure out how to do it with embedded debounces >>>>
    // <<<<TODO>>>> motorReqVel_filt_L = rateLimitWithZeroCross(motorReqVel_L);
    // <<<<TODO>>>> motorReqVel_filt_R = rateLimitWithZeroCross(motorReqVel_R);
    rateLimitMotors(); // returns motorReqVel_filt_L and motorReqVel_filt_R based on inputs motorReqVel_L and motorReqVel_R | function rate limits and debounces each input
    
  // OUTPUT MOTOR SIGNALS
    setMotorOutputs_VNH5019(motorReqVel_filt_L, motorReqVel_filt_R);

  // Print info to Serial screen
  if ( currentTime - lastDebugTime >= debugPeriod) {
    debug();
    lastDebugTime = currentTime;
  }
  // wait x milliseconds before the next loop
    //  delay(50);
  
}
