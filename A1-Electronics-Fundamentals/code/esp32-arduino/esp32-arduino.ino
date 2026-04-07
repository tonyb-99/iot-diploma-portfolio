#include "temperature_sensor.h"
#include <Arduino.h>

#define R_PIN 25
#define G_PIN 26
#define B_PIN 27
#define THERM_PIN 34

float avgTemperature = 0.0f;
unsigned long currentTime = 0;
unsigned long prevTime = 0;
const long interval = 25000;
ColourCode currentColour = ColourCode::NONE;
bool isRed = false;
bool isGreen = false;
bool isYellow = false;
uint8_t counter = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  initRGB(R_PIN, G_PIN, B_PIN);
  initThermistor(THERM_PIN);
  delay(100);
}

void loop() {
  currentTime = millis();
  // Determine colour state immediately within the first 5 seconds
  if(currentColour == ColourCode::NONE)
  {
    if(currentTime - prevTime >= 1000)
    {
      prevTime = currentTime;
      avgTemperature += getTemperature(THERM_PIN);
      int counter = prevTime / 1000;
      if(counter > 1)
      { 
        avgTemperature /= 2.0f;
      }
    }

    // After ~5 seconds, get temperature and new average.
    if(prevTime >= 5000)
    {
      prevTime = currentTime;
      avgTemperature += getTemperature(THERM_PIN);
      avgTemperature /= 2.0f;
      LED_Temperature();
    }
  }

  // Calculate the average temperature after 5 samples
  // Sample every 5 seconds
  // if(currentColour != ColourCode::NONE && counter % 3 == 0)
  // {
  //   prevTime = currentTime;
  //   avgTemperature += getTemperature(THERM_PIN);
  //   avgTemperature /= 2.0f;
  //   LED_Temperature();
  //   //currentColour = getTemperatureColour(avgTemperature);
  // }
  if(counter != 0 && counter % 2 == 0)
  {
    if(currentTime - prevTime > 2500)
    {
      avgTemperature += getTemperature(THERM_PIN);
      avgTemperature /= 2.0f;
      LED_Temperature();
    }
  }
  // Green LED pulse longer
  if(currentColour == ColourCode::GREEN && currentTime - prevTime > 2500)
  {
    counter++;
    prevTime = currentTime;
    if(isGreen)
    {
      G_LED();
      isGreen = false;
    }
    else
    {
      RGBOFF();
      isGreen = true;
    }
  }
  // Yellow LED pulse shorter
  // Red LED pulse quickly 
  
}

void RGBOFF() 
{
  rgbOFF(R_PIN, G_PIN, B_PIN); 
}

void R_LED()  { redON(R_PIN); }

void Y_LED() { redON(R_PIN, 0.7f); greenON(G_PIN, 0.7f); }

void G_LED() { greenON(G_PIN); } 

void LED_Temperature()
{
  currentColour = getTemperatureColour(avgTemperature);
  switch(currentColour) 
  {
    case ColourCode::RED:
      isRed = true;
      isGreen = isYellow = false;
      //R_LED();
      break;

    case ColourCode::YELLOW:
      //Y_LED();
      isYellow = true;
      isRed = isGreen = false;
      break;

    case ColourCode::GREEN:
      isGreen = true;
      isRed = isYellow = false;
      //G_LED();
      break;
  }
}
