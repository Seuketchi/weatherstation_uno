# AVR Weather Station

This project demonstrates how to read data from a DHT11 sensor and display temperature and humidity on an I2C-connected LCD using an AVR microcontroller (such as the ATmega328p). The code has been organized into modular components to improve readability, maintainability, and reusability.

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
   git clone https://github.com/yourusername/avr-dht11-lcd-i2c.git
   cd avr-dht11-lcd-i2c

2. **Compile the Code:**

   ```bash
    avr-gcc -mmcu=atmega328p -Os src/main.c src/dht11.c src/lcd_i2c.c src/i2c.c -I include -o main.elf
    avr-objcopy -O ihex main.elf main.hex
   
3. **Upload to Microcontroller**

    ```bash
    avrdude -c usbasp -p m328p -U flash:w:main.hex
    

### Makefile (Optional)

You can simplify the build process by using a Makefile. Create a file named Makefile with the following content:
    ```bash
    
    MCU = atmega328p
    TARGET = main
    CC = avr-gcc
    OBJCOPY = avr-objcopy
    CFLAGS = -mmcu=$(MCU) -Os -I include
    
    SRC = src/main.c src/dht11.c src/lcd_i2c.c src/i2c.c
    
    all: $(TARGET).hex
    
    $(TARGET).elf: $(SRC)
    	$(CC) $(CFLAGS) $(SRC) -o $(TARGET).elf
    
    $(TARGET).hex: $(TARGET).elf
    	$(OBJCOPY) -O ihex $(TARGET).elf $(TARGET).hex
    
    upload: $(TARGET).hex
    	avrdude -c usbasp -p m328p -U flash:w:$(TARGET).hex
    
    clean:
    	rm -f $(TARGET).elf $(TARGET).hex
    
    .PHONY: all upload clean


**Then run:**

  ```bash
    avrdude -c usbasp -p m328p -U flash:w:main.hex
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



  
