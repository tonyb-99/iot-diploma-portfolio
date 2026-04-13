#include "temperature_sensor.h"
#include <Arduino.h>

void initRGB(uint8_t redPin, uint8_t greenPin, uint8_t bluePin)
{
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  Serial.println("RGB pins initialised.");
}

void rgbOFF(uint8_t redPin, uint8_t greenPin, uint8_t bluePin, bool debug)
{
  analogWrite(redPin, 0);
  analogWrite(greenPin, 0);
  analogWrite(bluePin, 0);
  if(debug) { Serial.println("RGB OFF."); }
}

void redON(uint8_t redPin, float intensity, bool debug)
{
  if(intensity < 0.0f || intensity > 1.0f) { return; }
  int brightness = 255 * intensity;
  analogWrite(redPin, brightness);
  if(debug) { Serial.println("RED LED ON.");  }
}

void greenON(uint8_t greenPin, float intensity, bool debug)
{
  if(intensity < 0.0f || intensity > 1.0f) { return; }
  int brightness = 255 * intensity;
  analogWrite(greenPin, brightness);
  if(debug) { Serial.println("GREEN LED ON.");  }
}

void yellowON(uint8_t redPin, uint8_t greenPin, float intensity, bool debug)
{
  if(intensity < 0.0f || intensity > 1.0f) { return; }
  int brightness = 255 * intensity;
  analogWrite(redPin, brightness);
  analogWrite(greenPin, brightness);
  if(debug) { Serial.println("YELLOW LED ON.");  }
}

void blueON(uint8_t bluePin, float intensity, bool debug)
{
  if(intensity < 0.0f || intensity > 1.0f) { return; }
  int brightness = 255 * intensity;
  analogWrite(bluePin, brightness);
  if(debug) { Serial.println("BLUE LED ON.");  }
}

void initThermistor(uint8_t thermistorPin)
{
  pinMode(thermistorPin, INPUT);
  Serial.println("Thermistor initialised.");
}

float getTemperature(uint8_t thermistorPin, bool debug)
{
  // Temperature calculation based on the following code by Uri Shaked (2021): https://wokwi.com/projects/299330254810382858 
  int analogValue = analogRead(thermistorPin);
  float celsius = 1 / (log( 1 / (ANALOG_MAX / analogValue - 1)) / BETA + 1.0 / (ROOM_TEMP + KLVN)) - KLVN;
  if(debug)
  {
    Serial.print("Analog value: ");
    Serial.print(analogValue);
    Serial.print("\t\t");
    Serial.print("Temperature: ");
    Serial.print(celsius);
    Serial.println(" *C");
  }
  return celsius;
}

ColourCode getTemperatureColour(float temperature, bool debug)
{
  // Check dangers first before approaching safety
  // Danger if below -10 or above 43 C
  if(temperature < Conditions::FREEZING || temperature > Conditions::HOT)
  {
    if(debug) { Serial.println("Colour code: RED"); }
    return ColourCode::RED;
  }
  // Warning if below 10 or above 37 C
  else if(temperature < Conditions::COLD || temperature > Conditions::WARM)
  {
    if(debug) { Serial.println("Colour code: YELLOW"); }
    return ColourCode::YELLOW;
  }

  // Safe range: 11-36 C
  else
  {
    if(debug) { Serial.println("Colour code: GREEN"); }
    return ColourCode::GREEN;
  }
  
  // Fallback if fails
  if(debug) { Serial.println("Colour code: NONE. (AN ERROR HAS OCCURRED WITH OBTAINING COLOUR)"); }
  return ColourCode::NONE;
}






