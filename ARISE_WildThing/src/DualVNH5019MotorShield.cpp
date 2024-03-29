#include "DualVNH5019MotorShield.h"

// Constructors ////////////////////////////////////////////////////////////////

DualVNH5019MotorShield::DualVNH5019MotorShield()
{
  //Pin map
  _INA1 = 2;
  _INB1 = 4;
  _PWM1 = 9;
  _EN1DIAG1 = 6;
  _CS1 = A0;
  _INA2 = 7;
  _INB2 = 8;
  _PWM2 = 10;
  _EN2DIAG2 = 12;
  _CS2 = A1;
}

DualVNH5019MotorShield::DualVNH5019MotorShield(unsigned char INA1,
                                               unsigned char INB1,
                                               unsigned char PWM1,
                                               unsigned char EN1DIAG1,
                                               unsigned char CS1,
                                               unsigned char INA2,
                                               unsigned char INB2,
                                               unsigned char PWM2,
                                               unsigned char EN2DIAG2,
                                               unsigned char CS2)
{
  _INA1 = INA1;
  _INB1 = INB1;
  _PWM1 = PWM1;
  _EN1DIAG1 = EN1DIAG1;
  _CS1 = CS1;
  _INA2 = INA2;
  _INB2 = INB2;
  _PWM2 = PWM2;
  _EN2DIAG2 = EN2DIAG2;
  _CS2 = CS2;
}

// Public Methods //////////////////////////////////////////////////////////////
void DualVNH5019MotorShield::init()
{
// Define pinMode for the pins and set the frequency for timer1.

  pinMode(_INA1,OUTPUT);
  pinMode(_INB1,OUTPUT);
  pinMode(_PWM1,OUTPUT);
  pinMode(_EN1DIAG1,INPUT);
  pinMode(_CS1,INPUT);
  pinMode(_INA2,OUTPUT);
  pinMode(_INB2,OUTPUT);
  pinMode(_PWM2,OUTPUT);
  pinMode(_EN2DIAG2,INPUT);
  pinMode(_CS2,INPUT);

  #ifdef DUALVNH5019MOTORSHIELD_TIMER1_AVAILABLE
    if (_PWM1 == _PWM1_TIMER1_PIN && _PWM2 == _PWM2_TIMER1_PIN)
    {
      // Timer 1 configuration
      // prescaler: clockI/O / 1
      // outputs enabled
      // phase-correct PWM
      // top of 400
      //
      // PWM frequency calculation
      // 16MHz / 1 (prescaler) / 2 (phase-correct) / 400 (top) = 20kHz
      TCCR1A = 0b10100011; //Aaron: 0b10100011 Stock: 0b10100000
      TCCR1B = 0b00001010; //Aaron: 0b00001010 Stock: 0b00010001
      ICR1 = 400;
    }
  #endif
    // mod to change pwm frequency taken from https://forum.arduino.cc/t/mega-2560-pwm-frequency/71434/2
    int myEraser = 7; // this is 111 in binary and is used as an eraser
    TCCR2B &= ~myEraser; // this operation (AND plus NOT), set the three bits in TCCR2B to 0
    int myPrescaler = 1; // this could be a number in [1 , 6]. In this case, 3 corresponds in binary to 011.
    TCCR2B |= myPrescaler; //this operation (OR), replaces the last three bits in TCCR2B with our new value 011
    /*
    I registered those values on all PWM pins, changing the value of prescaler (the only exception are pins 13 and 14, see later):
      prescaler = 1 ---> PWM frequency is 31000 Hz
      prescaler = 2 ---> PWM frequency is 4000 Hz
      prescaler = 3 ---> PWM frequency is 490 Hz (default value)
      prescaler = 4 ---> PWM frequency is 120 Hz
      prescaler = 5 ---> PWM frequency is 30 Hz
      prescaler = 6 ---> PWM frequency is <20 Hz
    Those prescaler values are good for all timers (TCCR1B, TCCR2B, TCCR3B, TCCR4B) except for timer 0 (TCCR0B). In this case the values are:
      prescaler = 1 ---> PWM frequency is 62000 Hz
      prescaler = 2 ---> PWM frequency is 7800 Hz
      prescaler = 3 ---> PWM frequency is 980 Hz (default value)
      prescaler = 4 ---> PWM frequency is 250 Hz
      prescaler = 5 ---> PWM frequency is 60 Hz
      prescaler = 6 ---> PWM frequency is <20 Hz
    */
  // END mod

}
// Set speed for motor 1, speed is a number betwenn -400 and 400
void DualVNH5019MotorShield::setM1Speed(int speed)
{
  unsigned char reverse = 0;

  if (speed < 0)
  {
    speed = -speed;  // Make speed a positive quantity
    reverse = 1;  // Preserve the direction
  }
  if (speed > 400)  // Max PWM dutycycle
    speed = 400;

  #ifdef DUALVNH5019MOTORSHIELD_TIMER1_AVAILABLE
    if (_PWM1 == _PWM1_TIMER1_PIN && _PWM2 == _PWM2_TIMER1_PIN)
    {
      OCR1A = speed;
    }
    else
    {
      analogWrite(_PWM1,speed * 51 / 80); // map 400 to 255
    }
  #else
    analogWrite(_PWM1,speed * 51 / 80); // map 400 to 255
  #endif

  if (speed == 0)
  {
    digitalWrite(_INA1,LOW);   // Make the motor coast no
    digitalWrite(_INB1,LOW);   // matter which direction it is spinning.
  }
  else if (reverse)
  {
    digitalWrite(_INA1,LOW);
    digitalWrite(_INB1,HIGH);
  }
  else
  {
    digitalWrite(_INA1,HIGH);
    digitalWrite(_INB1,LOW);
  }

}

// Set speed for motor 2, speed is a number betwenn -400 and 400
void DualVNH5019MotorShield::setM2Speed(int speed)
{
  unsigned char reverse = 0;

  if (speed < 0)
  {
    speed = -speed;  // make speed a positive quantity
    reverse = 1;  // preserve the direction
  }
  if (speed > 400)  // Max
    speed = 400;

  #ifdef DUALVNH5019MOTORSHIELD_TIMER1_AVAILABLE
    if (_PWM1 == _PWM1_TIMER1_PIN && _PWM2 == _PWM2_TIMER1_PIN)
    {
      OCR1B = speed;
    }
    else
    {
      analogWrite(_PWM2,speed * 51 / 80); // map 400 to 255
    }
  #else
    analogWrite(_PWM2,speed * 51 / 80); // map 400 to 255
  #endif

  if (speed == 0)
  {
    digitalWrite(_INA2,LOW);   // Make the motor coast no
    digitalWrite(_INB2,LOW);   // matter which direction it is spinning.
  }
  else if (reverse)
  {
    digitalWrite(_INA2,LOW);
    digitalWrite(_INB2,HIGH);
  }
  else
  {
    digitalWrite(_INA2,HIGH);
    digitalWrite(_INB2,LOW);
  }
}

// Set speed for motor 1 and 2
void DualVNH5019MotorShield::setSpeeds(int m1Speed, int m2Speed)
{
  setM1Speed(m1Speed);
  setM2Speed(m2Speed);
}

// Brake motor 1, brake is a number between 0 and 400
void DualVNH5019MotorShield::setM1Brake(int brake)
{
  // normalize brake
  if (brake < 0)
  {
    brake = -brake;
  }
  if (brake > 400)  // Max brake
    brake = 400;
  digitalWrite(_INA1, LOW);
  digitalWrite(_INB1, LOW);

  #ifdef DUALVNH5019MOTORSHIELD_TIMER1_AVAILABLE
    if (_PWM1 == _PWM1_TIMER1_PIN && _PWM2 == _PWM2_TIMER1_PIN)
    {
      OCR1A = brake;
    }
    else
    {
      analogWrite(_PWM1,brake * 51 / 80); // map 400 to 255
    }
  #else
    analogWrite(_PWM1,brake * 51 / 80); // map 400 to 255
  #endif
}

// Brake motor 2, brake is a number between 0 and 400
void DualVNH5019MotorShield::setM2Brake(int brake)
{
  // normalize brake
  if (brake < 0)
  {
    brake = -brake;
  }
  if (brake > 400)  // Max brake
    brake = 400;
  digitalWrite(_INA2, LOW);
  digitalWrite(_INB2, LOW);

  #ifdef DUALVNH5019MOTORSHIELD_TIMER1_AVAILABLE
    if (_PWM1 == _PWM1_TIMER1_PIN && _PWM2 == _PWM2_TIMER1_PIN)
    {
      OCR1B = brake;
    }
    else
    {
      analogWrite(_PWM2,brake * 51 / 80); // map 400 to 255
    }
  #else
    analogWrite(_PWM2,brake * 51 / 80); // map 400 to 255
  #endif
}

// Brake motor 1 and 2, brake is a number between 0 and 400
void DualVNH5019MotorShield::setBrakes(int m1Brake, int m2Brake)
{
  setM1Brake(m1Brake);
  setM2Brake(m2Brake);
}

// Return motor 1 current value in milliamps.
unsigned int DualVNH5019MotorShield::getM1CurrentMilliamps()
{
  // 5V / 1024 ADC counts / 144 mV per A = 34 mA per count
  return analogRead(_CS1) * 34;
}

// Return motor 2 current value in milliamps.
unsigned int DualVNH5019MotorShield::getM2CurrentMilliamps()
{
  // 5V / 1024 ADC counts / 144 mV per A = 34 mA per count
  return analogRead(_CS2) * 34;
}

// Return error status for motor 1
unsigned char DualVNH5019MotorShield::getM1Fault()
{
  return !digitalRead(_EN1DIAG1);
}

// Return error status for motor 2
unsigned char DualVNH5019MotorShield::getM2Fault()
{
  return !digitalRead(_EN2DIAG2);
}
