/*
 * Copyright (c) 2016 Ruslan V. Uss <unclerus@gmail.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of itscontributors
 *    may be used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file hd44780.h
 * @defgroup hd44780 hd44780
 * @{
 *
 * ESP-IDF driver for HD44780 compatible LCD text displays
 *
 * Ported from esp-open-rtos
 *
 * Copyright (c) 2016 Ruslan V. Uss <unclerus@gmail.com>
 *
 * BSD Licensed as described in the file LICENSE
 */
#ifndef __HD44780_H__
#define __HD44780_H__

#include <stdint.h>
#include <stdbool.h>
#include <driver/gpio.h>
#include <esp_err.h>

#ifdef __cplusplus
extern "C" {
#endif

#define HD44780_NOT_USED 0xff

#define GPIO_PIN_RS         GPIO_NUM_1
#define GPIO_PIN_E          GPIO_NUM_0
#define GPIO_PIN_D4         GPIO_NUM_13
#define GPIO_PIN_D5         GPIO_NUM_12
#define GPIO_PIN_D6         GPIO_NUM_11
#define GPIO_PIN_D7         GPIO_NUM_10

#define DEGREE_SYMBOL       0xdf  // 223 in decimal

#define MS 1000

#define BV(x) (1 << (x))
#define GPIO_BIT(x) (1ULL << (x))

#define DELAY_CMD_LONG  (3 * MS) // >1.53ms according to datasheet
#define DELAY_CMD_SHORT (60)     // >39us according to datasheet
#define DELAY_TOGGLE    (1)      // E cycle time >= 1μs, E pulse width >= 450ns, Data set-up time >= 195ns
#define DELAY_INIT      (5 * MS)

#define CMD_CLEAR        0x01
#define CMD_RETURN_HOME  0x02
#define CMD_ENTRY_MODE   0x04
#define CMD_DISPLAY_CTRL 0x08
#define CMD_SHIFT        0x10
#define CMD_FUNC_SET     0x20
#define CMD_CGRAM_ADDR   0x40
#define CMD_DDRAM_ADDR   0x80

#define ARG_MOVE_RIGHT 0x04
#define ARG_MOVE_LEFT 0x00
#define CMD_SHIFT_LEFT  (CMD_SHIFT | CMD_DISPLAY_CTRL | ARG_MOVE_LEFT)
#define CMD_SHIFT_RIGHT (CMD_SHIFT | CMD_DISPLAY_CTRL | ARG_MOVE_RIGHT)

// CMD_ENTRY_MODE
#define ARG_EM_INCREMENT    BV(1)
#define ARG_EM_SHIFT        (1)

// CMD_DISPLAY_CTRL
#define ARG_DC_DISPLAY_ON   BV(2)
#define ARG_DC_CURSOR_ON    BV(1)
#define ARG_DC_CURSOR_BLINK (1)

// CMD_FUNC_SET
#define ARG_FS_8_BIT        BV(4)
#define ARG_FS_2_LINES      BV(3)
#define ARG_FS_FONT_5X10    BV(2)

#define init_delay()   do { esp_rom_delay_us(pdMS_TO_TICKS(DELAY_INIT)); } while (0)
#define short_delay()  do { esp_rom_delay_us(DELAY_CMD_SHORT); } while (0)
#define long_delay()   do { esp_rom_delay_us(DELAY_CMD_LONG); } while (0)
#define toggle_delay() do { esp_rom_delay_us(DELAY_TOGGLE); } while (0)

#define CHECK_ARG(VAL) do { if (!(VAL)) return ESP_ERR_INVALID_ARG; } while (0)
#define CHECK(x) do { esp_err_t __; if ((__ = x) != ESP_OK) return __; } while (0)


/**
 * LCD font type. Please refer to the datasheet
 * of your module.
 */
typedef enum
{
    HD44780_FONT_5X8 = 0,
    HD44780_FONT_5X10
} hd44780_font_t;

typedef struct hd44780 hd44780_t;

typedef esp_err_t (*hd44780_write_cb_t)(const hd44780_t *lcd, uint8_t data);

/**
 * LCD descriptor. Fill it before use.
 */
struct hd44780
{
    hd44780_write_cb_t write_cb; //!< Data write callback. Set it to NULL in case of direct LCD connection to GPIO
    struct
    {
        uint8_t rs;        //!< GPIO/register bit used for RS pin
        uint8_t e;         //!< GPIO/register bit used for E pin
        uint8_t d4;        //!< GPIO/register bit used for D4 pin
        uint8_t d5;        //!< GPIO/register bit used for D5 pin
        uint8_t d6;        //!< GPIO/register bit used for D5 pin
        uint8_t d7;        //!< GPIO/register bit used for D5 pin
        uint8_t bl;        //!< GPIO/register bit used for backlight. Set it `HD44780_NOT_USED` if no backlight used
    } pins;
    hd44780_font_t font;   //!< LCD Font type
    uint8_t lines;         //!< Number of lines for LCD. Many 16x1 LCD has two lines (like 8x2)
    bool backlight;        //!< Current backlight state
};

/**
 * @brief Init LCD
 *
 * Set cursor position to (0, 0)
 *
 * @param lcd LCD descriptor
 * @return `ESP_OK` on success
 */
esp_err_t hd44780_init(hd44780_t *lcd);

/**
 * @brief Control LCD
 *
 * On/off LCD, show/hide cursor, set cursor blink
 *
 * @param lcd LCD descriptor
 * @param on Switch LCD on if true
 * @param cursor Show cursor if true
 * @param cursor_blink Enable cursor blinking if true
 * @return `ESP_OK` on success
 */
esp_err_t hd44780_control(const hd44780_t *lcd, bool on, bool cursor, bool cursor_blink);

/**
 * @brief Clear LCD
 *
 * Clear memory and move cursor to (0, 0)
 *
 * @param lcd LCD descriptor
 * @return `ESP_OK` on success
 */
esp_err_t hd44780_clear(const hd44780_t *lcd);

/**
 * @brief Move cursor
 *
 * @param lcd LCD descriptor
 * @param col Column
 * @param line Line
 * @return `ESP_OK` on success
 */
esp_err_t hd44780_gotoxy(const hd44780_t *lcd, uint8_t col, uint8_t line);

/**
 * @brief Write character at cursor position
 *
 * @param lcd LCD descriptor
 * @param c Character to write
 * @return `ESP_OK` on success
 */
esp_err_t hd44780_putc(const hd44780_t *lcd, char c);

/**
 * @brief Write NULL-terminated string at cursor position
 *
 * @param lcd LCD descriptor
 * @param s String to write
 * @return `ESP_OK` on success
 */
esp_err_t hd44780_puts(const hd44780_t *lcd, const char *s);

/**
 * @brief Switch backlight
 *
 * @param lcd LCD descriptor
 * @param on Turn backlight on if true
 * @return `ESP_OK` on success
 */
esp_err_t hd44780_switch_backlight(hd44780_t *lcd, bool on);

/**
 * @brief Upload character data to the CGRAM
 *
 * After upload cursor will be moved to (0, 0).
 *
 * @param lcd LCD descriptor
 * @param num Character number (0..7)
 * @param data Character data: 8 or 10 bytes depending on the font
 * @return `ESP_OK` on success
 */
esp_err_t hd44780_upload_character(const hd44780_t *lcd, uint8_t num, const uint8_t *data);

/**
 * @brief Scroll the display content to left by one character
 *
 * @param lcd LCD descriptor
 * @return `ESP_OK` on success
 */
esp_err_t hd44780_scroll_left(const hd44780_t *lcd);

/**
 * @brief Scroll the display content to right by one character
 *
 * @param lcd LCD descriptor
 * @return `ESP_OK` on success
 */
esp_err_t hd44780_scroll_right(const hd44780_t *lcd);

#ifdef __cplusplus
}
#endif

/**@}*/

#endif /* __HD44780_H__ */