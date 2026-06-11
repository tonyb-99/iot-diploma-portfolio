// DS3231 RTC: https://randomnerdtutorials.com/esp32-ds3231-real-time-clock-arduino/

#ifndef CLOCK_H
#define CLOCH_H

#include "RTClib.h"
#include <Arduino.h>

extern RTC_DS3231 rtc;

void initRTC();
DateTime getTime();
String getTimestamp();
String getDate();
String getDay(bool shortened = true);

#endif