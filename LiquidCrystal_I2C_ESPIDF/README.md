# LiquidCrystal I2C ESP-IDF Library

C library for controlling 16x2 LCD displays with I2C backpack using ESP-IDF.

## Functions

### Initialization
**`lcd_init(gpio_num_t sda, gpio_num_t scl, uint8_t address)`**
- `sda`: SDA GPIO pin number (e.g., GPIO_NUM_21)
- `scl`: SCL GPIO pin number (e.g., GPIO_NUM_22)  
- `address`: I2C address of LCD (common: 0x27 ...)

### Display Control
**`lcd_clear()`**
- Clear entire screen and return cursor to home position
- **IMPORTANT: Always call after initialization before displaying content**

**`lcd_set_cursor(uint8_t row, uint8_t col)`**
- `row`: Row number (0 = first row, 1 = second row)
- `col`: Column number (0-15 for 16x2 display)

### Output

**`lcd_print(const char *str)`**
- `str`: Null-terminated string to display starting at current cursor

**`lcd_print_float(float value, int decimal_places)`**
- `value`: Float number to display
- `decimal_places`: Number of digits after decimal point (0-6 recommended)


## Usage Notes

- **Always call `lcd_clear()` immediately after `lcd_init()` before displaying content**
- Display dimensions: 16 columns × 2 rows (0-based indexing)
- Common I2C addresses: 0x27 (most common), 0x3F, 0x20, 0x38
- Use I2C scanner if LCD address is unknown
- Functions include appropriate timing delays - avoid rapid successive calls

## Troubleshooting
- **No display of display some strange characters**: Check I2C address, verify wiring, ensure `lcd_clear()` is called
- **Garbled text**: Verify I2C connections and pull-up resistors
- **Init fails**: Check GPIO pin assignments and I2C address