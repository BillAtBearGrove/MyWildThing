#ifndef motorControl_H
#define motorControl_H

#include "Arduino.h"
enum enum_motorDir {Rev, For}; // Motor Direction (0)Reverse (1)Forward
enum enum_motorState_ {Off, Driving, Braking}; // Motor State (0)Off (1)Driving (2)Braking

class motorControl
{
  public:
    motorControl();
    double calc(double velReq, double mA);
    void setMotorLocation(int LoR); // Left or Right
    //void setRateLimit(double rateLim_);
    //void reset(double resetValue);

  private:
  enum_motorDir motorDir = For;
  enum_motorState MotorState = Off;
};

#endif