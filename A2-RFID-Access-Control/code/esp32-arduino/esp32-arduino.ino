#include <MFRC522.h>
#include <SPI.h>
#include <Preferences.h>
#include "file_manager.h"
#include "sounds.h"

// Code based on RandomNerdTutorial (2026). https://randomnerdtutorials.com/esp32-mfrc522-rfid-reader-arduino/

#define SPEAKER_PIN 25
#define G_PIN   26
#define R_PIN   27
#define NR_KNOWN_KEYS   8
#define TABLESIZE   50
#define SS_PIN  5
#define RST_PIN   21

typedef enum {
  IDLE,
  TAPPED,
  PROCESSING,
  AUTHENTICATE,
} States;

States cardState = States::IDLE;

Preferences prefs;
unsigned long prevTime = 0;
unsigned long tick = 0;
unsigned long lastTick = -1;
unsigned long startTick = 0;

const char* passKey = "/passKey.txt";
const bool debug = true;
bool cardDetected = false;
MFRC522::Uid user[TABLESIZE];

MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;
// Known keys, see: https://code.google.com/p/mfcuk/wiki/MifareClassicfactoryKeys
byte knownKeys[NR_KNOWN_KEYS][MFRC522::MIFARE_Misc::MF_KEY_SIZE] =  {
    {0xff, 0xff, 0xff, 0xff, 0xff, 0xff}, // FF FF FF FF FF FF = factory default
    {0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5}, // A0 A1 A2 A3 A4 A5
    {0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5}, // B0 B1 B2 B3 B4 B5
    {0x4d, 0x3a, 0x99, 0xc3, 0x51, 0xdd}, // 4D 3A 99 C3 51 DD
    {0x1a, 0x98, 0x2c, 0x7e, 0x45, 0x9a}, // 1A 98 2C 7E 45 9A
    {0xd3, 0xf7, 0xd3, 0xf7, 0xd3, 0xf7}, // D3 F7 D3 F7 D3 F7
    {0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff}, // AA BB CC DD EE FF
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}  // 00 00 00 00 00 00
};

void setup()
{
  Serial.begin(9600);
  while(!Serial);
  SPI.begin();
  mfrc522.PCD_Init();
}

void loop() {
    if (millis() - prevTime >= 1000) {
        Serial.println("1 second passed");
        prevTime = millis();
    }

    // RFID logic (independent)
    if (!mfrc522.PICC_IsNewCardPresent())
        return;

    if (!mfrc522.PICC_ReadCardSerial())
        return;

    Serial.print(F("Card UID:"));
    dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
    Serial.println();
}

/**
 * Helper routine to dump a byte array as hex values to Serial.
 */
void dump_byte_array(byte *buffer, byte bufferSize) {
    for (byte i = 0; i < bufferSize; i++) {
        Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        Serial.print(buffer[i], HEX);
    }
}



