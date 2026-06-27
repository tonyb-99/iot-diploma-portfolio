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
#include <Wire.h>

#define DHTPIN 33
#define MQ2PIN 35
#define MQ2ISR 34
#define SERVOPIN 25
#define RGBRPIN 27
#define RGBGPIN 14
#define RGBBPIN 12
#define THERMPIN 13
#define SDPIN 33
#define SSPIN 5
#define RSTPIN 4
#define REDPIN 15
#define GREENPIN 16
#define SPKRPIN 17

const bool debug = true;
const String fileName = "/vibration_data.csv";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  // initDisplay();
  delay(50);

  initRTC();
  delay(50);

  initLittleFS();
  delay(50);

  createDataFile(LittleFS, fileName, debug);
  delay(50);

  initRGB(RGBRPIN, RGBGPIN, RGBBPIN);
  delay(50);

  initThermistor(THERMPIN);
  delay(50);

  initDHT11(DHTPIN);
  delay(50);  

  initVibrationMonitor(MPU6050_RANGE_2_G, MPU6050_RANGE_250_DEG, MPU6050_BAND_260_HZ);    // High freq for fast readings, no smoothing.
  delay(50);

  initVibrationInterrupt(MPU6050_HIGHPASS_5_HZ, 10);            // Picks fast moving vibrations
  delay(50);

  // initCardReader(SSPIN, RSTPIN, SPKRPIN, REDPIN, GREENPIN);
  // delay(50);

  // initServo(SERVOPIN);
  // delay(50);

  initMQ2(MQ2PIN);
  delay(50);

  initMQ2ISR(MQ2ISR);
  delay(50);

  calibrateMQ2(true);
  delay(50);
}

void loop() {
  initCustomTimer(debug);
  temperatureAlert(debug);
  processDHT(true, true, 90, debug);
  measureVibrations(LittleFS, debug);
  monitorGas(100, debug);
  RGBAlert(debug);
}

