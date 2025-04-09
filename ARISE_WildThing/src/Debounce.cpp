/*
****************************************************************
ARISE Adaptive Design
Project: WildThing ESP32 Port
Author: Bill Smith (unchanged code)
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

// Debounce.cpp
// Boolean debounce library
#include "../include/Debounce.h"
#include <Arduino.h>

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