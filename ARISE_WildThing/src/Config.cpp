#include "../include/Config.h".h"

bool showDetail = true; // hide/show details in serialprint
int debugPeriod = 200; // ms time between serialprint debug when running
bool echoPID = false; // echo PID info
bool echoMotorL = false; // echo Left Motor info
bool echoMotorR = false; // echo Reft Motor info
bool echoJoyTether = false; // echo tether joystick info
bool echoJoyOnboard = false; // echo onboard joystick info
bool echoDT = false; // echo delta time
bool echoAR = false; // echo joystick Angle and Radius

// Analog Bit Resolution (ref ESP32 has 12bit A2D, Arduino MEGA has 10bit A2D)
  const int a2dMid = 512; // middle value of A2D
  const int a2dMax = 1023; // max value of A2D

// Occupant Joystick
  const float occupantDownrate = 1; // downrate of occupant input for scaling speed (0 to 1)

// Tethered Joystick
  const float tetherDownrate = 1.0; // downrate of thether input for scaling speed (0 to 1)

// Angles
const float spinZone  = 25; // number of degrees above or below East or West that one motor turning and other is stopped
const float trimAngle = 10; // degrees travel on either side of forward/backward that is treated as true forward/backward
const float spinDwell = 3; // degrees travel on either side of left/right that is treated as true spin
// Speeds
const float minSpeed = 0.08; // overall scaling for all Vehicle speeds (except motorDropout which remains fixed)
const float maxSpeed = 0.45; // overall scaling for all Vehicle speeds (except motorDropout which remains fixed)
// Detailed Speeds (relative to maxSpeed)
const float motorForward_PowerLevels = 1.0 ; // range: 0.01 to 1 max FORWARD Speed  (% output for Motor pwm)
const float motorReverse_PowerLevels = 0.75 ; // range: 0.01 to 1 max REVERSE Speed (% output for Motor pwm)
const float motorTurnFor_PowerLevels = 0.9 ; // range: 0.01 to 1 motor speeds used for FORWARD Turn movement (aka one wheel spins at this speed while other is stopped)  (% output for Motor pwm)
const float motorTurnRev_PowerLevels = 0.70 ; // range: 0.01 to 1 motor speeds used for REVERSE Turn movement (aka one wheel spins at this speed while other is stopped)  (% output for Motor pwm)
const float motorSpin_PowerLevels = 0.65 ; // range: 0.01 to 1 motor speeds used for Spin movement (aka each motor spins at this speed in opposite directions)  (% output for Motor pwm)
const float trimPlus = 0.1 ; // range: 0.00 to 0.2 Added Speed for Outside Wheel within trimAngle (% output for Motor pwm)
const float trimMinus = 0.1 ; // range: 0.00 to 0.2 Minus Speed for Inside Wheel within trimAngle (% output for Motor pwm)

// Radius Lookup Table (Scale)
const float deadBand   = 0.05 ; // 0 to .1 (%) suggested around rest position before moving (NOTE this will be used in + and - directions, so actual deadband is 2x)
const float motorDropout = 0.05; // typical range: 0.01 to 0.15 ;motor dropout ( % of motor speed ) Set this to the minimum % it takes to drive the motor. "Too high" will cause troubles when turning because the inside motor "will not shut off smoothly"
const float brakeZone = 0.02; // typical range: 0.01 to 0.15 ; % of desired speed = transition point to switch to/from braking & driving PWM when converting motor desired speed
const float endBand    = .01 ; // 0 to .1 (%) suggested at end of range to ignore
const float slowRadius = 0.4 ; // 0 to 1 (%) of the Joystick range to use for low speed control (x-axis end of 1st ramp & begin 2nd ramp in lookup table)
const float slowPower  = 0.2 ; // 0 to 1 (%) of Power to use for low speed control (y-axis end of 1st ramp & begin 2nd ramp in lookup table)

// Acceleration & Deceleration Limits
  const float maxAccel = 0.0001 ; // est. range: 0.0001 to 1 max acceleration of motor % per ms
  const float maxDecel = 0.0002 ; // est. range: 0.0001 to 1 max deceleration of motor % per ms
  const float maxAccel_Brake = 0.5;//0.0001; // 0.00007 ; // est. range: 0.0001 to 1 max acceleration of motor % per ms Below MotorDropout
  const float maxDecel_Brake = 0.5; // est. range: 0.0001 to 1 max deceleration of motor % per ms Below MotorDropout
  const float zeroCrossingDwell = 500 ; // time (ms) to keep motor off between changes in direction
  const float maxBrake = 400 ; // max braking for vnh5019
  const float minBrake = 10 ; // min braking for vnh5019

// rescales the potentiometer output to get a speed multiplier
  float potValueRescale[4] = {(a2dMax*0.03), (a2dMax*0.97), minSpeed + 0.1 * (maxSpeed - minSpeed), maxSpeed}; // cheap pot 

// JOYSTICK STARTUP TEST CALIBRATIONS
int joyPosRestingMin = (a2dMid * 0.94); // min assumed "resting" position (A2D)
int joyPosRestingMax = (a2dMid * 1.06); // max assumed 'resting" position (A2D)
int joyPosRestingTol = (a2dMax * 0.016); // max allowable noise to consider joystick still at rest (total range max-min recorded during check) (A2D)

float mA_Rate = 2; // rate limit applied to current measure
  float mA_Pct = 0.03; // first order filter factor for current measure

  float motorKe = 1.05; // 1/Kt
  float motorRa = 0.0035; // Ohm/1000

  double kP = 0.06; //10; //0.015;3
  double kI = 0; //0.001; //0.002;
  double kD = 0.5; //1; //0.02;
  double iForget = 0.010; // 0-1 = % to forget the Integrator each step
  double minPID = -1;
  double maxPID = 1;
  double rateLimPID = 0.01; //0.001

#endif
