
void readPot() {

  // sets the PWM input to a 100% duty cycle of 5v
    switch (motorDriverType) {
      case 1: case 2:
        analogWrite(PowerLevelSupply, 255);
      break;
      case 3:
        // do nothing
      break;
    }

  // reads potentiometer output and rescales
  double potValue = analogRead(PowerLevelPotInput);
  // rescales the potentiometer output to get a speed multiplier
      //Serial.print("potValue = "); Serial.print(potValue); Serial.print("\t");
      //Serial.print("\n");
  speedMultiplier = rescale(potValue, 540, 950, .35, .85);
      //Serial.print("speedMultiplier = "); Serial.print(speedMultiplier); Serial.print("\t");
      //Serial.print("\n");
  speedMultiplier = speedMultiplier * speedMultiplier;
  // return speedMultiplier;
}
