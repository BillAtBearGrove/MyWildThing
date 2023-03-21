
#ifndef pinouts_vnh5019_H
#define pinouts_vnh5019_H

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

  // Battery level input
    //const int batteryLevelAnalog = A3;

  // POWER LEVEL POTENTIOMETER
    //#define PowerLevelSupply 8 // define digital pin 8 as extra +5V pin to power the potentiometer
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

  // LEDs
      const int statusLED = 5;  // GREEN Status LED near Big Red Button
    //const int ledPins[] = {9, 10, 11, 12};
    //const int LEDArrayLength = sizeof(ledPins)/sizeof(ledPins[0]);

  // We need to define these just to keep code similar, even though they are not used on this config
  // OUTPUTS //
  // Motor 1 (LEFT)
    int pwm1 = 5; //enA for L298N
    int dir1 = 4; //in1  for L298N
    int dir1_opp = 3; //in2 (opposite of dir1) for L298N
  // Motor 2 (RIGHT)
    int pwm2 = 7; //enB for L298N
    int dir2 = 6; //in3 for L298N
    int dir2_opp = 2; //in4 (opposite of dir2) for L298N
  // SPARK Controllers
    #define powerSpark1 52 // define digital pin 52 as extra +5V pin to power SPARK1
    #define powerSpark2 53 // define digital pin 53 as extra +5V pin to power SPARK2
  // Extra Power Pin
    #define PowerLevelSupply 8 // define digital pin 8 as extra +5V pin to power the potentiometer
    
#endif
