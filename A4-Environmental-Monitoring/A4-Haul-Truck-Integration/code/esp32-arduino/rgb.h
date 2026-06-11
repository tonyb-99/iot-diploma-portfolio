#ifndef RGB_H
#define RGB_H

#include <Arduino.h>

struct RGB 
{
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

extern RGB rgb;

void initRGB(uint8_t r_pin, uint8_t g_pin, uint8_t b_pin);
void RGBOFF(bool debug = false);
void redON(float intensity = 0.1f);
void greenON(float intensity = 0.1f);
void blueON(float intensity = 0.1f);
void yellowON(float intensity = 0.2f);

#endif


