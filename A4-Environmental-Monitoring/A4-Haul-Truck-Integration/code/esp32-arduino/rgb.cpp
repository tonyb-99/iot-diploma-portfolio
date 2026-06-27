#include "rgb.h"

RGB rgb;

ColourCode tempColour = ColourCode::GREEN;
ColourCode gasColour = ColourCode::GREEN;
ColourCode vibrationColour = ColourCode::GREEN;

namespace{
  unsigned long startTick = 0;
  bool isRGB_ON = false;
  // int interval = 30;
  int interval = (int)ColourCode::GREEN;
  ColourCode currentColour = ColourCode::GREEN;
}

void initRGB(uint8_t r_pin, uint8_t g_pin, uint8_t b_pin)
{
  rgb = {r_pin, g_pin, b_pin};
  Serial.printf("R: %i \tG: %i \tB: %i\n", rgb.r, rgb.g, rgb.b);
  pinMode(rgb.r, OUTPUT);
  pinMode(rgb.g, OUTPUT);
  pinMode(rgb.b, OUTPUT);
  Serial.println("Initialised RGB LED!");
}

void RGBOFF(bool debug)
{
  analogWrite(rgb.r, 0);
  analogWrite(rgb.g, 0);
  analogWrite(rgb.b, 0);
  if(debug) { Serial.println("RGB OFF."); }
}

void redON(float intensity)
{
  if(intensity < 0.0f || intensity > 1.0f) { return; }
  int brightness = 255 * intensity;
  analogWrite(rgb.r, brightness);
}

void greenON(float intensity)
{
  if(intensity < 0.0f || intensity > 1.0f) { return; }
  int brightness = 255 * intensity;
  analogWrite(rgb.g, brightness);
}

void blueON(float intensity)
{
  if(intensity < 0.0f || intensity > 1.0f) { return; }
  int brightness = 255 * intensity;
  analogWrite(rgb.b, brightness);
}

void yellowON(float intensity)
{
  if(intensity < 0.0f || intensity > 1.0f) { return; }
  int brightness = 255 * intensity;
  analogWrite(rgb.r, brightness);
  analogWrite(rgb.g, brightness);
}

void getCurrentColour()
{
  if(gasColour == ColourCode::GREEN || vibrationColour == ColourCode::GREEN)
  {
    if(tempColour == ColourCode::YELLOW)
    {
      currentColour = ColourCode::YELLOW;
    }
    else if(tempColour == ColourCode::RED)
    {
      currentColour = ColourCode::RED;
    }
    else
    {
      currentColour = ColourCode::GREEN;
    }
  }
  else
  {
    currentColour = ColourCode::RED;
  }
}

void handleLED(bool debug)
{ 
  getCurrentColour();
  interval = (int)currentColour;
  if(debug) { Serial.printf("Interval: %i\n", interval); }
  isRGB_ON = !isRGB_ON;
  if(isRGB_ON)
  {
    switch(currentColour)
    {
      case ColourCode::RED:
        redON();
        if(debug) { Serial.println("LED Colour: RED"); }
        break;

      case ColourCode::YELLOW:
        yellowON();
        if(debug) { Serial.println("LED Colour: YELLOW"); }
        break;

      case ColourCode::GREEN:
        greenON();
        if(debug) { Serial.println("LED Colour: GREEN"); }
        break;

      default: return;
    }
  }
  else
  {
    RGBOFF();
  }
}


void RGBAlert(bool debug)
{
  if(getTick() - startTick >= interval)
  {
    startTick = getTick();
    handleLED(debug);
  } 
}


















