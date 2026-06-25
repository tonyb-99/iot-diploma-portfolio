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
  initDHT11(DHTPIN);
  initMQ2(MQ2PIN);
  initMQ2ISR(MQ2ISR);
  calibrateMQ2(true);
  // initRTC();
  // initServo(SERVOPIN);
  // initRGB(RGBRPIN, RGBGPIN, RGBBPIN);
  // initThermistor(THERMPIN);
  // Wire.begin(21, 22);
  // Wire.setClock(100000);
  // delay(200);


  // initRTC();
  // delay(200);
  // initLittleFS();
  // delay(100);
  // createDataFile(LittleFS, fileName, debug);
  // delay(200);
  // initVibrationMonitor(MPU6050_RANGE_2_G, MPU6050_RANGE_250_DEG, MPU6050_BAND_260_HZ);    // High freq for fast readings, no smoothing.
  // initVibrationInterrupt(MPU6050_HIGHPASS_5_HZ, 10);            // Picks fast moving vibrations
  // delay(100);
  // initCardReader(SSPIN, RSTPIN, SPKRPIN, REDPIN, GREENPIN);
  // delay(200);
}

void loop() {


  // displayText("Hello");

  // delay(1000);
  // displayText("World!");

  // rotate(90, 15, true);
  // delay(1000);
  // rotate(180, 15, true);
  // delay(1000);
  // rotate(0, 15, true);
  // delay(1000);

  // initCustomTimer(debug);
  // temperatureAlert(debug);

  // initCustomTimer(debug);
  // measureVibrations(LittleFS, debug);

  // initCustomTimer(debug);
  // cardReadProcess(debug);

  initCustomTimer(debug);
  processDHT(true, true, 100, debug);
  monitorGas(120, debug);
}

