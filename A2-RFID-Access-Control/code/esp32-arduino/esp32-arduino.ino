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
  pinMode(R_PIN, OUTPUT);
  pinMode(G_PIN, OUTPUT);

  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println("Tap to begin ...");
}

void loop() {
    if (millis() - prevTime >= 100) {
        prevTime = millis();
        tick++;
    }

    if(lastTick != tick)
    {
        lastTick = tick;
        if(tick != 0 && tick % 10 == 0)
        {
            Serial.print("Time: ");
            Serial.print(tick / 10);
            Serial.println(" sec");
        } 
    }

    switch(cardState)
    {
        case States::IDLE:
            if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial())
            {
                cardDetected = false;
                digitalWrite(G_PIN, LOW);
                if((tick % 30) < 10)
                {
                    digitalWrite(R_PIN, HIGH);
                //   Serial.println("RED LED: HIGH");
                }
                else
                {
                    digitalWrite(R_PIN, LOW);
                //   Serial.println("RED LED: LOW");
                }
                return;
            }
            else
            {
                startTick = tick;
                cardDetected = true;
                cardState = States::TAPPED;
                digitalWrite(R_PIN, LOW);
                Serial.println("Card detected!");
                Serial.print(F("Card UID:"));
                dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
                Serial.println();
                Serial.print("PICC type: ");
                MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
                Serial.println(mfrc522.PICC_GetTypeName(piccType));
                mfrc522.PICC_HaltA();       // Halt PICC
                //mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
            }
            break;

        case States::TAPPED: {
            if(tick - startTick > 60)
            {
                cardState = States::IDLE;
                Serial.println("Process timed out! Please tap again ...");
                break;
            }
            break;
        }
    }
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



