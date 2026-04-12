#include "temperature_sensor.h"
#include <Arduino.h>

#define R_PIN 25
#define G_PIN 26
#define B_PIN 27
#define THERM_PIN 34

float avgTemperature = 0.0f;
unsigned long currentTime = 0;
unsigned long prevTime = 0;
unsigned long tick = 0;
unsigned long lastTick = -1;
ColourCode currentColour = ColourCode::NONE;
bool isRed = false;
bool isGreen = false;
bool isYellow = false;
bool isLED_ON = false;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  initRGB(R_PIN, G_PIN, B_PIN);
  initThermistor(THERM_PIN);
  delay(100);
}

void loop() {
  currentTime = millis();
  // New time variable where a tick is 100ms. 
  if(currentTime - prevTime >= 100)
  {
    prevTime = currentTime;
    tick++;
  }

  // Execute once per tick 
  if(tick != lastTick)
  {
    lastTick = tick;

    if(tick % 10 == 0)
    {
      Serial.print("Time: ");
      Serial.print(tick / 10);
      Serial.print(" s\n");
    }

    // Determine colour state immediately within the first 5 seconds
    if(tick % 10 == 0 && currentColour == ColourCode::NONE)
    {
        avgTemperature += getTemperature(THERM_PIN);

        if(tick != 0 && tick % 10 == 0)
        { 
          avgTemperature /= 2.0f;
        }

        if(tick >= 50)
        {
          LED_Temperature();
        }
      
    }

    // After the initial 5 seconds and contains a colour code
    if(currentColour != ColourCode::NONE)
    {
      // Determine the avg temperature and colour every 5 seconds.
      if(tick % 50 == 0)
      {
        avgTemperature += getTemperature(THERM_PIN);
        avgTemperature /= 2.0f;
        LED_Temperature();
      }

      // Green LED pulse longer (Flicker every 3 sec)
      if(currentColour == ColourCode::GREEN && tick % 30 == 0)
      {
        isLED_ON = !isLED_ON;
        //prevTime = currentTime;
        if(isGreen && isLED_ON)
        {
          G_LED();
          isGreen = false;
        }
        
        if(!isGreen && !isLED_ON)
        {
          RGBOFF();
          isGreen = true;
        }
      }
      // Yellow LED pulse shorter (Flicker every 1.5 sec)
      if(currentColour == ColourCode::YELLOW && tick % 15 == 0)
      {
        isLED_ON = !isLED_ON;
        if(isYellow && isLED_ON)
        {
          Y_LED();
          isYellow = false;
        }
        
        if(!isYellow && !isLED_ON)
        {
          RGBOFF();
          isYellow = true;
        }
      }

      // Red LED pulse quickly (Flicker every 0.5 sec)
      if(currentColour == ColourCode::RED && tick % 5 == 0)
      {
        isLED_ON = !isLED_ON;
        if(isRed && isLED_ON)
        {
          R_LED();
          isRed = false;
        }
        
        if(!isRed && !isLED_ON)
        {
          RGBOFF();
          isRed = true;
        }
      }
    }
  }
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
  if(!isLED_ON)
  {
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

      default: return;
    }
  }
}
