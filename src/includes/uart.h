#ifndef UART_H
#define UART_H

#include <avr/io.h>

// Define the baud rate
#define BAUD 9600
#define MYUBRR F_CPU / 16 / BAUD - 1

// Function prototypes
void UART_init(unsigned int ubrr);
void UART_transmit(char data);
void UART_print(const char *str);

#endif // UART_H