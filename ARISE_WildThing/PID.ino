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

      if (EchoPID && motorReqVel_filt_L == FFterm) {
        if (EchoNow) {
          Serial.print("Err= "); Serial.print(Error); Serial.print("  ");
          Serial.print("FF= "); Serial.print(FFterm); Serial.print("  ");
          Serial.print("P= "); Serial.print(Pterm); Serial.print("  ");
          Serial.print("D= "); Serial.print(Dterm); Serial.print("  ");
          //Serial.print("Out_min= "); Serial.print(Out_min); Serial.print("  ");
          //Serial.print("Out_max= "); Serial.print(Out_max); Serial.print("  ");
          //Serial.print("rateLim= "); Serial.print(rateLim); Serial.print("  ");
          //Serial.print("dT= "); Serial.print(dT); Serial.print("  ");
          //Serial.print("Pre+rLdT= "); Serial.print(Out_prev + rateLim*dT); Serial.print("  ");
          //Serial.print("thisMin= "); Serial.print(thisMin); Serial.print("  ");
          //Serial.print("thisMax= "); Serial.print(thisMax); Serial.print("  ");
          Serial.print("FPD= "); Serial.print(FPDterm); Serial.print("  ");
          Serial.print("I= "); Serial.print(Iterm); Serial.print("  ");
          Serial.print("OUT= "); Serial.print(OUT*100); Serial.print("  ");\
        }
      }

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
