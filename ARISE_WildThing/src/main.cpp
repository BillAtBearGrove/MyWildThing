/*
****************************************************************
ARISE Adaptive Design
Project: WildThing ESP32 Port
Author: ejc
Version Scheme: major.minor.build-number.git-revision
(Git revision = 'git rev-parse --short HEAD')
--Custom PCA with ESP32-S3 Dev Module installed. Compile for ESP32S3 DEV MODULE
****************************************************************
*
Change Log:
-Changes Made. MM.DD.YYYY
--Initial codebase - 11.21.2024
****************************************************************
Notes:
-Note. MM.DD.YYYY
--Initial codebase - 11.21.2024
****************************************************************
*/

// ARISE WildThing code for VNH5019 Motor Driver Shield for Arduino
/*
  ARISE - Syracuse, NY
  Main Contact: Connor McGough
  Coders: Bill Smith 2024/02/09
  this version contains Speed Control trials
*/

/*
********************
Include Header Files
********************
*/

#include "../include/Config.h"
#include "../include/Init.h"
#include "../include/build_info_constants.h"
#include "../include/pinouts.h"
#include "../include/Wire.h"
#include "../include/MCP23017.h"
#include "../include/setupIO.h"
#include "../include/miscFunctions.h"
#include "../include/joystickFunctions.h"
#include "../include/joyHealth.h"
#include "../include/setMotorOutputs_VNH5019.h"
#include "../include/setMotorOutputs_VNH7070AY.h"
#include "../include/globals.h"

#include "../include/algorithm.h"

// For debug only
#include "../include/debug.h"

// define md, which is declared extern in globals.h
DualVNH5019MotorShield md = DualVNH5019MotorShield();  //setup vnh5019 method
//DualVNH7070AYESP32Driver md = DualVNH7070AYESP32Driver();
// defining lastDebugTime declared extern in globals.h
unsigned long lastDebugTime = millis(); // only send serialmonitor debug() every debugPeriod ms
// Joystick inputs struct
joyInputs joyInputs_;

void setup() {
	Serial.begin(115200); // set communication between computer & Arduino
  md.init(); // initialize VNH5019
  pinMode(JoySwitch_Main,INPUT);
  createJoystickTables(); // Generate mix tables for Left & Right mix based on joystick angle and scale based on radius

  // PID setup
    motorPID_L.setLimits(minPID, maxPID);
    motorPID_L.setConstants(kP, kI, kD, iForget);
    motorPID_L.setRateLimit(rateLimPID);
    motorPID_R.setLimits(minPID, maxPID);
    motorPID_R.setConstants(kP, kI, kD, iForget);
    motorPID_R.setRateLimit(rateLimPID);

}

void loop() {
  // set timesteps and get time info
  prevTime = currentTime; // previous timestamp (ms)
  currentTime = millis(); // current timestamp (ms)
  timestep = max(0.1, currentTime - prevTime); // last timestep (ms)
  //timestep = std::max(0.1F, (float)(currentTime - prevTime)); // last timestep (ms) // ESP32
  if ( currentTime - lastDebugTime >= debugPeriod) {
    Serial.println("");
    echoNow = true;
    lastDebugTime = currentTime;
  } else {
    echoNow = false;
  }

  // filter joystick inputs
  joyHealth(joyInputs_); // reads and filters joystick inputs,  runs diagnostics and returns x.pos, y.pos, radius(r) and angle(a) data, plus diagnostic informationa about joystick
  bool joySwitch_Main_isON = deb_joySwitch_Main.debounceBoth( analogRead(JoySwitch_Main) > a2dMid); // main joystick select switch
  potScale = readPot(powerLevelPotInput); // Read speed potentiometer and calc potScale

  // TODO: Make other cases to cover bluetooth or RC?
  // TODO future will need more logic here to determine if we want to change modes
    inputMode_prev = inputMode;
    if(joySwitch_Main_isON){
      inputMode = 1; // tether
    } else {
      inputMode = 0; // occupant
    }
    if(inputMode != inputMode_prev) { joyInputs_ = joyReset(joyInputs_); } //reset if changed mode

  // set requested Angle and Radius according to selected input
    if(inputMode == 1){ // using tether
      joyAngle = joyInputs_.T.a;
      joyRadius = joyInputs_.T.r*tetherDownrate;
    } else { // using occupant
      joyAngle = joyInputs_.O.a;
      joyRadius = joyInputs_.O.r*occupantDownrate;
    }

  // Always wait for input to come to rest when switching inputs
  
    // Convert Polar Coordinates to Desired Speed & Mix using 2D Joystick Table Lookups
    int numRows = sizeof(radTable)/sizeof(radTable[0]);
    scale = interpolate(joyRadius, radTable, numRows); // calc scale from joyRadius
    numRows = sizeof(mixTable_L)/sizeof(mixTable_L[0]);
    mix_L = interpolate(joyAngle, mixTable_L, numRows); // calc mix_L from joyAngle
    numRows = sizeof(mixTable_R)/sizeof(mixTable_R[0]);
    mix_R = interpolate(joyAngle, mixTable_R, numRows); // calc mix_R from joyAngle

  // Calc desired motor velocity as product of Mix (joystick angle), Scale (joystick radius), potScale Pot)
    motorReqVel_L = min( 1, max( mix_L * scale * potScale , -1)); // limited between +/-1
    motorReqVel_R = min( 1, max( mix_R * scale * potScale , -1)); // limited between +/-1
    //motorLVel = std::min( 1.0F, std::max( mix_L * scale * (float)potScale * (1.0F + trimFactor), -1.0F)); // limited between +/-1
    //motorRVel = std::min( 1.0F, std::max( mix_R * scale * (float)potScale * (1.0F - trimFactor), -1.0F)); // limited between +/-1

    if(motorLVel != 0){
      float minSpeed_ = (motorLVel>0)? minSpeed:-minSpeed;
      float maxSpeed_ = (motorLVel>0)? 1:-1;
      motorLVel = rescale(motorLVel,0,maxSpeed_,minSpeed_,maxSpeed_); // ensure some minimum speed
    }
    if(motorRVel != 0){
      float minSpeed_ = (motorRVel>0)? minSpeed:-minSpeed;
      float maxSpeed_ = (motorRVel>0)? 1:-1;
      motorRVel = rescale(motorRVel,0,maxSpeed_,minSpeed_,maxSpeed_); // ensure some minimum speed
    }

  // Rate Limit Motor Velocity & Delay in case of zero crossing
    // <<< TODO: Change code so inputs/outputs are visible - but haven't taken time to figure out how to do it with embedded debounces for zero crossing >>>>
    // <<<<TODO>>>> motorReqVel_filt_L = rateLimitWithZeroCross(motorReqVel_L);
    // <<<<TODO>>>> motorReqVel_filt_R = rateLimitWithZeroCross(motorReqVel_R);
  rateLimitMotors(); // returns motorReqVel_filt_L and motorReqVel_filt_R based on inputs motorReqVel_L and motorReqVel_R | function rate limits and debounces each input

  // OUTPUT MOTOR SIGNALS
  setMotorOutputs_VNH5019(motorReqVel_filt_L, motorReqVel_filt_R);
  //setMotorOutputs_VNH7070AY(motorLVel, motorRVel);

  // Print info to Serial screen
  if (echoNow) {
    debug();
  }
  
}
