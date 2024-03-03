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
      { TrimAngle , motorForward_PowerLevels + TrimPlus },
      { 90 - spinZone , motorTurnFor_PowerLevels    },
      { 90 - spinDwell, motorSpin_PowerLevels    },
      { 90 + spinDwell, motorSpin_PowerLevels    },
      { 90 + spinZone , 0 },
      { 180 - TrimAngle, - motorReverse_PowerLevels + TrimMinus },
      { 180 , - motorReverse_PowerLevels },
      { 180 + TrimAngle, - motorReverse_PowerLevels - TrimPlus },
      { 270 - spinZone, - motorTurnRev_PowerLevels },
      { 270 - spinDwell, - motorSpin_PowerLevels },
      { 270 + spinDwell, - motorSpin_PowerLevels },
      { 270 + spinZone, 0 },
      { 360 - TrimAngle, motorForward_PowerLevels - TrimMinus  },
      { 360              , motorForward_PowerLevels }
  };
  memcpy(mixTable_L, mixTableTemp_L, sizeof(mixTableTemp_L));
    
  // Generate LookupTables for R Mix  
  float mixTableTemp_R[15][2] = {
      { 0                , motorForward_PowerLevels },
      { TrimAngle , motorForward_PowerLevels - TrimMinus},
      { 90 - spinZone , 0    },
      { 90 - spinDwell, - motorSpin_PowerLevels    },
      { 90 + spinDwell, - motorSpin_PowerLevels    },
      { 90 + spinZone , - motorTurnRev_PowerLevels },
      { 180 - TrimAngle, - motorReverse_PowerLevels - TrimPlus},
      { 180 , - motorReverse_PowerLevels },
      { 180 + TrimAngle, - motorReverse_PowerLevels + TrimMinus },
      { 270 - spinZone, 0 },
      { 270 - spinDwell, motorSpin_PowerLevels },
      { 270 + spinDwell, motorSpin_PowerLevels },
      { 270 + spinZone, motorTurnFor_PowerLevels },
      { 360 - TrimAngle, motorForward_PowerLevels + TrimPlus },
      { 360              , motorForward_PowerLevels }
    
  };
  memcpy(mixTable_R, mixTableTemp_R, sizeof(mixTableTemp_R));

// Radius Lookup Table (Scale)

  // Generate LookupTable for Scale
  float radTableTemp[6][2] = {
      { 0             , 0 },
      { Deadband   , 0 },
      { Deadband + .01 , 0 },
      { SlowRadius , SlowPower },
      { 1 -Endband , 1 },
      { 1             , 1 }
  };
  memcpy(radTable, radTableTemp, sizeof(radTableTemp));
  
}
