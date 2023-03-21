
#ifndef pinouts_H
#define pinouts_H

///////////////////////////////////////////////////////////////////////////////////////////////////
// << ARDUINO PINOUTS (Only Modify When Wiring Diagram Changes) >>
///////////////////////////////////////////////////////////////////////////////////////////////////

// INPUTS //
  // Joystick Inputs
    const int joyX_Occupant = A0; // Horizontal Joystick input <Occupant>
    const int joyY_Occupant = A1; // Vertical Joystick input <Occupant>
    const int joyX_Tether = A3; // Horizontal Joystick input <Tethered>
    const int joyY_Tether = A4; // Vertical Joystick input <Tethered>
    
  // Battery level input
    //const int batteryLevelAnalog = A3;

  // POWER LEVEL POTENTIOMETER
    #define PowerLevelSupply 8 // define digital pin 8 as extra +5V pin to power the potentiometer
    int PowerLevelPotInput = A6; // Input pin for potentiometer 
    
  // REMOTE (HC-05)
    // TBD
    int HC05_GND = 2; // Set Ground Pin for HC-05

  // SWITCH FOR TETHER/OCCUPANT
    int JoySwitch_Onboard = A2;
    int JoySwitch_Main = A5;
    int JoySwitch_Tether = A9;  // SPECIAL PINOUT TO MIX OCCUPANT WITH TETHER IF TETHER SWITCH IS PRESSED  //
    const int statusLED = 5;  // GREEN Status LED near Big Red Button  

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

  // LEDs
    int ledPins[] = {9, 10, 11, 12};
    int LEDArrayLength = sizeof(ledPins)/sizeof(ledPins[0]);

#endif
