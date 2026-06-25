// MQ-2 Sensor: https://randomnerdtutorials.com/guide-for-mq-2-gas-smoke-sensor-with-arduino/
// MQ-2 Datasheet: https://www.mouser.com/datasheet/2/321/605-00008-MQ-2-Datasheet-370464.pdf

#ifndef MQ_2_H
#define MQ_2_H

#include "rgb.h"
#include "custom_timer.h"
#include <Arduino.h>

void IRAM_ATTR gasISR();
void initMQ2(uint8_t analogPin);
void initMQ2ISR(uint8_t digitalPin);
void calibrateMQ2(bool debug = false);
float gasResistanceRatio();
bool presenceOfGas();
void detectGas(bool debug = false);
void checkGasLevel(bool debug = false);
void monitorGas(int interval = 200, bool debug = false);
#endif