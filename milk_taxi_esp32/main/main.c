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
#include "ds3231.h"

uint32_t ctr = 0;




void app_main(void)
{
    extern const struct hd44780 my_lcd;

    adc_oneshot_init();

    hd44780_init(&my_lcd);
    hd44780_clear(&my_lcd);

    //display_menu(menu_actualTime);

	// Initialize RTC
	i2c_dev_t dev;
	if (ds3231_init_desc(&dev, I2C_NUM_0, CONFIG_SDA_GPIO, CONFIG_SCL_GPIO) != ESP_OK) {
		ESP_LOGE(pcTaskGetName(0), "Could not init device descriptor.");
		while (1) { vTaskDelay(1); }
	}

	struct tm time = {
		.tm_year = 2020,
		.tm_mon  = 1,  // 0-based
		.tm_mday = 1,
		.tm_hour = 1,
		.tm_min  = 1,
		.tm_sec  = 1
	};

	if (ds3231_set_time(&dev, &time) != ESP_OK) {
		ESP_LOGE(pcTaskGetName(0), "Could not set time.");
		while (1) { vTaskDelay(1); }
	}
	ESP_LOGI(pcTaskGetName(0), "Set initial date time done");

    // Get RTC date and time
	float temp;
	struct tm rtcinfo;

    gpio_init_with_interrupt();
    xTaskCreate(gpio_task, "gpio_task", 2048, NULL, 10, NULL);

    uint32_t voltage = 0;

    printf("Hello world!\n");

    uint16_t ctr = 0;

    int measure = 0;

    while (1) 
    {
        measure = adc_oneshot_voltage_to_temperature();

        vTaskDelay(pdMS_TO_TICKS(1000));

        if (ds3231_get_temp_float(&dev, &temp) != ESP_OK) {
		    ESP_LOGE(pcTaskGetName(0), "Could not get temperature.");
		    while (1) { vTaskDelay(1); }
	    }

	    if (ds3231_get_time(&dev, &rtcinfo) != ESP_OK) {
		    ESP_LOGE(pcTaskGetName(0), "Could not get time.");
		    while (1) { vTaskDelay(1); }
	    }

	    ESP_LOGI(pcTaskGetName(0), "%04d-%02d-%02d %02d:%02d:%02d, %.2f deg Cel", 
	    rtcinfo.tm_year, rtcinfo.tm_mon + 1,
	    rtcinfo.tm_mday, rtcinfo.tm_hour, rtcinfo.tm_min, rtcinfo.tm_sec, temp);
    }

}
