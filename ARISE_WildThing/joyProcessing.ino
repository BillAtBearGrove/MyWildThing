// measure and filter joystick inputs
// return filtered joystick x and y values plus health status
  #include "joyProcessing.h"
  #include "src/Debounce.h"
  #include "src/Filter.h"

struct joyInput joyProcessing() {
    struct joyInput joyInputs_;
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
    joyInputs_.O.x.health = max(0, min(1000, joyInputs_.O.x.health + timestep*((joyInputs_.O.x.raw - joyInputs_.O.x.ref < joyMaxpos) ? joyIncGood : joyDecNoise) ));
    joyInputs_.O.y.health = max(0, min(1000, joyInputs_.O.y.health + timestep*((joyInputs_.O.y.raw - joyInputs_.O.y.ref < joyMaxpos) ? joyIncGood : joyDecNoise) ));
    joyInputs_.T.x.health = max(0, min(1000, joyInputs_.T.x.health + timestep*((joyInputs_.T.x.raw - joyInputs_.T.x.ref < joyMaxpos) ? joyIncGood : joyDecNoise) ));
    joyInputs_.T.y.health = max(0, min(1000, joyInputs_.T.y.health + timestep*((joyInputs_.T.y.raw - joyInputs_.T.y.ref < joyMaxpos) ? joyIncGood : joyDecNoise) ));

    // modify health based on OutOfRange (inc/decr if raw measure is outside diagnostic bands)
    joyInputs_.O.x.health = max(0, min(1000, joyInputs_.O.x.health + timestep*((joyInputs_.O.x.raw>joyFaultBand && joyInputs_.O.x.raw<1023-joyFaultBand) ? joyIncGood : joyDecOOR) ));
    joyInputs_.O.y.health = max(0, min(1000, joyInputs_.O.y.health + timestep*((joyInputs_.O.y.raw>joyFaultBand && joyInputs_.O.x.raw<1023-joyFaultBand) ? joyIncGood : joyDecOOR) ));
    joyInputs_.T.x.health = max(0, min(1000, joyInputs_.T.x.health + timestep*((joyInputs_.T.x.raw>joyFaultBand && joyInputs_.T.x.raw<1023-joyFaultBand) ? joyIncGood : joyDecOOR) ));
    joyInputs_.T.y.health = max(0, min(1000, joyInputs_.T.y.health + timestep*((joyInputs_.T.y.raw>joyFaultBand && joyInputs_.T.x.raw<1023-joyFaultBand) ? joyIncGood : joyDecOOR) ));

    // calc pos (distance from center)
    joyInputs_.O.x.pos = joyInputs_.O.x.filt - joyInputs_.O.x.center;
    joyInputs_.O.y.pos = joyInputs_.O.y.filt - joyInputs_.O.y.center;
    joyInputs_.T.x.pos = joyInputs_.T.x.filt - joyInputs_.T.x.center;
    joyInputs_.T.y.pos = joyInputs_.T.y.filt - joyInputs_.T.y.center;

    // calc Radius and Angle

    return joyInputs_;
}
