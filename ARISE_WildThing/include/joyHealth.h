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

#pragma once

#ifndef joyHealth_H
#define joyHealth_H

Filter filtJoyOx(a2dMid); // filter for JoyOcc X w/ init at middle of A2D range
Filter filtJoyOy(a2dMid); // filter for JoyOcc Y w/ init at middle of A2D range
Filter filtJoyTx(a2dMid); // filter for JoyTether X w/ init at middle of A2D range
Filter filtJoyTy(a2dMid); // filter for JoyTether Y w/ init at middle of A2D range

Filter filtJoyOxRef(a2dMid); // filter for JoyOcc X w/ init at middle of A2D range to detect noise
Filter filtJoyOyRef(a2dMid); // filter for JoyOcc Y w/ init at middle of A2D range to detect noise
Filter filtJoyTxRef(a2dMid); // filter for JoyTether X w/ init at middle of A2D range to detect noise
Filter filtJoyTyRef(a2dMid); // filter for JoyTether Y w/ init at middle of A2D range to detect noise

unsigned long isNotMoving_ms = 200; // ms to debounce if josyticks are !moving so we can auto learn center 
Debounce isReady2Learn_O(isNotMoving_ms); // define Debounce class for if Occupant joystick is moving
Debounce isReady2Learn_T(isNotMoving_ms); // define Debounce class for if Tether joystick is moving

extern float joyMaxRate; // rate limit applied to joystick x and y inputs (A2D Units)
extern float joyFiltPct; // first order filter factor for joystick x and y inputs (0.0001 is slow, 0.1 is fast, 1 is no filter)
extern float joyMaxRate_Ref; // rate limit applied to joystick x and y inputs to detect noise (A2D Units)
extern float joyFiltPct_Ref; // first order filter factor for joystick x and y inputs to detect noise (0.0001 is slow, 0.1 is fast, 1 is no filter)
extern float joyLearnPct; // first order filter factor for learning joystick center
extern int joyLearnOKCount; // number of learn cycles we need before we can use the joystick input

// Joystick FaultHandling
extern float joyMaxDelta; // threshold pos A2D from Ref to detect noise
extern float joyIncGood; // Increment Health if all OK
extern float joyDecNoise; // Decrement Health if noise detected
extern float joyDecOOR; // Decrement Health if OutOfRange (disconnected)

// Assemble the joyInput Structure (bottom to top)
// contents: joyInputs.joyAxis.joyData
struct joyData {
  float pos; // relative position after learn - used in actual motor controls (filt - center)
  float raw; // latest measured value
  float filt; // slow filtered raw value (used in future calculation for "pos" which goes to motor control)
  float ref; // fast filtered raw value (used for diagnostic detection)
  float center; // calculated center position (averaged when not moving and radius is close to center) initialized at middle of A2D
  float health; // health measure (0 to 1000) 1000 if all good, 0 if disconnected, decremented if noise
};
  struct joyAxis {
    struct joyData x; // x axis input
    struct joyData y; // y axis input
    int learnCount = 0; // number of times the centering routine has run on this input; value is saturated at 1000 and reset to 0 when fault occurs
    float r; // calculated radius (after learning)
    float a; // calculated input angle (after learning)
    float status; // 1=good to use (learned/initialized), 0= not initialized, -1= bad connection (ex. health <500)
    bool isOK2Learn; // flag to be able to trigger auto learn of center (ie. close to center and not moving for debounced time)
  };
  struct joyInputs {
    struct joyAxis O; //Occupant
    struct joyAxis T; //Tether
  };

  void joyHealth(joyInputs &ji);

#endif