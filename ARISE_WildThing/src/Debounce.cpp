// Debounce.cpp
// Boolean debounce library
#include "Debounce.h"

// usage example
  //Debounce myDebounce1(debounce_ms); // defines Debounce with a debounce_ms delay
  //bool debouncedState = myDebounce1.debounceInput(mySignal); // debouncedState will follow mySignal and debounce change after debounce_ms, (ie. debounces both directions)
  //bool debouncedHigh = myDebounce1.debounceHigh(mySignal); // debouncedHigh will follow mySignal and debounce high change after debounce_ms, change to low happens immediately
  //bool debouncedLow = myDebounce1.debounceLow(mySignal); // debouncedLow will follow mySignal and debounce low change after debounce_ms, change to high happens immediately

Debounce::Debounce(unsigned long debounce_ms) {
   debounceDelay = debounce_ms;
}

bool Debounce::debounceInput(bool thisInput) {
  if (thisInput != lastInput) { // reset on change
    lastDebounceTime = millis();
    lastInput = thisInput;
  }
  bool isDebounced = (millis() - lastDebounceTime) >= debounceDelay; // isDebounced if no change for debounceDelay ms
  return isDebounced;
}

bool Debounce::debounceHigh(bool thisInput) {
  if (thisInput != lastInput) { // reset on change
    lastDebounceTime = millis();
    lastInput = thisInput;
  }
  bool isDebounced = (millis() - lastDebounceTime) >= debounceDelay; // isDebounced if no change for debounceDelay ms
  return isDebounced && thisInput;
}

bool Debounce::debounceLow(bool thisInput) {
  if (thisInput != lastInput) { // reset on change
    lastDebounceTime = millis();
    lastInput = thisInput;
  }
  bool isDebounced = (millis() - lastDebounceTime) >= debounceDelay; // isDebounced if no change for debounceDelay ms
  return isDebounced && !thisInput;
}

bool Debounce::debounceBoth(bool thisInput) {
  if (thisInput != lastInput) { // reset on change
    lastDebounceTime = millis();
    lastInput = thisInput;
  }
  bool isDebounced = (millis() - lastDebounceTime) >= debounceDelay; // isDebounced if no change for debounceDelay ms
  if (isDebounced) {
    lastOutput = thisInput; //only change if signal is debounced
  }

  return lastOutput;
}