#ifndef DHT_H
#define DHT_H
#include <stdint.h>

uint8_t dht_read(uint8_t *humidity, uint8_t *temperature);

#endif