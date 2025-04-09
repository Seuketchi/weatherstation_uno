#ifdef LCD_I2C_H
#define LCD_I2C_H

#include <stdint.h>

void LCD_init(void);
void LCD_clear(void);
void LCD_goto(uint8_t row, uint8_t col);
void LCD_print(const char *str);

#endif