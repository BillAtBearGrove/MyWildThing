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

#include "../include/init.h"
#include "../include/Config.h"
#include "../include/globals.h"
#include "../include/setMotorOutputs_VNH7070AY.h"
#include "../include/miscFunctions.h"
#include "../include/Filter.h"

#include "../include/algorithm.h" // For min/max

#include <Arduino.h> // For serial print

Filter LmA_filt = Filter(0.0);   // filter for left motor current - init 0
Filter RmA_filt = Filter(0.0);   // filter for right motor current - init 0
Filter LmPID_filt = Filter(0.0); // filter for left motor PID
Filter RmPID_filt = Filter(0.0); // filter for right motor PID

void setMotorOutputs_VNH7070AY(float motorReqL, float motorReqR){

  // Motor Current And Speed Calcs
    float Ke = 1.2; // 1/Kt
    float Ra = 1.6;
    float Lv = motorReqL*10000; // random scale to start discussions
    float Rv = motorReqR*10000; // random scale to start discussions

    // measure and filter current both motors
    float LmA = LmA_filt.firstOrdFiltx4(md.getM1Multisense(), mA_Pct); // Left Current mA
    float RmA = RmA_filt.firstOrdFiltx4(md.getM2Multisense(), mA_Pct); // Right Current mA

    // calculate speed
    float Lspeed = (Lv - (LmA *( (Lv>=0)? Ra:-Ra) ) ) * Ke; // Left calculated speed
    float Rspeed = (Rv - (RmA *( (Rv>=0)? Ra:-Ra) ) ) * Ke; // Right calculated speed

    float KpL, KpR;
    if (Lv >= 0) { KpL = ((Lv - Lspeed)>0)? Kp:Kp_minus;
    } else { KpL = ((Lv - Lspeed)>0)? Kp_minus:Kp;
    }
    if (Rv >= 0) { KpR = ((Rv - Rspeed)>0)? Kp:Kp_minus;
    } else { KpR = ((Rv - Rspeed)>0)? Kp_minus:Kp;
    }

    float PIDcorrL = LmPID_filt.rateLimitedFOF(std::max(-1.0F-motorReqL, std::min(1.0F-motorReqL, (float)(KpL * (Lv - Lspeed)/1000.0))), PID_Pct, PID_Rate); // simple gain error controller (Kp only)
    float PIDcorrR = RmPID_filt.rateLimitedFOF(std::max(-1.0F-motorReqR, std::min(1.0F-motorReqR, (float)(KpR * (Rv - Rspeed)/1000.0))), PID_Pct, PID_Rate); // simple gain error controller (Kp only)

    float motorReqL_wPID = motorReqL + ( (usePID)? PIDcorrL:0 ); // final requested output to motor
    float motorReqR_wPID = motorReqR + ( (usePID)? PIDcorrR:0 ); // final requested output to motor

  // Motor Braking and Driving
  float brakeSlope = (maxBrake - minBrake) / brakeZone; // slope of brake line from maxBrake at 0  to minBrake at motorZone
  float pwmSlope = (1 - motorDropout) / (1-brakeZone); // slope of pwm line
  float motorLdir, motorLpwm, motorRdir, motorRpwm;

    if (abs(motorReqL_wPID)<=brakeZone){
      // Reset motor
      motorLpwm = 0; //reset pwm level if applying brake
      // Drive Brake
      if (BrakeL < maxBrake) {// Braking function (init maxBrake, where maxBrake is latched until motorOut > brakeZone, otherwise scaled from maxBrake to minBrake)
        BrakeL = minBrake + ((brakeZone - abs(motorReqL_wPID)) * brakeSlope); // amount to brake  (0 to 400)
      }
      md.setM1Brake(BrakeL); // set brake pwm
    } else {
      // Reset Brake
      BrakeL = 0; //reset brake level if driving motor
      // Drive Motor
      motorLdir = (motorReqL_wPID>=0)? 1:-1;
      float motorLpwm_ = (motorDropout + pwmSlope * (abs(motorReqL_wPID) - brakeZone) ) * motorLdir;
      motorLpwm = rescale(motorLpwm_, -1, 1, -95, 95);
      md.setM1Speed( motorLpwm ); // set motor drive pwm
    }

    if (abs(motorReqR_wPID)<=brakeZone){
      // Reset motor
      motorRpwm = 0; //reset pwm level if applying brake
      // Drive Brake
      if (BrakeR < maxBrake) {// Braking function (init maxBrake, where maxBrake is latched until motorOut > brakeZone, otherwise scaled from maxBrake to minBrake)
        BrakeR = minBrake + ((brakeZone - abs(motorReqR_wPID)) * brakeSlope); // amount to brake  (0 to 400)
      }
      md.setM2Brake(BrakeR); // set brake pwm
    } else {
      // Reset Brake
      BrakeR = 0; //reset brake level if driving motor
      // Drive Motor
      motorRdir = (motorReqR_wPID>=0)? 1:-1;
      float motorRpwm_ = (motorDropout + pwmSlope * (abs(motorReqR_wPID) - brakeZone) ) * motorRdir;
      motorRpwm = rescale(motorRpwm_, -1, 1, -95, 95);
      md.setM2Speed( motorRpwm ); // set motor drive pwm
    }

    if ( currentTime - lastDebugTime >= DEBUG_PERIOD) {
      //Serial.print("LmA = "); Serial.print(LmA); Serial.print("\t");
      //Serial.print("Lsp "); Serial.print(Lspeed); Serial.print(" ");
      //Serial.print("Lv "); Serial.print(Lv); Serial.print(" ");
      // Serial.print("Lerr "); Serial.print(Lv-Lspeed); Serial.print(" ");
      // Serial.print("motorReqL "); Serial.print(motorReqL*1000); Serial.print(" ");
      // Serial.print("Lpidx10k "); Serial.print(PIDcorrL*1000); Serial.print(" ");
      // Serial.print("motorLpwm "); Serial.print(motorLpwm); Serial.print(" ");
      // Serial.println("");
    }

}
