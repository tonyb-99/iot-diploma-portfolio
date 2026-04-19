/**************************************************
Sounds based on code by Urish (2016). 
https://wokwi.com/projects/344891334169985618 
***************************************************/

#include "sounds.h"


void playOnTap(uint8_t speakerPin)
{
  tone(speakerPin, NOTE_G5);
  delay(300);
  noTone(speakerPin);
}

void playDeclined(uint8_t speakerPin)
{
  // Play a Wah-Wah-Wah-Wah sound
  tone(speakerPin, NOTE_DS5);
  delay(300);
  tone(speakerPin, NOTE_D5);
  delay(300);
  tone(speakerPin, NOTE_CS5);
  delay(300);
  for (byte i = 0; i < 10; i++) {
    for (int pitch = -10; pitch <= 10; pitch++) {
      tone(speakerPin, NOTE_C5 + pitch);
      delay(5);
    }
  }
  noTone(speakerPin);
  delay(500);
}

void playSuccess(uint8_t speakerPin)
{
  tone(speakerPin, NOTE_E4);
  delay(150);
  tone(speakerPin, NOTE_G4);
  delay(150);
  tone(speakerPin, NOTE_E5);
  delay(150);
  tone(speakerPin, NOTE_C5);
  delay(150);
  tone(speakerPin, NOTE_D5);
  delay(150);
  tone(speakerPin, NOTE_G5);
  delay(150);
  noTone(speakerPin);
}