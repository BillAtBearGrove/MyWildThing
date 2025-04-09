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

// Filter.h
#ifndef FILTER_H
#define FILTER_H
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