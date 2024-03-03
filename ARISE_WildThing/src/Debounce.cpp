// Debounce.cpp
// Boolean debounce library
#include "Debounce.h"

// usage example
  //Debounce myDebounce1(Debounce_ms); // defines Debounce with a Debounce_ms delay
  //bool debouncedState = myDebounce1.debounceInput(mySignal); // debouncedState will follow mySignal and debounce change after Debounce_ms, (ie. debounces both directions)
  //bool debouncedHigh = myDebounce1.debounceHigh(mySignal); // debouncedHigh will follow mySignal and debounce high change after Debounce_ms, change to low happens immediately
  //bool debouncedLow = myDebounce1.debounceLow(mySignal); // debouncedLow will follow mySignal and debounce low change after Debounce_ms, change to high happens immediately

Debounce::Debounce(unsigned long Debounce_ms) {
   debounceDelay = Debounce_ms;
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
