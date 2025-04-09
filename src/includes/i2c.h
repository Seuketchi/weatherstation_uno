#ifndef I2C_H
#define I2C_H

#include <avr/io.h>

void I2C_init(void);
void I2C_start(void);
void I2C_stop(void);
void I2C_write(uint8_t data);

#endif