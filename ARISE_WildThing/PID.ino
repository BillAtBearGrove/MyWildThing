// PID.cpp
// PID Error Controller with FeedForward library
#include "PID.h"

PID::PID() {
    Iterm_prev = 0;
    Error_prev = 0;
    Out_prev = 0;
    previousTime = millis();
}

double PID::calc(double Error, double FFterm ) {
    unsigned long thisTime = millis();
    double dT = max(0.0001,(thisTime - previousTime));
    previousTime = thisTime;

    double Pterm = Kp * Error;
    double Dterm = Kd * ( Error - Error_prev ) / dT;
    double thisMax = max(Out_min, min(Out_max, Out_prev + rateLim*dT ));
    double thisMin = min(Out_max, max(Out_min, Out_prev - rateLim*dT ));
    double FPDterm = min( thisMax, max( thisMin, FFterm + Pterm + Dterm )); // Pterm + Dterm + FeedForwardterm (limited to overall Min/Max and rate limited from previous OUT value)
    
    double Iterm = min( thisMax - FPDterm, max( thisMin - FPDterm, (Ki * Error * dT) + ( (1-(iForget*dT) ) * Iterm_prev ) ));
    double OUT =  FPDterm + Iterm;

    Error_prev = Error;
    Iterm_prev = Iterm;
    Out_prev = OUT;
    return OUT;
}

void PID::setConstants(double Kp_, double Ki_, double Kd_, double iForget_) {
    Kp = Kp_;
    Ki = Ki_;
    Kd = Kd_;
    iForget = iForget_;
    return;
}

void PID::setLimits(double Out_min_, double Out_max_) {
    Out_min = Out_min_;
    Out_max = Out_max_;
    return void();
}

void PID::setRateLimit(double rateLim_) {
    rateLim = rateLim_;
    return void();
}

void PID::reset(double resetValue) {
    Iterm_prev = 0;
    Error_prev = resetValue;
    Out_prev = 0;
    previousTime = millis();
    return;
}
