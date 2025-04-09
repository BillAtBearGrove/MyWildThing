#include "../include/setupIO.h"
//#include "driver/gpio.h"
//#include "driver/adc.h"
#include "../include/pinouts.h"

void setup_io()
{
    // Setup GPIO //

    gpio_config_t gp;

    uint64_t GPIO_OUTPUT_BITMASK = (1ULL<<esp_spare_0)    | (1ULL<<esp_spare_1)      |
                                   (1ULL<<sdcard_mosi)    | (1ULL<<sdcard_cs)        |
                                   (1ULL<<sdcard_detect)  | (1ULL<<sdcard_clk)       |
                                   (1ULL<<red_status_led) | (1ULL<<green_status_led) |
                                   (1ULL<<JoySwitch_Main);

    gp.intr_type = GPIO_INTR_DISABLE;
    gp.mode = GPIO_MODE_OUTPUT;
    gp.pin_bit_mask = GPIO_OUTPUT_BITMASK;
    gpio_config(&gp);

    uint64_t GPIO_INPUT_BITMASK = (1ULL<<auxio_0) | (1ULL<<auxio_1) |
                                  (1ULL<<auxio_3) | (1ULL<<auxio_4) |
                                  (1ULL<<sdcard_miso) | (1ULL<<JoySwitch_Main);

    gp.mode = GPIO_MODE_INPUT;
    gp.pin_bit_mask = GPIO_INPUT_BITMASK;
    gpio_config(&gp);

    // Setup ADCs //

    adc1_config_width(ADC_WIDTH_BIT_12);

    adc2_config_channel_atten(vcc_sense, ADC_ATTEN_DB_12);
    adc2_config_channel_atten(vdd_sense, ADC_ATTEN_DB_12);
    adc1_config_channel_atten(v3p3_sense, ADC_ATTEN_DB_12);
    adc1_config_channel_atten(joyX_Occupant, ADC_ATTEN_DB_12);
    adc1_config_channel_atten(joyY_Occupant, ADC_ATTEN_DB_12);
    adc1_config_channel_atten(joyX_Tether, ADC_ATTEN_DB_12);
    adc1_config_channel_atten(joyY_Tether, ADC_ATTEN_DB_12);
}