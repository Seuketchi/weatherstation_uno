#include "includes/dht.h"
#include <avr/io.h>
#include <util/delay.h>

#define DHT11_PIN PD2

uint8_t dht_read(uint8_t *humidity, uint8_t *temperature) {
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