#include "DHT11.h"
#include "MQ-2.h"
#include <Arduino.h>

// Humidity sensor: https://randomnerdtutorials.com/esp32-dht11-dht22-temperature-humidity-sensor-arduino-ide/
// OLED Display: https://randomnerdtutorials.com/esp32-ssd1306-oled-display-arduino-ide/
// DS3231 RTC: https://randomnerdtutorials.com/esp32-ds3231-real-time-clock-arduino/
// MQ-2 Sensor: https://randomnerdtutorials.com/guide-for-mq-2-gas-smoke-sensor-with-arduino/
// MQ-2 Datasheet: https://www.mouser.com/datasheet/2/321/605-00008-MQ-2-Datasheet-370464.pdf

#define DHTPIN 2
#define MQ2PIN 13
#define ISRPIN 12


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  initDHT11(DHTPIN);
  initMQ2(MQ2PIN);
  initMQ2ISR(ISRPIN);
  calibrateMQ2(true);
}

void loop() {
  getTemperature(true);
  getHumidity(true);
  detectGas(true);
  delay(3000);
}
