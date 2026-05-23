#ifndef DHT11_H
#define DHT11_H

#include "DHT.h"
#include <Arduino.h>

#define DHTTYPE DHT11

void initDHT11(uint8_t dhtPin);
float getTemperature(bool debug = false);
float getHumidity(bool debug = false);

#endif 