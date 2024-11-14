#ifndef GPIO_H
#define GPIO_H

// Include necessary standard libraries
#include <stdint.h>
#include "driver/gpio.h"
#include "esp_attr.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

// Define macros for GPIO pins
#define GPIO_PIN_UP         18
#define GPIO_PIN_DOWN       19
#define GPIO_PIN_SELECT     20
#define GPIO_PIN_SET        21

#define GPIO_INPUT_PIN_SEL  ((1ULL << GPIO_PIN_UP) | (1ULL << GPIO_PIN_DOWN) | (1ULL << GPIO_PIN_SELECT) | (1ULL << GPIO_PIN_SET)) 

enum pinFlags{
    pin_null = 0,
    pin_up = GPIO_PIN_UP,
    pin_down = GPIO_PIN_DOWN,
    pin_select = GPIO_PIN_SELECT,
    pin_set = GPIO_PIN_SET
};

// Tag for logging
static const char *TAG = "GPIO_ISR";

// Declare global variable for storing button state
static void IRAM_ATTR gpio_isr_handler(void* arg);


// Init function for GPIO and interrupt
void gpio_init_with_interrupt();





#endif // GPIO_H