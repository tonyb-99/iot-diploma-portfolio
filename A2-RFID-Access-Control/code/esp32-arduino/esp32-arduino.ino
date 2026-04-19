#include <MFRC522v2.h>
#include <MFRC522DriverSPI.h>
#include <MFRC522DriverPinSimple.h>
#include <MFRC522Debug.h>
#include "file_manager.h"
#include "sounds.h"

// Code based on RandomNerdTutorial (2026). https://randomnerdtutorials.com/esp32-mfrc522-rfid-reader-arduino/

#define SPEAKER_PIN 
#define G_PIN
#define R_PIN
#define NR_KNOWN_KEYS   8
// Known keys, see: https://code.google.com/p/mfcuk/wiki/MifareClassicDefaultKeys
byte knownKeys[NR_KNOWN_KEYS][MFRC522::MF_KEY_SIZE] =  {
    {0xff, 0xff, 0xff, 0xff, 0xff, 0xff}, // FF FF FF FF FF FF = factory default
    {0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5}, // A0 A1 A2 A3 A4 A5
    {0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5}, // B0 B1 B2 B3 B4 B5
    {0x4d, 0x3a, 0x99, 0xc3, 0x51, 0xdd}, // 4D 3A 99 C3 51 DD
    {0x1a, 0x98, 0x2c, 0x7e, 0x45, 0x9a}, // 1A 98 2C 7E 45 9A
    {0xd3, 0xf7, 0xd3, 0xf7, 0xd3, 0xf7}, // D3 F7 D3 F7 D3 F7
    {0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff}, // AA BB CC DD EE FF
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}  // 00 00 00 00 00 00
};

MFRC522DriverPinSimple ss_pin(5);
MFRC522DiverSPI driver{ss_pin}; // Create SPI driver
MFRC522 mfrc522{driver};  // Create MFRC522 instance
MFRC522::MIFARE_Key key;

const char* keyPath = "/keypath.txt";
const char* userPath = "/userpath.txt";
const bool debug = true;


void setup()
{
  Serial.begin(115200);
  while(!Serial);

  initLittleFS();
  mfrc522.PCD_Init();
}

void loop();
{
  // Check card presence
  if(!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) { delay(500); return; }
  else { playOnTap(SPEAKER_PIN); }

  Serial.print("-----------------------------------------\nCard UID:");
  MFRC522Debug::PrintUID(Serial, (mfrc522.uid));
  Serial.println();

  // Check if new card with default key
  for(byte j = 0; j < NR_KNOWN_KEYS; j++)
  {
    for(byte i = 0; i < MFRC522::MF_KEY_SIZE; i++) 
    {
      key.keyByte[i] = knownKeys[j][i];
    }

    if(try_key(&key))   // Authentication process ends during test.
    {
      Serial.println("Contains default key.");
      break;
    }

    if(!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) { break; }
  }
    // Load key file (made in setup)
    String loadedKey = readFile(LittleFS, keyPath);
    // If key is empty, generate a key
    if(loadedKey == "")
    {
      
    }
    // Overwrite default key with new key
    // Register card UID to memory/file
    // Re-authenticate

  // Authenticate with matching key file to trailer
  byte trailerAddress = 3;
  if(mfrc522.PCD_Authenticate(0x60, trailerAddress, &key, &(mfrc522.uid)) != 0)     //Block address = 3
  {
    Serial.println("Authentication failed");
    playDeclined(SPEAKER_PIN);
    return;
  }
  
  // Check a specific empty block on card for current status
  // Write current status to card in an empty block




  byte blockAddress = 2;
  byte bufferblocksize = 18;
  byte blockDataRead[bufferblocksize];
  if(mfrc522.MIFARE_Read(blockAddress, blockDataRead, &bufferblocksize) != 0)
  {
    Serial.println("Authentication failed");
    playDeclined(SPEAKER_PIN);
    return;
  }
  else
  {
    Serial.println("Read successfully!");
    playSuccess(SPEAKER_PIN);

    /********************DEBUG**************************/
    Serial.print("Data in block ");
    Serial.print(blockAddress);
    Serial.print(": ");
    for (byte i = 0; i < 16; i++) {
      Serial.print((char)blockDataRead[i]);  // Print as character
    }
    Serial.println();
    /**************************************************/
  }

  //Keep access open for short duration (3 sec)
  //Record state to file (IN/OUT)
  
  // End process
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();

  delay(1000);
}


bool try_key(MFRC522::MIFARE_Key *key)
{
    bool result = false;
    byte buffer[18];
    byte block = 0;
    MFRC522::StatusCode status;

    // Serial.println(F("Authenticating using key A..."));
    status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
        // Serial.print(F("PCD_Authenticate() failed: "));
        // Serial.println(mfrc522.GetStatusCodeName(status));
        return false;
    }

    // Read block
    byte byteCount = sizeof(buffer);
    status = mfrc522.MIFARE_Read(block, buffer, &byteCount);
    if (status != MFRC522::STATUS_OK) {
        // Serial.print(F("MIFARE_Read() failed: "));
        // Serial.println(mfrc522.GetStatusCodeName(status));
    }
    else {
        // Successful read
        result = true;
        Serial.print(F("Success with key:"));
        dump_byte_array((*key).keyByte, MFRC522::MF_KEY_SIZE);
        Serial.println();
        // Dump block data
        Serial.print(F("Block ")); Serial.print(block); Serial.print(F(":"));
        dump_byte_array(buffer, 16);
        Serial.println();
    }
    Serial.println();

    mfrc522.PICC_HaltA();       // Halt PICC
    mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
    return result;
}


