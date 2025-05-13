#include "includes/i2c.h"
#include <util/delay.h>

void I2C_init(void) {
    TWSR = 0x00;         // Prescaler = 1
    TWBR = 72;           // SCL frequency ~100kHz (for 16MHz clock)
    TWCR = (1 << TWEN);  // Enable TWI
}

void I2C_start(void) {
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
}
void I2C_stop(void) {
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
    _delay_us(10);
}

void I2C_write(uint8_t data) {
    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
}