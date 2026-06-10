/**************************************************
Sounds based on code by Urish (2016). 
https://wokwi.com/projects/344891334169985618 
***************************************************/

#ifndef SOUNDS_H
#define SOUNDS_H

#include "pitches.h"
#include <Arduino.h>

void playOnTap(uint8_t speakerPin);
void playDeclined(uint8_t speakerPin);
void playSuccess(uint8_t speakerPin);

#endif