#ifndef PID_H
#define PID_H

#include "Arduino.h"

class PID
{
  public:
    PID();
    double calc(double error, double ffTerm = 0);
    void setConstants(double kP_, double kI_, double kD_, double iForget_ = 0); // setConstants(double kP, double kI, double kD, optional double iForget = 0)
    void setLimits(double outMin_, double outMax_);
    void setRateLimit(double rateLim_);
    void reset(double resetValue);

  private:
    double kP = 0.1;
    double kI = 0;
    double kD = 0;
    double iForget = 0;
    double iTermPrev = 0;
    double errorPrev = 0;
    double outMin = -1;
    double outMax = 1;
    double outPrev = 0;
    double rateLim = 10000;
    unsigned long previousTime;
};

#endif