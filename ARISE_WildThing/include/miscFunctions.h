// Miscellaneous functions

//#include "driver/adc.h"
#include "DualVNH7070AYESP32Driver.h" 

void getInputs();
float interpolate(float x, float table[][2], int numRows);
void rateLimitMotors();
float readPot(adc1_channel_t pinNum);
float rescale(float x, float x1, float x2, float y1, float y2);
void setJoystickOutOfRangeError();
void setJoystickLearnError();