#ifndef MQ_2_H
#define MQ_2_H

#include <Arduino.h>

void IRAM_ATTR gasISR();
void initMQ2(uint8_t analogPin);
void initMQ2ISR(uint8_t digitalPin);
void calibrateMQ2(bool debug = false);
float gasResistanceRatio();
bool presenceOfGas();
void detectGas(bool debug = false);
void checkGasLevel(bool debug = false);

#endif