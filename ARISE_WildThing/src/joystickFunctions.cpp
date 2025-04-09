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

#include "../include/joystickFunctions.h"
#include "../include/init.h"
#include "../include/Config.h"
#include "../include/pinouts.h"
#include <Arduino.h>
#include "../include/algorithm.h"
#include <math.h>
#include "../include/globals.h"


// custom functions for x-y joysticks

void createJoystickTables() {
// conversion tables to convert joystick angle (0to360 degrees) to Motor Left & Right Mix (-100to100 %)
// convention is joystick angle 0 degrees = straight ahead (North) and 90 degrees = to the right (East)
// also provides lookuptable for non-linear radius control (calculates %power from 0-100 radius)

//#include "joystickCals.h";

// Angle Lookup Table (Mix)

  // Generate LookupTables for L Mix
  float mixTableTemp_L[15][2] = {
      { 0                , motorForward_PowerLevels },
      { trimAngle , motorForward_PowerLevels + trimPlus },
      { 90 - spinZone , motorTurnFor_PowerLevels    },
      { 90 - spinDwell, motorSpin_PowerLevels    },
      { 90 + spinDwell, motorSpin_PowerLevels    },
      { 90 + spinZone , 0 },
      { 180 - trimAngle, - motorReverse_PowerLevels + trimMinus },
      { 180 , - motorReverse_PowerLevels },
      { 180 + trimAngle, - motorReverse_PowerLevels - trimPlus },
      { 270 - spinZone, - motorTurnRev_PowerLevels },
      { 270 - spinDwell, - motorSpin_PowerLevels },
      { 270 + spinDwell, - motorSpin_PowerLevels },
      { 270 + spinZone, 0 },
      { 360 - trimAngle, motorForward_PowerLevels - trimMinus  },
      { 360              , motorForward_PowerLevels }
  };
  memcpy(mixTable_L, mixTableTemp_L, sizeof(mixTableTemp_L));

  // Generate LookupTables for R Mix
  float mixTableTemp_R[15][2] = {
      { 0                , motorForward_PowerLevels },
      { trimAngle , motorForward_PowerLevels - trimMinus},
      { 90 - spinZone , 0    },
      { 90 - spinDwell, - motorSpin_PowerLevels    },
      { 90 + spinDwell, - motorSpin_PowerLevels    },
      { 90 + spinZone , - motorTurnRev_PowerLevels },
      { 180 - trimAngle, - motorReverse_PowerLevels - trimPlus},
      { 180 , - motorReverse_PowerLevels },
      { 180 + trimAngle, - motorReverse_PowerLevels + trimMinus },
      { 270 - spinZone, 0 },
      { 270 - spinDwell, motorSpin_PowerLevels },
      { 270 + spinDwell, motorSpin_PowerLevels },
      { 270 + spinZone, motorTurnFor_PowerLevels },
      { 360 - trimAngle, motorForward_PowerLevels + trimPlus },
      { 360              , motorForward_PowerLevels }

  };
  memcpy(mixTable_R, mixTableTemp_R, sizeof(mixTableTemp_R));

// Radius Lookup Table (Scale)

  // Generate LookupTable for Scale
  float radTableTemp[6][2] = {
      { 0             , 0 },
      { deadBand   , 0 },
      { deadBand + .01 , 0 },
      { slowRadius , slowPower },
      { 1 -endBand , 1 },
      { 1             , 1 }
  };
  memcpy(radTable, radTableTemp, sizeof(radTableTemp));

}
