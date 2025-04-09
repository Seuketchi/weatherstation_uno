#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#define F_CPU 16000000UL



// Define Pins
#define DHT11_PIN  PD2  // DHT11 Data Pin connected to PD2
#define LED_PIN    PB5  // LED on D13 (PB5)

// I2C LCD Definitions
#define LCD_ADDR    0x27  // I2C Address of LCD

// I2C (TWI) Functions
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

// LCD Control Functions (PCF8574 based)
// Mapping:
// bit3: Enable (E)
// bit2: RW (fixed low)
// bit1: RS (0: command, 1: data)
// bit0: Backlight (on if set)
#define LCD_BACKLIGHT 0x08
#define LCD_ENABLE    0x04
#define LCD_RS        0x01

// Send a nibble (upper 4 bits) with control bits.
void LCD_sendNibble(uint8_t nibble, uint8_t rs) {
    uint8_t data = (nibble & 0xF0) | LCD_BACKLIGHT | (rs ? LCD_RS : 0);
    
    // Pulse Enable high then low
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

// Send a full byte by splitting into two nibbles.
void LCD_sendByte(uint8_t byte, uint8_t rs) {
    LCD_sendNibble(byte & 0xF0, rs);
    LCD_sendNibble((byte << 4) & 0xF0, rs);
}

void LCD_command(uint8_t cmd) {
    LCD_sendByte(cmd, 0);
}

void LCD_data(uint8_t data) {
    LCD_sendByte(data, 1);
}

void LCD_init(void) {
    _delay_ms(50);  // Wait for LCD to power up

    // Initialization sequence in 4-bit mode
    LCD_sendNibble(0x30, 0);
    _delay_ms(5);
    LCD_sendNibble(0x30, 0);
    _delay_us(150);
    LCD_sendNibble(0x30, 0);
    _delay_us(150);
    LCD_sendNibble(0x20, 0);  // Set to 4-bit mode
    _delay_us(150);

    // Function set: 4-bit mode, 2 lines, 5x8 font
    LCD_command(0x28);
    // Display on, cursor off, blink off
    LCD_command(0x0C);
    // Clear display
    LCD_command(0x01);
    _delay_ms(2);
    // Entry mode: increment cursor, no display shift
    LCD_command(0x06);
}

void LCD_clear(void) {
    LCD_command(0x01);
    _delay_ms(2);
}

void LCD_goto(uint8_t row, uint8_t col) {
    uint8_t pos = (row == 0 ? 0x80 : 0xC0) + col;
    LCD_command(pos);
}

void LCD_print(const char *str) {
    while (*str) {
        LCD_data(*str++);
    }
}

// DHT11 Functions with Checksum Verification
uint8_t DHT11_read(uint8_t *humidity, uint8_t *temperature) {
    uint8_t data[5] = {0};
    uint16_t timeout;
    uint8_t i, j;
    
    // Start signal: drive low for 20ms
    DDRD |= (1 << DHT11_PIN);      // Set as output
    PORTD &= ~(1 << DHT11_PIN);    // Pull low
    _delay_ms(20);
    PORTD |= (1 << DHT11_PIN);     // Pull high
    _delay_us(40);
    DDRD &= ~(1 << DHT11_PIN);     // Set as input

    // Wait for sensor response: sensor should pull line low
    timeout = 0;
    while ((PIND & (1 << DHT11_PIN)) && timeout < 1000) {
        _delay_us(1);
        timeout++;
    }
    if (timeout >= 1000) return 0;  // Sensor did not pull low

    // Wait for sensor to pull line high (end of 80us low period)
    timeout = 0;
    while (!(PIND & (1 << DHT11_PIN)) && timeout < 1000) {
        _delay_us(1);
        timeout++;
    }
    if (timeout >= 1000) return 0;  // Sensor did not pull high

    // Wait for sensor to pull line low again before data transmission starts
    timeout = 0;
    while ((PIND & (1 << DHT11_PIN)) && timeout < 1000) {
        _delay_us(1);
        timeout++;
    }
    if (timeout >= 1000) return 0;  // Sensor did not pull low

    // Now read 40 bits (5 bytes) of data
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 8; j++) {
            // Wait for the pin to go high: beginning of bit transmission
            timeout = 0;
            while (!(PIND & (1 << DHT11_PIN)) && timeout < 1000) {
                _delay_us(1);
                timeout++;
            }
            if (timeout >= 1000) return 0;  // Timeout waiting for high pulse

            // Delay to sample the bit value
            _delay_us(30);  // 30us delay: if the line is still high, it's a "1"
            if (PIND & (1 << DHT11_PIN))
                data[i] |= (1 << (7 - j));

            // Wait for the line to go low to end this bit's transmission
            timeout = 0;
            while ((PIND & (1 << DHT11_PIN)) && timeout < 1000) {
                _delay_us(1);
                timeout++;
            }
            if (timeout >= 1000) return 0;  // Timeout waiting for low pulse
        }
    }
    
    // Verify checksum: The fifth byte should equal the sum of bytes 0-3
    uint8_t checksum = data[0] + data[1] + data[2] + data[3];
    if (data[4] != checksum)
        return 0; // Checksum failed

    *humidity = data[0];
    *temperature = data[2];

    return 1;
}

int main(void) {
    uint8_t humidity = 0, temperature = 0;
    char buffer[16];

    // Setup LED on D13 (PB5) as output
    DDRB |= (1 << LED_PIN);
    PORTB &= ~(1 << LED_PIN);

    // Initialize I2C and LCD
    I2C_init();
    LCD_init();
    LCD_clear();
    LCD_print("Initializing...");
    _delay_ms(2000);
    LCD_clear();

    while (1) {
        if (DHT11_read(&humidity, &temperature)) {
            // Turn LED on if reading is successful
            PORTB |= (1 << LED_PIN);
            
            // Display results on LCD
            LCD_clear();
            LCD_goto(0, 0);
            snprintf(buffer, 16, "Temp: %dC", temperature);
            LCD_print(buffer);
            LCD_goto(1, 0);
            snprintf(buffer, 16, "Hum: %d%%", humidity);
            LCD_print(buffer);
        } else {
            // Turn LED off if reading fails and show error on LCD
            PORTB &= ~(1 << LED_PIN);
            LCD_clear();
            LCD_print("DHT11 Error");
        }
        _delay_ms(2000);  // Update every 2 seconds
    }
    
    return 0;
}
