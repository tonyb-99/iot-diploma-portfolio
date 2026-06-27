#ifndef RGB_H
#define RGB_H

#include "custom_timer.h"
#include <Arduino.h>

typedef enum { 
  NONE = 1, 
  GREEN = 20, 
  YELLOW = 10, 
  RED = 5,
  } ColourCode;

struct RGB 
{
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

extern RGB rgb;
extern ColourCode tempColour;
extern ColourCode gasColour;
extern ColourCode vibrationColour;

void initRGB(uint8_t r_pin, uint8_t g_pin, uint8_t b_pin);
void RGBOFF(bool debug = false);
void redON(float intensity = 0.1f);
void greenON(float intensity = 0.1f);
void blueON(float intensity = 0.1f);
void yellowON(float intensity = 0.2f);
void getCurrentColour();
void handleLED(bool debug = false);
void RGBAlert(bool debug = false);

#endif


