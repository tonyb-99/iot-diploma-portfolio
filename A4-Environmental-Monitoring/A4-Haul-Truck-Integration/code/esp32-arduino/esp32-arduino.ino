#include "DHT.h"
#include <Arduino.h>

// Humidity sensor: https://randomnerdtutorials.com/esp32-dht11-dht22-temperature-humidity-sensor-arduino-ide/
// OLED Display: https://randomnerdtutorials.com/esp32-ssd1306-oled-display-arduino-ide/
// DS3231 RTC: https://randomnerdtutorials.com/esp32-ds3231-real-time-clock-arduino/
// MQ-2 Sensor: https://randomnerdtutorials.com/guide-for-mq-2-gas-smoke-sensor-with-arduino/

#define DHTPIN
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
