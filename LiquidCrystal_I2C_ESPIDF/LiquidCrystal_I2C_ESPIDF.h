#pragma once

#include "driver/i2c.h"
#include "esp_err.h"
#include <stdio.h>
// LCD Commands
#define LCD_CLEARDISPLAY        0x01
#define LCD_RETURNHOME          0x02
#define LCD_ENTRYMODESET        0x04
#define LCD_DISPLAYCONTROL      0x08
#define LCD_CURSORSHIFT         0x10
#define LCD_FUNCTIONSET         0x20
#define LCD_SETCGRAMADDR        0x40
#define LCD_SETDDRAMADDR        0x80

// Flags for display entry mode
#define LCD_ENTRYRIGHT          0x00
#define LCD_ENTRYLEFT           0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// Flags for display on/off control
#define LCD_DISPLAYON           0x04
#define LCD_DISPLAYOFF          0x00
#define LCD_CURSORON            0x02
#define LCD_CURSOROFF           0x00
#define LCD_BLINKON             0x01
#define LCD_BLINKOFF            0x00

// Flags for function set
#define LCD_8BITMODE            0x10
#define LCD_4BITMODE            0x00
#define LCD_2LINE               0x08
#define LCD_1LINE               0x00
#define LCD_5x10DOTS            0x04
#define LCD_5x8DOTS             0x00

// Backlight control
#define LCD_BACKLIGHT           0x08
#define LCD_NOBACKLIGHT         0x00
#define LCD_ENABLE              0x04
#define LCD_READWRITE           0X02
#define LCD_REGSELECT           0x01

void lcd_init(gpio_num_t sda, gpio_num_t scl, uint8_t address);
void lcd_command(uint8_t cmd);
void lcd_write_char(char chr);
void lcd_print(const char *str);
void lcd_print_float(float value, int decimal_places);
void lcd_set_cursor(uint8_t row, uint8_t col);
void lcd_clear();
void lcd_backlight_on();
void lcd_backlight_off();