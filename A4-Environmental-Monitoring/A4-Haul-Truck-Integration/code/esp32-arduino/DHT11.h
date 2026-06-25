// Humidity sensor: https://randomnerdtutorials.com/esp32-dht11-dht22-temperature-humidity-sensor-arduino-ide/

#ifndef DHT11_H
#define DHT11_H

#include "safe_temperatures.h"
#include "rgb.h"
#include "DHT.h"
#include "custom_timer.h"
#include <Arduino.h>

#define DHTTYPE DHT11

// Temperature tolerance for humans 
// https://pmc.ncbi.nlm.nih.gov/articles/PMC10687011/
// https://calc2chart.com/temperature-body-chart/
// typedef enum { 
//   FREEZING = -10, 
//   COLD = 10, 
//   SAFE = 25, 
//   WARM = 37, 
//   HOT = 43, 
//   } Conditions;

void initDHT11(uint8_t dhtPin);
float getTemperature(bool debug = false);
float getHumidity(bool debug = false);
void selectColour(float temp, bool debug = false);
void processDHT(bool humidityON = true, bool tempON = true, int interval = 100, bool debug = false);

#endif 