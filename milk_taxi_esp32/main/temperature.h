#ifndef __TEMPERATURE_H__
#define __TEMPERATURE_H__

#include <stdio.h>
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "esp_log.h"

// One shot mode configuration

#define NO_OF_SAMPLES               64  // Numer of samples to average
#define CALIBRATION_VALUE_DEFAULT   0
#define ADC_CHAN_2_GPIO_2           ADC_CHANNEL_2

// Return values
#define ADC_OK                           0
#define ADC_VALUE_UNDER_RANGE           -1
#define ADC_VALUE_OVER_RANGE            -2


struct voltage_to_temperature {
    int voltage;
    int temperature;
};

typedef struct voltage_to_temperature voltage_to_temperature_t;

void adc_oneshot_init();
void adc_oneshot_deinit();
void adc_oneshot_raw_read(int* out_raw);
int adc_oneshot_multi_raw_read();
int adc_oneshot_voltage_to_temperature();


// Continuous mode configuration

void adc_continuous_init();


#define TEMPERATURE_TABLE           \
    {1200, 0},                      \
    {1630, 22},                     \
    {1650, 23},                     \
    {1670, 24},                     \
    {1700, 25},                     \
    {1730, 26},                     \
    {1770, 27},                     \
    {1800, 28},                     \
    {1850, 29},                     \
    {1890, 30},                     \
    {1940, 31},                     \
    {1990, 32},                     \
    {2040, 33},                     \
    {2100, 34},                     \
    {2150, 35},                     \
    {2200, 60}, 

#endif /* __TEMPERATURE_H__ */