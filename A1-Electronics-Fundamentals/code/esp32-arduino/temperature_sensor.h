#ifndef TEMPERATURE_SENSOR_H
#define TEMPERATURE_SENSOR_H

#include <Arduino.h>

#define BETA 3950.
#define KLVN 273.15
#define ANALOG_MAX 4095.
#define ROOM_TEMP 25.0

typedef enum { 
  NONE, 
  GREEN, 
  YELLOW, 
  RED
  } ColourCode;

// Temperature tolerance for humans 
// https://pmc.ncbi.nlm.nih.gov/articles/PMC10687011/
// https://calc2chart.com/temperature-body-chart/
typedef enum { 
  FREEZING = -10, 
  COLD = 10, 
  SAFE = 25, 
  WARM = 37, 
  HOT = 43, 
  } Conditions;

void initRGB(uint8_t redPin, uint8_t greenPin, uint8_t bluePin);
void rgbOFF(uint8_t redPin, uint8_t greenPin, uint8_t bluePin, bool debug = false);
void redON(uint8_t redPin, float intensity = 1.0f, bool debug = false);
void greenON(uint8_t greenPin, float intensity = 1.0, bool debug = false);
void yellowON(uint8_t redPin, uint8_t greenPin, float intensity = 1.0f, bool debug = false);
void blueON(uint8_t bluePin, float intensity = 1.0f, bool debug = false);

void initThermistor(uint8_t thermistorPin);
float getTemperature(uint8_t thermistorPin, bool debug = false);
ColourCode getTemperatureColour(float temperature, bool debug = false);


#endif






