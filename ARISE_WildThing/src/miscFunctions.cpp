// misc functions

float interpolate(float x, const float table[][2], int numRows) {
  // Function to perform linear interpolation on a 2D lookup table

  // Check if x is outside the range of the table
  if (x <= table[0][0]) {
    return table[0][1];
  } else if (x >= table[numRows - 1][0]) {
    return table[numRows - 1][1];
  }

  // Find the index of the two nearest x values in the table
  for (int i = 0; i < numRows; i+=1) {
    // Perform linear interpolation
    float x1 = table[i][0];
    float x2 = table[i + 1][0];
    float y1 = table[i][1];
    float y2 = table[i + 1][1];
    if ( x<=x2 ) { 
      if (x2==x1) {// protect divByZero
        return y1;
      } else {
        return y1 + (x - x1) * (y2 - y1) / (x2 - x1);
      }
    }
  }
  // next i
  
  return table[numRows - 1][1]; // if not found return last y
}

void rateLimitMotors() {
  // <<<< TODO >>>> CHANGE THIS TO REUSABLE CODE: "Rate Limit with Zero Cross Debounce" (incl. reset method) with inputs: {value, maxAccel, maxDecel, debounce}
  //NOTE - need to figure out how to handle zeroCrossDeb_L.debounceHigh(motorIsOff_L) - like is debounce embedded or can that method be instanced within the new "rateLimitWithDebounceZeroCross" call method

  ///////////////////////////////////////////////////////////////////////////////////////////////////
  // Rate Limit Motor Velocity & Delay in case of zero crossing
  ///////////////////////////////////////////////////////////////////////////////////////////////////

    float dT = max(0.1, millis() - RateLimPrevTime); // this timestep (ms)
    RateLimPrevTime = millis();

    float maxAccel_L = maxAccel*dT;
    float maxDecel_L = maxDecel*dT;
    float maxAccel_R = maxAccel*dT;
    float maxDecel_R = maxDecel*dT;

    if(abs(motorReqVel_filt_L)<brakeZone){
      maxAccel_L = maxAccel_Brake*dT;
      maxDecel_L = maxDecel_Brake*dT;
    }
    if(abs(motorReqVel_filt_R)<brakeZone){
      maxAccel_R = maxAccel_Brake*dT;
      maxDecel_R = maxDecel_Brake*dT;
    }    

    // check if xxx_Filt is 0; ok2ChangeDir_L = true if xxx_Filt is 0 for debounced time
    // because if xxx_Filt changes direction then we want to wait until ok2ChangeDir_x is true so we don't spike current or cause clunky behavior
    bool motorIsOff_L = motorReqVel_filt_L==0;
    bool motorIsOff_R = motorReqVel_filt_R==0;
    bool motorForwardReq_L = motorReqVel_L >= 0;
    bool motorForwardReq_R = motorReqVel_R >= 0;
    
    int ok2ChangeDir_L = zeroCrossDeb_L.debounceHigh(motorIsOff_L); // debounce if motor desired velocity is 0 for debounced time
    int ok2ChangeDir_R = zeroCrossDeb_R.debounceHigh(motorIsOff_R); // debounce if motor desired velocity is 0 for debounced time
        
    // rate limit LEFT motor
    if ( (motorForward_L == motorForwardReq_L) || ok2ChangeDir_L ) {
        motorReqVel_filt_L = filtMotorReqVel_L.rateLimit(motorReqVel_L, maxAccel_L, maxDecel_L); // Rate Limit motor_Vel
      } else { // changing direction and waiting for debounce
        motorReqVel_filt_L = filtMotorReqVel_L.rateLimit(0, maxAccel_L, maxDecel_L); // keep at 0
      }

    // rate limit LEFT motor
    if ( (motorForward_R == motorForwardReq_R) || ok2ChangeDir_R ) {
        motorReqVel_filt_R = filtMotorReqVel_R.rateLimit(motorReqVel_R, maxAccel_R, maxDecel_R); // Rate Limit motor_Vel
      } else { // changing direction and waiting for debounce
        motorReqVel_filt_R = filtMotorReqVel_R.rateLimit(0, maxAccel_R, maxDecel_R); // keep at 0
      }

    // keep track of motor direction (Note velocity of 0 does not change direction)
    if (motorReqVel_filt_L > 0) { // last direction is forward (motorForward_L initialized as true)
      motorForward_L = true;
    }
    if (motorReqVel_filt_L < 0) { // last direction is reverse
      motorForward_L = false;
    }
    if (motorReqVel_filt_R > 0) { // last direction is forward (motorForward_R initialized as true)
      motorForward_R = true;
    }
    if (motorReqVel_filt_R < 0) { // last direction is reverse
      motorForward_R = false;
    }
//    Serial.print("ok2ChangeDir_L = "); Serial.print(ok2ChangeDir_L); Serial.print("\t");
//    Serial.print("motorForward_L = "); Serial.print(motorForward_L); Serial.print("\t");
//    Serial.print("motorForwardReq_L = "); Serial.print(motorForwardReq_L); Serial.print("\t");
//    Serial.print("motorReqVel_L = "); Serial.print(motorReqVel_L); Serial.print("\t");
//    Serial.print("motorReqVel_filt_L = "); Serial.print(motorReqVel_filt_L); Serial.print("\t");
//    Serial.println("");

}

float readPot(int pinNum) {
  float potValue = analogRead(pinNum);
  float speedMult = rescale(potValue, potValueRescale[0], potValueRescale[1], potValueRescale[2], potValueRescale[3]); // linear conversion of speed dial to max speed
  return speedMult;
}

float rescale(float x, float x1, float x2, float y1, float y2) {
  return (x - x1) * (y2 - y1) / (x2 - x1) + y1;
}

void setJoystickOutOfRangeError(){
      // If we got here, we probably have a bad connection in the Joystick Circuit so stop (ramp down motors) and throw error
      if (showDetail) {
        Serial.println(""); Serial.print("ERROR: Joystick out of Range. Check Joystick Pin and Connector Connections."); Serial.println("");
      }
      // Need to set motor speeds to 0 (motorReqVel_filt_L & motorReqVel_filt_R will let rate limiter take care of decel rate limits)
      joyAngle = 0;
      joyRadius = 0;
}

