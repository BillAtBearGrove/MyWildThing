
#ifndef pinouts_H
#define pinouts_H

///////////////////////////////////////////////////////////////////////////////////////////////////
// << ARDUINO PINOUTS (Only Modify When Wiring Diagram Changes) >>
///////////////////////////////////////////////////////////////////////////////////////////////////

// INPUTS //
  // Joystick Inputs
    const int joyX_Occupant = A7; // Horizontal Joystick input <Occupant>
    const int joyY_Occupant = A8; // Vertical Joystick input <Occupant>
    const int JoySwitch_Onboard = A9; // OFF disables Onboard Joystick
    const int joyX_Tether = A12; // Horizontal Joystick input <Tethered>
    const int joyY_Tether = A14; // Vertical Joystick input <Tethered>
    const int JoySwitch_Tether = A13;  // ON gives control temporarily back to Onboard (overrides JoySwitch_Main)
    const int JoySwitch_Main = A6;  // ON Selects Tether OFF Selects Onboard

  // Current Measurement
    const int vnh_M1CS = A0; // Left Motor Current Sense
    const int vnh_M2CS = A1; // Right Motor Current Sense

  // POWER LEVEL POTENTIOMETER
    const int PowerLevelPotInput = A15; // Input pin for potentiometer

// OUTPUTS //
  // Pinouts for VNH5019
    // Motor 1 (LEFT)
      const int vnh_M1PWM = 9; //LEFT Motor PWM (M1PWM)
      const int vnh_M1INA = 2; //LEFT Motor A Polarity (M1INA)
      const int vnh_M1INB = 4; //LEFT Motor B Polarity (M1INB)
      const int vnh_enableDiag1 = 6; // LEFT Motor EN/DIAG (M1EN/DIAG)
    // Motor 2 (RIGHT)
      const int vnh_M2PWM = 10; //RIGHT Motor PWM (M2PWM)
      const int vnh_M2INA = 7; //RIGHT Motor A Polarity (M2INA)
      const int vnh_M2INB = 8; //RIGHT Motor B Polarity (M2INB)
      const int vnh_enableDiag2 = 12;// RIGHT Motor EN/DIAG (M2EN/DIAG)

#endif
