
///////////////////////////////////////////////////////////////////////////////////////////////////
// << AutoCenterJoystick >>
///////////////////////////////////////////////////////////////////////////////////////////////////

void AutoCenterJoystick() {
  // Check joystick is in rest position (noone is moving it) and find nominal X & Y resting positions before starting loop
  /*
    The following section of code checks the X & Y positions of the joystick before starting the Loop 
    This ensures the joystick is not pushed at startup (aka should be hands-off, so machine does not take off immediately when turned on)
    Also monitoring the X & Y positions for a certain amount of time allows us to automatically find the REST Position (Center) of X & Y
    We will record a Window of X & Y positions ensure they are within the expected "resting" range.
    Calibrations allow to define the number of samples to check in a single window and how many windows to check (aka. # of attempts is basically a timeout)
  */

  // JOYSTICK STARTUP TEST CALIBRATIONS
  int joyPosRestingMin = (512 - 60); // min assumed "resting" position (0-1023 A2D)
  int joyPosRestingMax = (512 + 60); // max assumed 'resting" position (0-1023 A2D)
  int joyPosRestingTol = 16; // max allowable noise at rest (total range max-min recorded during check) (0-1023 A2D)

  Serial.print("<<< START Joystick Check >>> "); Serial.print(joySelect);  
  Serial.print("\n");
  
  bool joyCheckSuccessful = false; // flag to keep track if joystick passes startup test
  // Variables for Joystick min & max measured during window
    int minX;
    int maxX;
    int minY;
    int maxY;
  // Check joystick is in center and not moving (if it's out of center or moving, then try again up to numWindows)
  for (int window = 1; window <= numWindows; window++) {
    // initialize min & max at first measurment in window
    minX = analogRead(joyX);
    maxX = analogRead(joyX);
    minY = analogRead(joyY);
    maxY = analogRead(joyY);
    // sample the joystick the requested number of times
    for (int sample = 0; sample < numSamples; sample++) {
      //sample joystick
      joyPosX = analogRead(joyX);
      joyPosY = analogRead(joyY);
      // keep track of min & max for X & Y
      minX = min(joyPosX,minX);
      maxX = max(joyPosX,maxX);
      minY = min(joyPosY,minY);
      maxY = max(joyPosY,maxY);
    }
    // check min & max within allowed range and the difference between them is within resting tolerance (aka not moving or noisy)
    if ( minX >= joyPosRestingMin && maxX <= joyPosRestingMax && minY >= joyPosRestingMin && maxY <= joyPosRestingMax && (maxX - minX) <= joyPosRestingTol && (maxY - minY) <= joyPosRestingTol ) {
      joyCheckSuccessful = true;
      Serial.print("<<< Joystick Passed Startup Check >>>");
      Serial.print("\t");
      Serial.print("Number of attempts = ");
      Serial.print(window);
      Serial.print("\n");
      break;
    } else {
      // Print out results
      Serial.print("Attempt # "); Serial.print(window); Serial.print(" FAILED. Joystick "); Serial.print(joySelect); Serial.print("\n");
      Serial.print("minX = "); Serial.print(minX); Serial.print("\t");
      Serial.print("maxX = "); Serial.print(maxX); Serial.print("\t");
      Serial.print("minY = "); Serial.print(minY); Serial.print("\t");
      Serial.print("maxY = "); Serial.print(maxY); Serial.print("\t");
      Serial.print("\n");
    }
  }

  if (joyCheckSuccessful) {
      joyPassed = true;
      joyInit = true;
      
      xRest = (minX + maxX)/2; //set to mid-range of recorded signal
      yRest = (minY + maxY)/2; //set to mid-range of recorded signal
      
      // Print out results
      Serial.print("joyX at Rest = "); Serial.print(xRest); Serial.print("\t");
      Serial.print("minX = "); Serial.print(minX); Serial.print("\t");
      Serial.print("maxX = "); Serial.print(maxX); Serial.print("\t");
      
      Serial.print("joyY at Rest = "); Serial.print(yRest); Serial.print("\t");
      Serial.print("minY = "); Serial.print(minY); Serial.print("\t");
      Serial.print("maxY = "); Serial.print(maxY); Serial.print("\t");
      
      Serial.print("\n");
      
  } else {
      joyPassed = false;
      
      Serial.print("<<< Joystick FAILED Startup Check >>>");
      Serial.print("\n");
      
      // Print out results
      Serial.print("minX = "); Serial.print(minX); Serial.print("\t");
      Serial.print("maxX = "); Serial.print(maxX); Serial.print("\t");
      
      Serial.print("minY = "); Serial.print(minY); Serial.print("\t");
      Serial.print("maxY = "); Serial.print(maxY); Serial.print("\t");
      
      Serial.print("\n");
      /*
      bool hold = true; // set system to hold here forever & Blink LEDs
      while (hold == true) {
        hold = true;
      }
      */
  }

  Serial.print("<<< END Joystick Check >>>");
  Serial.print("\n");
  
}
