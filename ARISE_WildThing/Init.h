
#ifndef Init_H
#define Init_H

///////////////////////////////////////////////////////////////////////////////////////////////////
// << OTHER GLOBAL INITIALIZATIONS (Do Not Modify) >>
///////////////////////////////////////////////////////////////////////////////////////////////////

    // MOTOR SPEEDS (min & max) //
    const int motorMaxSpeed = 255; // maximum D2A output for Motor pwm
    bool StopMotors = false; // flag to stop motors immediately

    // OTHER //
    int motoForwardRMaxSpeed; // max FORWARD Speed (for this POWER level) (D2A output for Motor pwm)
    int motorReverseMaxSpeed; // max REVERSE Speed (for this POWER level) (D2A output for Motor pwm)
    float scaleSpin; // Downrate motor speeds used for Spin movement (for this POWER level) (% of motoForwardRMaxSpeed)

    float scaleVel = 1; // default direction of velocity
    float vel = 0; // vehicle desired % velocity w/ magnitude and direction -1 to 1
    float spin = 0; // vehicle desired % spin w/ magnitude and direction -1 to 1

    int xRest = 512; // nominal "At rest" position for joystick (middle of 10bit A2D (0-1023) )
    int yRest = 512; // nominal "At rest" position for joystick (middle of 10bit A2D (0-1023) )
    int posX = 0;  // Joystick Horizontal Position (A2D units from "at rest")
    int posY = 0;  // Joystick Vertical Position   (A2D units from "at rest")
    float joyAngle = 0; // (0 to 360 degrees) angle of joystick position (0 = North, 90 = East, 180 = South, 270 = West)
    float joyRadius = 0; // (0 to 1) radius of joystick position (0 = center, 1 = edge)

  // Variables for lookup table interpolation
    float x1 = 0;
    float x2 = 0;
    float y1 = 0;
    float y2 = 0;

  // Motor Speed Values - Start at zero
    float  motorReqVel_L = 0;
    float  motorReqVel_R = 0;
    float  motorReqVel_L_plusDropout = 0;
    float  motorReqVel_R_plusDropout = 0;
    float  motorReqVel_filt_L = 0;
    float  motorReqVel_filt_R = 0;
    double potScale = 0;

    float motorMaxL = 1;
    float motorMaxR = 1;

  // Joystick Values - Start at 512 (middle position)
    int joyPosX = 512; // 0 to 1023 Range
    int joyPosY = 512; // 0 to 1023 Range
    int joyX; // Horizontal Joystick input; <Changes depending on which joysticks are connected.>
    int joyY; // Vertical Joystick input; <Changes depending on which joysticks are connected.>
    float joyOffsetAngle; // polar orientation of joystick (offset # of degrees to get forward to equal 0); <Changes depending on which joysticks are connected.>
    bool flipSpin; // use true or false to change SPIN direction (flip if chair forward/reverse is wrong); <Changes depending on which joysticks are connected.>
    int numSamples; // number of samples to check in a "window"
    int numWindows; // maximum attempts to check joystick
    Debounce deb_joySwitch_Main(200); // define Debounce class to debounce joySwitch
  // Joystick FaultHandling
    int joyFaultBand = 50; // # of A2D units at each end of joystick range that we interpret as a fault (aka. at/near 0 and at/near full scale)
    bool joyPassed = false; // Flag to indicate the joysick passed setup test
    bool joyInit = false; // Flag to indicate the joysick has been started at least once
  // Polar Coordinates data structure
  struct polarCoord{ float Radius; float Angle; }; // joystick coordinates of Radius and Angle

  // Input Selection
  int inputMode = -1; // -1 undefined, 0 Occupant Joystick, 1 Tether Joystick
  int inputMode_prev = -1; // previous input mode
  int joyOcc = 0; // enumeration for Occupant Joystick
  int joyTeth = 1; // enumeration for Tether Joystick

  // zero crossing variables
    bool motorForward_L = true;
    bool motorForward_R = true;
    float Brake_L = maxBrake ; // LEFT motor Value to brake for vnh5019, braking is value 0 min and 400 max
    float Brake_R = maxBrake ; // RIGHT motor Value to brake for vnh5019, braking is value 0 min and 400 max
    Filter filtMotorReqVel_L(0); // filter for rate limiting motor speeds
    Filter filtMotorReqVel_R(0); // filter for rate limiting motor speeds
   
  // Mix Lookup Tables (converting joystick angle to Left & Right Mix) (3 power settings x 7 points)
    float mixTable_L[15][2]; // Lookuptable to convert joystick angle to Left motor Mix
    float mixTable_R[15][2]; // Lookuptable to convert joystick angle to Right motor Mix
    float radTable[6][2];  // Lookuptable to convert joystick radius to motor speed scale

    float scale;
    float mix_L;
    float mix_R;
    
  // Time variables
    unsigned long prevTime = millis(); // previous timestamp (ms)
    unsigned long RateLimPrevTime = millis(); // current timestamp (ms)
    unsigned long currentTime = millis(); // current timestamp (ms)
    float timestep = 1;
    unsigned long dT;

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
    float potInput=0;
    float PIDout_L = 0;
    float PIDout_R = 0;

#endif
