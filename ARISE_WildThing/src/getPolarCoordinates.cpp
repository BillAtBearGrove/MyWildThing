polarCoord getPolarCoordinates(float thisX, float thisY) {
  #include <math.h>
  float maxRadius = (a2dMax * 0.4); // (A2D) maximum assumed radius of joystick
  float centerZone = (a2dMax * 0.001); // (A2D) zone near center to assume no motion
  float posXpercent = thisX/maxRadius;
  float posYpercent = thisY/maxRadius;
  double angleRadians; // Angle in Radians
  float radius, angle;
  
  // to calculate the distance from the center (and, therefore, resulting force of motors) we use Pythagoras equation:
  radius = sqrt(posXpercent*posXpercent + posYpercent*posYpercent); // use this silly ^2 multiplication, as Arduino's pow() function is prone to errors
  radius = min( 1,max(0,radius));
  
  if (posYpercent == 0) {
    if (posXpercent >= 0) { angleRadians = 1.570796326795;} else { angleRadians = -1.570796326795;} // Beware of horizontal (posYpercent ==0) angles as those may result in division by zero!
  } else {
    angleRadians = atan2 (posXpercent, posYpercent);
  }
  
  if (radius <= centerZone / maxRadius) {
    angle = 0;
    } else {
    angle = angleRadians * 57.3; // we assume that 1 radian = 57.29 (and a bit) degrees.
  }

  // protect for angle outside of 0 to 360
  if (angle < 0){
    angle = 360 + angle;
  }
  if (angle > 360) {
    angle = angle - 360;
  }

  struct polarCoord PolarCoord_;
  PolarCoord_.Radius = radius;
  PolarCoord_.Angle = angle;

  return PolarCoord_;
}
