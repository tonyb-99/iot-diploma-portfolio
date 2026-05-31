#ifndef CARD_READER_H
#define CARD_READER_H

#include "file_manager.h"
#include "custom_timer.h"
#include "sounds.h"
#include <MFRC522.h>
#include <SPI.h>
#include <Preferences.h>
#include <Arduino.h>


#define TABLESIZE   50
#define NR_KNOWN_KEYS   8

typedef enum {
  IDLE,
  INSPECT,
  SETUP,
  WRITE,
  READ,
  ENTER,
  EXIT,
  ACCEPT,
  REJECT,
} States;

extern States cardState;
extern Preferences prefs;


#endif