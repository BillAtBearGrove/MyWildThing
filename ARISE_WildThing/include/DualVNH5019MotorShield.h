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

#include <Arduino.h>
//#include "hal/gpio_types.h"

class DualVNH5019MotorShield
{
  public:
    // CONSTRUCTORS
    // Default pin selection.
    DualVNH5019MotorShield();
    // User-defined pin selection.
    DualVNH5019MotorShield(unsigned char INA1,
                           unsigned char INB1,
                           unsigned char PWM1,
                           unsigned char EN1DIAG1,
                           unsigned char CS1,
                           unsigned char INA2,
                           unsigned char INB2,
                           unsigned char PWM2,
                           unsigned char EN2DIAG2,
                           unsigned char CS2);

    // PUBLIC METHODS
    void init(); // Initialize TIMER 1, set the PWM to 20kHZ.
    void setM1Speed(int speed); // Set speed for M1.
    void setM2Speed(int speed); // Set speed for M2.
    void setSpeeds(int m1Speed, int m2Speed); // Set speed for both M1 and M2.
    void setM1Brake(int brake); // Brake M1.
    void setM2Brake(int brake); // Brake M2.
    void setBrakes(int m1Brake, int m2Brake); // Brake both M1 and M2.
    unsigned int getM1CurrentMilliamps(); // Get current reading for M1.
    unsigned int getM2CurrentMilliamps(); // Get current reading for M2.
    unsigned char getM1Fault(); // Get fault reading from M1.
    unsigned char getM2Fault(); // Get fault reading from M2.

  private:
    unsigned char _INA1;
    unsigned char _INB1;
    unsigned char _PWM1;
    static const unsigned char _PWM1_TIMER1_PIN = 9;
    unsigned char _EN1DIAG1;
    unsigned char _CS1;
    unsigned char _INA2;
    unsigned char _INB2;
    unsigned char _PWM2;
    static const unsigned char _PWM2_TIMER1_PIN = 10;
    unsigned char _EN2DIAG2;
    unsigned char _CS2;

};
