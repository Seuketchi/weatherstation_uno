#include "avr/io.h"
#include "util/delay.h"
#include "includes/lcd_i2c.h"
#include "includes/dht.h"
#include "includes/i2c.h"
#include "includes/uart.h"  // Include UART header
#include <stdio.h>

#define LED_PIN PB5  // LED on D13 (PB5)

int main(void) {
    uint8_t humidity = 0, temperature = 0;

    // Initialize UART for debugging
    UART_init(MYUBRR);
    UART_print("Initializing I2C and LCD...\n");

    // Set up LED on D13 (PB5) as output
    DDRB |= (1 << LED_PIN);
    PORTB &= ~(1 << LED_PIN);  // Ensure LED is off initially

    // Initialize I2C and LCD
    I2C_init();
    LCD_init();
    

    while (1) {
        // Attempt to read the DHT11 sensor
        if (dht_read(&humidity, &temperature)) {
            // If successful, light up the LED briefly
            PORTB |= (1 << LED_PIN);  // Turn on LED


            // Display temperature and humidity on LCD
            LCD_clear();
            LCD_goto(0, 0);
            char buffer[16];
            snprintf(buffer, sizeof(buffer), "Temp: %d C", temperature);
            UART_print(buffer);
            UART_print("\n");
            LCD_print(buffer);
            LCD_goto(1, 0);
            snprintf(buffer, sizeof(buffer), "Humidity: %d%%", humidity);
            UART_print(buffer);
            UART_print("\n");
            LCD_print(buffer);
        } else {
            // If reading failed, display error message
            LCD_clear();
            LCD_goto(0, 0);
            LCD_print("Sensor Error");
            UART_print("Sensor Error\n");
        }

        _delay_ms(2000);  // Wait before next reading
    }
}