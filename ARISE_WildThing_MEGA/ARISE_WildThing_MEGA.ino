// ARISE WildThing code for VNH5019 Motor Driver Shield for Arduino
/*
  ARISE - Syracuse, NY
  Main Contact: Connor McGough
  Coders: Bill Smith 2021/11/10
*/

///////////////////////////////////////////////////////////////////////////////////////////////////
// << CALIBRATIONS >> //
///////////////////////////////////////////////////////////////////////////////////////////////////

  // #include "pinouts.h"; // use for SPARK, 298N, Amazon
  #include "pinouts_vnh5019.h"; // Use for VNH5019 MotorShield
  #include "CalibrationsGlobal.h";
  #include "CalibrationJoystick.h";
  #include "DualVNH5019MotorShield.h"

  DualVNH5019MotorShield md;

void setup()
{
  ///////////////////////////////////////////////////////////////////////////////////////////////////
  // << initialize serial communications >>
  ///////////////////////////////////////////////////////////////////////////////////////////////////
    Serial.begin(9600); // set communication between computer & Arduino; and Arduino to HC-50 Remote

  ///////////////////////////////////////////////////////////////////////////////////////////////////
  // << DEFINE OUTPUTS/INPUTS >>
  ///////////////////////////////////////////////////////////////////////////////////////////////////
    switch (motorDriverType) {
      case 1: case 2:
        // set all the motor control pins to outputs
          pinMode(pwm1, OUTPUT);
          pinMode(pwm2, OUTPUT);
          pinMode(dir1, OUTPUT);
          pinMode(dir1_opp, OUTPUT);
          pinMode(dir2, OUTPUT);
          pinMode(dir2_opp, OUTPUT);
          //digitalWrite(HC05_GND, LOW); // sets digital pin to LOW to function as GND

        ///////////////////////////////////////////////////////////////////////////////////////////////////
        // << SET MOTORS OFF >>
        ///////////////////////////////////////////////////////////////////////////////////////////////////
          delay(50); // wait a little before turning on Motor Controllers
          pinMode(PowerLevelSupply, OUTPUT); // define digital pin as output
          digitalWrite(PowerLevelSupply, HIGH); // sets digital pin to HIGH to function as 5V
          pinMode(powerSpark1, OUTPUT); // define digital pin as output
          digitalWrite(powerSpark1, HIGH); // sets digital pin to HIGH to function as 5V
          pinMode(powerSpark2, OUTPUT); // define digital pin as output
          digitalWrite(powerSpark2, HIGH); // sets digital pin to HIGH to function as 5V

          // Start with motors disabled and direction forward
          // Motor 1
          analogWrite(pwm1, 0 );
          digitalWrite(dir1, HIGH);
          digitalWrite(dir1_opp, LOW);

          // Motor 2
          analogWrite(pwm2, 0 );
          digitalWrite(dir2, HIGH);
          digitalWrite(dir2_opp, LOW);
      break;
      case 3:
        // initialize VNH5019
          md.init();
      break;
      }
      // PullUp all pot inputs HIGH (avoid float measurement in case of broken wire or disconnect - aka with pullup pins will read full A2D if broken wire)
//        digitalWrite(joyX_Occupant, HIGH); // Horizontal Joystick input <Occupant>
//        digitalWrite(joyY_Occupant, HIGH); // Vertical Joystick input <Occupant>
//        digitalWrite(joyX_Tether, HIGH); // Horizontal Joystick input <Tethered>
//        digitalWrite(joyY_Tether, HIGH); // Vertical Joystick input <Tethered>
//        digitalWrite(PowerLevelPotInput, HIGH); // Input pin for potentiometer
        pinMode(JoySwitch_Main,INPUT);
        //digitalWrite(JoySwitch_Main, HIGH); // +5v pin for main switch

      // LEDs
        pinMode(statusLED,OUTPUT);
        analogWrite(statusLED, 255); // Turn ON GREEN STATUS LED

      // Generate mix tables for Left & Right mix based on joystick angle and scale based on radius
        createJoystickTables();
}


void loop()
{
  selectJoystick(); // Check if we use occupant or tether joystick & Autocenter if it changes

  // might need to move this somewhere else? not completely sure
  if (joyPassed == true) {
  ///////////////////////////////////////////////////////////////////////////////////////////////////
  // << Calculate Joystick Polar Coordinates (Angle and Radius) from Cartesian Coordinates (X & Y) >>
  ///////////////////////////////////////////////////////////////////////////////////////////////////

  // << Read the Joystick X and Y positions >>
  joyPosX = analogRead(joyX);
  joyPosY = analogRead(joyY);

  // Check for broken wires or bad connection. Stop motors if bad <ELSE> Calc Desired Motor Velocities if good
  if ( joyPosX < joyFaultBand || joyPosX > 1023-joyFaultBand || joyPosY < joyFaultBand || joyPosY > 1023-joyFaultBand) {
      // If we got here, we probably have a bad connection in the Joystick Circuit so stop (ramp down motors) and throw error
      Serial.println("");
      Serial.print("ERROR: Joystick out of Range. Check Joystick Pin and Connector Connections.");
      Serial.println("");
      // Need to set motor speeds to 0 (motorLVel_Filt & motorRVel_Filt will let rate limiter take care of decel rate limits)
      //motorLVel = 0;
      //motorRVel = 0;
      joyAngle = 0;
      joyRadius = 0;

    } else {
      // Joystick wires seem OK, so go ahead...
      deltaX = joyPosX - xRest; // A2D cartesian position from "at Rest"
      deltaY = joyPosY - yRest; // A2D cartesian position from "at Rest"

      // convert cartesian deltaX and deltaY of joystick into polar coordinates joyAngle & joyRadius
      getPolarCoordinates();
    }


  ///////////////////////////////////////////////////////////////////////////////////////////////////
  // << CALC DESIRED MOTOR VELOCITY >>
  ///////////////////////////////////////////////////////////////////////////////////////////////////
    // figure out scale from radius
    for (int i = 0; i < 5; i+=1) {
      x1 = radTable[i][0];
      y1 = radTable[i][1];
      x2 = radTable[i+1][0];
      y2 = radTable[i+1][1];
      if ( joyRadius<=x2 ) {
        if (x2==x1) {scale = y1;} else {  scale = y1 + (joyRadius - x1) * (y2 - y1) / (x2 - x1);}
/*
        Serial.print("i = "); Serial.print(i); Serial.print("\t");
        Serial.print("x1 = "); Serial.print(x1); Serial.print("\t");
        Serial.print("x2 = "); Serial.print(x2); Serial.print("\t");
        Serial.print("y1 = "); Serial.print(y1); Serial.print("\t");
        Serial.print("y2 = "); Serial.print(y2); Serial.print("\t");
        Serial.print("joyRadius = "); Serial.print(joyRadius); Serial.print("\t");
        Serial.print("scale = "); Serial.print(scale); Serial.print("\t");
        Serial.println("");
*/
        break;
      }
    }

    // figure out Lmix from angle
    for (int i = 0; i < 14; i+=1) {
      x1 = mixTableL[i][0];
      y1 = mixTableL[i][1];
      x2 = mixTableL[i+1][0];
      y2 = mixTableL[i+1][1];

      if ( joyAngle<=x2 ) {
        if (x2==x1) {Lmix = y1;} else {  Lmix = y1 + (joyAngle - x1) * (y2 - y1) / (x2 - x1);}
        //Serial.print("LIndex= "); Serial.print(i); Serial.print("  ");
        break;
      }
    }

    // figure out Rmix from angle
    for (int i = 0; i < 14; i+=1) {
      x1 = mixTableR[i][0];
      y1 = mixTableR[i][1];
      x2 = mixTableR[i+1][0];
      y2 = mixTableR[i+1][1];
      if ( joyAngle<=x2 ) {
        if (x2==x1) {Rmix = y1;} else {  Rmix = y1 + (joyAngle - x1) * (y2 - y1) / (x2 - x1);}
        //Serial.print("RIndex= "); Serial.print(i); Serial.print("  "); 
        break;
      }
    }

    // Read potentiometer output and get the speed multiplier from there

    readPot();
    motorLVel = Lmix * scale * speedMultiplier;
    motorRVel = Rmix * scale * speedMultiplier;

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    // << FILTER MOTOR VELOCITY & SET DIRECTIONS >>
    ///////////////////////////////////////////////////////////////////////////////////////////////////
      // set LEFT motor directions (Note: dir1_opp and dir2_opp not used for SPARK but needed to keep code consistent)
      if (motorLForward) {
        // we are in Forward direction
        motorLVel_next = motorLVel_Filt + min( maxAccel, max( -maxDecel, motorLVel - motorLVel_Filt ) ); // Rate Limit on Positive direction
        if (motorLVel_next > 0 || zeroCrossingCountL >= zeroCrossingDwell) {
          motorLVel_Filt = motorLVel_next ; // if both xxx_Filt and xxx__next same sign, then no dwell
          zeroCrossingCountL = 0 ;
        } else {
          motorLVel_Filt = 0 ;
          zeroCrossingCountL++ ;
        }
      } else {
        // we are in Reverse direction
        motorLVel_next = motorLVel_Filt + max( -maxAccel, min( maxDecel, motorLVel - motorLVel_Filt ) );  // Rate Limit on Negative direction
        if (motorLVel_next < 0 || zeroCrossingCountL >= zeroCrossingDwell) {
          motorLVel_Filt = motorLVel_next ; // if both xxx_Filt and xxx__next same sign, then no dwell
          zeroCrossingCountL = 0 ;
        } else {
          motorLVel_Filt = 0 ;
          zeroCrossingCountL++ ;
        }
      }

      // keep track of motor direction (Note velocity of 0 does not change direction)
      if (motorLVel_Filt > 0) {
        motorLForward = true; // last direction is forward (motorLForward initialized as true)
        digitalWrite(dir1, HIGH);
        digitalWrite(dir1_opp, LOW);
      }
      if (motorLVel_Filt < 0) {
        motorLForward = false;  // last direction is reverse
        digitalWrite(dir1, LOW);
        digitalWrite(dir1_opp, HIGH);
      }

      // set RIGHT motor directions (Note: dir1_opp and dir2_opp not used for SPARK but needed to keep code consistent)
      if (motorRForward) {
        motorRVel_next = motorRVel_Filt + min( maxAccel, max( -maxDecel, motorRVel - motorRVel_Filt ) ); // Rate Limit on Positive direction
        if (motorRVel_next > 0 || zeroCrossingCountR >= zeroCrossingDwell) {
          motorRVel_Filt = motorRVel_next ; // if both xxx_Filt and xxx__next same sign, then no dwell
          zeroCrossingCountR = 0 ;
        } else {
          motorRVel_Filt = 0 ;
          zeroCrossingCountR++ ;
        }
      } else {
        // we are in Reverse direction
        motorRVel_next = motorRVel_Filt + max( -maxAccel, min( maxDecel, motorRVel - motorRVel_Filt ) );  // Rate Limit on Negative direction
        if (motorRVel_next < 0 || zeroCrossingCountR >= zeroCrossingDwell) {
          motorRVel_Filt = motorRVel_next ; // if both xxx_Filt and xxx__next same sign, then no dwell
          zeroCrossingCountR = 0 ;
        } else {
          motorRVel_Filt = 0 ;
          zeroCrossingCountR++ ;
        }
      }

      // keep track of motor direction (Note velocity of 0 does not change direction)
      if (motorRVel_Filt > 0) {
        motorRForward = true; // last direction is forward (motorLForward initialized as true)
        digitalWrite(dir2, HIGH);
        digitalWrite(dir2_opp, LOW);
      }
      if (motorRVel_Filt < 0) {
        motorRForward = false;  // last direction is reverse
        digitalWrite(dir2, LOW);
        digitalWrite(dir2_opp, HIGH);
      }

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    // << SCALE and OUTPUT MOTOR SIGNALS >>
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    // need to make sure that when turning on we start at motor min value
      if( motorLVel_Filt > 0 ) {
        motorLVel_Final = rescale(motorLVel_Filt, 0, 1, motorDropout, 1);
      } else if ( motorLVel_Filt < 0 ) {
        motorLVel_Final = rescale(motorLVel_Filt, -1, 0, -1, -motorDropout);
      } else {
        motorLVel_Final = 0;
      }
      if( motorRVel_Filt > 0 ) {
        motorRVel_Final = rescale(motorRVel_Filt, 0, 1, motorDropout, 1);
      } else if ( motorRVel_Filt < 0 ) {
        motorRVel_Final = rescale(motorRVel_Filt, -1, 0, -1, -motorDropout);
      } else {
        motorRVel_Final = 0;
      }

    switch (motorDriverType) {
      case 1:
          // scale motor speeds based on whether the car is in reverse
          // SPARK is special //
              // https://www.revrobotics.com/content/docs/REV-11-1200-QS.pdf
              //                                Pulse Width (µs)
              //                       Full Reverse     Prop. Reverse       Neutral         Prop. Forward  Full Forward
              // Factory Default Range   p ≤ 1000      1000 < p < 1440   1440 ≤ p ≤ 1440   1500 < p < 2000   2000 ≤ p
          // Therefore SPARK driver conversion (when using MEGA w/ 490Hz PWM Frequency) is 124 A2D = full reverse, 188 A2D = stopped, 252 A2D = full forward
            motorLSpeed = rescale(motorLVel_Final, -1, 1, 124, 252); // pwm output 124 A2D = full reverse, 252 A2D = full forward
            motorRSpeed = rescale(motorRVel_Final, -1, 1, 124, 252); // pwm output 124 A2D = full reverse, 252 A2D = full forward
          // OUTPUT motor speeds
            analogWrite(pwm1, motorLSpeed );
            analogWrite(pwm2, motorRSpeed );
      break;
      case 2:
          // AMAZON or 298N
            motorLSpeed = rescale(abs(motorLVel_Filt), 0, 1, 0, motorMaxSpeed);
            motorRSpeed = rescale(abs(motorRVel_Filt), 0, 1, 0, motorMaxSpeed);
          // OUTPUT motor speeds
            analogWrite(pwm1, motorLSpeed );
            analogWrite(pwm2, motorRSpeed );
      break;
      case 3:
        if (motorLVel_Final==0){
          BrakeL = min(BrakeL+BrakeRamp,maxBrake); //increment brake at defined rate to avoid skids ;), Limit at maxBrake
          md.setM1Brake(BrakeL); // set brake pwm
        } else {
          BrakeL = 0; //reset brake level if driving motor
          motorLSpeed = rescale(motorLVel_Final, -1, 1, -400, 400);
          md.setM1Speed(motorLSpeed);
        }
        if (motorRVel_Final==0){
          BrakeR = min(BrakeR+BrakeRamp,maxBrake); //increment brake at defined rate to avoid skids ;), limit at maxBrake
          md.setM2Brake(BrakeR); // set brake pwm
        } else {
          BrakeR = 0; //reset brake level if driving motor
          motorRSpeed = rescale(motorRVel_Final, -1, 1, -400, 400);
          md.setM2Speed(motorRSpeed);
        }
        //stopIfFault();
      break;
    }


    ///////////////////////////////////////////////////////////////////////////////////////////////////
    // << Turns on LED's to indicate the battery level >>
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    //Get battery level
    //int batteryLevel = readVcc();
    //Todo: figure out best scaling for Battery, maybe 9V should be only 1 LED, and 13V is all 5 LEDs ?

    //Lights up some of the LEDs based on how much battery there is
    //if (batteryLevel > 0) { digitalWrite(ledPins[0], HIGH); } else { digitalWrite(ledPins[0], LOW); }
    //if (batteryLevel > 4000) { digitalWrite(ledPins[1], HIGH); } else { digitalWrite(ledPins[1], LOW); }
    //if (batteryLevel > 5000) { digitalWrite(ledPins[2], HIGH); } else { digitalWrite(ledPins[2], LOW); }
    //if (batteryLevel > 5300) { digitalWrite(ledPins[3], HIGH); } else { digitalWrite(ledPins[3], LOW); }

    // Print info to Serial screen
    debug();

  ///////////////////////////////////////////////////////////////////////////////////////////////////
  // << CONTROL LOOP SPEED with Delay >>
  ///////////////////////////////////////////////////////////////////////////////////////////////////
    delay(10); // wait x milliseconds before the next loop:
  } else {
    Serial.print("Failed Joystick Checks. Stopping Run. "); Serial.print("\n");
    joySelect = 0; // <1> Tethered (default) or <2> Occupant <0> Error
    // Print Joystick Readings;
      Serial.print("Occupant X "); Serial.print("\t"); Serial.print(analogRead(joyX_Occupant)); Serial.print("\t");
      Serial.print("Occupant Y "); Serial.print("\t"); Serial.print(analogRead(joyY_Occupant)); Serial.print("\t");
      Serial.print("Tether X "); Serial.print("\t"); Serial.print(analogRead(joyX_Tether)); Serial.print("\t");
      Serial.print("Tether Y "); Serial.print("\t"); Serial.print(analogRead(joyY_Tether)); Serial.print("\t");
      //Serial.print("JoySwitch_Tether = "); Serial.print(digitalRead(JoySwitch_Tether)); Serial.print("\t");
      //Serial.print("JoySwitch_Onboard = "); Serial.print(digitalRead(JoySwitch_Onboard)); Serial.print("\t");
      Serial.print("JoySwitch_Main = "); Serial.print(analogRead(JoySwitch_Main)); Serial.print("\t");
      Serial.print("PowerLevelPotInput = "); Serial.print(analogRead(PowerLevelPotInput)); Serial.print("\t");
      Serial.print("\n");
  }
}
