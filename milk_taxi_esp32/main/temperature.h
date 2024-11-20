#ifndef __TEMPERATURE_H__
#define __TEMPERATURE_H__

#include <stdio.h>
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "esp_log.h"

#define ADC_CHANNEL     ADC_CHANNEL_6 // GPIO4 (przykład, sprawdź dokumentację ESP32-C6)
#define ADC_WIDTH       ADC_WIDTH_BIT_12 // Rozdzielczość ADC
#define ADC_ATTEN       ADC_ATTEN_DB_11 // Attenuacja dla większego zakresu napięć (0-3.6V)

#define NO_OF_SAMPLES   64 // Ilość próbek do uśrednienia   


// One shot mode configuration

#define ADC_CHAN_2_GPIO_2          ADC_CHANNEL_2

void adc_oneshot_init();
void adc_oneshot_deinit();
void adc_oneshot_raw_read(int* out_raw);


// Continuous mode configuration

void adc_continuous_init();




#endif /* __TEMPERATURE_H__ */