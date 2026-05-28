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
  REJECT,
} States;

States cardState = States::IDLE;

Preferences prefs;
unsigned long prevTime = 0;
unsigned long tick = 0;
unsigned long lastTick = -1;
unsigned long startTick = 0;

const char* uidFile = "/uids.csv";
const bool debug = true;
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

bool findUID(byte* uid, bool debug);
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
  initPrefs();
  initLittleFS();
  initDatabase();
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
            playOnTap(SPKR_PIN);
            return;
        }
        case States::INSPECT: 
        {
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

            if(!setupComplete)
            {
                Serial.println("Preparing setup for master keycard ...");
                cardState = States::SETUP;
                break;
            }


            // If uid does not exists but is in register mode skip to setup
            if(!findUID(mfrc522.uid.uidByte, true) && registerMode)
            {
                cardState = States::SETUP;
                Serial.println("Assigning new card ...");
                break;
            }
            
    
            // Otherwise process authentication
            byte buffer[18];
            byte block = 0;
            MFRC522::StatusCode status;
            status = mfrc522.PCD_Authenticate(MFRC522::PICC_Command::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
            if(status != MFRC522::StatusCode::STATUS_OK)
            {
                cardState = States::IDLE;
                Serial.print("PCD_Authenticate() failed: ");
                Serial.println(mfrc522.GetStatusCodeName(status));
                mfrc522.PICC_HaltA();       // Halt PICC
                mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
                playDeclined(SPKR_PIN);
                break;
            }

            Serial.println("Keys match! Now reading ...");
            cardState = States::READ;
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
                mfrc522.PICC_HaltA();       // Halt PICC
                mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
                playDeclined(SPKR_PIN);
                break;
            }

            cardState = States::WRITE;
            break;
        }
        case States::WRITE:     //Assign? //Registration?
        {
            byte trailerAddress = 3;
            byte dataAddress = 2;
            byte readBlock[18];
            byte writeBlock[16];
            byte readSize = sizeof(readBlock);
            byte writeSize = sizeof(writeBlock);
            String text = "";
            MFRC522::StatusCode status;

            // Overwrite address with new key, keeping other elements the same
            mfrc522.MIFARE_Read(trailerAddress, readBlock, &readSize);
            Serial.println("Before: ");
            dump_byte_array(readBlock, readSize);
            Serial.println();
            memcpy(readBlock, key.keyByte, MFRC522::MIFARE_Misc::MF_KEY_SIZE);
            Serial.println("After: ");
            dump_byte_array(readBlock, readSize);
            Serial.println();

            // Write key to trailer address
            status = mfrc522.MIFARE_Write(trailerAddress, readBlock, writeSize);
            if(status != MFRC522::StatusCode::STATUS_OK)
            {
                cardState = States::IDLE;
                Serial.print("MIFARE_Write() failed: ");
                Serial.println(mfrc522.GetStatusCodeName(status));
                mfrc522.PICC_HaltA();       // Halt PICC
                mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
                playDeclined(SPKR_PIN);
                break;
            }

            // Write to csv file
            registerUID(mfrc522.uid.uidByte);
            Serial.println("Factory key overwritten!");
            

            // FOR SETUP OF MASTER KEY CARD ONLY
            if(!setupComplete)
            {
                // Overwrite default key in trailer block
                text = "MASTER";
                memcpy(writeBlock, text.c_str(), text.length() + 1);      // Include the null terminator '\0'
                Serial.println("Data to write: ");
                dump_byte_array(writeBlock, writeSize);
                Serial.println();
            }

            // By default replace datablock at the address (empty for non-master key cards)
            status = mfrc522.MIFARE_Write(dataAddress, writeBlock, writeSize);
            if(status != MFRC522::StatusCode::STATUS_OK)
            {
                cardState = States::IDLE;
                Serial.print("MIFARE_Write() failed: ");
                Serial.println(mfrc522.GetStatusCodeName(status));
                mfrc522.PICC_HaltA();       // Halt PICC
                mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
                playDeclined(SPKR_PIN);
                break;
            }
            if(text != "")
            {
                Serial.printf("Write successful at address (%i)!\n", dataAddress);
                setupCompleted();
            }
            else
            {
                Serial.printf("Cleared data block at addres (%i)!\n", dataAddress);
            }

            cardState = States::IDLE;
            mfrc522.PICC_HaltA();
            mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
            break;
        }
        case States::READ:      //Extract?  //Modify?
        {
            byte dataAddress = 2;
            byte buffer[18];
            byte blockSize = sizeof(buffer);
            String text = "MASTER";
            bool isMaster = false;
            MFRC522::StatusCode status;


            status = mfrc522.MIFARE_Read(dataAddress, buffer, &blockSize);
            if(status != MFRC522::StatusCode::STATUS_OK)
            {
                cardState = States::IDLE;
                Serial.print("MIFARE_Read() failed: ");
                Serial.println(mfrc522.GetStatusCodeName(status));
                mfrc522.PICC_HaltA();       // Halt PICC
                mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
                playDeclined(SPKR_PIN);
                break;
            }

            byte count = 0;
            // Check datablock
            for(byte i = 0; i < text.length(); i++)
            {
                if(text.c_str()[i] != buffer[i])
                {
                    Serial.printf("%i: %c\n", i, text.c_str()[i]);
                    break;
                }
                count++;
            }

            // If contains label master, toggle bool to allow card registration until tagged off
            isMaster = count == text.length();
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
                Serial.print("Register Mode: ");
                Serial.println(registerMode);
                cardState = States::IDLE;
                mfrc522.PICC_HaltA();       // Halt PICC
                mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
                break;
            }

            // Deregister key card from register (NOTE: CARD HAS ALREADY BEEN DETECTED ON LIST)
            if(!isMaster && registerMode)
            {
                deregisterUID(mfrc522.uid.uidByte);     // Remove card uid from list
                // Reset to factory key in trailer address
                writeToTrailer(knownKeys[0]);
                // Reset datablock for sign in
                clearBlock(2);

                mfrc522.PICC_HaltA();       // Halt PICC
                mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
                cardState = States::IDLE;       // TEMP
                break;
            }
                
            if(!isMaster && !registerMode)
            {
                byte writeBlock[16];
                byte writeSize = sizeof(writeBlock);
                int hashValue = uidHash(mfrc522.uid.uidByte);

                // If none, create a hash and save to card
                if(isBlockEmpty(buffer))
                {
                    memcpy(writeBlock, &hashValue, sizeof(hashValue));
                    status = mfrc522.MIFARE_Write(dataAddress, writeBlock, writeSize);
                    if(status != MFRC522::StatusCode::STATUS_OK)
                    {
                        cardState = States::IDLE;
                        Serial.print("MIFARE_Write() failed: ");
                        Serial.println(mfrc522.GetStatusCodeName(status));
                        mfrc522.PICC_HaltA();       // Halt PICC
                        mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
                        playDeclined(SPKR_PIN);
                        break;
                    }
                    Serial.println("Hash successfully written to datablock!");
                    cardState = States::IDLE;       
                    mfrc522.PICC_HaltA();       // Halt PICC
                    mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
                    playSuccess(SPKR_PIN);
                    break;

                }
                else
                {
                    int storedHash;
                    memcpy(&storedHash, buffer, sizeof(storedHash));
                    // If exists, clear uid at hash value & empty the data block
                    if(hashValue != storedHash)
                    {
                        Serial.println("Hash values do not match!");
                        cardState = States::IDLE;       
                        break;
                    }
                    status = mfrc522.MIFARE_Write(dataAddress, writeBlock, writeSize);
                    if(status != MFRC522::StatusCode::STATUS_OK)
                    {
                        cardState = States::IDLE;
                        Serial.print("MIFARE_Write() failed: ");
                        Serial.println(mfrc522.GetStatusCodeName(status));
                        mfrc522.PICC_HaltA();       // Halt PICC
                        mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
                        playDeclined(SPKR_PIN);
                        break;
                    }
                    Serial.printf("Reset hash at datablock (%i)\n", dataAddress);       // CURRENTLY RUNS EVERY TIME
                    cardState = States::IDLE;       
                    mfrc522.PICC_HaltA();       // Halt PICC
                    mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
                    playSuccess(SPKR_PIN);
                    break;
                }
            }

            cardState = States::IDLE;  
            mfrc522.PICC_HaltA();       // Halt PICC
            mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
            break;
            
        }
        case States::ACCEPT:
        {
            // Play success sound and time within 3 seconds before returning to idle.
            // if(!mfrc522.PICC_ReadCardSerial())
            // {
            //     cardState = States::IDLE;
            //     return;
            // }
            Serial.println("TESTING: CURRENTLY ACCEPTING!!! RETURNING TO IDLE");
            cardState = States::IDLE;
            mfrc522.PICC_HaltA();       // Halt PICC
            mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
            break;
        }
        // case States::REJECT:
        // {
        //     break;
        // }
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
        Serial.print("PCD_Authenticate() failed: ");
        Serial.println(mfrc522.GetStatusCodeName(status));
        return false;
    }

    // Read block
    byte byteCount = sizeof(buffer);
    status = mfrc522.MIFARE_Read(block, buffer, &byteCount);
    if (status != MFRC522::StatusCode::STATUS_OK) {
        Serial.print("MIFARE_Read() failed: ");
        Serial.println(mfrc522.GetStatusCodeName(status));
    }
    else {
        // Successful read
        result = true;
        Serial.print("Success with key:");
        dump_byte_array((*key).keyByte, MFRC522::MIFARE_Misc::MF_KEY_SIZE);
        Serial.println();
        // Dump block data
        Serial.print("Block "); Serial.print(block); Serial.print(F(":"));
        dump_byte_array(buffer, 16);
        Serial.println();
    }
    Serial.println();

    // The following stops communication between the card and reader.
    //mfrc522.PICC_HaltA();       // Halt PICC
    //mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
    return result;
}


void initPrefs()
{
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
}

void setupCompleted()
{
    setupComplete = true;
    prefs.begin("rfid", false);
    prefs.putBool("setup", true);
    Serial.println("Setup has been completed!");
    prefs.end();
}

void initDatabase()
{
    File file = LittleFS.open(uidFile);
    if(!file || file.isDirectory())
    {
        String values = "";
        for(int i = 0; i < TABLESIZE; i++)
        {
            values += "00000000\n";
        }
        writeFile(LittleFS, uidFile, values.c_str());
    }
    file.close();
}

int uidHash(byte* uidByte)
{
    int hash = 0;
    for(int i = 0; i < mfrc522.uid.size; i++)
    {
        hash = (hash * 31) + uidByte[i];
    }
    return abs(hash);
}

int uidIndex(byte* uidByte)
{
    return uidHash(uidByte) % TABLESIZE;
}


// May not need to store hash, but just ids
void getUserFileContent(String* content, byte* uid, int size = 2, bool debug = false)
{
    String line = readLine(LittleFS, uidFile, uidIndex(uid), true);
    if(line == "")
    {
        Serial.println("Failed to retrieve user uid content!");
        return;
    }

    int comma =  line.indexOf(',');
    String id = line.substring(0, comma);
    String hash = line.substring(comma);
    content = new String(size);
    content[0] = id;
    content[1] = hash;

    if(debug)
    {
        Serial.printf("UID: %s || Hash: %s\n", id, hash);
    }
}

bool findUID(byte* uid, bool debug)
{
    String line = readLine(LittleFS, uidFile, uidIndex(uid), true);
    if(line == "")
    {
        Serial.println("Failed to retrieve user uid content!");
        return false;
    }

    for(byte i = 0; i < mfrc522.uid.size; i++)
    {
        if(debug)
        {
            Serial.printf("Line[%i]: %c || uid[%i]: %c\n", i, line.c_str()[i], i, uid[i]);
        }
        if(line.c_str()[i] != uid[i])
        {
            return false;
        }
    }

    if(debug)
    {
        Serial.printf("UID: %s", line);
    }
    return true;
}

bool hashCheck(byte* uid)
{
    String* content;
    getUserFileContent(content, uid);
    String hash =  content[1];
    return hash.toInt() == uidHash(uid);
}

bool isBlockEmpty(byte* buffer)
{
    bool isEmpty = true;
    for(byte i = 0; i < sizeof(buffer); i++)
    {
        if(buffer[i] != 0x00)
        {
            isEmpty = false;
            break;
        }
    }
    return isEmpty;
}


// bool idAvailable(byte* uid)

void registerUID(byte* uid)
{
    // Note that once index is found need to multiply by uid length to get cursor position (ignores new line key)
    File file = LittleFS.open(uidFile, "r+");
    int index = uidIndex(uid);
    // int position = index * (mfrc522.uid.size + 1);
    int position = index * 9;
    file.seek(position); 
    String line = file.readStringUntil('\n');
    if(line == "00000000")
    {
        file.seek(position);
        file.write(uid, 8);
    }
    else
    {
        Serial.printf("Unable to overwrite existing UID: %s\n", line);
    }
    file.close();
}


void deregisterUID(byte* uid)
{
    // This needs to restore factory key otherwise it becomes bricked!!!
    File file = LittleFS.open(uidFile, "r+");
    int index = uidIndex(uid);
    // int position = index * (mfrc522.uid.size + 1);
    int position = index * 9;
    file.seek(position); 
    String line = file.readStringUntil('\n');
    if(line != "00000000")
    {
        file.seek(position);
        file.write((const uint8_t*)"00000000", 8);
        Serial.println("UID has been deregistered!");
    }
    file.close();
}

void writeToTrailer(byte* key)
{
    byte trailerAddress = 3;
    byte readBlock[18];
    byte readSize = sizeof(readBlock);
    
    // Overwrite address with new key, keeping other elements the same
    mfrc522.MIFARE_Read(trailerAddress, readBlock, &readSize);
    memcpy(readBlock, key, MFRC522::MIFARE_Misc::MF_KEY_SIZE);

    // Write key to trailer address
    MFRC522::StatusCode status = mfrc522.MIFARE_Write(trailerAddress, readBlock, readSize - 2);
    if(status != MFRC522::StatusCode::STATUS_OK)
    {
        cardState = States::IDLE;
        Serial.print("MIFARE_Write() failed: ");
        Serial.println(mfrc522.GetStatusCodeName(status));
        Serial.println("Failed to overwrite trailer address!");
        mfrc522.PICC_HaltA();       // Halt PICC
        mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
        playDeclined(SPKR_PIN);
        return;
    }
    Serial.println("Successfully overwridden trailer address!");
    dump_byte_array(readBlock, readSize);
    Serial.println();
}

void clearBlock(byte address)
{
    byte writeBlock[16];
    byte writeSize = sizeof(writeBlock);
    MFRC522::StatusCode status = status = mfrc522.MIFARE_Write(address, writeBlock, writeSize);
    if(status != MFRC522::StatusCode::STATUS_OK)
    {
        cardState = States::IDLE;
        Serial.print("MIFARE_Write() failed: ");
        Serial.println(mfrc522.GetStatusCodeName(status));
        Serial.printf("Datablock [%i] failed to clear!\n", address);
        mfrc522.PICC_HaltA();       // Halt PICC
        mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
        playDeclined(SPKR_PIN);
        return;
    }
    Serial.printf("Datablock [%i] cleared successfully!\n", address);
}

void writeToDataBlock(byte address, String data)
{
    // // Clear block as leftover values may be carried over
    // clearBlock(address);   
    byte writeBlock[16];
    byte dataSize = sizeof(data.c_str());
    if(dataSize > sizeof(writeBlock))
    {
        Serial.println("Data exceeded write block max size! ");
        mfrc522.PICC_HaltA();       // Halt PICC
        mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
        playDeclined(SPKR_PIN);
        return;
    }

    memcpy(writeBlock, data.c_str(), dataSize + 1);
    MFRC522::StatusCode status = status = mfrc522.MIFARE_Write(address, writeBlock, sizeof(writeBlock));
    if(status != MFRC522::StatusCode::STATUS_OK)
    {
        cardState = States::IDLE;
        Serial.print("MIFARE_Write() failed: ");
        Serial.println(mfrc522.GetStatusCodeName(status));
        Serial.printf("Datablock [%i] failed to write!\n", address);
        mfrc522.PICC_HaltA();       // Halt PICC
        mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
        playDeclined(SPKR_PIN);
        return;
    }
    Serial.printf("Wrote to Datablock [%i] successfully!\n", address);
    
}


// Can only be used post-authentication
void factoryResetCard()
{
    Serial.println("Resetting key card to defaults!");
    deregisterUID(mfrc522.uid.uidByte);
    clearBlock(2);
    writeToTrailer(knownKeys[0]);
    mfrc522.PICC_HaltA();       // Halt PICC
    mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
}