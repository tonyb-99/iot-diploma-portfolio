#include "rgb.h"

RGB rgb;

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
