/*
****************************************************************
ARISE Adaptive Design
Project: WildThing ESP32 Port
Author: Bill Smith
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

#include "../include/joyHealth.h"
#include "../include/Debounce.h"
#include "../include/Config.h"
#include "../include/init.h"
#include "../include/pinouts.h"
#include "../include/Filter.h"
#include "../include/algorithm.h"

#include <Arduino.h>

Filter filtJoyOx(a2dMid); // filter for JoyOcc X w/ init at middle of A2D range
Filter filtJoyOy(a2dMid); // filter for JoyOcc Y w/ init at middle of A2D range
Filter filtJoyTx(a2dMid); // filter for JoyTether X w/ init at middle of A2D range
Filter filtJoyTy(a2dMid); // filter for JoyTether Y w/ init at middle of A2D range

Filter filtJoyOxRef(a2dMid); // filter for JoyOcc X w/ init at middle of A2D range to detect noise
Filter filtJoyOyRef(a2dMid); // filter for JoyOcc Y w/ init at middle of A2D range to detect noise
Filter filtJoyTxRef(a2dMid); // filter for JoyTether X w/ init at middle of A2D range to detect noise
Filter filtJoyTyRef(a2dMid); // filter for JoyTether Y w/ init at middle of A2D range to detect noise

float joyMaxRate = (a2dMax * 0.01); // rate limit applied to joystick x and y inputs (A2D Units)
float joyFiltPct = 0.05; // first order filter factor for joystick x and y inputs (0.0001 is slow, 0.1 is fast, 1 is no filter)
float joyMaxRate_Ref = (a2dMax * 0.08); // rate limit applied to joystick x and y inputs to detect noise (A2D Units)
float joyFiltPct_Ref = 0.25; // first order filter factor for joystick x and y inputs to detect noise (0.0001 is slow, 0.1 is fast, 1 is no filter)

// Joystick FaultHandling
float joyMaxDelta = (a2dMax * 0.03); // threshold delta A2D from Ref to detect noise
float joyIncGood = 0.2; // Increment Health if all OK
float joyDecNoise = -30; // Decrement Health if noise detected
float joyDecOOR = -50; // Decrement Health if OutOfRange (disconnected)

void joyHealth(joyInputs &ji) {

    // << Read the raw Joystick X and Y positions >>
    ji.O.x.raw = analogRead(joyX_Occupant);
    ji.O.y.raw = analogRead(joyY_Occupant);
    ji.T.x.raw = analogRead(joyX_Tether);
    ji.T.y.raw = analogRead(joyY_Tether);

    // filter joy inputs for noise detect
    ji.O.x.ref = filtJoyOxRef.rateLimitedFOF(ji.O.x.raw, joyFiltPct_Ref, joyMaxRate_Ref);
    ji.O.y.ref = filtJoyOyRef.rateLimitedFOF(ji.O.y.raw, joyFiltPct_Ref, joyMaxRate_Ref);
    ji.T.x.ref = filtJoyTxRef.rateLimitedFOF(ji.T.x.raw, joyFiltPct_Ref, joyMaxRate_Ref);
    ji.T.y.ref = filtJoyTyRef.rateLimitedFOF(ji.T.y.raw, joyFiltPct_Ref, joyMaxRate_Ref);

    // filter joy inputs for usage
    ji.O.x.filt = filtJoyOx.firstOrdFiltx4(ji.O.x.raw, joyFiltPct);
    ji.O.y.filt = filtJoyOy.firstOrdFiltx4(ji.O.y.raw, joyFiltPct);
    ji.T.x.filt = filtJoyTx.firstOrdFiltx4(ji.T.x.raw, joyFiltPct);
    ji.T.y.filt = filtJoyTy.firstOrdFiltx4(ji.T.y.raw, joyFiltPct);

    // calc delta (raw measured vs ref expected)
    ji.O.x.delta = ji.O.x.raw - ji.O.x.ref;
    ji.O.y.delta = ji.O.y.raw - ji.O.y.ref;
    ji.T.x.delta = ji.T.x.raw - ji.T.x.ref;
    ji.T.y.delta = ji.T.y.raw - ji.T.y.ref;

    // modify health based on noise (inc/decr based on "large" deltas)
    ji.O.x.health = std::max(0.0F, std::min(1000.0F, ji.O.x.health + timestep*((ji.O.x.delta<joyMaxDelta) ? joyIncGood : joyDecNoise) ));
    ji.O.y.health = std::max(0.0F, std::min(1000.0F, ji.O.y.health + timestep*((ji.O.y.delta<joyMaxDelta) ? joyIncGood : joyDecNoise) ));
    ji.T.x.health = std::max(0.0F, std::min(1000.0F, ji.T.x.health + timestep*((ji.T.x.delta<joyMaxDelta) ? joyIncGood : joyDecNoise) ));
    ji.T.y.health = std::max(0.0F, std::min(1000.0F, ji.T.y.health + timestep*((ji.T.y.delta<joyMaxDelta) ? joyIncGood : joyDecNoise) ));

    // modify health based on OutOfRange (inc/decr if raw measure is outside diagnostic bands)
    ji.O.x.health = std::max(0.0F, std::min(1000.0F, ji.O.x.health + timestep*((ji.O.x.raw>joyFaultBand && ji.O.x.raw<(a2dMax-joyFaultBand)) ? joyIncGood : joyDecOOR) ));
    ji.O.y.health = std::max(0.0F, std::min(1000.0F, ji.O.y.health + timestep*((ji.O.y.raw>joyFaultBand && ji.O.x.raw<(a2dMax-joyFaultBand)) ? joyIncGood : joyDecOOR) ));
    ji.T.x.health = std::max(0.0F, std::min(1000.0F, ji.T.x.health + timestep*((ji.T.x.raw>joyFaultBand && ji.T.x.raw<(a2dMax-joyFaultBand)) ? joyIncGood : joyDecOOR) ));
    ji.T.y.health = std::max(0.0F, std::min(1000.0F, ji.T.y.health + timestep*((ji.T.y.raw>joyFaultBand && ji.T.x.raw<(a2dMax-joyFaultBand)) ? joyIncGood : joyDecOOR) ));

    // determine if isOK2Learn based on close to center and not moving
    float thisX, thisY;
    // Occupant
      thisX = ji.O.x.filt;
      thisY = ji.O.y.filt;
      if ( thisX >= joyPosRestingMin && thisX <= joyPosRestingMax && thisY >= joyPosRestingMin && thisY <= joyPosRestingMax && abs(ji.O.x.raw - thisX) <= joyPosRestingTol && abs(ji.O.y.raw - thisY) <= joyPosRestingTol) {
        ji.O.isOK2Learn = isReady2Learn_O.debounceHigh(true);
      } else {
        ji.O.isOK2Learn = isReady2Learn_O.debounceHigh(false);
      }
    // Tether
      thisX = ji.T.x.filt;
      thisY = ji.T.y.filt;
      if ( thisX >= joyPosRestingMin && thisX <= joyPosRestingMax && thisY >= joyPosRestingMin && thisY <= joyPosRestingMax && abs(ji.T.x.raw - thisX) <= joyPosRestingTol && abs(ji.T.y.raw - thisY) <= joyPosRestingTol) {
        ji.T.isOK2Learn = isReady2Learn_T.debounceHigh(true);
      } else {
        ji.T.isOK2Learn = isReady2Learn_T.debounceHigh(false);
      }

    // Calculate Center and increment learnCount if isOK2Learn
      if (ji.O.isOK2Learn) {
        ji.O.x.center = ji.O.x.center + joyLearnPct * ( ji.O.x.filt - ji.O.x.center);
        ji.O.y.center = ji.O.y.center + joyLearnPct * ( ji.O.y.filt - ji.O.y.center);
        if (ji.O.learnCount <= joyLearnOKCount) { ji.O.learnCount +=1;} // stop counting after 9999
      }
      if (ji.T.isOK2Learn) {
        ji.T.x.center = ji.T.x.center + joyLearnPct * ( ji.T.x.filt - ji.T.x.center);
        ji.T.y.center = ji.T.y.center + joyLearnPct * ( ji.T.y.filt - ji.T.y.center);
        if (ji.T.learnCount <= joyLearnOKCount) { ji.T.learnCount +=1;} // stop counting after 9999
      }

    // calc pos (distance from center)
      ji.O.x.pos = ji.O.x.filt - ji.O.x.center;
      ji.O.y.pos = ji.O.y.filt - ji.O.y.center;
      ji.T.x.pos = ji.T.x.filt - ji.T.x.center;
      ji.T.y.pos = ji.T.y.filt - ji.T.y.center;

    //  set status as 1=good to use (learned/initialized), 0= not initialized, -1 = bad connection (ex. health <500)
      if (ji.O.x.health > 500 && ji.O.y.health > 500) {
        if (ji.O.learnCount >= joyLearnOKCount) {
          ji.O.status = 1; // if healthy and learned then OK to use
        } else {
          ji.O.status = 0; // if healthy but not learned then not OK to use
        }
      } else {
        ji.O.status = -1; // if not healthy then error
        ji.O.learnCount = 0; // reset learn count if errored
      }
      if (ji.T.x.health > 500 && ji.T.y.health > 500) {
        if (ji.T.learnCount >= joyLearnOKCount) {
          ji.T.status = 1; // if healthy and learned then OK to use
        } else {
          ji.T.status = 0; // if healthy but not learned then not OK to use
        }
      } else {
        ji.T.status = -1; // if not healthy then error
        ji.T.learnCount = 0; // reset learn count if errored
      }

    // calc Radius and Angle
      if (ji.O.status == 1) {
        struct polarCoord polarCoord_O = getPolarCoordinates(ji.O.x.pos, ji.O.y.pos);
        ji.O.r = polarCoord_O.Radius;
        ji.O.a = polarCoord_O.Angle;
      } else {
        ji.O.r = 0;
        ji.O.a = 0;
      }
      if (ji.T.status == 1) {
        struct polarCoord polarCoord_T = getPolarCoordinates(ji.T.x.pos, ji.T.y.pos);
        ji.T.r = polarCoord_T.Radius;
        ji.T.a = polarCoord_T.Angle;
      } else {
        ji.T.r = 0;
        ji.T.a = 0;
      }
    
    return ji;
}
