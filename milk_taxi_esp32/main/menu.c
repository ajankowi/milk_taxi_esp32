#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

#include "menu.h"
#include "keyboard.h"

QueueHandle_t gpio_evt_queue = NULL;
extern enum pinFlags flag;

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