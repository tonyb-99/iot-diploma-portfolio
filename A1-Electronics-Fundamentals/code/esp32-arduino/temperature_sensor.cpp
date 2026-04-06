#include "temperature_sensor.h"
#include <Arduino.h>

void initRGB(uint8_t redPin, uint8_t greenPin, uint8_t bluePin)
{
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  Serial.println("RGB pins initialised.");
}

void rgbOFF(uint8_t redPin, uint8_t greenPin, uint8_t bluePin)
{
  analogWrite(redPin, 0);
  analogWrite(greenPin, 0);
  analogWrite(bluePin, 0);
  Serial.println("RGB OFF.");
}

void redON(uint8_t redPin, float intensity)
{
  if(intensity < 0.0f || intensity > 1.0f) { return; }
  int brightness = 255 * intensity;
  analogWrite(redPin, brightness);
  Serial.println("RED LED ON.");
}

void greenON(uint8_t greenPin, float intensity)
{
  if(intensity < 0.0f || intensity > 1.0f) { return; }
  int brightness = 255 * intensity;
  analogWrite(greenPin, brightness);
  Serial.println("GREEN LED ON.");
}

void yellowON(uint8_t redPin, uint8_t greenPin, float intensity)
{
  if(intensity < 0.0f || intensity > 1.0f) { return; }
  int brightness = 255 * intensity;
  analogWrite(redPin, brightness);
  analogWrite(greenPin, brightness);
  Serial.println("YELLOW LED ON.");
}

void blueON(uint8_t bluePin, float intensity)
{
  if(intensity < 0.0f || intensity > 1.0f) { return; }
  int brightness = 255 * intensity;
  analogWrite(bluePin, brightness);
  Serial.println("BLUE LED ON.");
}

void initThermistor(uint8_t thermistorPin)
{
  pinMode(thermistorPin, INPUT);
  Serial.println("Thermistor initialised.");
}

float getTemperature(uint8_t thermistorPin)
{
  // Temperature calculation based on the following code by Uri Shaked (2021): https://wokwi.com/projects/299330254810382858 
  int analogValue = analogRead(thermistorPin);
  float celsius = 1 / (log( 1 / (ANALOG_MAX / analogValue - 1)) / BETA + 1.0 / (ROOM_TEMP + KLVN)) - KLVN;
  Serial.print("Temperature: ");
  Serial.print(celsius);
  Serial.println(" ℃");
  return celsius;
}







