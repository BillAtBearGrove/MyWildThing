void createJoystickTables() {
// conversion tables to convert joystick angle (0to360 degrees) to Motor Left & Right Mix (-100to100 %)
// convention is joystick angle 0 degrees = straight ahead (North) and 90 degrees = to the right (East)
// also provides lookuptable for non-linear radius control (calculates %power from 0-100 radius)

//#include "joystickCals.h";

// Angle Lookup Table (Mix)

  // Generate LookupTables for L Mix
  float mixTableLTemp[15][2] = 
    {
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
  memcpy(mixTableL, mixTableLTemp, sizeof(mixTableLTemp));
  Serial.print("mixTableL= ");   Serial.println("");
    for(int i = 0; i < 15; i++)
    {
      Serial.print(i); Serial.print("  "); Serial.print(mixTableL[i][0]); Serial.print("  "); Serial.print(mixTableL[i][1]); Serial.println("");
    }
  Serial.println("");
    
  // Generate LookupTables for R Mix  
  float mixTableRTemp[15][2] = {
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
  memcpy(mixTableR, mixTableRTemp, sizeof(mixTableRTemp));
  Serial.print("mixTableR= ");   Serial.println("");
    for(int i = 0; i < 15; i++)
    {
      Serial.print(i); Serial.print("  "); Serial.print(mixTableR[i][0]); Serial.print("  "); Serial.print(mixTableR[i][1]); Serial.println("");
    }
  Serial.println("");

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
