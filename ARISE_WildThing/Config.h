// CONFIGURATION PARAMETERS //

#ifndef Config_H
#define Config_H

bool showDetail = true; // hide/show details in serialprint
int debugPeriod = 200; // ms time between serialprint debug when running
bool EchoPID = false;
bool EchoSetMotors = false;
bool EchoJoyTether = false;

// Occupant Joystick
//const float joyOffsetAngle_Occupant = 0; // polar orientation of joystick (offset # of degrees to get forward to equal 0) <OCCUPANT>
//const bool flipSpin_Occupant = false; // use true or false to change SPIN direction (flip if chair left/right is wrong) <OCCUPANT>
const float OccupantDownrate = 1; // downrate of occupant input for scaling speed (0 to 1)

// Tethered Joystick
//const float joyOffsetAngle_Tether = 0; // polar orientation of joystick (offset # of degrees to get forward to equal 0) <TETHER>
//const bool flipSpin_Tether = false; // use true or false to change SPIN direction (flip if chair left/right is wrong) <TETHER>
const float TetherDownrate = 1.0; // downrate of thether input for scaling speed (0 to 1)

  // Angles
  const float spinZone      = 25; // number of degrees above or below East or West that one motor turning and other is stopped
  const float TrimAngle = 10; // degrees travel on either side of forward/backward that is treated as true forward/backward
  const float spinDwell     = 3; // degrees travel on either side of left/right that is treated as true spin
  // Speeds
  const float minSpeed = 0.1; // overall scaling for all Vehicle speeds (except motorDropout which remains fixed)
  const float maxSpeed = 0.5; // overall scaling for all Vehicle speeds (except motorDropout which remains fixed)
    // Detailed Speeds (relative to maxSpeed)
    const float motorForward_PowerLevels = 1.0 ; // range: 0.01 to 1 max FORWARD Speed  (% output for Motor pwm)
    const float motorReverse_PowerLevels = 0.75 ; // range: 0.01 to 1 max REVERSE Speed (% output for Motor pwm)
    const float motorTurnFor_PowerLevels = 0.9 ; // range: 0.01 to 1 motor speeds used for FORWARD Turn movement (aka one wheel spins at this speed while other is stopped)  (% output for Motor pwm)
    const float motorTurnRev_PowerLevels = 0.70 ; // range: 0.01 to 1 motor speeds used for REVERSE Turn movement (aka one wheel spins at this speed while other is stopped)  (% output for Motor pwm)
    const float motorSpin_PowerLevels = 0.65 ; // range: 0.01 to 1 motor speeds used for Spin movement (aka each motor spins at this speed in opposite directions)  (% output for Motor pwm)
    const float TrimPlus = 0.1 ; // range: 0.00 to 0.2 Added Speed for Outside Wheel within TrimAngle (% output for Motor pwm)
    const float TrimMinus = 0.1 ; // range: 0.00 to 0.2 Minus Speed for Inside Wheel within TrimAngle (% output for Motor pwm)

// Radius Lookup Table (Scale)
  const float Deadband   = 0.05 ; // 0 to .1 (%) suggested around rest position before moving (NOTE this will be used in + and - directions, so actual deadband is 2x)
  const float motorDropout = 0.05; // typical range: 0.01 to 0.15 ;motor dropout ( % of motorMaxSpeed ) Set this to the minimum % it takes to drive the motor. "Too high" will cause troubles when turning because the inside motor "will not shut off smoothly"
  const float brakeZone = 0.05; // typical range: 0.01 to 0.15 ; % of desired speed = transition point to switch to/from braking & driving PWM when converting motor desired speed
  const float Endband    = .01 ; // 0 to .1 (%) suggested at end of range to ignore
  const float SlowRadius = 0.4 ; // 0 to 1 (%) of the Joystick range to use for low speed control (x-axis end of 1st ramp & begin 2nd ramp in lookup table)
  const float SlowPower  = 0.2 ; // 0 to 1 (%) of Power to use for low speed control (y-axis end of 1st ramp & begin 2nd ramp in lookup table)

// Acceleration & Deceleration Limits
  const float maxAccel = 0.0001 ; // est. range: 0.0001 to 1 max acceleration of motor % per ms
  const float maxDecel = 0.0002 ; // est. range: 0.0001 to 1 max deceleration of motor % per ms
  const float maxAccel_Brake = 0.0001; // 0.00007 ; // est. range: 0.0001 to 1 max acceleration of motor % per ms Below MotorDropout
  const float maxDecel_Brake = 0.0002; // est. range: 0.0001 to 1 max deceleration of motor % per ms Below MotorDropout
  const float zeroCrossingDwell = 500 ; // time (ms) to keep motor off between changes in direction
  const float maxBrake = 400 ; // max braking for vnh5019
  const float minBrake = 10 ; // min braking for vnh5019

// rescales the potentiometer output to get a speed multiplier
  float potValueRescale[4] = {30, 990, minSpeed + 0.1 * (maxSpeed - minSpeed), maxSpeed}; // cheap pot 
  //float potValueRescale[4] = {550, 950, minSpeed + 0.1 * (maxSpeed - minSpeed), maxSpeed}; // fancy pot

// JOYSTICK STARTUP TEST CALIBRATIONS
  int joyPosRestingMin = (512 - 32); // min assumed "resting" position (0-1023 A2D)
  int joyPosRestingMax = (512 + 32); // max assumed 'resting" position (0-1023 A2D)
  int joyPosRestingTol = 16; // max allowable noise at rest (total range max-min recorded during check) (0-1023 A2D)

  float mA_Rate = 2; // rate limit applied to current measure
  float mA_Pct = 0.03; // first order filter factor for current measure

  float Ke = 1.05; // 1/Kt
  float Ra = 0.0035; // Ohm/1000

  double Kp = 0.03; //10; //0.015;3
  double Ki = 0.001; //0.001; //0.002;
  double Kd = 1; //0.02;
  double iForget = 0.005; // 0-1 = % to forget the Integrator each step
  double PID_min = -1;
  double PID_max = 1;
  double PID_rateLim = 0.005; //0.001

#endif
