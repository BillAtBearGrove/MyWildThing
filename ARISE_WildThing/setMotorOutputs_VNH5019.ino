void setMotorOutputs_VNH5019(float joyReq_L, float joyReq_R){

  float motorDir_L, motorPwm_L, motorDir_R, motorPwm_R;

  // Keep track of directions
  if (joyReq_L>0 && PIDout_L>=0) {
    motorDir_L = 1;
    motorPID_L.setLimits(0, PID_max);
  } else if (joyReq_L<0 && PIDout_L<=0) {
    motorDir_L = -1;
    motorPID_L.setLimits(-PID_max, 0);
  }
  if (joyReq_R>0 && PIDout_R>=0) {
    motorDir_R = 1;
    motorPID_R.setLimits(0, PID_max);
  } else if (joyReq_R<0 && PIDout_R<=0) {
    motorDir_R = -1;
    motorPID_R.setLimits(-PID_max, 0);
  }

  // Calculate Requested (Target) Speed based on filtered joystick requests
    float noLoadSpeedFactor = 120; // No Load Speed factor for converting Motor %PWM to RPMs
    float speedReq_L = joyReq_L*noLoadSpeedFactor; // Left Speed Request approx scale: -1To1 (Full Reverse to Full Forward) = -120 to 120 RPM
    float speedReq_R = joyReq_R*noLoadSpeedFactor; // Right Speed Request approx scale: -1To1 (Full Reverse to Full Forward) = -120 to 120 RPM

    // measure and filter current both motors
    float mA_L = mA_filt_L.firstOrdFiltx4(md.getM1CurrentMilliamps(), mA_Pct); // Left Current mA
    float mA_R = mA_filt_R.firstOrdFiltx4(md.getM2CurrentMilliamps(), mA_Pct); // Right Current mA

    // calculate speed
    float speedCalc_L, speedCalc_R;
    float speedFilt_pct = 0.1;
    float speedFilt_rate = 0.3;
    float LowSpeedCutoff = 500;
    float speedCalcMin_L = motorDir_L *( abs(joyReq_L)*noLoadSpeedFactor*1.5*(1-min(1, max(0,mA_L/LowSpeedCutoff) )) ); // elevated speed to help PID get to Braking in overrun condition
    float speedCalcMin_R = motorDir_R *( abs(joyReq_R)*noLoadSpeedFactor*1.5*(1-min(1, max(0,mA_R/LowSpeedCutoff) )) ); // elevated speed to help PID get to Braking in overrun condition

    if (Brake_L>350){
      speedCalc_L = speedCalc_filt_L.rateLimitedFOF((noLoadSpeedFactor*PIDout_L - (mA_L *( motorForward_L? Ra:-Ra) ) ) * Ke, speedFilt_pct, speedFilt_rate); // Left calculated speed
    } else {
      if (motorDir_L>0) {
        speedCalc_L = speedCalc_filt_L.rateLimitedFOF( max( speedCalcMin_L, (noLoadSpeedFactor*PIDout_L - (mA_L *( motorForward_L? Ra:-Ra) ) ) * Ke) , speedFilt_pct, speedFilt_rate); // Left calculated speed
      } else {
        speedCalc_L = speedCalc_filt_L.rateLimitedFOF( min( speedCalcMin_L, (noLoadSpeedFactor*PIDout_L - (mA_L *( motorForward_L? Ra:-Ra) ) ) * Ke) , speedFilt_pct, speedFilt_rate); // Left calculated speed
      }
    }
    if (Brake_R>350){
      speedCalc_R = speedCalc_filt_R.rateLimitedFOF((noLoadSpeedFactor*PIDout_R - (mA_R *( motorForward_R? Ra:-Ra) ) ) * Ke, speedFilt_pct, speedFilt_rate); // Left calculated speed
    } else {
      if (motorDir_R>0) {
        speedCalc_R = speedCalc_filt_R.rateLimitedFOF( max( speedCalcMin_R, (noLoadSpeedFactor*PIDout_R - (mA_R *( motorForward_R? Ra:-Ra) ) ) * Ke) , speedFilt_pct, speedFilt_rate); // Left calculated speed
      } else {
        speedCalc_R = speedCalc_filt_R.rateLimitedFOF( min( speedCalcMin_R, (noLoadSpeedFactor*PIDout_R - (mA_R *( motorForward_R? Ra:-Ra) ) ) * Ke) , speedFilt_pct, speedFilt_rate); // Left calculated speed
      }
    }

    PIDout_L = motorPID_L.calc( speedReq_L - speedCalc_L, joyReq_L ); // error controller PID(error, FeedForward)
    PIDout_R = motorPID_R.calc( speedReq_R - speedCalc_R, joyReq_R ); // error controller PID(error, FeedForward)

//    PIDout_L = joyReq_L; // no error controller
//    PIDout_R = joyReq_R; // no error controller

  // Motor Braking and Driving
  float brakeSlope = (maxBrake - minBrake) / brakeZone; // slope of brake line from maxBrake at 0  to minBrake at motorZone
  float pwmSlope = (1 - motorDropout) / (1-brakeZone); // slope of pwm line

    if (abs(PIDout_L)<=brakeZone){
      // Reset motor
      motorPwm_L = 0; //reset pwm level if applying brake
      // Drive Brake
      if (Brake_L < maxBrake) {// Braking function (init maxBrake, where maxBrake is latched until motorOut > brakeZone, otherwise scaled from maxBrake to minBrake)
        Brake_L = minBrake + ((brakeZone - abs(PIDout_L)) * brakeSlope); // amount to brake  (0 to 400)
      }
      md.setM1Brake(Brake_L); // set brake pwm
    } else {
      // Reset Brake
      Brake_L = 0; //reset brake level if driving motor
      // Drive Motor
      motorPwm_L = (motorDropout + pwmSlope * (abs(PIDout_L) - brakeZone) ) * motorDir_L;
      md.setM1Speed( motorPwm_L * 400); // set motor drive pwm
    }

    if (abs(PIDout_R)<=brakeZone){
      // Reset motor
      motorPwm_R = 0; //reset pwm level if applying brake
      // Drive Brake
      if (Brake_R < maxBrake) {// Braking function (init maxBrake, where maxBrake is latched until motorOut > brakeZone, otherwise scaled from maxBrake to minBrake)
        Brake_R = minBrake + ((brakeZone - abs(PIDout_R)) * brakeSlope); // amount to brake  (0 to 400)
      }
      md.setM2Brake(Brake_R); // set brake pwm
    } else {
      // Reset Brake
      Brake_R = 0; //reset brake level if driving motor
      // Drive Motor
      motorPwm_R = (motorDropout + pwmSlope * (abs(PIDout_R) - brakeZone) ) * motorDir_R;
      md.setM2Speed( motorPwm_R * 400 ); // set motor drive pwm
    }
    if ( currentTime - lastDebugTime >= debugPeriod) {
/*
      Serial.print("R: ");
      Serial.print("joyReq "); Serial.print(joyReq_R); Serial.print(" ");
      Serial.print("speedReq_R "); Serial.print(speedReq_R); Serial.print(" ");
      Serial.print("M1Speed "); Serial.print(motorPwm_R*400); Serial.print(" ");
//      Serial.print("mA "); Serial.print(mA_R); Serial.print(" ");
      Serial.print("SpeedTarget "); Serial.print(speedReq_R); Serial.print(" ");
      Serial.print("speedCalcMin "); Serial.print(speedCalcMin_R); Serial.print(" ");
      Serial.print("speedCalc "); Serial.print(speedCalc_R); Serial.print(" ");
      Serial.print("speedErr "); Serial.print(speedReq_R-speedCalc_R); Serial.print(" ");
      Serial.print("PIDout "); Serial.print(PIDout_R); Serial.print(" ");
//      Serial.print("Brake "); Serial.print(Brake_R); Serial.print(" ");
*/
      Serial.print("L: ");
      Serial.print("joyReq "); Serial.print(joyReq_L); Serial.print(" ");
      Serial.print("speedReq "); Serial.print(speedReq_L); Serial.print(" ");
      Serial.print("M1Speed "); Serial.print(motorPwm_L*400); Serial.print(" ");
//      Serial.print("mA "); Serial.print(mA_L); Serial.print(" ");
      Serial.print("SpeedTarget "); Serial.print(speedReq_L); Serial.print(" ");
      Serial.print("speedCalcMin "); Serial.print(speedCalcMin_L); Serial.print(" ");
      Serial.print("speedCalc "); Serial.print(speedCalc_L); Serial.print(" ");
      Serial.print("speedErr "); Serial.print(speedReq_L-speedCalc_L); Serial.print(" ");
      Serial.print("PIDout "); Serial.print(PIDout_L); Serial.print(" ");
//      Serial.print("Brake "); Serial.print(Brake_L); Serial.print(" ");

      Serial.println("");
    }

}
