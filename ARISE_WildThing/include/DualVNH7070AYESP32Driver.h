/*
****************************************************************
ARISE Adaptive Design
Project: WildThing ESP32 Port
Author: Aaron Lim
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

#pragma once

#ifndef DUALVNH7070AY_ESP32_DRIVER_H
#define DUALVNH7070AY_ESP32_DRIVER_H

#include <stdint.h>
#include <stdbool.h>
//#include "driver/gpio.h"
//#include "driver/mcpwm.h"
//#include "driver/adc.h"
//#include "MCP23017.h"

const uint32_t MCP_I2C_FREQUENCY_HZ = 100000UL;
const uint32_t VNH7070AY_PWM_FREQUENCY_HZ = 15000;
const adc_atten_t VNH7070AY_ADC_ATTEN = ADC_ATTEN_DB_12; // TODO: Calibrate (ADC_ATTEN_DB_[0, 2_5, 6, 12])
const adc_bits_width_t VNH7070AY_ADC_WIDTH = ADC_WIDTH_BIT_12;

class DualVNH7070AYESP32Driver
{
  public:
    // CONSTRUCTORS
    // Default pin selection.
    DualVNH7070AYESP32Driver();
    // User-defined pin selection.
    DualVNH7070AYESP32Driver(MCP23017Port MCP_PORT_M1,
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
                             mcpwm_generator_t MCWPM_GEN_M1,
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
                             mcpwm_generator_t MCWPM_GEN_M2);

    // PUBLIC METHODS
    /*
     * Sets up the GPIO, I2C bus, PWM, and ADCs.
     */
    void init();
    /**
     * Set speed for motor 1, speed is a number betwenn -100 to 100
     */
    void setM1Speed(float speed);
    /**
     * Set speed for motor 2, speed is a number betwenn -100 to 100
     */
    void setM2Speed(float speed);
    /*
     * Sets speeds for both motors. Calls setM1Speed and setM2Speed.
     */
    void setSpeeds(float m1Speed, float m2Speed);
    /*
     * Set motor 1 speed to zero.
     */
    void setM1Brake(float brake);
    /*
     * Set motor 2 speed to zero.
     */
    void setM2Brake(float brake);
    /*
     * Set speeds for motors 1 and 2 to zero.
     * Calls setM1Brake and setM2Brake.
     */
    void setBrakes(float m1Brake, float m2Brake);
    /*
     * Enable multisense for motor 1.
     */
    void enableM1Multisense(bool en);
    /*
     * Enable multisense for motor 2.
     */
    void enableM2Multisense(bool en);
    /*
     * Enable multisense for both motors.
     * Calls enableM1Multisense and enableM2Multisense.
     */
    void enableMultisense(bool en);
    /*
     * Read the multisense pin for motor 1.
     */
    int getM1Multisense();
    /*
     * Read the multisense pin for motor 2.
     */
    int getM2Multisense();
    /*
     * Set/Clear the red/green LEDs for motor 1.
     */
    void setM1FaultStatus(bool fault);
    /*
     * Set/Clear the red/green LEDs for motor 2.
     */
    void setM2FaultStatus(bool fault);
    /*
     * Set/Clear the red/green LEDs for both motors.
     * Calls setM1FaultStatus and setM2FaultStatus.
     */
    void setFaultStatus(bool m1fault, bool m2fault);

  // Note: _DIRXX -> 01 = counter-clockwise, 10 = clockwise, 00/11 = stopped
  //       _DIRA/B corresponds to INA/B on the VNH7070AY datasheet

    mcpwm_config_t pwm_config;
    MCP23017 ioexp; // IO Expander

  private:
    MCP23017Port _MCP_PORT_M1; // MCP23017 port for motor 1
    MCP23017Pin::Names _DIRA1; // direction control for VNH-1
    MCP23017Pin::Names _DIRB1; // direction control for VNH-1
    gpio_num_t _PWM1;
    MCP23017Pin::Names _SEL01; // SEL0 of VNH-1, enables/disables multisense
    MCP23017Pin::Names _SEL11; // SEL1 of VNH-1
    MCP23017Pin::Names _MSEN1;
    MCP23017Pin::Names _LEDG_M1;
    MCP23017Pin::Names _LEDR_M1;
    adc1_channel_t _MS1;
    mcpwm_unit_t _MCPWM_UNIT_M1;
    mcpwm_timer_t _MCPWM_TIMER_M1;
    mcpwm_io_signals_t _MCPWM_IO_M1;
    mcpwm_generator_t _MCPWM_GEN_M1;

    MCP23017Port _MCP_PORT_M2; // MCP23017 port for motor 2
    MCP23017Pin::Names _DIRA2;
    MCP23017Pin::Names _DIRB2;
    gpio_num_t _PWM2;
    MCP23017Pin::Names _SEL02; // SEL0 of VNH-2
    MCP23017Pin::Names _SEL12; // SEL1 of VNH-2
    MCP23017Pin::Names _MSEN2;
    MCP23017Pin::Names _LEDG_M2;
    MCP23017Pin::Names _LEDR_M2;
    adc1_channel_t _MS2;
    mcpwm_unit_t _MCPWM_UNIT_M2;
    mcpwm_timer_t _MCPWM_TIMER_M2;
    mcpwm_io_signals_t _MCPWM_IO_M2;
    mcpwm_generator_t _MCPWM_GEN_M2;
};

#endif