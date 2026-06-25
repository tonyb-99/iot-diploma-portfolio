#ifndef TEMPERATURE_SENSOR_H
#define TEMPERATURE_SENSOR_H

#include "safe_temperatures.h"
#include "rgb.h"
#include "custom_timer.h"
#include <Arduino.h>

#define BETA 3950.
#define KLVN 273.15
#define ANALOG_MAX 4095.
#define ROOM_TEMP 25.0

// typedef enum { 
//   NONE = 1, 
//   GREEN = 20, 
//   YELLOW = 10, 
//   RED = 5,
//   } ColourCode;

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

void initThermistor(uint8_t thermistorPin);
void calcTemperature(bool debug = false);
float getAvgTemp(bool debug = false);
void selectColour(bool debug = false);
// ColourCode getCurrentColour();
// void handleLED(bool debug = false);
void temperatureAlert(int interval = 90, bool debug = false);


#endif






