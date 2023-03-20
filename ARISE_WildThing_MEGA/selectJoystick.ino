///////////////////////////////////////////////////////////////////////////////////////////////////
// << selectJoystick >>
///////////////////////////////////////////////////////////////////////////////////////////////////

void selectJoystick() {
  
  ///////////////////////////////////////////////////////////////
  // check whether we need to use the occupant or tether joystick
  // Autocenter if it changes
  ///////////////////////////////////////////////////////////////
  //if (analogRead(JoySwitch_Main) < 1022 && digitalRead(JoySwitch_Tether) == HIGH)
  //Serial.print("JoySwitch_Main = "); Serial.print(analogRead(JoySwitch_Main)); Serial.print("\t");
  if (analogRead(JoySwitch_Main) > 800 )
  {
    //  Serial.print("Selected tether ");
    // If the onboard switch is on and the tether switch is not pushed, we use the tether joystick
    if (!usingTether || !joyInit)
    {
      // If we weren't using the tether previously, we must set it up
      Serial.print("Switching to the tether ");
      usingTether = true;
      Serial.print("Checking Tether Joystick "); Serial.print(joySelect); Serial.print("\n");
        joySelect = 1;
        joyX = joyX_Tether;
        joyY = joyY_Tether;
        joyOffsetAngle = joyOffsetAngle_Tether;
        flipSpin = flipSpin_Tether;
        numSamples = numSamples_Tether; // number of samples to check in a "window"
        numWindows = numWindows_Tether; // maximum attempts to check joystick

      // Check Joystick and AutoCenter
      AutoCenterJoystick(); // Check Joystick is near center and not moving, return "at Rest" values if pass test
    }
  }
  else
  {
     // Serial.print("Selected Onboard ");
    // We use the occupant joystick
    if (usingTether || !joyInit)
    {
      // If we weren't using the occupant previously, we must set it up
      Serial.print("Switching to the occupant ");
      usingTether = false;
      Serial.print("Checking Occupant Joystick "); Serial.print(joySelect); Serial.print("\n");
        joySelect = 2;
        joyX = joyX_Occupant;
        joyY = joyY_Occupant;
        joyOffsetAngle = joyOffsetAngle_Occupant;
        flipSpin = flipSpin_Occupant;
        numSamples = numSamples_Occupant; // number of samples to check in a "window"
        numWindows = numWindows_Occupant; // maximum attempts to check joystick

      // Check Joystick and AutoCenter
      AutoCenterJoystick(); // Check Joystick is near center and not moving, return "at Rest" values if pass test
    }
  }

}
