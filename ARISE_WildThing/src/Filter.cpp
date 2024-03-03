// Filter.cpp
// double Filter library
#include "Filter.h"


Filter::Filter(double initValue) {
        prevValue1_ = initValue;
        prevValue2_ = initValue;
        prevValue3_ = initValue;
        prevValue4_ = initValue;
        previousTime_ = millis();
}

double Filter::rateLimit(double inputValue, double maxRateUp, double maxRateDown) {
    double maxRateUp_ = maxRateUp;
    double maxRateDown_ = (maxRateDown==0) ? maxRateUp : maxRateDown;
    double dT = max(0.0001,(millis() - previousTime_));
    previousTime_ = millis();
    double rate = inputValue - prevValue1_;

    if (prevValue1_ > 0) {
        inputValue = prevValue1_ + min(maxRateUp_*dT,max(rate,-maxRateDown_*dT));
    } else if (prevValue1_ < 0) {
        inputValue = prevValue1_ + min(maxRateDown_*dT,max(rate,-maxRateUp_*dT));
    } else if (inputValue >= 0) {
        inputValue = prevValue1_ + min(maxRateUp_*dT,max(rate,-maxRateDown_*dT));
    } else {
        inputValue = prevValue1_ + min(maxRateDown_*dT,max(rate,-maxRateUp_*dT));
    }
    
    prevValue1_ = inputValue;
    return inputValue;
}

double Filter::firstOrdFilt(double inputValue, double filterPct ) {
    double filterPct_ = filterPct;
    double dT = max(0.0001,(millis() - previousTime_));
    previousTime_ = millis();
    float wtPct = max(0.0001, min(1, filterPct_*dT));

    prevValue1_ = prevValue1_ + (inputValue - prevValue1_) * wtPct;

    return prevValue1_;
}

double Filter::firstOrdFiltx4(double inputValue, double filterPct ) {
    double filterPct_ = filterPct;
    double dT = max(0.0001,(millis() - previousTime_));
    previousTime_ = millis();
    float wtPct = max(0.0001, min(1, filterPct_*dT));
    
    prevValue1_ = prevValue1_ + wtPct*(inputValue - prevValue1_);
    prevValue2_ = prevValue2_ + wtPct*(prevValue1_ - prevValue2_);
    prevValue3_ = prevValue3_ + wtPct*(prevValue2_ - prevValue3_);
    prevValue4_ = prevValue4_ + wtPct*(prevValue3_ - prevValue4_);
    
    return prevValue4_;
}
double Filter::rateLimitedFOF(double inputValue, double filterPct, double maxRateUp, double maxRateDown) {
    double filterPct_ = filterPct;
    double maxRateUp_ = maxRateUp;
    double maxRateDown_ = (maxRateDown==0) ? maxRateUp : maxRateDown;
    double dT = max(0.0001,(millis() - previousTime_));
    previousTime_ = millis();
    float wtPct = max(0.0001, min(1, filterPct_*dT));
    double rate = (inputValue - prevValue1_) * wtPct;
    
    if (prevValue1_ > 0) {
        inputValue = prevValue1_ + min(maxRateUp_*dT,max(rate,-maxRateDown_*dT));
    } else if (prevValue1_ < 0) {
        inputValue = prevValue1_ + min(maxRateDown_*dT,max(rate,-maxRateUp_*dT));
    } else if (inputValue >= 0) {
        inputValue = prevValue1_ + min(maxRateUp_*dT,max(rate,-maxRateDown_*dT));
    } else {
        inputValue = prevValue1_ + min(maxRateDown_*dT,max(rate,-maxRateUp_*dT));
    }
   
    prevValue1_ = inputValue;
    return inputValue;
}

double Filter::reset(double inputValue) {
    previousTime_ = millis();
    prevValue1_ = inputValue;
    prevValue2_ = inputValue;
    prevValue3_ = inputValue;
    prevValue4_ = inputValue;
    return inputValue;
}