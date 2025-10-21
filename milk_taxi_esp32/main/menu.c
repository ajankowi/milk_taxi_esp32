#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "esp_log.h"

#include "menu.h"
#include "keyboard.h"
#include "HD44780.h"
#include "ds3231.h"
#include "temperature.h"

QueueHandle_t gpio_evt_queue = NULL;
enum pinFlags io_num;

char tabToPrint[17]; 

struct tm rtcinfo;
enum menu_state menu_position = 0;
int tempMeasure = 0;
bool select_newValue = false;
bool changeParameter = false;

extern const struct hd44780 my_lcd;

void app_menu(enum pinFlags io_num);
void display_menu(enum menu_state menu_position);
void display_actualTime();
void display_alarmTime();
void display_temperature();

// GPIO task with debouncing functionality
void gpio_task(void* arg) {
    TickType_t last_time = 0;

    for(;;) {
        if(xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY)) {
            TickType_t now = xTaskGetTickCount();

            // Check if debounce time has passed to eliminate button bouncing
            if((now - last_time) * portTICK_PERIOD_MS > DEBOUNCE_TIME_MS) {
               // If time has passed, process the event
                last_time = now;

                printf("\t\tPin pushed %d \r\n", io_num);

                app_menu(io_num);
            }
        }

    }
}

// Display menu task - continuously updates the LCD display
void displayMenu_task(void* arg) {

    for(;;) {
        vTaskDelay(pdMS_TO_TICKS(DISPLAY_LOOP_TIME_MS));
        display_menu(menu_position);
    }
}

// Menu logic backend - handles button input and navigation
void app_menu(enum pinFlags io_num)
{
    // ToDo
    // Add display backlight control (flag change)
    // Add timer to automatically turn off display backlight after e.g. 60sec

    switch(io_num)
    {
        case pin_up: 
        {
            if(select_newValue == 1)
            {
                if(changeParameter)
                {
                    if (ds3231_get_time(&dev_rtc, &rtcinfo) != ESP_OK) {
	                    ESP_LOGE(pcTaskGetName(0), "Could not get time, RTC error!");
	                }
                    changeParameter = false;
                }

            }else
            {
                if(menu_position == 0)
            {
                menu_position = menu_temperature;
            }
            else
            {
                --menu_position;
            }
            }

            break;
        }
        case pin_down: 
        {
            ++menu_position;

            if(menu_position >= menu_max)
            {
                menu_position = menu_actualTime;
            }
            
            break;
        }   

        case pin_select: 
        {
            if(!select_newValue)
            {
                select_newValue = true;
                changeParameter = true;
            }
            
            break;
        }
        case pin_set: 
        {
            select_newValue = 0;

            break;
        }
        default:     
        {
            break;
        } 
    }
    printf("Pin pushed %d \r\n", io_num);
    printf("Actual position: %d \r\n", menu_position);
}



// Display frontend - renders appropriate menu screen based on current state
void display_menu(enum menu_state menu_position)
{
    switch(menu_position)
    {
        case menu_actualTime: 
        {
            if(select_newValue)
            {
                change_actualTime();
            }
            else           
            {
                display_actualTime();
            }
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


// Display current time from RTC on LCD screen
void display_actualTime()
{
	if (ds3231_get_time(&dev_rtc, &rtcinfo) != ESP_OK) {
	    ESP_LOGE(pcTaskGetName(0), "Could not get time, RTC error!");
	}


    snprintf(tabToPrint, 17, "1.Aktualny czas:");

    hd44780_gotoxy(&my_lcd, 0, 0);
    hd44780_puts(&my_lcd, tabToPrint);

    snprintf(tabToPrint, 17, "  %02d:%02d", rtcinfo.tm_hour, rtcinfo.tm_min);

    hd44780_gotoxy(&my_lcd, 0, 1);
    hd44780_puts(&my_lcd, tabToPrint);
}

// Display alarm time setting on LCD screen
void  display_alarmTime()
{
    snprintf(tabToPrint, 17,  "2.Godzina alarmu");

    hd44780_gotoxy(&my_lcd, 0, 0);
    hd44780_puts(&my_lcd, tabToPrint);

    snprintf(tabToPrint, 17, "  07:00       ");

    hd44780_gotoxy(&my_lcd, 0, 1);
    hd44780_puts(&my_lcd, tabToPrint);

}

// Display current temperature reading on LCD screen
void display_temperature()
{
    tempMeasure = adc_oneshot_voltage_to_temperature();

    snprintf(tabToPrint, 17,  "3.Temperatura:  ");

    hd44780_gotoxy(&my_lcd, 0, 0);
    hd44780_puts(&my_lcd, tabToPrint);

    snprintf(tabToPrint, 17, "  %d%cC      ", tempMeasure, DEGREE_SYMBOL);

    hd44780_gotoxy(&my_lcd, 0, 1);
    hd44780_puts(&my_lcd, tabToPrint);
}

// Display time change interface for setting new time
void change_actualTime()
{
    snprintf(tabToPrint, 17, "1.Godzina:");

    hd44780_gotoxy(&my_lcd, 0, 0);
    hd44780_puts(&my_lcd, tabToPrint);

    snprintf(tabToPrint, 17, "  %02d:%02d", rtcinfo.tm_hour, rtcinfo.tm_min);

    hd44780_gotoxy(&my_lcd, 0, 1);
    hd44780_puts(&my_lcd, tabToPrint);
}