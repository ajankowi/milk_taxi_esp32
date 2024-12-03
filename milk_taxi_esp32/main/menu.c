#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

#include "menu.h"
#include "keyboard.h"
#include "HD44780.h"

QueueHandle_t gpio_evt_queue = NULL;
enum pinFlags io_num;

char tabToPrint[17]; 

extern const struct hd44780 my_lcd;

void app_menu(enum pinFlags io_num);
void display_menu(enum menu_state menu_position);
void display_actualTime();
void display_alarmTime();
void display_temperature();

// Debouncing task
void gpio_task(void* arg) {
    TickType_t last_time = 0;

    for(;;) {
        if(xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY)) {
            TickType_t now = xTaskGetTickCount();

            // Sprawdź, czy minął czas eliminacji drgań
            if((now - last_time) * portTICK_PERIOD_MS > DEBOUNCE_TIME_MS) {
                // Jeśli czas minął, przetwarzamy zdarzenie
                last_time = now;

                printf("\t\tPin pushed %d \r\n", io_num);

                app_menu(io_num);
            }
        }

    }
}

void app_menu(enum pinFlags io_num)
{
    // Dodaj podświetlanie wyświetlacza (zmiana flagi)
    // Dodaj timer, który automatycznie wyłączy podświetlenie wyświetlacza po np. 60sek

    static enum menu_state menu_position = 0;
    int8_t select_newValue = 0;

    switch(io_num)
    {
        case pin_up: 
        {
            if(menu_position == 0)
            {
                menu_position = menu_temperature;
            }
            else
            {
                --menu_position;
            }

            printf("Pin pushed %d \r\n", io_num);
            break;
        }
        case pin_down: 
        {
            ++menu_position;

            if(menu_position >= menu_max)
            {
                menu_position = menu_actualTime;
            }
            
            printf("Pin pushed %d \r\n", io_num);
            break;
        }   

        case pin_select: 
        {
            printf("Pin pushed %d \r\n", io_num);
            select_newValue = 1;
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

    printf("Actual position: %d \r\n", menu_position);

    display_menu(menu_position);
}




void display_menu(enum menu_state menu_position)
{
    static int8_t actual_posision =-1;

    if(actual_posision == menu_position)
    {
        return;
    }
    else
    {
        actual_posision = menu_position;
    }

    switch(menu_position)
    {
        case menu_actualTime: 
        {
            display_actualTime();
            break;
        }
        case menu_alarmTime: 
        {
            display_alarmTime();
            break;
        }   

        case menu_temperature: 
        {
            display_temperature();
            break;
        }

        default:     
        {
            break;
        } 
    }
}



void display_actualTime()
{
    snprintf(tabToPrint, 17,  "actualTime:    ");

    hd44780_gotoxy(&my_lcd, 0, 0);
    hd44780_puts(&my_lcd, tabToPrint);
}

void  display_alarmTime()
{
    snprintf(tabToPrint, 17,  "alarmTime:    ");

    hd44780_gotoxy(&my_lcd, 0, 0);
    hd44780_puts(&my_lcd, tabToPrint);
}

void display_temperature()
{
    snprintf(tabToPrint, 17,  "Temperatura:  ");

    hd44780_gotoxy(&my_lcd, 0, 0);
    hd44780_puts(&my_lcd, tabToPrint);

    snprintf(tabToPrint, 17, "%cC", DEGREE_SYMBOL);

    hd44780_gotoxy(&my_lcd, 0, 1);
    hd44780_puts(&my_lcd, tabToPrint);
}