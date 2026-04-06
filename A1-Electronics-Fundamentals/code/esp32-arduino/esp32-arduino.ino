#include "temperature_sensor.h"
#include <Arduino.h>

#define R_PIN 25
#define G_PIN 26
#define B_PIN 27
#define THERM_PIN 34

float temperatureValues[5] = {0.0f};
float tmp = 0.0f;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  initRGB(R_PIN, G_PIN, B_PIN);
  initThermistor(THERM_PIN);
  delay(100);
}

void loop() {
  rgbOFF(R_PIN, G_PIN, B_PIN);
  redON(R_PIN);
  tmp = getTemperature(THERM_PIN);
  delay(1000);
  
  rgbOFF(R_PIN, G_PIN, B_PIN);
  greenON(G_PIN);
  tmp = getTemperature(THERM_PIN);
  delay(1000);

  rgbOFF(R_PIN, G_PIN, B_PIN);
  blueON(B_PIN);
  tmp = getTemperature(THERM_PIN);
  delay(1000);

  rgbOFF(R_PIN, G_PIN, B_PIN);
  yellowON(R_PIN, G_PIN, 0.75f);
  tmp = getTemperature(THERM_PIN);
  delay(1000);
}
