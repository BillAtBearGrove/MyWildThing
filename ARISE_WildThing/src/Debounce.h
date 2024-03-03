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
