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

#define DEBOUNCE_TIME_MS 200 // Debouncing time in ms

QueueHandle_t gpio_evt_queue = NULL;
extern enum pinFlags flag;
uint32_t ctr = 0;




// Debouncing task
void gpio_task(void* arg) {
    enum pinFlags io_num;
    TickType_t last_time = 0;

    uint32_t ctr = 0;

    for(;;) {
        if(xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY)) {
            TickType_t now = xTaskGetTickCount();

            // Sprawdź, czy minął czas eliminacji drgań
            if((now - last_time) * portTICK_PERIOD_MS > DEBOUNCE_TIME_MS) {
                // Jeśli czas minął, przetwarzamy zdarzenie
                printf("Pin pushed %d, %ld \r\n", io_num, ctr++);
                last_time = now;
            }
        }
    }
}















void app_main(void)
{
    uint8_t tab[5] = {'a', 'b', 'c', 'd', 'e'};

    //lcd_print(tab, sizeof(tab)/sizeof(tab[0])); 

    gpio_init_with_interrupt();

    xTaskCreate(gpio_task, "gpio_task", 2048, NULL, 10, NULL);

    hd44780_t lcd;

    lcd.pins.rs = 1;
    lcd.pins.e = 0;
    lcd.pins.d4 = 13;
    lcd.pins.d5 = 12;
    lcd.pins.d6 = 11;
    lcd.pins.d7 = 10;
    lcd.pins.bl = HD44780_NOT_USED;
    lcd.font = HD44780_FONT_5X8;
    lcd.lines = 2;
    lcd.backlight = false;

    hd44780_init(&lcd);

    hd44780_clear(&lcd);

    hd44780_puts(&lcd, "XD! lol");

    printf("Hello world!\n");


    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

}
