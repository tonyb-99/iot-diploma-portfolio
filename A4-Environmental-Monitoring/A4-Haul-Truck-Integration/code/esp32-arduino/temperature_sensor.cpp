#include "temperature_sensor.h"

namespace{
  uint8_t tempPin;
  float avgTemp = 0;
  bool isRGB_ON = false;
  int interval = 50;
  ColourCode currentColour = ColourCode::NONE;
}

void initThermistor(uint8_t thermistorPin)
{
  tempPin = thermistorPin;
  pinMode(tempPin, INPUT);
  Serial.println("Thermistor initialised.");
}

// Has tendency to have outliers, should reject extreme values given intended for safe temperatures. 
void calcTemperature(bool debug)
{
  // Temperature calculation based on the following code by Uri Shaked (2021): https://wokwi.com/projects/299330254810382858 
  avgTemp = 0;
  int count = 0;
  int samples = 5;

  while(count < samples)
  {
    int analogValue = analogRead(tempPin);
    float celsius = 1.0 / (log( 1.0 / (ANALOG_MAX / analogValue - 1.0)) / BETA + 1.0 / (ROOM_TEMP + KLVN)) - KLVN;

    if(debug)
    {
      Serial.print("Analog value: ");
      Serial.print(analogValue);
      Serial.print("\t\t");
      Serial.print("Temperature: ");
      Serial.print(celsius);
      Serial.println(" *C");
    }
    
    avgTemp += celsius;
    count++;

    delay(100);
  }

  avgTemp /= (float)samples;
  if(debug) { Serial.printf("Average Temperature: %.2f *C\n", avgTemp); }
}

float getAvgTemp(bool debug)
{
  if(debug) { Serial.printf("Average Temperature: %.2f *C\n", avgTemp); }
  return avgTemp;
}

void selectColour(bool debug)
{
  // Check dangers first before approaching safety
  // Danger if below -10 or above 43 C
  // if(avgTemp < Conditions::FREEZING || avgTemp > Conditions::HOT)
  // {
  //   if(debug) { Serial.println("Colour code: RED"); }
  //   currentColour = ColourCode::RED;
  // }
  // // Warning if below 10 or above 37 C
  // else if(avgTemp < Conditions::COLD || avgTemp > Conditions::WARM)
  // {
  //   if(debug) { Serial.println("Colour code: YELLOW"); }
  //   currentColour = ColourCode::YELLOW;
  // }

  // // Safe range: 11-36 C
  // else
  // {
  //   if(debug) { Serial.println("Colour code: GREEN"); }
  //   currentColour = ColourCode::GREEN;
  // }
  

  currentColour = ColourCode::RED;
}

ColourCode getCurrentColour()
{
  return currentColour;
}

void handleLED(bool debug)
{ 
  // int interval = (int)getColour();
  int interval = (int)currentColour;
  Serial.printf("Interval: %i\n", interval);
  isRGB_ON = !isRGB_ON;
  // Serial.printf("RGB: %s\n", isRGB_ON ? "ON" : "OFF");
  if(isRGB_ON)
  {
    switch(currentColour)
    {
      case ColourCode::RED:
        redON();
        break;

      case ColourCode::YELLOW:
        yellowON();
        break;

      case ColourCode::GREEN:
        greenON();
        break;

      default: return;
    }
  }
  else
  {
    RGBOFF(debug);
  }

  // if(getTick() % interval == 0)
  // {
  //   isRGB_ON = !isRGB_ON;
  //   Serial.printf("RGB: %s\n", isRGB_ON ? "ON" : "OFF");
  //   if(isRGB_ON)
  //   {
  //     switch(currentColour)
  //     {
  //       case ColourCode::RED:
  //         redON();
  //         break;

  //       case ColourCode::YELLOW:
  //         yellowON();
  //         break;

  //       case ColourCode::GREEN:
  //         greenON();
  //         break;

  //       default: return;
  //     }
  //   }
  //   else
  //   {
  //     RGBOFF(debug);
  //   }
  // }
}


void temperatureAlert(bool debug)
{
  // Determine the avg temperature and colour every 5 seconds.
  if(getTick() % 100 == 0)
  {
    calcTemperature(debug);
    // selectColour(debug);
  }

  if(getTick() % interval == 0)
  {
    selectColour(debug);
    handleLED(debug);
  }
  
}



