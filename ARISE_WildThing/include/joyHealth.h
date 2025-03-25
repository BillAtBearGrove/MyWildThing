#ifndef joyProcessing_H
#define joyProcessing_H

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

  float joyMaxRate = (a2dMax * 0.01); // rate limit applied to joystick x and y inputs (A2D Units)
  float joyFiltPct = 0.05; // first order filter factor for joystick x and y inputs (0.0001 is slow, 0.1 is fast, 1 is no filter)
  float joyMaxRate_Ref = (a2dMax * 0.08); // rate limit applied to joystick x and y inputs to detect noise (A2D Units)
  float joyFiltPct_Ref = 0.25; // first order filter factor for joystick x and y inputs to detect noise (0.0001 is slow, 0.1 is fast, 1 is no filter)
  float joyLearnPct = 0.01; // first order filter factor for learning joystick center
  int joyLearnOKCount = 100; // number of learn cycles we need before we can use the joystick input

  // Joystick FaultHandling
  float joyMaxDelta  = (a2dMax * 0.03); // threshold pos A2D from Ref to detect noise
  float joyIncGood = 10; // Increment Health if all OK
  float joyDecNoise = -20; // Decrement Health if noise detected
  float joyDecOOR = -50; // Decrement Health if OutOfRange (disconnected)

  // Assemble the joyInput Structure (botton to top)
  // contents: joyInput.joyAxis.joyData
  struct joyData {
    float pos; // relative position after learn - used in actual motor controls (filt - center)
    float raw; // latest measured value
    float filt; // slow filtered raw value (used in future calculation for "pos" which goes to motor control)
    float ref; // fast filtered raw value (used for diagnostic detection)
    float center = a2dMid; // calculated center position (averaged when not moving and radius is close to center) initialized at middle of A2D
    float health = 0; // health measure (0 to 1000) 1000 if all good, 0 if disconnected, decremented if noise
  };
  struct joyAxis {
    struct joyData x; // x axis input
    struct joyData y; // y axis input
    int learnCount = 0; // number of times the centering routine has run on this input; value is saturated at 1000 and reset to 0 when fault occurs
    float r; // calculated radius (after learning)
    float a; // calculated input angle (after learning)
    float status = 0; // 1=good to use (learned/initialized), 0= not initialized, -1= bad connection (ex. health <500)
    bool isOK2Learn = false; // flag to be able to trigger auto learn of center (ie. close to center and not moving for debounced time)
  };
  struct joyInput {
    struct joyAxis O; // Occupant
    struct joyAxis T; // Tether
  };

#endif