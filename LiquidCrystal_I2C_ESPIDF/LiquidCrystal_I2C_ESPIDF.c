#include "LiquidCrystal_I2C_ESPIDF.h"
#include "driver/i2c.h"
#include "freertos/task.h"
#include "esp_log.h"
#include <stdio.h>


static uint8_t lcd_displayfunction = 0;
static uint8_t lcd_displaycontrol = 0;
static uint8_t lcd_displaymode = 0;
static uint8_t lcd_backlight_val = LCD_BACKLIGHT;
static i2c_port_t lcd_i2c_port = I2C_NUM_0;
static uint8_t lcd_addr;
static uint8_t lcd_backlight = LCD_BACKLIGHT;
static const char *TAG = "LCD_I2C";


static esp_err_t expanderWrite(uint8_t data) {
    data |= lcd_backlight;
    ESP_LOGD(TAG, "Writing to expander: 0x%02X (backlight: 0x%02X)", data, lcd_backlight);
    
    esp_err_t ret = i2c_master_write_to_device(
        lcd_i2c_port, lcd_addr, &data, 1, pdMS_TO_TICKS(1000)
    );
    
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "I2C write failed (data=0x%02X), error: %s", data, esp_err_to_name(ret));
    }
    return ret;
}

static void pulseEnable(uint8_t data) {
    ESP_LOGD(TAG, "Pulse enable with data: 0x%02X", data);
    expanderWrite(data | LCD_ENABLE);
    esp_rom_delay_us(1);
    expanderWrite(data & ~LCD_ENABLE);
    esp_rom_delay_us(50);
}

// send 4 bit to LCD
static void write4bits(uint8_t data) {
    ESP_LOGD(TAG, "Write4bits: 0x%02X", data);
    expanderWrite(data);
    pulseEnable(data);
}

static void send(uint8_t value, uint8_t mode) {
    uint8_t high = value & 0XF0;
    uint8_t low = (value << 4) & 0XF0;
    
    ESP_LOGD(TAG, "Sending value: 0x%02X, mode: 0x%02X (high: 0x%02X, low: 0x%02X)", 
             value, mode, high, low);
    
    write4bits(high | mode);
    write4bits(low | mode);
}

// command to LCD
void lcd_command(uint8_t value) {
    ESP_LOGD(TAG, "LCD Command: 0x%02X", value);
    send(value, 0);
    vTaskDelay(pdMS_TO_TICKS(1)); 
}

// user send data to LCD
void lcd_write_char(char chr) {
    ESP_LOGD(TAG, "LCD Write char: '%c' (0x%02X)", chr, chr);
    send(chr, LCD_REGSELECT);
    vTaskDelay(pdMS_TO_TICKS(1)); 
}

// iPrint string to LCD
void lcd_print(const char *str) {
    while (*str) {
        lcd_write_char(*str++);
    }
}
// print float value to LCD
void lcd_print_float(float value, int decimal_places) {
    char buffer[32]; 
    char format[10];

    snprintf(format, sizeof(format), "%%.%df", decimal_places);
    snprintf(buffer, sizeof(buffer), format, value);
    lcd_print(buffer);
}

// === Set row and col ===
void lcd_set_cursor(uint8_t row, uint8_t col) {
    const uint8_t offsets[] = {0x00, 0x40};
    uint8_t address = 0x80 | (offsets[row] + col);
    lcd_command(address);
}

void lcd_clear() {
    lcd_command(0x01);
    vTaskDelay(pdMS_TO_TICKS(15)); 
}

void lcd_backlight_on() {
    lcd_backlight = LCD_BACKLIGHT;
    expanderWrite(0x00);
}

void lcd_backlight_off() {
    lcd_backlight = 0x00;
    expanderWrite(0x00); 
}

void lcd_init(gpio_num_t sda, gpio_num_t scl, uint8_t address) {
    // Update address LCD
    lcd_addr = address;
    
    // I2C setup
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = sda,
        .scl_io_num = scl,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 100000
    };

    i2c_param_config(I2C_NUM_0, &conf);
    i2c_driver_install(I2C_NUM_0, conf.mode, 0, 0, 0);
    
    vTaskDelay(pdMS_TO_TICKS(300));

    lcd_displayfunction = LCD_4BITMODE | LCD_2LINE | LCD_5x8DOTS;
    lcd_backlight = LCD_BACKLIGHT;

    // Turn on backlight 
    lcd_backlight_on();
    vTaskDelay(pdMS_TO_TICKS(200));

    // Init sequence
    write4bits(0x30);
    vTaskDelay(pdMS_TO_TICKS(15));
    
    write4bits(0x30);
    vTaskDelay(pdMS_TO_TICKS(15));
    
    write4bits(0x30);
    vTaskDelay(pdMS_TO_TICKS(10));
    
    write4bits(0x20);
    vTaskDelay(pdMS_TO_TICKS(10));

    lcd_command(LCD_FUNCTIONSET | lcd_displayfunction);
    vTaskDelay(pdMS_TO_TICKS(5));

    lcd_displaycontrol = LCD_DISPLAYOFF | LCD_CURSOROFF | LCD_BLINKOFF;
    lcd_command(LCD_DISPLAYCONTROL | lcd_displaycontrol);
    vTaskDelay(pdMS_TO_TICKS(5));

    lcd_clear();
    vTaskDelay(pdMS_TO_TICKS(20));

    lcd_displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
    lcd_command(LCD_ENTRYMODESET | lcd_displaymode);
    vTaskDelay(pdMS_TO_TICKS(5));

    lcd_displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
    lcd_command(LCD_DISPLAYCONTROL | lcd_displaycontrol);
    vTaskDelay(pdMS_TO_TICKS(5));
}