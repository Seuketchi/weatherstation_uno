#include "i2c.h"
#include "lcd_i2c.h"
#include <util/delay.h>

#define LCD_ADDR 0x27  // I2C Address of LCD
#define LCD_BACKLIGHT 0x08
#define LCD_ENABLE    0x04
#define LCD_RS        0x01

static void LCD_sendNibble(uint8_t nibble, uint8_t rs);
static void LCD_sendByte(uint8_t byte, uint8_t rs);
static void LCD_command(uint8_t cmd);
static void LCD_data(uint8_t data);

void LCD_init(void) {
    _delay_ms(50);  // Wait for LCD to power up
    LCD_sendNibble(0x30, 0); _delay_ms(5);
    LCD_sendNibble(0x30, 0); _delay_us(150);
    LCD_sendNibble(0x30, 0); _delay_us(150);
    LCD_sendNibble(0x20, 0); _delay_us(150);
    LCD_command(0x28);  // Function set: 4-bit mode, 2 lines, 5x8 font
    LCD_command(0x01);  _delay_ms(2);
    LCD_command(0x06); 
}

void LCD_clear(void) {
    LCD_command(0x01);  
    _delay_ms(2);
}

void LCD_goto(uint8_t row, uint8_t col) {
    LCD_command((row == 0 ? 0x80 : 0xC0) + col);
}

void LCD_print(const char *str) {
    while (*str) {
        LCD_data(*str++);
    }
}

static void LCD_sendNibble(uint8_t nibble, uint8_t rs) {
    uint8_t data = (nibble & 0xF0) | LCD_BACKLIGHT | (rs ? LCD_RS : 0);
    I2C_start();
    I2C_write(LCD_ADDR << 1);
    I2C_write(data | LCD_ENABLE);
    I2C_stop();
    _delay_us(1);

    I2C_start();
    I2C_write(LCD_ADDR << 1);
    I2C_write(data & ~LCD_ENABLE);
    I2C_stop();
    _delay_us(50);
}

static void LCD_sendByte(uint8_t byte, uint8_t rs) {
    LCD_sendNibble(byte & 0xF0, rs);
    LCD_sendNibble((byte << 4) & 0xF0, rs);
}

static void LCD_command(uint8_t cmd) {
    LCD_sendByte(cmd, 0);
}

static void LCD_data(uint8_t data) {
    LCD_sendByte(data, 1);
}