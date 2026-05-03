#include <Arduino.h>
#include <MFRC522.h>
#include <SPI.h>
#include <Preferences.h>
#include "file_manager.h"
#include "sounds.h"

// Code based on RandomNerdTutorial (2026). https://randomnerdtutorials.com/esp32-mfrc522-rfid-reader-arduino/  https://randomnerdtutorials.com/esp32-rfid-user-management-web-server/

#define SPKR_PIN 25
#define G_PIN   26
#define R_PIN   27
#define NR_KNOWN_KEYS   8
#define TABLESIZE   50
#define SS_PIN  5
#define RST_PIN   21

typedef enum {
  IDLE,
  INSPECT,
  SETUP,
  WRITE,
  READ,
  ACCEPT,
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
bool playedSFX = false;
bool setupComplete;
bool registerMode = false;
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


// Assume the first card is the master key which enables privilege to register new cards until tagged off.

void setup()
{
  Serial.begin(9600);
  while(!Serial);
  pinMode(R_PIN, OUTPUT);
  pinMode(G_PIN, OUTPUT);

  SPI.begin();
  mfrc522.PCD_Init();
  delay(1000);
  prefs.begin("rfid", false);
  //prefs.clear();
  byte keyData[MFRC522::MIFARE_Misc::MF_KEY_SIZE];
  size_t len = prefs.getBytesLength("key");
  Serial.print("Key length: ");
  Serial.println(len);

  if(len == MFRC522::MIFARE_Misc::MF_KEY_SIZE)
  {
    Serial.println("Key exists in memory!");
    prefs.getBytes("key", key.keyByte, MFRC522::MIFARE_Misc::MF_KEY_SIZE);
  }
  else
  {
    Serial.println("Generating new key in memory ...");
    for(byte i = 0; i < MFRC522::MIFARE_Misc::MF_KEY_SIZE; i++)
    {
      keyData[i] = (byte)(esp_random() & 0xff);
      key.keyByte[i] = keyData[i];
    }
    prefs.putBytes("key", keyData, MFRC522::MIFARE_Misc::MF_KEY_SIZE);

  }
  
  setupComplete = prefs.getBool("setup");
  if(!setupComplete)
  {
    Serial.print("Master key card has not been created yet ...");
  }
  prefs.end();
  dump_byte_array(key.keyByte, MFRC522::MIFARE_Misc::MF_KEY_SIZE);
  Serial.println();

  initLittleFS();

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
        {
            if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial())
            {
                //cardDetected = false;
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

            startTick = tick;
            //cardDetected = true;
            cardState = States::INSPECT;

            digitalWrite(R_PIN, LOW);
            digitalWrite(G_PIN, HIGH);
            

            Serial.println("Card detected!");
            Serial.print(F("Card UID:"));
            dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
            Serial.println();
            Serial.print("PICC type: ");
            MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
            Serial.println(mfrc522.PICC_GetTypeName(piccType));
            //mfrc522.PICC_HaltA();       // Halt PICC
            
            playOnTap(SPKR_PIN);
            return;
        }
        case States::INSPECT: 
        {
            // if(!mfrc522.PICC_ReadCardSerial())
            // {
            //     cardState = States::IDLE;
            //     return;
            // }

            if(tick - startTick > 5)
            {
                digitalWrite(G_PIN, LOW);
            }

            if(tick - startTick > 60)
            {
                startTick = 0;
                cardState = States::IDLE;
                Serial.println("Process timed out! Please tap again ...");
                playDeclined(SPKR_PIN);
                break;
            }

            if(!setupComplete || registerMode)
            {
                cardState = States::SETUP;
                break;
            }

            byte buffer[18];
            byte block = 0;
            MFRC522::StatusCode status;

            status = mfrc522.PCD_Authenticate(MFRC522::PICC_Command::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
            if(status != MFRC522::StatusCode::STATUS_OK)
            {
                cardState = States::IDLE;
                Serial.print("PCD_Authenticate() failed: ");
                Serial.println(mfrc522.GetStatusCodeName(status));
                playDeclined(SPKR_PIN);
                break;

                
            }
            else
            {
                cardState = States::READ;
                playSuccess(SPKR_PIN);
            }

            break;
        }

        case States::SETUP:
        {
            // Try the known default keys
            MFRC522::MIFARE_Key keys;
            bool hasKey = false;
            for (byte k = 0; k < NR_KNOWN_KEYS; k++) {
                // Copy the known key into the MIFARE_Key structure
                for (byte i = 0; i < MFRC522::MIFARE_Misc::MF_KEY_SIZE; i++) {
                    keys.keyByte[i] = knownKeys[k][i];
                }
                hasKey = try_key(&keys);
                // Try the key
                if (hasKey) {
                    // Found and reported on the key and block,
                    // no need to try other keys for this PICC
                    Serial.println("Contains known key: ");
                    dump_byte_array(keys.keyByte, MFRC522::MIFARE_Misc::MF_KEY_SIZE);
                    Serial.println();
                    //mfrc522.PICC_HaltA();
                    break;
                }
                
                // // End check if new card is detected
                // // http://arduino.stackexchange.com/a/14316
                if ( ! mfrc522.PICC_IsNewCardPresent())
                    break;
                if ( ! mfrc522.PICC_ReadCardSerial())
                    break;
            }

            if(!hasKey)
            {
                cardState = States::IDLE;
                Serial.println("PCD_Authenticate() failed: No default keys found!");
                playDeclined(SPKR_PIN);
                break;
            }
            else
            {
                cardState = States::WRITE;
                // byte block = 0;
                // MFRC522::StatusCode status;
                // // Re-authenticate
                // status = mfrc522.PCD_Authenticate(MFRC522::PICC_Command::PICC_CMD_MF_AUTH_KEY_A, block, &keys, &(mfrc522.uid));
                // if(status != MFRC522::StatusCode::STATUS_OK)
                // {
                //     cardState = States::IDLE;
                //     Serial.print("PCD_Authenticate() failed: ");
                //     Serial.println(mfrc522.GetStatusCodeName(status));
                //     playDeclined(SPKR_PIN);
                //     break;
                // }
                // else
                // {
                //     cardState = States::WRITE;
                // }
            }


            break;
        }
        case States::WRITE:
        {
            byte trailerAddress = 3;
            byte dataAddress = 2;
            // byte buffer[18];
            byte dataBlock[16];
            byte blockSize = 18;
            std::string text = "";
            MFRC522::StatusCode status;


            mfrc522.MIFARE_Read(trailerAddress, dataBlock, &blockSize);
            Serial.println("Before: ");
            dump_byte_array(dataBlock, blockSize);
            Serial.println();
            //memcpy(dataBlock, key.keyByte, MFRC522::MIFARE_Misc::MF_KEY_SIZE);
            for(byte i = 0; i < MFRC522::MIFARE_Misc::MF_KEY_SIZE; i++)
            {
                dataBlock[i] = key.keyByte[i];
            }

            Serial.println("After: ");
            dump_byte_array(dataBlock, blockSize);
            Serial.println();


            // status = mfrc522.MIFARE_Write(trailerAddress, dataBlock, 16);
            // if(status != MFRC522::StatusCode::STATUS_OK)
            // {
            //     cardState = States::IDLE;
            //     Serial.print("MIFARE_Write() failed: ");
            //     Serial.println(mfrc522.GetStatusCodeName(status));
            //     playDeclined(SPKR_PIN);
            //     break;
            // }

            // if(!setupComplete)
            // {

            //     // Overwrite default key in trailer block
            //     text = "MASTER";
            //     memcpy(dataBlock, text.c_str(), 16);
            //     status = mfrc522.MIFARE_Write(dataAddress, dataBlock, 16);
            //     if(status != MFRC522::StatusCode::STATUS_OK)
            //     {
            //         cardState = States::IDLE;
            //         Serial.print("MIFARE_Write() failed: ");
            //         Serial.println(mfrc522.GetStatusCodeName(status));
            //         playDeclined(SPKR_PIN);
            //         break;
            //     }
            //     setupCompleted();
            // }

            // // Check key and data block
            // status = mfrc522.MIFARE_Read(dataAddress, buffer, &blockSize);
            // if(status != MFRC522::StatusCode::STATUS_OK)
            // {
            //     cardState = States::IDLE;
            //     Serial.print(F("MIFARE_Read() failed: "));
            //     Serial.println(mfrc522.GetStatusCodeName(status));
            //     playDeclined(SPKR_PIN);
            //     break;
            // }
            // byte count = 0;
            // for(byte i = 0; i < 16; i++)
            // {
            //     if(buffer[i] == dataBlock[i])
            //     {
            //         count++;
            //     }
            // }

            // if(count == 16)
            // {
            //     Serial.print("Number of bytes that match = ");
            //     Serial.println(count);
            //     playSuccess(SPKR_PIN);
            // }
            // else
            // {
            //     Serial.println("No match!"); 
            //     playDeclined(SPKR_PIN);
            // }
            
            cardState = States::IDLE;
            mfrc522.PICC_HaltA();
            mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD

            break;
        }
        case States::READ: 
        {
            // if(!mfrc522.PICC_ReadCardSerial())
            // {
            //     cardState = States::IDLE;
            //     return;
            // }
            // Check trailer address
            byte dataAddress = 2;
            byte buffer[18];
            byte blockSize = sizeof(buffer);
            std::string text = "MASTER";
            bool isMaster = false;
            MFRC522::StatusCode status;


            status = mfrc522.MIFARE_Read(dataAddress, buffer, &blockSize);
            if(status != MFRC522::StatusCode::STATUS_OK)
            {
                cardState = States::IDLE;
                Serial.print("MIFARE_Read() failed: ");
                Serial.println(mfrc522.GetStatusCodeName(status));
                playDeclined(SPKR_PIN);
                break;
            }

            byte count = 0;
            // Check datablock
            for(byte i = 0; i < text.size(); i++)
            {
                if(text.c_str()[i] != buffer[i])
                {
                    break;
                }
                count++;
            }

            isMaster = count == text.size();
            if(isMaster)
            {
                cardState = States::IDLE;
                Serial.println("Master card detected!");
                if(!registerMode)
                {
                    registerMode = true;
                }
                else
                {
                    registerMode = false;
                }
                break;
            }

                
            count = 0;
            // If contains label master, toggle bool to allow card registration until tagged off
            // Otherwise if empty/zeroes, hash the uid and store in the datablock and esp32 file.
            if(count != MFRC522::MIFARE_Misc::MF_KEY_SIZE)
            {
                cardState = States::IDLE;
                mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
                Serial.print("PCD_Authenticate() failed: ");
                playDeclined(SPKR_PIN);
                break;
            }

            
            cardState = States::ACCEPT;

            break;
            
        }
        case States::ACCEPT:
        {
            // if(!mfrc522.PICC_ReadCardSerial())
            // {
            //     cardState = States::IDLE;
            //     return;
            // }
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

/*
 * Try using the PICC (the tag/card) with the given key to access block 0.
 * On success, it will show the key details, and dump the block data on Serial.
 *
 * @return true when the given key worked, false otherwise.
 */
bool try_key(MFRC522::MIFARE_Key *key)
{
    bool result = false;
    byte buffer[18];
    byte block = 0;
    MFRC522::StatusCode status;

    Serial.println(F("Authenticating using key A..."));
    status = mfrc522.PCD_Authenticate(MFRC522::PICC_Command::PICC_CMD_MF_AUTH_KEY_A, block, key, &(mfrc522.uid));
    if (status != MFRC522::StatusCode::STATUS_OK) {
        Serial.print(F("PCD_Authenticate() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return false;
    }

    // Read block
    byte byteCount = sizeof(buffer);
    status = mfrc522.MIFARE_Read(block, buffer, &byteCount);
    if (status != MFRC522::StatusCode::STATUS_OK) {
        Serial.print(F("MIFARE_Read() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
    }
    else {
        // Successful read
        result = true;
        Serial.print(F("Success with key:"));
        dump_byte_array((*key).keyByte, MFRC522::MIFARE_Misc::MF_KEY_SIZE);
        Serial.println();
        // Dump block data
        Serial.print(F("Block ")); Serial.print(block); Serial.print(F(":"));
        dump_byte_array(buffer, 16);
        Serial.println();
    }
    Serial.println();

    //mfrc522.PICC_HaltA();       // Halt PICC
    //mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
    return result;
}

void setupCompleted()
{
    prefs.begin("rfid", false);
    prefs.putBool("setup", true);
    setupComplete = true;
    Serial.println("Setup has been completed!");
    prefs.end();
}
