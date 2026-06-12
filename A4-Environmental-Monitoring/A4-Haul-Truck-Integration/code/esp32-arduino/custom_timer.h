#ifndef CUSTOM_TIMER_H
#define CUSTOM_TIMER_H

#include <Arduino.h>

// extern unsigned long prevTime;
// extern unsigned long tick;
// extern unsigned long lastTick;
// extern unsigned long startTick;

void initCustomTimer(bool debug = false);
unsigned long getPrevTime();
unsigned long getTick();
unsigned long getLastTick();

#endif