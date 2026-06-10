// Code based on RandomNerdTutorial (2026). https://randomnerdtutorials.com/esp32-mfrc522-rfid-reader-arduino/  https://randomnerdtutorials.com/esp32-rfid-user-management-web-server/

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

void initCardReader(uint8_t ssPin, uint8_t rstPin, uint8_t speaker, uint8_t redLED, uint8_t greenLED);
void cardReadProcess(bool debug = false);
void dump_byte_array(byte *buffer, byte bufferSize);
bool try_key(MFRC522::MIFARE_Key *key, bool debug = false);
void initPrefs();
void setupCompleted();
void initDatabase();
int uidHash(byte* uidByte);
int uidIndex(byte* uidByte);
String uidToString(byte* uid);
bool findUID(byte* uid, bool debug = false);
void registerUID(byte* uid, bool debug = false);
void deregisterUID(byte* uid, bool debug = false);
void writeToTrailer(byte* key, bool debug = false);
void clearBlock(byte address, bool debug = false);
void writeToDataBlock(byte address, String data, bool debug = false);
String readDataBlock(byte address, bool debug = false);
void factoryResetCard(bool debug = false);

#endif