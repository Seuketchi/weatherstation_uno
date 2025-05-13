# AVR Weather Station

This project demonstrates how to read data from a DHT11 sensor and display temperature and humidity on an I2C-connected LCD using an AVR microcontroller (such as the ATmega328p). The code has been organized into modular components to improve readability, maintainability, and reusability.

# Team Member
- Tristan Jadman
- Nassem Maruhom

## Table of Contents

- [Project Overview](#project-overview)
- [Features](#features)
- [Setup and Requirements](#setup-and-requirements)
- [Compilation and Upload](#compilation-and-upload)
- [Makefile (Optional)](#makefile-optional)
- [How It Works](#how-it-works)
- [Contributing](#contributing)

## Project Overview

This project reads temperature and humidity data from a DHT11 sensor with checksum verification. The sensor values are then displayed on a 16x2 LCD using an I2C interface (via the PCF8574 I/O expander). An onboard LED (typically connected to D13) indicates a successful sensor reading.

## Features

- **Sensor Reading:** Implements a reliable protocol to read temperature and humidity from the DHT11 with checksum verification.
- **LCD Display:** Uses a PCF8574-based I2C module to operate a standard 16x2 LCD in 4-bit mode.
- **Status LED:** Indicates a successful sensor reading by turning on an LED.
- **Modular Code Structure:** Organized into dedicated modules (`i2c`, `lcd_i2c`, and `dht11`) for clarity and reusability.

## Setup and Requirements

- **Hardware:**
  - AVR microcontroller board (e.g., ATmega328p on an Arduino Uno)
  - DHT11 temperature and humidity sensor
  - I2C-connected 16x2 LCD (with a PCF8574 I/O expander)
  - AVR programmer (e.g., USBasp or similar)

- **Software:**
  - AVR-GCC (compiler)
  - AVR-Objcopy (for converting ELF to HEX files)
  - (Optional) Make utility for simplified builds

## Compilation and Upload

### Using AVR-GCC from the Command Line

1. **Clone the Repository:**

   ```bash
   git clone https://github.com/yourusername/weatherstation_uno.git
   cd weatherstation_uno

2. **Compile the Code:**

   ```bash
    avr-gcc -mmcu=atmega328p -Os src/main.c src/dht.c src/lcd_i2c.c src/i2c.c -I include -o main.elf
    avr-objcopy -O ihex main.elf main.hex
   
3. **Upload to Microcontroller**

    ```bash
    avrdude -c usbasp -p m328p -U flash:w:main.hex
    

### Makefile (Optional)

You can simplify the build process by using a Makefile. Create a file named Makefile with the following content:
    ```bash
    
        ➜  build_scripts git:(main) ✗ make
    avr-gcc -Wall -Os -DF_CPU=16000000 -mmcu=atmega328p -I../src -I../src/includes -I../src/peripherals -c ../src/peripherals/dht.c -o ../src/peripherals/dht.o
    avr-gcc -Wall -Os -DF_CPU=16000000 -mmcu=atmega328p -I../src -I../src/includes -I../src/peripherals -c ../src/peripherals/i2c.c -o ../src/peripherals/i2c.o
    avr-gcc -Wall -Os -DF_CPU=16000000 -mmcu=atmega328p -I../src -I../src/includes -I../src/peripherals -c ../src/peripherals/lcd_i2c.c -o ../src/peripherals/lcd_i2c.o
    avr-gcc -Wall -Os -DF_CPU=16000000 -mmcu=atmega328p -I../src -I../src/includes -I../src/peripherals -c ../src/peripherals/uart.c -o ../src/peripherals/uart.o
    avr-gcc -Wall -Os -DF_CPU=16000000 -mmcu=atmega328p -I../src -I../src/includes -I../src/peripherals -o main.elf ../src/main.o ../src/peripherals/dht.o ../src/peripherals/i2c.o ../src/peripherals/lcd_i2c.o ../src/peripherals/uart.o
    avr-objcopy -j .text -j .data -O ihex main.elf main.hex
    avr-size --format=avr --mcu=atmega328p main.elf
    AVR Memory Usage
    ----------------
    Device: atmega328p
    
    Program:    2786 bytes (8.5% Full)
    (.text + .data + .bootloader)
    
    Data:         82 bytes (4.0% Full)
    (.data + .bss + .noinit)
    
    
    ➜  build_scripts git:(main) ✗ make flash
    avrdude -c arduino -b 115200 -P /dev/ttyACM0 -p atmega328p -U flash:w:main.hex:i
    avrdude OS error: cannot open port /dev/ttyACM0: No such file or directory
    avrdude error: unable to open programmer arduino on port /dev/ttyACM0
    
    avrdude done.  Thank you.
    
    make: *** [Makefile:45: flash] Error 1


**Then run:**

  ```bash
make && make flash
  ```


### How It Works
**I2C Communication:**
The I2C module sets up the Two-Wire Interface (TWI) on the AVR to communicate with the LCD. Basic functions include starting and stopping I2C transactions and writing data.

**LCD Control:**
The LCD functions use the PCF8574 I2C expander to control a standard 16x2 LCD in 4-bit mode. Commands and data are sent in two nibbles with the appropriate control signals (RS, Enable, Backlight).

**DHT11 Sensor:**
The DHT11 module sends a start signal, waits for the sensor’s response, and reads 40 bits of data. It then checks the checksum for validity and extracts temperature and humidity values.

**Main Application:**
After initialization, the main loop continuously polls the DHT11 sensor every 2 seconds, updates the LCD display with the sensor values, and toggles an LED based on the reading result.

### Contributing
Contributions, bug fixes, and feature enhancements are welcome! Please fork the repository, create a new branch for your changes, and submit a pull request.



  
