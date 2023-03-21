
#ifndef CalibrationsGlobal_H
#define CalibrationsGlobal_H

  // MOTOR DRIVERs
  // Spark = SPARK DC Brush Motor Driver
  //         (doesn't use dir for direction, uses midrange pwm as OFF, lower pwm as negative, and higher pwm as positive direction)
  // Maxmartt = Maxmartt Dual Motor Driver Module Board (aka. Amazon Driver)
  //            H-Bridge Driver Chip DC IRF3205
  //            MOSFET 3-36V 10A Peak 30A
  //            (uses dir for direction)
  // L298N = L298N (uses dir and dir_opp for direction)
  int motorDriverType = 3; // 1 = SPARK; 2 = Maxmartt or L298N; 3 = VNH5019

///////////////////////////////////////////////////////////////////////////////////////////////////
// << OTHER GLOBAL INITIALIZATIONS (Do Not Modify) >>
///////////////////////////////////////////////////////////////////////////////////////////////////

    // MOTOR SPEEDS (min & max) //
    const int motorMaxSpeed = 255; // maximum D2A output for Motor pwm

    // OTHER //
    int motorForwardMaxSpeed; // max FORWARD Speed (for this POWER level) (D2A output for Motor pwm)
    int motorReverseMaxSpeed; // max REVERSE Speed (for this POWER level) (D2A output for Motor pwm)
    float scaleSpin; // Downrate motor speeds used for Spin movement (for this POWER level) (% of motorForwardMaxSpeed)

    float scaleVel = 1; // default direction of velocity
    float vel = 0; // vehicle desired % velocity w/ magnitude and direction -1 to 1
    float spin = 0; // vehicle desired % spin w/ magnitude and direction -1 to 1

    int xRest = 512; // nominal "At rest" position for joystick (middle of 10bit A2D (0-1023) )
    int yRest = 512; // nominal "At rest" position for joystick (middle of 10bit A2D (0-1023) )
    int deltaX = 0;  // Joystick Horizontal Position (A2D units from "at rest")
    int deltaY = 0;  // Joystick Vertical Position   (A2D units from "at rest")
    float joyAngle = 0; // (0 to 360 degrees) angle of joystick position (0 = North, 90 = East, 180 = South, 270 = West)
    float joyRadius = 0; // (0 to 1) radius of joystick position (0 = center, 1 = edge)

  // Variables for lookup table interpolation
    float x1 = 0;
    float x2 = 0;
    float y1 = 0;
    float y2 = 0;

  // Motor Speed Values - Start at zero
    float  motorLVel = 0;
    float  motorRVel = 0;
    float  motorLVel_Filt = 0;
    float  motorRVel_Filt = 0;
    float  motorLVel_Final = 0;
    float  motorRVel_Final = 0;
    int  motorLSpeed = 0;
    int  motorRSpeed = 0;
    int  motorLSpeed_Filt = 0;
    int  motorRSpeed_Filt = 0;
    double speedMultiplier = 0;


  // Joystick Values - Start at 512 (middle position)
    int joyPosX = 512; // 0 to 1023 Range
    int joyPosY = 512; // 0 to 1023 Range
    int joyFaultBand = 50; // # of A2D units at each end of joystick range that we interpret as a fault (aka. at/near 0 and at/near full scale)
    int joyX; // Horizontal Joystick input; <Changes depending on which joysticks are connected.>
    int joyY; // Vertical Joystick input; <Changes depending on which joysticks are connected.>
    float joyOffsetAngle; // polar orientation of joystick (offset # of degrees to get forward to equal 0); <Changes depending on which joysticks are connected.>
    bool flipSpin; // use true or false to change SPIN direction (flip if chair forward/reverse is wrong); <Changes depending on which joysticks are connected.>
    int numSamples; // number of samples to check in a "window"
    int numWindows; // maximum attempts to check joystick
    bool joyPassed = false; // Flag to indicate the joysick passed setup test
    bool joyInit = false; // Flag to indicate the joysick has been started at least once
    int joySelect = 0; // <1> Tethered (default) or <2> Occupant <0> Error

  // POWER SETTINGS //
    //int switchLevel; // Default power level set by the measured switch value. Determines how fast the motors can move; 0 is slowest, 1 is medium (default), 2 is fastest
    //int powerLevel = 1; // Default power level set by the measured switch value or BT. Determines how fast the motors can move; 0 is slowest, 1 is medium (default), 2 is fastest

  // Mix Lookup Tables (converting joystick angle to Left & Right Mix) (3 power settings x 7 points)
    float mixTableL[15][2]; // Lookuptable to convert joystick angle to Left motor Mix
    float mixTableR[15][2]; // Lookuptable to convert joystick angle to Right motor Mix
    float radTable[6][2];  // Lookuptable to convert joystick radius to motor speed scale

    float scale;
    float Lmix;
    float Rmix;

#endif
