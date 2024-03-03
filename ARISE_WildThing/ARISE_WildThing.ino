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

  // INPUT FILTER AND DIAGNOSTICS
    joyInputs_ = joyProcessing(); // reads and filters joystick inputs,  runs diagnostics and returns x.pos, y.pos, radius(r) and angle(a) data, plus diagnostic informationa about joystick
    potScale = readPot(PowerLevelPotInput); // Read speed potentiometer and calc potScale
    usingTether = deb_joySwitch_Main.debounceInput( analogRead(JoySwitch_Main) > 800); // main joystick select switch
  
  // set joystick requested Angle and Radius according to selected input
    if(usingTether){ // using tether
      joyAngle = joyInputs_.T.a;
      joyRadius = joyInputs_.T.r;
    } else { // using occupant
      joyAngle = joyInputs_.O.a;
      joyRadius = joyInputs_.O.r;
    }

  // Convert Polar Coordinates to Desired Speed & Mix using 2D Joystick Table Lookups 
    int numRows = sizeof(radTable)/sizeof(radTable[0]);
    scale = interpolate(joyRadius, radTable, numRows); // calc scale from joyRadius
    if (usingTether) {scale = scale*TetherDownrate;} else { scale = scale*OccupantDownrate;} // modify scale per customer desired speeds on tether vs onboard joysticks
    numRows = sizeof(mixTable_L)/sizeof(mixTable_L[0]);
    mix_L = interpolate(joyAngle, mixTable_L, numRows); // calc mix_L from joyAngle
    numRows = sizeof(mixTable_R)/sizeof(mixTable_R[0]);
    mix_R = interpolate(joyAngle, mixTable_R, numRows); // calc mix_R from joyAngle
    
  // Calc desired motor velocity as product of Mix (joystick angle), Scale (joystick radius), potScale Pot)
    motorReqVel_L = min( 1, max( mix_L * scale * potScale , -1)); // limited between +/-1
    motorReqVel_R = min( 1, max( mix_R * scale * potScale , -1)); // limited between +/-1
  
  // Rate Limit Motor Velocity & Delay in case of zero crossing
    // <<< TODO: Change code so inputs/outputs are visible - but haven't taken time to figure out how to do it with embedded debounces for zero crossing >>>>
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
  
}
