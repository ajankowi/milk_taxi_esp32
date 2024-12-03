/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"
#include "esp_gdbstub.h"


#include "keyboard.h"
#include "HD44780.h"
#include "temperature.h"
#include "menu.h"

uint32_t ctr = 0;




void app_main(void)
{
    char tabToPrint[17]; 

    extern const struct hd44780 my_lcd;

    adc_oneshot_init();

    hd44780_init(&my_lcd);
    hd44780_clear(&my_lcd);


    gpio_init_with_interrupt();
    xTaskCreate(gpio_task, "gpio_task", 2048, NULL, 10, NULL);

    uint32_t voltage = 0;

    printf("Hello world!\n");

    uint16_t ctr = 0;

    int measure = 0;

    while (1) {


        measure = adc_oneshot_voltage_to_temperature();
        //printf("Temperature: %d*C\r\n", measure);


        vTaskDelay(pdMS_TO_TICKS(1000));
        ctr++;
        snprintf(tabToPrint, 17,  "Temperatura:");

        hd44780_gotoxy(&my_lcd, 0, 0);
        hd44780_puts(&my_lcd, tabToPrint);

        snprintf(tabToPrint, 17, "%d%cC", measure, DEGREE_SYMBOL);

        hd44780_gotoxy(&my_lcd, 0, 1);
        hd44780_puts(&my_lcd, tabToPrint);

    }

}
