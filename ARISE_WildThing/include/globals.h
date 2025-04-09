/*
****************************************************************
ARISE Adaptive Design
Project: WildThing ESP32 Port
Author: ejc
****************************************************************
Change Log:
-Changes Made. MM.DD.YYYY
--Initial codebase - 11.18.2024
--Updated include list and separated by internal/custom - 11.21.2024
****************************************************************
Notes:
-Note. MM.DD.YYYY
--Initial codebase - 11.18.2024
****************************************************************
*/

// pragma once prevents the file to be included twice
#pragma once

// globals.h (Declare global variables)
#ifndef GLOBALS_H
#define GLOBALS_H

// #include <DualVNH5019MotorShield.h>
#include "../include/DualVNH7070AYESP32Driver.h"

// Global Variables
extern struct joyInputs joyInputs_;
// extern DualVNH5019MotorShield md;
extern DualVNH7070AYESP32Driver md;

#endif