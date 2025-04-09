// #include <avr/io.h>
// #include <util/delay.h>
// #define F_CPU 16000000UL

// // Define pins
// #define DHT11_PIN  PD2  // DHT11 Data connected to PD2
// #define LED_PIN    PB5  // LED on D13 (PB5)

// // Basic DHT11 Read Function (no timeout, no checksum)
// uint8_t DHT11_read(uint8_t *humidity, uint8_t *temperature) {
//     uint8_t data[5] = {0};
//     uint8_t i, j;

//     // Start signal: drive low for 20ms
//     DDRD |= (1 << DHT11_PIN);     // Set PD2 as output
//     PORTD &= ~(1 << DHT11_PIN);   // Pull low
//     _delay_ms(20);
//     PORTD |= (1 << DHT11_PIN);    // Pull high
//     _delay_us(40);
//     DDRD &= ~(1 << DHT11_PIN);    // Set PD2 as input

//     // Wait for sensor response:
//     // Sensor pulls the line low, then high, then low again before sending data
//     while (PIND & (1 << DHT11_PIN));   // Wait for sensor to pull low
//     while (!(PIND & (1 << DHT11_PIN)));  // Wait for sensor to pull high
//     while (PIND & (1 << DHT11_PIN));     // Wait for sensor to pull low again

//     // Read 40 bits (5 bytes) of data from the sensor
//     for (i = 0; i < 5; i++) {
//         for (j = 0; j < 8; j++) {
//             // Wait for the pin to go high: start of bit transmission
//             while (!(PIND & (1 << DHT11_PIN)));
//             // Wait ~35µs to sample the data bit
//             _delay_us(35);
//             // If the pin is still high after 35µs, it's a '1'
//             if (PIND & (1 << DHT11_PIN))
//                 data[i] |= (1 << (7 - j));
//             // Wait for the line to go low, ending this bit
//             while (PIND & (1 << DHT11_PIN));
//         }
//     }

//     // For this test, we ignore the checksum and just use the first and third bytes.
//     *humidity = data[0];
//     *temperature = data[2];

//     return 1;
// }

// int main(void) {
//     uint8_t humidity = 0, temperature = 0;

//     // Set up LED on D13 (PB5) as output
//     DDRB |= (1 << LED_PIN);
//     PORTB &= ~(1 << LED_PIN);  // Ensure LED is off initially

//     while (1) {
//         // Attempt to read the DHT11 sensor
//         if (DHT11_read(&humidity, &temperature)) {
//             // If successful, light up the LED briefly
//             PORTB |= (1 << LED_PIN);
//             _delay_ms(200);
//             PORTB &= ~(1 << LED_PIN);
//         }
//         // Wait 2 seconds before the next reading
//         _delay_ms(2000);
//     }

//     return 0;
// }
