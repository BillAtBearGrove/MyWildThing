/*
****************************************************************
ARISE Adaptive Design
Project: WildThing ESP32 Port
Author: Aaron Lim (unchanged code)
****************************************************************
*
Change Log:
-Changes Made. MM.DD.YYYY
--Initial codebase - 11.18.2024
****************************************************************
Notes:
-Note. MM.DD.YYYY
--Initial codebase - 11.18.2024
****************************************************************
*/

#include "../include/DualVNH7070AYESP32Driver.h"
#include "Wire.h"

// Constructors ////////////////////////////////////////////////////////////////

DualVNH7070AYESP32Driver::DualVNH7070AYESP32Driver()
{
  //Pin map
  _MCP_PORT_M1 = MCP23017Port::A;
  _DIRA1 = MCP23017Pin::Names::GPA0;
  _DIRB1 = MCP23017Pin::Names::GPA1;
  _PWM1  = GPIO_NUM_13;
  _SEL01 = MCP23017Pin::Names::GPA2;
  _SEL11 = MCP23017Pin::Names::GPA3;
  _MSEN1 = MCP23017Pin::Names::GPA4;
  _LEDG_M1 = MCP23017Pin::Names::GPA6;
  _LEDR_M1 = MCP23017Pin::Names::GPA7;
  _MS1 = ADC1_CHANNEL_8;
  _MCPWM_UNIT_M1 = MCPWM_UNIT_0;
  _MCPWM_TIMER_M1 = MCPWM_TIMER_0;
  _MCPWM_IO_M1 = MCPWM0A;
  _MCPWM_GEN_M1 = MCPWM_GEN_A;

  _MCP_PORT_M2 = MCP23017Port::B;
  _DIRA2 = MCP23017Pin::Names::GPB0;
  _DIRB2 = MCP23017Pin::Names::GPB1;
  _PWM2  = GPIO_NUM_14;
  _SEL02 = MCP23017Pin::Names::GPB2;
  _SEL12 = MCP23017Pin::Names::GPB3;
  _MSEN2 = MCP23017Pin::Names::GPB4;
  _LEDG_M2 = MCP23017Pin::Names::GPB6;
  _LEDR_M2 = MCP23017Pin::Names::GPB7;
  _MS2   = ADC1_CHANNEL_9;
  _MCPWM_UNIT_M2 = MCPWM_UNIT_0;
  _MCPWM_TIMER_M2 = MCPWM_TIMER_0;
  _MCPWM_IO_M2 = MCPWM0B;
  _MCPWM_GEN_M2 = MCPWM_GEN_B;
}

DualVNH7070AYESP32Driver::DualVNH7070AYESP32Driver(MCP23017Port MCP_PORT_M1,
                                                   MCP23017Pin::Names DIRA1,
                                                   MCP23017Pin::Names DIRB1,
                                                   gpio_num_t PWM1,
                                                   MCP23017Pin::Names SEL01,
                                                   MCP23017Pin::Names SEL11,
                                                   MCP23017Pin::Names MSEN1,
                                                   MCP23017Pin::Names LEDG_M1,
                                                   MCP23017Pin::Names LEDR_M1,
                                                   adc1_channel_t MS1,
                                                   mcpwm_unit_t MCPWM_UNIT_M1,
                                                   mcpwm_timer_t MCPWM_TIMER_M1,
                                                   mcpwm_io_signals_t MCPWM_IO_M1,
                                                   mcpwm_generator_t MCPWM_GEN_M1,
                                                   MCP23017Port MCP_PORT_M2,
                                                   MCP23017Pin::Names DIRA2,
                                                   MCP23017Pin::Names DIRB2,
                                                   gpio_num_t PWM2,
                                                   MCP23017Pin::Names SEL02,
                                                   MCP23017Pin::Names SEL12,
                                                   MCP23017Pin::Names MSEN2,
                                                   MCP23017Pin::Names LEDG_M2,
                                                   MCP23017Pin::Names LEDR_M2,
                                                   adc1_channel_t MS2,
                                                   mcpwm_unit_t MCPWM_UNIT_M2,
                                                   mcpwm_timer_t MCPWM_TIMER_M2,
                                                   mcpwm_io_signals_t MCPWM_IO_M2,
                                                   mcpwm_generator_t MCPWM_GEN_M2)
{
  _MCP_PORT_M1 = MCP_PORT_M1;
  _DIRA1  = DIRA1;
  _DIRB1  = DIRB1;
  _PWM1  = PWM1;
  _SEL01 = SEL01;
  _SEL11 = SEL11;
  _MSEN1 = MSEN1;
  _LEDG_M1 = LEDG_M1;
  _MS1   = MS1;
  _MCPWM_UNIT_M1 = MCPWM_UNIT_M1;
  _MCPWM_TIMER_M1 = MCPWM_TIMER_M1;
  _MCPWM_IO_M1 = MCPWM_IO_M1;
  _MCPWM_GEN_M1 = MCPWM_GEN_M1;

  _MCP_PORT_M2 = MCP_PORT_M2;
  _DIRA2  = DIRA2;
  _DIRB2  = DIRB2;
  _PWM2  = PWM2;
  _SEL02 = SEL02;
  _SEL12 = SEL12;
  _MSEN2 = MSEN2;
  _LEDG_M2 = LEDG_M2;
  _LEDR_M2 = LEDR_M2;
  _MS2 = MS2;
  _MCPWM_UNIT_M2 = MCPWM_UNIT_M2;
  _MCPWM_TIMER_M2 = MCPWM_TIMER_M2;
  _MCPWM_IO_M2 = MCPWM_IO_M2;
  _MCPWM_GEN_M2 = MCPWM_GEN_M2;
}

// Public Methods //////////////////////////////////////////////////////////////
void DualVNH7070AYESP32Driver::init()
{
// Define pin modes, set up MCPWM unit, and initialize ADCs

  gpio_config_t gp;

  // Configure the output pins
  uint64_t GPIO_OUTPUT_BITMASK = (1ULL<<_PWM1) | (1ULL<<_PWM2);
  gp.intr_type = GPIO_INTR_DISABLE;
  gp.mode = GPIO_MODE_OUTPUT;
  gp.pin_bit_mask = GPIO_OUTPUT_BITMASK;
  gpio_config(&gp);

  // Some output pins are part of the MCP23017 IO expander.
  // Need to set up comms with that expander.

  ioexp = MCP23017(); // Uses default I2C address of 0x20
  ioexp.begin();
  ioexp.portMode(_MCP_PORT_M1, 0); // set whole port to output
  ioexp.portMode(_MCP_PORT_M2, 0); // set whole port to output
  // Set motors to coast
  setSpeeds(0.0f, 0.0f);
  // Disable multisense by default
  enableM1Multisense(false);
  // Set the motor status LEDs
  setFaultStatus(false, false);

  // Configure the input pins and ADCs
  uint64_t GPIO_INPUT_BITMASK = (1ULL<<_MS1) | (1ULL<<_MS2);
  gp.mode = GPIO_MODE_INPUT;
  gp.pin_bit_mask = GPIO_INPUT_BITMASK;
  gpio_config(&gp);

  adc1_config_channel_atten(_MS1, VNH7070AY_ADC_ATTEN);
  adc1_config_channel_atten(_MS2, VNH7070AY_ADC_ATTEN);
  adc1_config_width(VNH7070AY_ADC_WIDTH); // applies to all channels

  // Assign PWM drivers to pins
  mcpwm_gpio_init(_MCPWM_UNIT_M1, _MCPWM_IO_M1, _PWM1);
  mcpwm_gpio_init(_MCPWM_UNIT_M2, _MCPWM_IO_M2, _PWM2);

  // Configure PWM timers
  pwm_config.frequency = VNH7070AY_PWM_FREQUENCY_HZ;
  pwm_config.cmpr_a = 0.0;
  pwm_config.cmpr_b = 0.0;
  pwm_config.counter_mode = MCPWM_UP_COUNTER;
  pwm_config.duty_mode = MCPWM_DUTY_MODE_0;
  mcpwm_init(_MCPWM_UNIT_M1, _MCPWM_TIMER_M1, &pwm_config);
  mcpwm_start(_MCPWM_UNIT_M1, _MCPWM_TIMER_M1);
  if ((_MCPWM_UNIT_M1 != _MCPWM_UNIT_M2) | (_MCPWM_TIMER_M1 != _MCPWM_TIMER_M2)) {
    mcpwm_init(_MCPWM_UNIT_M2, _MCPWM_TIMER_M2, &pwm_config);
    mcpwm_start(_MCPWM_UNIT_M2, _MCPWM_TIMER_M2);
  }

}

// Set speed for motor 1, speed is a number betwenn -100 to 100
void DualVNH7070AYESP32Driver::setM1Speed(float speed)
{
  unsigned char reverse = 0;

  if (speed < 0)
  {
    speed = -speed;  // Make speed a positive quantity
    reverse = 1;  // Preserve the direction
  }
  if (speed > 100)  // Max PWM dutycycle
    speed = 100;

  if (speed == 0)
  {
    // coast high to allow for multisense
    ioexp.digitalWrite(_DIRA1, 1);   // Make the motor coast no
    ioexp.digitalWrite(_DIRB1, 1);   // matter which direction it is spinning.
    ioexp.digitalWrite(_SEL01, 0);
    ioexp.digitalWrite(_SEL11, 0);
  }
  else if (reverse)
  {
    ioexp.digitalWrite(_DIRA1, 0);
    ioexp.digitalWrite(_DIRB1, 1);
    ioexp.digitalWrite(_SEL01, 0);
    ioexp.digitalWrite(_SEL11, 0);
  }
  else
  {
    ioexp.digitalWrite(_DIRA1, 1);
    ioexp.digitalWrite(_DIRB1, 0);
    ioexp.digitalWrite(_SEL01, 1);
    ioexp.digitalWrite(_SEL11, 0);
  }

  mcpwm_set_duty(_MCPWM_UNIT_M1, _MCPWM_TIMER_M1, _MCPWM_GEN_M1, speed);
}

// Set speed for motor 2, speed is a number betwenn -100 and 100
void DualVNH7070AYESP32Driver::setM2Speed(float speed)
{
  unsigned char reverse = 0;

  if (speed < 0)
  {
    speed = -speed;  // make speed a positive quantity
    reverse = 1;  // preserve the direction
  }
  if (speed > 100)  // Max
    speed = 100;

  if (speed == 0)
  {
    // Stop high so we can use multisense
    ioexp.digitalWrite(_DIRA2,1);   // Make the motor coast no
    ioexp.digitalWrite(_DIRB2,1);   // matter which direction it is spinning.
    ioexp.digitalWrite(_SEL02,0);   // multisense high side B = 00, A = 10
    ioexp.digitalWrite(_SEL12,0);
  }
  else if (reverse)
  {
    ioexp.digitalWrite(_DIRA2,0);
    ioexp.digitalWrite(_DIRB2,1);
    ioexp.digitalWrite(_SEL02,0);
    ioexp.digitalWrite(_SEL12,0);
  }
  else
  {
    ioexp.digitalWrite(_DIRA2,1);
    ioexp.digitalWrite(_DIRB2,0);
    ioexp.digitalWrite(_SEL02,1);
    ioexp.digitalWrite(_SEL12,0);
  }

  mcpwm_set_duty(_MCPWM_UNIT_M2, _MCPWM_TIMER_M2, _MCPWM_GEN_M2, speed);
}

// Set speed for motor 1 and 2
void DualVNH7070AYESP32Driver::setSpeeds(float m1Speed, float m2Speed)
{
  setM1Speed(m1Speed);
  setM2Speed(m2Speed);
}

// Brake motor 1, brake is a number between 0 and 100
void DualVNH7070AYESP32Driver::setM1Brake(float brake)
{
  // normalize brake
  if (brake < 0)
  {
    brake = -brake;
  }
  if (brake > 100)  // Max brake
    brake = 100;
  ioexp.digitalWrite(_DIRA1, 1); // brake high so we can use multisense
  ioexp.digitalWrite(_DIRB1, 1);
  ioexp.digitalWrite(_SEL01,0);  // multisense high side B = 00, A = 10
  ioexp.digitalWrite(_SEL11,0);

  mcpwm_set_duty(_MCPWM_UNIT_M1, _MCPWM_TIMER_M1, _MCPWM_GEN_M1, brake);
}

// Brake motor 2, brake is a number between 0 and 100
void DualVNH7070AYESP32Driver::setM2Brake(float brake)
{
  // normalize brake
  if (brake < 0)
  {
    brake = -brake;
  }
  if (brake > 100)  // Max brake
    brake = 100;
  ioexp.digitalWrite(_DIRA2, 1);
  ioexp.digitalWrite(_DIRB2, 1);
  ioexp.digitalWrite(_SEL02,0);  // multisense high side B = 00, A = 10
  ioexp.digitalWrite(_SEL12,0);

  mcpwm_set_duty(_MCPWM_UNIT_M2, _MCPWM_TIMER_M2, _MCPWM_GEN_M2, brake);
}

// Brake motor 1 and 2, brake is a number between 0 and 100
void DualVNH7070AYESP32Driver::setBrakes(float m1Brake, float m2Brake)
{
  setM1Brake(m1Brake);
  setM2Brake(m2Brake);
}

void DualVNH7070AYESP32Driver::enableM1Multisense(bool en) {
  ioexp.digitalWrite(_MSEN1, (uint8_t)en);
}

void DualVNH7070AYESP32Driver::enableM2Multisense(bool en) {
  ioexp.digitalWrite(_MSEN2, (uint8_t)en);
}

void DualVNH7070AYESP32Driver::enableMultisense(bool en) {
  enableM1Multisense(en);
  enableM2Multisense(en);
}

// Return motor 1 multisense reading
int DualVNH7070AYESP32Driver::getM1Multisense()
{
  return adc1_get_raw(_MS1);
}

// Return motor 2 multisense reading
int DualVNH7070AYESP32Driver::getM2Multisense()
{
  return adc1_get_raw(_MS2);
}

void DualVNH7070AYESP32Driver::setM1FaultStatus(bool fault)
{
  ioexp.digitalWrite(_LEDG_M1, (uint8_t)fault);
  ioexp.digitalWrite(_LEDR_M1, (uint8_t)!fault);
}

void DualVNH7070AYESP32Driver::setM2FaultStatus(bool fault)
{
  this->ioexp.digitalWrite(_LEDG_M2, (uint8_t)fault);
  this->ioexp.digitalWrite(_LEDR_M2, (uint8_t)!fault);
}

void DualVNH7070AYESP32Driver::setFaultStatus(bool m1fault, bool m2fault)
{
  setM1FaultStatus(m1fault);
  setM2FaultStatus(m2fault);
}
