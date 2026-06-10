#include <Arduino.h>
#include "cardReader.h"



#define SPKR_PIN 25
#define G_PIN   26
#define R_PIN   27
#define SS_PIN  5
#define RST_PIN   21

void setup()
{
  Serial.begin(9600);
  while(!Serial);
  initCardReader(SS_PIN, RST_PIN, SPKR_PIN, R_PIN, G_PIN);
}

void loop() 
{
    initCustomTimer(true);
    cardReadProcess(true);
}