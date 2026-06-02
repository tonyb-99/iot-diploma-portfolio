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
bool try_key(MFRC522::MIFARE_Key *key);
void initPrefs();
void setupCompleted();
void initDatabase();
int uidHash(byte* uidByte);
int uidIndex(byte* uidByte);
String uidToString(byte* uid);
void getUserFileContent(String* content, byte* uid, int size = 2, bool debug = false);
bool findUID(byte* uid, bool debug = false);
bool hashCheck(byte* uid);
bool isBlockEmpty(byte* buffer);
void registerUID(byte* uid);
void deregisterUID(byte* uid);
void writeToTrailer(byte* key);
void clearBlock(byte address);
void writeToDataBlock(byte address, String data);
String readDataBlock(byte address);
void factoryResetCard();



#endif