#include "DHT11.h"
#include "MQ-2.h"
#include "clock.h"
#include "display.h"
#include "SG90-Servo.h"
#include "cardReader.h"
#include "custom_timer.h"
#include "file_manager.h"
#include "sounds.h"
#include "vibration_monitor.h"
#include "rgb.h"
#include "temperature_sensor.h"
#include <Arduino.h>

#define DHTPIN 25
#define MQ2PIN 35
#define MQ2ISR 34
#define SERVOPIN 26
#define RGBRPIN 27
#define RGBGPIN 14
#define RGBBPIN 12
#define THERMPIN 13

const bool debug = true;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  // initDisplay();
  // initDHT11(DHTPIN);
  // initMQ2(MQ2PIN);
  // initMQ2ISR(MQ2ISR);
  // calibrateMQ2(true);
  // initRTC();
  // initServo(SERVOPIN);
  initRGB(RGBRPIN, RGBGPIN, RGBBPIN);
  initThermistor(THERMPIN);
}

void loop() {
  // getTemperature(true);
  // getHumidity(true);
  // detectGas(true);
  // delay(3000);

  // Serial.printf("Timestamp: %s\n", getTimestamp());
  // Serial.printf("Date: %s\n", getDate());
  // Serial.printf("Day: %s\n", getDay());
  // Serial.printf("Day (full): %s\n", getDay(false));
  // delay(2000);
  // rotate(90, 15, true);
  // delay(1000);
  // rotate(180, 15, true);
  // delay(1000);
  // rotate(0, 15, true);
  // delay(1000);
  initCustomTimer(debug);
  temperatureAlert(debug);
  // testRGB();

}

void testRGB()
{
  redON();
  delay(1000);
  RGBOFF();
  delay(1000);
  greenON();
  delay(1000);
  RGBOFF();
  delay(1000);
  yellowON();
  delay(1000);
  RGBOFF();
  delay(1000);
}
