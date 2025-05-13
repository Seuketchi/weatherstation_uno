// #include <avr/io.h>
// #include <util/delay.h>
// #define F_CPU 16000000UL

// // I2C LCD Address (change if needed)
// #define LCD_ADDR 0x27

// // I2C (TWI) Functions
// void I2C_init(void) {
//     TWSR = 0x00;         // Prescaler = 1
//     TWBR = 72;           // SCL frequency ~100kHz (for 16MHz clock)
//     TWCR = (1 << TWEN);  // Enable TWI
// }

// void I2C_start(void) {
//     TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
//     while (!(TWCR & (1 << TWINT)));
// }

// void I2C_stop(void) {
//     TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
//     _delay_us(10);
// }

// void I2C_write(uint8_t data) {
//     TWDR = data;
//     TWCR = (1 << TWINT) | (1 << TWEN);
//     while (!(TWCR & (1 << TWINT)));
// }

// // LCD Control Functions
// // The PCF8574 provides 8 bits which are usually mapped as follows:
// // bit 3: Enable (E)
// // bit 2: Read/Write (RW) - we keep this low (write)
// // bit 1: Register Select (RS) - 0 for command, 1 for data
// // bit 0: Backlight control (we assume backlight on if set)
// #define LCD_BACKLIGHT 0x08
// #define LCD_ENABLE    0x04
// #define LCD_RS        0x01

// // Send a nibble (upper 4 bits) with control bits.
// // 'rs' is 0 for command, 1 for data.
// void LCD_sendNibble(uint8_t nibble, uint8_t rs) {
//     uint8_t data = (nibble & 0xF0)       // use upper 4 bits
//                    | LCD_BACKLIGHT      // backlight on
//                    | (rs ? LCD_RS : 0);  // set RS if data

//     // Pulse Enable high
//     I2C_start();
//     I2C_write(LCD_ADDR << 1);
//     I2C_write(data | LCD_ENABLE);
//     I2C_stop();
//     _delay_us(1);
    
//     // Then, bring Enable low
//     I2C_start();
//     I2C_write(LCD_ADDR << 1);
//     I2C_write(data & ~LCD_ENABLE);
//     I2C_stop();
//     _delay_us(50);
// }

// // Send a full byte (command or data) by splitting into two nibbles.
// void LCD_sendByte(uint8_t byte, uint8_t rs) {
//     // Send upper nibble
//     LCD_sendNibble(byte & 0xF0, rs);
//     // Send lower nibble (shift left into upper nibble position)
//     LCD_sendNibble((byte << 4) & 0xF0, rs);
// }

// void LCD_command(uint8_t cmd) {
//     LCD_sendByte(cmd, 0);
// }

// void LCD_data(uint8_t data) {
//     LCD_sendByte(data, 1);
// }

// void LCD_init(void) {
//     _delay_ms(50);  // Wait for LCD to power up

//     // Initialization sequence in 4-bit mode
//     LCD_sendNibble(0x30, 0);
//     _delay_ms(5);
//     LCD_sendNibble(0x30, 0);
//     _delay_us(150);
//     LCD_sendNibble(0x30, 0);
//     _delay_us(150);
//     LCD_sendNibble(0x20, 0);  // Set to 4-bit mode
//     _delay_us(150);

//     // Function set: 4-bit mode, 2 lines, 5x8 font
//     LCD_command(0x28);
//     // Display on, cursor off, blink off
//     LCD_command(0x0C);
//     // Clear display
//     LCD_command(0x01);
//     _delay_ms(2);
//     // Entry mode: increment cursor, no shift
//     LCD_command(0x06);
// }

// void LCD_clear(void) {
//     LCD_command(0x01);
//     _delay_ms(2);
// }

// void LCD_goto(uint8_t row, uint8_t col) {
//     uint8_t pos = (row == 0 ? 0x80 : 0xC0) + col;
//     LCD_command(pos);
// }

// void LCD_print(const char *str) {
//     while(*str) {
//         LCD_data(*str++);
//     }
// }

// // Main function: initialize I2C, the LCD, and print "Hello World"
// int main(void) {
//     I2C_init();
//     LCD_init();
    
//     LCD_clear();
//     LCD_goto(0, 0);
//     LCD_print("Hello World");
    
//     while (1) {
//         // Loop forever
//     }
    
//     return 0;
// }
