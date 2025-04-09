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

// Debounce.h
#ifndef Debounce_h
#define Debounce_h
#include "Arduino.h"
class Debounce
{
  public:
    Debounce(unsigned long Debounce_ms = 50);
    bool debounceInput(bool thisInput);
    bool debounceHigh(bool thisInput);
    bool debounceLow(bool thisInput);
    bool debounceBoth(bool thisInput);

  private:
    bool lastInput = false;
    bool lastOutput = false;
    unsigned long lastDebounceTime = 0;
    unsigned long debounceDelay;
};

#endif
