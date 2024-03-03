// measure and filter joystick inputs
// return filtered joystick x and y values plus health status
  #include "joyProcessing.h"
  #include "src/Debounce.h"
  #include "src/Filter.h"
/*
struct joyInput joyInit() {
    struct joyInput joyInputs_;
    joyInputs_.O.x.center = 512;
    joyInputs_.O.y.center = 512;
    joyInputs_.T.x.health = 0;
    joyInputs_.T.y.health = 0;
    
    joyInputs_.O.learnCount = 0;
    joyInputs_.T.learnCount = 0;

}
*/

struct joyInput joyProcessing(struct joyInput joyInputs_) {
    //struct joyInput joyInputs_;
    // << Read the raw Joystick X and Y positions >>
    joyInputs_.O.x.raw = analogRead(joyX_Occupant);
    joyInputs_.O.y.raw = analogRead(joyY_Occupant);
    joyInputs_.T.x.raw = analogRead(joyX_Tether);
    joyInputs_.T.y.raw = analogRead(joyY_Tether);

    // filter joy inputs for noise detect
    joyInputs_.O.x.ref = filtJoyOxRef.rateLimitedFOF(joyInputs_.O.x.raw, joyFiltPct_Ref, joyMaxRate_Ref);
    joyInputs_.O.y.ref = filtJoyOyRef.rateLimitedFOF(joyInputs_.O.y.raw, joyFiltPct_Ref, joyMaxRate_Ref);
    joyInputs_.T.x.ref = filtJoyTxRef.rateLimitedFOF(joyInputs_.T.x.raw, joyFiltPct_Ref, joyMaxRate_Ref);
    joyInputs_.T.y.ref = filtJoyTyRef.rateLimitedFOF(joyInputs_.T.y.raw, joyFiltPct_Ref, joyMaxRate_Ref);

    // filter joy inputs for usage
    joyInputs_.O.x.filt = filtJoyOx.firstOrdFiltx4(joyInputs_.O.x.raw, joyFiltPct);
    joyInputs_.O.y.filt = filtJoyOy.firstOrdFiltx4(joyInputs_.O.y.raw, joyFiltPct);
    joyInputs_.T.x.filt = filtJoyTx.firstOrdFiltx4(joyInputs_.T.x.raw, joyFiltPct);
    joyInputs_.T.y.filt = filtJoyTy.firstOrdFiltx4(joyInputs_.T.y.raw, joyFiltPct);

    // modify health based on noise (inc/decr based on "large" change)
    joyInputs_.O.x.health = max(0, min(1000, joyInputs_.O.x.health + ( abs(joyInputs_.O.x.raw - joyInputs_.O.x.ref) < joyMaxpos ? joyIncGood : joyDecNoise ) ));
    joyInputs_.O.y.health = max(0, min(1000, joyInputs_.O.y.health + ( abs(joyInputs_.O.y.raw - joyInputs_.O.y.ref) < joyMaxpos ? joyIncGood : joyDecNoise ) ));
    joyInputs_.T.x.health = max(0, min(1000, joyInputs_.T.x.health + ( abs(joyInputs_.T.x.raw - joyInputs_.T.x.ref) < joyMaxpos ? joyIncGood : joyDecNoise ) ));
    joyInputs_.T.y.health = max(0, min(1000, joyInputs_.T.y.health + ( abs(joyInputs_.T.y.raw - joyInputs_.T.y.ref) < joyMaxpos ? joyIncGood : joyDecNoise ) ));

    // modify health based on OutOfRange (inc/decr if raw measure is outside diagnostic bands)
    joyInputs_.O.x.health = max(0, min(1000, joyInputs_.O.x.health + ( (joyInputs_.O.x.raw>joyFaultBand && joyInputs_.O.x.raw<1023-joyFaultBand) ? joyIncGood : joyDecOOR ) ));
    joyInputs_.O.y.health = max(0, min(1000, joyInputs_.O.y.health + ( (joyInputs_.O.y.raw>joyFaultBand && joyInputs_.O.x.raw<1023-joyFaultBand) ? joyIncGood : joyDecOOR ) ));
    joyInputs_.T.x.health = max(0, min(1000, joyInputs_.T.x.health + ( (joyInputs_.T.x.raw>joyFaultBand && joyInputs_.T.x.raw<1023-joyFaultBand) ? joyIncGood : joyDecOOR ) ));
    joyInputs_.T.y.health = max(0, min(1000, joyInputs_.T.y.health + ( (joyInputs_.T.y.raw>joyFaultBand && joyInputs_.T.x.raw<1023-joyFaultBand) ? joyIncGood : joyDecOOR ) ));

    // determine if isOK2Learn based on close to center and not moving
    float thisX, thisY;
    // Occupant
      thisX = joyInputs_.O.x.filt;
      thisY = joyInputs_.O.y.filt;
      if ( thisX >= joyPosRestingMin && thisX <= joyPosRestingMax && thisY >= joyPosRestingMin && thisY <= joyPosRestingMax && abs(joyInputs_.O.x.raw - thisX) <= joyPosRestingTol && abs(joyInputs_.O.y.raw - thisY) <= joyPosRestingTol) {
        joyInputs_.O.isOK2Learn = isReady2Learn_O.debounceHigh(true);
      } else {
        joyInputs_.O.isOK2Learn = isReady2Learn_O.debounceHigh(false);
      }
    // Tether
      thisX = joyInputs_.T.x.filt;
      thisY = joyInputs_.T.y.filt;
      if ( thisX >= joyPosRestingMin && thisX <= joyPosRestingMax && thisY >= joyPosRestingMin && thisY <= joyPosRestingMax && abs(joyInputs_.T.x.raw - thisX) <= joyPosRestingTol && abs(joyInputs_.T.y.raw - thisY) <= joyPosRestingTol) {
        joyInputs_.T.isOK2Learn = isReady2Learn_T.debounceHigh(true);
      } else {
        joyInputs_.T.isOK2Learn = isReady2Learn_T.debounceHigh(false);
      }

    // Calculate Center and increment learnCount if isOK2Learn
      if (joyInputs_.O.isOK2Learn) {
        joyInputs_.O.x.center = joyInputs_.O.x.center + joyLearnPct * ( joyInputs_.O.x.filt - joyInputs_.O.x.center);
        joyInputs_.O.y.center = joyInputs_.O.y.center + joyLearnPct * ( joyInputs_.O.y.filt - joyInputs_.O.y.center);
        if (joyInputs_.O.learnCount < 10000) { joyInputs_.O.learnCount +=1;} // stop counting after 9999
      }
      if (joyInputs_.T.isOK2Learn) {
        joyInputs_.T.x.center = joyInputs_.T.x.center + joyLearnPct * ( joyInputs_.T.x.filt - joyInputs_.T.x.center);
        joyInputs_.T.y.center = joyInputs_.T.y.center + joyLearnPct * ( joyInputs_.T.y.filt - joyInputs_.T.y.center);
        if (joyInputs_.T.learnCount < 10000) { joyInputs_.T.learnCount +=1;} // stop counting after 9999
      }

    // calc pos (distance from center)
      joyInputs_.O.x.pos = joyInputs_.O.x.filt - joyInputs_.O.x.center;
      joyInputs_.O.y.pos = joyInputs_.O.y.filt - joyInputs_.O.y.center;
      joyInputs_.T.x.pos = joyInputs_.T.x.filt - joyInputs_.T.x.center;
      joyInputs_.T.y.pos = joyInputs_.T.y.filt - joyInputs_.T.y.center;

    //  set status as 1=good to use (learned/initialized), 0= not initialized, -1 = bad connection (ex. health <500)
      if (joyInputs_.O.x.health > 500 && joyInputs_.O.y.health > 500) {
        if (joyInputs_.O.learnCount >= joyLearnOKCount) {
          joyInputs_.O.status = 1; // if healthy and learned then OK to use
        } else {
          joyInputs_.O.status = 0; // if healthy but not learned then not OK to use
        }
      } else {
        joyInputs_.O.status = -1; // if not healthy then error
        joyInputs_.O.learnCount = 0; // reset learn count if errored
      }
      if (joyInputs_.T.x.health > 500 && joyInputs_.T.y.health > 500) {
        if (joyInputs_.T.learnCount >= joyLearnOKCount) {
          joyInputs_.T.status = 1; // if healthy and learned then OK to use
        } else {
          joyInputs_.T.status = 0; // if healthy but not learned then not OK to use
        }
      } else {
        joyInputs_.T.status = -1; // if not healthy then error
        joyInputs_.T.learnCount = 0; // reset learn count if errored
      }

    // calc Radius and Angle
      if (joyInputs_.O.status == 1) {
        struct polarCoord polarCoord_O = getPolarCoordinates(joyInputs_.O.x.pos, joyInputs_.O.y.pos);
        joyInputs_.O.r = polarCoord_O.Radius;
        joyInputs_.O.a = polarCoord_O.Angle;
      } else {
        joyInputs_.O.r = 0;
        joyInputs_.O.a = 0;
      }
      if (joyInputs_.T.status == 1) {
        struct polarCoord polarCoord_T = getPolarCoordinates(joyInputs_.T.x.pos, joyInputs_.T.y.pos);
        joyInputs_.T.r = polarCoord_T.Radius;
        joyInputs_.T.a = polarCoord_T.Angle;
      } else {
        joyInputs_.T.r = 0;
        joyInputs_.T.a = 0;
      }
    
/* // TODO Maybe get rid of this or make it useful for each joystick
    // adjust joyAngle for joystick installed orientation
    joyAngle = joyAngle + joyOffsetAngle;
    // avoid overrun (joyAngle always should be between 0 and 360)
    if (joyAngle >= 360) {
      joyAngle = joyAngle - 360;
      } else {
        if (joyAngle < 0) {
          joyAngle = joyAngle + 360;
        }
      }
    // swap spin direction if required
    if (flipSpin) { joyAngle = map(joyAngle,0,360,360,0); } // reverse direction if flipSpin = true
*/

    return joyInputs_;
}
