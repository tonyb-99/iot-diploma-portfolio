#ifndef MQ_2_H
#define MQ_2_H

#include <Arduino.h>

void initMQ2(uint8_t analogPin);
void calibrateMQ2(bool debug = false);
float gasResistanceRatio();
bool presenceOfGas();
void detectGas(bool debug = false);
void checkGasLevel(bool debug = false);

#endif