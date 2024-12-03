#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

#include "menu.h"
#include "keyboard.h"

QueueHandle_t gpio_evt_queue = NULL;
extern enum pinFlags flag;

void app_menu(enum pinFlags io_num);

// Debouncing task
void gpio_task(void* arg) {
    enum pinFlags io_num;
    TickType_t last_time = 0;

    for(;;) {
        if(xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY)) {
            TickType_t now = xTaskGetTickCount();

            // Sprawdź, czy minął czas eliminacji drgań
            if((now - last_time) * portTICK_PERIOD_MS > DEBOUNCE_TIME_MS) {
                // Jeśli czas minął, przetwarzamy zdarzenie
                last_time = now;

                app_menu(io_num);
            }
        }

    }
}

void app_menu(enum pinFlags io_num)
{
    // Dodaj podświetlanie wyświetlacza (zmiana flagi)
    // Dodaj timer, który automatycznie wyłączy podświetlenie wyświetlacza po np. 60sek

    uint8_t 

    switch(io_num)
    {
        case pin_up: 
        {
            // statement_1;
            printf("Pin pushed %d \r\n", io_num);
            break;
        }
        case pin_down: 
        {
            printf("Pin pushed %d \r\n", io_num);
            break;
        }   

        case pin_select: 
        {
            printf("Pin pushed %d \r\n", io_num);
            break;
        }
        case pin_set: 
        {
            printf("Pin pushed %d \r\n", io_num);
            break;
        }
        default:     
        {
            break;
        } 
    }










    
}