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
int interval = 0;
bool isLED_ON = false;
ColourCode currentColour = ColourCode::NONE;

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

    if(tick % 10 == 0)    // Per second
    {
      Serial.print("Time: ");
      Serial.print(tick / 10);
      Serial.println(" seconds.");
    }

    // Determine colour state immediately within the first 5 seconds
    if(tick % 10 == 0 && currentColour == ColourCode::NONE)
    {
        avgTemperature += getTemperature(THERM_PIN, true);

        if(tick != 0 && tick % 10 == 0)     // Per second
        { 
          avgTemperature /= 2.0f;
        }

        if(tick >= 50)    // 5 seconds
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
        avgTemperature += getTemperature(THERM_PIN, true);
        avgTemperature /= 2.0f;
        LED_Temperature();
      }

      // Toggle LED to corresponding colour
      handleLED();
    }

    //Fallback to restart the desired loop in event of failure.
    fallback();

  }
}

void RGBOFF() { rgbOFF(R_PIN, G_PIN, B_PIN); }

void R_LED()  { redON(R_PIN); }

void Y_LED() { redON(R_PIN, 0.7f); greenON(G_PIN, 0.7f); }

void G_LED() { greenON(G_PIN); } 

void LED_Temperature() { currentColour = getTemperatureColour(avgTemperature, true); }
  
void handleLED()
{ 
  switch(currentColour) 
  {
    case ColourCode::RED:
      interval = 5;
      break;

    case ColourCode::YELLOW:
      interval = 15;
      break;

    case ColourCode::GREEN:
      interval = 30;
      break;

    default: return;
  }
  

  if(tick % interval == 0)
  {
    isLED_ON = !isLED_ON;
    
    if(isLED_ON)
    {
      switch(currentColour)
      {
        case ColourCode::RED:
          R_LED();
          break;

        case ColourCode::YELLOW:
          Y_LED();
          break;

        case ColourCode::GREEN:
          G_LED();
          break;

        default: return;
      }
    }
    else
    {
      RGBOFF();
    }
  }
}

void fallback()
{
  if(currentColour == ColourCode::NONE && isLED_ON)
    {
      // Reset all variables and turn off LEDS
      RGBOFF();
      avgTemperature = 0.0f;
      prevTime = currentTime;   // Set prev time to current which acts as a timer reset.
      tick = 0;
      lastTick = -1;
      interval = 0;
      isLED_ON = false;
      currentColour = ColourCode::NONE;

      Serial.println("A SERIOUS ERROR HAS OCCURRED! RESETTING TEMPERATURE LED SENSOR ........");
    }
}
