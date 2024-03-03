// Filter.h
#ifndef Filter_h
#define Filter_h

#include "Arduino.h"

class Filter
{
  public:
    Filter(double initValue);
    double rateLimit(double inputValue, double maxRateUp, double maxRateDown = 0);
    double firstOrdFilt(double inputValue, double filterPct);
    double firstOrdFiltx4(double inputValue, double filterPct);
    double rateLimitedFOF(double inputValue, double filterPct, double maxRateUp, double maxRateDown = 0);
    double reset(double inputValue);

private:
    double maxRateUp_;
    double maxRateDown_;
    double filterPct_;
    double prevValue1_;
    double prevValue2_;
    double prevValue3_;
    double prevValue4_;
    unsigned long previousTime_;
};

#endif