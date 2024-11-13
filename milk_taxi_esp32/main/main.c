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

#include "gpio.h"

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
    //esp_gdbstub_init();

    printf("Hello world!\n");

    gpio_init_with_interrupt();

    xTaskCreate(gpio_task, "gpio_task", 2048, NULL, 10, NULL);

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

}
