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
i2c_dev_t dev_rtc;

void app_main(void)
{
    extern const struct hd44780 my_lcd;

    adc_oneshot_init();

    hd44780_init(&my_lcd);
    hd44780_clear(&my_lcd);

    //display_menu(menu_actualTime);

	// Initialize RTC
	if (ds3231_init_desc(&dev_rtc, I2C_NUM_0, CONFIG_SDA_GPIO, CONFIG_SCL_GPIO) != ESP_OK) {
		ESP_LOGE(pcTaskGetName(0), "Could not init device descriptor.");
		while (1) { vTaskDelay(1); }
	}

	struct tm time = {
		.tm_year = 2020,
		.tm_mon  = 1,  // 0-based
		.tm_mday = 1,
		.tm_hour = 18,
		.tm_min  = 43,
		.tm_sec  = 20
	};

	if (ds3231_set_time(&dev_rtc, &time) != ESP_OK) {
		ESP_LOGE(pcTaskGetName(0), "Could not set time.");
	}
	ESP_LOGI(pcTaskGetName(0), "Set initial date time done");

    // Get RTC date and time
	float temp;
	struct tm rtcinfo;

    gpio_init_with_interrupt();
    xTaskCreate(gpio_task, "gpio_task", 4096, NULL, 8, NULL);
    xTaskCreate(displayMenu_task, "displayMenu_task", 4096, NULL, 10, NULL);

}
