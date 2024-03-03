polarCoord getPolarCoordinates(float thisX, float thisY) {
  #include <math.h>
  float maxRadius = 400; // maximum assumed radius of joystick (in A2D units, example 400 leaves 112 A2D units for diagnostics (512-400)
  float centerZone = 1; // A2D zone near center to assume no motion
  float posXpercent = thisX/maxRadius;
  float posYpercent = thisY/maxRadius;
  double AngleRadians_; // Angle in Radians
  float Radius_, Angle_;
  
  // to calculate the distance from the center (and, therefore, resulting force of motors) we use Pythagoras equation:
  Radius_ = sqrt(posXpercent*posXpercent + posYpercent*posYpercent); // use this silly ^2 multiplication, as Arduino's pow() function is prone to errors
  Radius_ = min( 1,max(0,Radius_));
  
  if (posYpercent == 0) {
    if (posXpercent >= 0) { AngleRadians_ = 1.570796326795;} else { AngleRadians_ = -1.570796326795;} // Beware of horizontal (posYpercent ==0) angles as those may result in division by zero!
  } else {
    AngleRadians_ = atan2 (posXpercent, posYpercent);
  }
  
  if (Radius_ <= centerZone / maxRadius) {
    Angle_ = 0;
    } else {
    Angle_ = AngleRadians_ * 57.3; // we assume that 1 radian = 57.29 (and a bit) degrees.
  }  

  struct polarCoord PolarCoord_;
  PolarCoord_.Radius = Radius_;
  PolarCoord_.Angle = Angle_;

  return PolarCoord_;
}
