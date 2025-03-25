// PID.cpp
// PID error Controller with FeedForward library
#include "PID.h"

PID::PID() {
    iTermPrev = 0;
    errorPrev = 0;
    outPrev = 0;
    previousTime = millis();
}

double PID::calc(double error, double ffTerm ) {
    unsigned long thisTime = millis();
    double dT = max(0.0001,(thisTime - previousTime));
    previousTime = thisTime;

    double pTerm = kP * error;
    double dTerm = kD * ( error - errorPrev ) / dT;
    double thisMax = max(outMin, min(outMax, outPrev + rateLim*dT ));
    double thisMin = min(outMax, max(outMin, outPrev - rateLim*dT ));
    double fpdTerm = min( thisMax, max( thisMin, ffTerm + pTerm + dTerm )); // pTerm + dTerm + FeedForwardterm (limited to overall Min/Max and rate limited from previous OUT value)
    
    double Iterm = min( thisMax - fpdTerm, max( thisMin - fpdTerm, (kI * error * dT) + ( (1-(iForget*dT) ) * iTermPrev ) ));
    double OUT =  fpdTerm + Iterm;

    errorPrev = error;
    iTermPrev = Iterm;
    outPrev = OUT;

      if (echoPID && echoNow) {
        if ( (echoMotorL && ffTerm==motorReqVel_filt_L) || (echoMotorR && ffTerm==motorReqVel_filt_R) ) { // hack to only echo the pid for the motor that is called for
          Serial.print("Err= "); Serial.print(error); Serial.print("  ");
          Serial.print("FF= "); Serial.print(ffTerm); Serial.print("  ");
          Serial.print("P= "); Serial.print(pTerm); Serial.print("  ");
          Serial.print("D= "); Serial.print(dTerm); Serial.print("  ");
          //Serial.print("outMin= "); Serial.print(outMin); Serial.print("  ");
          //Serial.print("outMax= "); Serial.print(outMax); Serial.print("  ");
          //Serial.print("rateLim= "); Serial.print(rateLim); Serial.print("  ");
          //Serial.print("dT= "); Serial.print(dT); Serial.print("  ");
          //Serial.print("Pre+rLdT= "); Serial.print(outPrev + rateLim*dT); Serial.print("  ");
          //Serial.print("thisMin= "); Serial.print(thisMin); Serial.print("  ");
          //Serial.print("thisMax= "); Serial.print(thisMax); Serial.print("  ");
          Serial.print("FPD= "); Serial.print(fpdTerm); Serial.print("  ");
          Serial.print("I= "); Serial.print(Iterm); Serial.print("  ");
          Serial.print("OUT= "); Serial.print(OUT*100); Serial.print("  ");\
        }
      }

    return OUT;
}

void PID::setConstants(double kP_, double kI_, double kD_, double iForget_) {
    kP = kP_;
    kI = kI_;
    kD = kD_;
    iForget = iForget_;
    return;
}

void PID::setLimits(double outMin_, double outMax_) {
    outMin = outMin_;
    outMax = outMax_;
    return void();
}

void PID::setRateLimit(double rateLim_) {
    rateLim = rateLim_;
    return void();
}

void PID::reset(double resetValue) {
    iTermPrev = 0;
    errorPrev = resetValue;
    outPrev = 0;
    previousTime = millis();
    return;
}
