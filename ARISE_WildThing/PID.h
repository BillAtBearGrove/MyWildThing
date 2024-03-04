#ifndef PID_H
#define PID_H

#include "Arduino.h"

class PID
{
  public:
    PID();
    double calc(double Error, double FFterm = 0);
    void setConstants(double Kp_, double Ki_, double Kd_, double iForget_ = 0); // setConstants(double Kp, double Ki, double Kd, optional double iForget = 0)
    void setLimits(double Out_min_, double Out_max_);
    void setRateLimit(double rateLim_);
    void reset(double resetValue);

  private:
    double Kp = 0.1;
    double Ki = 0;
    double Kd = 0;
    double iForget = 0;
    double Iterm_prev = 0;
    double Error_prev = 0;
    double Out_min = -1;
    double Out_max = 1;
    double Out_prev = 0;
    double rateLim = 10000;
    unsigned long previousTime;
};

#endif