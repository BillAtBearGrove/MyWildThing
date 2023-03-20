void getPolarCoordinates() {
  // code adapted from: https://forum.arduino.cc/index.php?topic=609064.0
  float maxRadius = 400; // maximum assumed radius of joystick
  float deadZone = 3; // A2D zone near center to assume no motion

  #include <math.h>

  float deltaXpercent = deltaX/maxRadius;
  float deltaYpercent = deltaY/maxRadius;
  
  // to calculate the distance from the center (and, therefore, resulting force of motors) we use classic Pythagoras equation:
  joyRadius = sqrt(deltaXpercent*deltaXpercent + deltaYpercent*deltaYpercent); // use this silly ^2 multiplication, as Arduino's pow() function is prone to errors
  //joyRadius = (abs(deltaX) + abs(deltaY))/2 / maxRadius; // use this silly ^2 multiplication, as Arduino's pow() function is prone to errors
  joyRadius = min( 1,max(-1,joyRadius));
  
  double rad;
  if (deltaY == 0) {
    if (deltaX >= 0) { rad = 1.570796326795;} else { rad = -1.570796326795;} // Beware of horizontal (deltaY ==0) angles as those may result in division by zero!
  } else {
    rad = atan2 (deltaX, deltaY);
  }
  
  if (joyRadius <= deadZone / maxRadius) { joyAngle = 0; } else {
      
    // For conversion to degrees you use the following formula:
    joyAngle = rad * 57.295779513082320876798154814105; // we assume that a radian is 57.29 (and a bit) degrees.
    
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
  }

/*
    Serial.print("deltaX = "); Serial.print(deltaX); Serial.print("\t");
    Serial.print("deltaY = "); Serial.print(deltaY); Serial.print("\t");
    Serial.print("joyAngle = "); Serial.print(joyAngle); Serial.print("\t");
    Serial.print("joyRadius = "); Serial.print(joyRadius); Serial.print("\t");
    Serial.println("");
*/
  
}
