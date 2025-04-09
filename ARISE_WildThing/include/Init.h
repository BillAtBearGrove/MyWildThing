/*
****************************************************************
ARISE Adaptive Design
Project: WildThing ESP32 Port
Author: Bill Smith
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

#pragma once

#ifndef Init_H
#define Init_H

//#include "driver/adc.h"

// MOTOR SPEEDS (min & max) //
extern const int motorMaxSpeed; // maximum D2A output for Motor pwm
extern bool StopMotors; // flag to stop motors immediately

// OTHER //
extern int motorForwardMaxSpeed; // max FORWARD Speed (for this POWER level) (D2A output for Motor pwm)
extern int motorReverseMaxSpeed; // max REVERSE Speed (for this POWER level) (D2A output for Motor pwm)
extern float scaleSpin; // Downrate motor speeds used for Spin movement (for this POWER level) (% of motorForwardMaxSpeed)

extern float scaleVel; // default direction of velocity
extern float vel; // vehicle desired % velocity w/ magnitude and direction -1 to 1
extern float spin; // vehicle desired % spin w/ magnitude and direction -1 to 1

extern int xRest; // nominal "At rest" position for joystick (A2D)
extern int yRest; // nominal "At rest" position for joystick (A2D)
extern int deltaX;  // Joystick Horizontal Position (A2D units from "at rest")
extern int deltaY;  // Joystick Vertical Position   (A2D units from "at rest")
extern float joyAngle; // (0 to 360 degrees) angle of joystick position (0 = North, 90 = East, 180 = South, 270 = West)
extern float joyRadius; // (0 to 1) radius of joystick position (0 = center, 1 = edge)

// Variables for lookup table interpolation
extern float x1;
extern float x2;
extern float y1;
extern float y2;

// Motor Speed Values - Start at zero
extern float  motorReqVel_L;
extern float  motorReqVel_R;
extern float  motorReqVel_L_plusDropout;
extern float  motorReqVel_R_plusDropout;
extern float  motorReqVel_filt_L;
extern float  motorReqVel_filt_R;
extern double potScale;

extern float motorMaxL;
extern float motorMaxR;

// Joystick Values - Start at 512 (middle position)
extern int joySelect; // <1> Tethered (default) or <2> Occupant <0> Error
extern int joyPosX; // (A2D)
extern int joyPosY; // (A2D)
extern int joyX; // Horizontal Joystick input; <Changes depending on which joysticks are connected.>
extern int joyY; // Vertical Joystick input; <Changes depending on which joysticks are connected.>
extern float joyOffsetAngle; // polar orientation of joystick (offset # of degrees to get forward to equal 0); <Changes depending on which joysticks are connected.>
extern bool flipSpin; // use true or false to change SPIN direction (flip if chair forward/reverse is wrong); <Changes depending on which joysticks are connected.>
extern int numSamples; // number of samples to check in a "window"
extern int numWindows; // maximum attempts to check joystick
Debounce deb_joySwitch_Main(200); // define Debounce class to debounce joySwitch
// Joystick FaultHandling
extern int joyFaultBand; // # of A2D units at each end of joystick range that we interpret as a fault (aka. at/near 0 and at/near full scale)
extern bool joyPassed; // Flag to indicate the joysick passed setup test
extern bool joyInit; // Flag to indicate the joysick has been started at least once

  // Polar Coordinates data structure
  struct polarCoord{ float Radius; float Angle; }; // joystick coordinates of Radius and Angle

  // Input Selection
  int inputMode; // -1 undefined, 0 Occupant Joystick, 1 Tether Joystick
  int inputMode_prev; // previous input mode
  int joyOcc; // enumeration for Occupant Joystick
  int joyTeth; // enumeration for Tether Joystick

// zero crossing variables
extern bool motorLForward;
extern bool motorRForward;
extern float brake_L; // LEFT motor Value to brake for vnh5019, braking is value 0 min and 400 max
extern float brake_R; // RIGHT motor Value to brake for vnh5019, braking is value 0 min and 400 max
Filter filtMotorReqVel_L(0); // filter for rate limiting motor speeds
Filter filtMotorReqVel_R(0); // filter for rate limiting motor speeds

// Mix Lookup Tables (converting joystick angle to Left & Right Mix) (3 power settings x 7 points)
extern float mixTable_L[15][2]; // Lookuptable to convert joystick angle to Left motor Mix
extern float mixTable_R[15][2]; // Lookuptable to convert joystick angle to Right motor Mix
extern float radTable[6][2];  // Lookuptable to convert joystick radius to motor speed scale

extern float scale;
extern float mix_L;
extern float mix_R;

// Time variables
extern unsigned long prevTime; // previous timestamp (ms)
extern unsigned long RateLimPrevTime; // current timestamp (ms)
extern unsigned long currentTime; // current timestamp (ms)
extern float timestep;
extern unsigned long dT;
extern bool echoNow;

  // Current Filters
  Debounce zeroCrossDeb_L(zeroCrossingDwell); // define Debounce class to allocate memory for zero crossing motor direction (ref Debounce.cpp)
  Debounce zeroCrossDeb_R(zeroCrossingDwell); // define Debounce class to allocate memory for zero crossing motor direction (ref Debounce.cpp)
  Filter mA_filt_L(0); // filter for left motor current - init 0
  Filter mA_filt_R(0); // filter for right motor current - init 0
  //Filter motorPID_filt_L(0); // filter for left motor PID
  //Filter motorPID_filt_R(0); // filter for right motor PID
  //Filter speedReq_filt_L(0); // filter for right motor PID
  //Filter speedReq_filt_R(0); // filter for right motor PID
  Filter speedCalc_filt_L(0); // filter for right motor PID
  Filter speedCalc_filt_R(0); // filter for right motor PID

// PID setup classes
  PID motorPID_L;
  PID motorPID_R;
extern float potInput;
extern float PIDout_L;
extern float PIDout_R;

extern float LmA_last;
extern float LmA_last2;
extern float RmA_last;
extern float LmA_1d_last;
extern float LmA_d_last;

#endif
