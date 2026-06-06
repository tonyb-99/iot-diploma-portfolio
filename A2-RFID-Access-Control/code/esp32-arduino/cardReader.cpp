// Code based on RandomNerdTutorial (2026). https://randomnerdtutorials.com/esp32-mfrc522-rfid-reader-arduino/  https://randomnerdtutorials.com/esp32-rfid-user-management-web-server/

#include "cardReader.h"

States cardState = States::IDLE;
Preferences prefs;
namespace {
  uint8_t redPin;
  uint8_t greenPin;
  uint8_t speakerPin;
  const char* uidFile = "/uids.csv";
  bool setupComplete;
  bool registerMode = false;
  MFRC522::Uid user[TABLESIZE];
  MFRC522* mfrc522;
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
}

void initCardReader(uint8_t ssPin, uint8_t rstPin, uint8_t speaker, uint8_t redLED, uint8_t greenLED)
{
  speakerPin = speaker;
  redPin = redLED;
  greenPin = greenLED;
  mfrc522 = new MFRC522(ssPin, rstPin);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);

  SPI.begin();
  mfrc522->PCD_Init();
  Serial.println("RFID Initiated ...");
  delay(1000);
  initPrefs();
  initLittleFS();
  initDatabase();
}

void cardReadProcess(bool debug)
{
  switch(cardState)
  {
    case States::IDLE:
    {
      // When no card is detected, end the process early
      if (!mfrc522->PICC_IsNewCardPresent() || !mfrc522->PICC_ReadCardSerial())
      {
        digitalWrite(greenPin, LOW);          
        if((getTick() % 30) < 10)             // Cycle every 3 seconds (Display for 1 second every 2 seconds).
        { 
          digitalWrite(redPin, HIGH); 
        }
        else 
        { 
          digitalWrite(redPin, LOW); 
        }
        return;
      }

      // Card detected
      startTick = getTick();            // Track amount of time pass per tick.
      cardState = States::INSPECT;    

      // Turn LED green on, red off
      digitalWrite(redPin, LOW);
      digitalWrite(greenPin, HIGH);
      
      MFRC522::PICC_Type piccType = mfrc522->PICC_GetType(mfrc522->uid.sak);
      if(debug)
      {
        Serial.println("Card detected!");
        Serial.print("Card UID:");
        dump_byte_array(mfrc522->uid.uidByte, mfrc522->uid.size);
        Serial.println();
        Serial.print("PICC type: ");
        Serial.println(mfrc522->PICC_GetTypeName(piccType));
      }
      playOnTap(speakerPin);
      break;
    }
    case States::INSPECT: 
    {
      if(getTick() - startTick > 5)         // ~ 0.5 seconds
      {
        digitalWrite(greenPin, LOW);
      }

      if(getTick() - startTick > 7)       // Timeout if exceeds ~ 0.7 seconds. 
      {
        cardState = States::REJECT;
        if(debug) { Serial.println("Process timed out! Please tap again ..."); }
        break;
      }

      // This checks if the first card has created which will setup as the master key card
      if(!setupComplete)
      {
        startTick = getTick();
        cardState = States::SETUP;
        if(debug) { Serial.println("Preparing setup for master keycard ..."); }
        break;
      }

      // After creating the master key card, check for master key card permission and database to create a new key card.
      if(findUID(mfrc522->uid.uidByte, true) == false && registerMode)           
      {
        startTick = getTick();
        cardState = States::SETUP;
        if(debug) { Serial.println("Assigning new card ..."); }
        break;
      }
      

      // Otherwise process authentication to gain access
      byte buffer[18];
      byte block = 0;
      MFRC522::StatusCode status;
      status = mfrc522->PCD_Authenticate(MFRC522::PICC_Command::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522->uid));
      if(status != MFRC522::StatusCode::STATUS_OK)
      {
        if(debug)
        {
          Serial.print("PCD_Authenticate() failed during INSPECTION: ");
          Serial.println(mfrc522->GetStatusCodeName(status));
        }
        break;
      }

      startTick = getTick();
      cardState = States::READ;
      if(debug) { Serial.println("Keys match! Now reading ...") ;}


      /*UNCOMMENT IF CARD FAILS TO RESET*/
      // factoryResetCard(debug);
      // cardState = States::IDLE;

      break;
    }

    case States::SETUP:
    {
      if(getTick() - startTick > 30)                // Reject if timeout exceeds 3 seconds.
      {
        cardState = States::REJECT;
        Serial.println("Timed out during SETUP.");
        break;
      }
      // Try the known default keys
      MFRC522::MIFARE_Key keys;
      bool hasKey = false;
      for (byte k = 0; k < NR_KNOWN_KEYS; k++) {
        // Copy the known key into the MIFARE_Key structure
        for (byte i = 0; i < MFRC522::MIFARE_Misc::MF_KEY_SIZE; i++) 
        {
          keys.keyByte[i] = knownKeys[k][i];
        }
        hasKey = try_key(&keys, debug);
        // Try the key
        if (hasKey) 
        {
          // Found and reported on the key and block,
          // no need to try other keys for this PICC
          if(debug)
          {
            Serial.println("Contains known key: ");
            dump_byte_array(keys.keyByte, MFRC522::MIFARE_Misc::MF_KEY_SIZE);
            Serial.println();
          }
          break;
        }
        
        // End check if new card is detected
        // http://arduino.stackexchange.com/a/14316
        if ( ! mfrc522->PICC_IsNewCardPresent())
          break;
        if ( ! mfrc522->PICC_ReadCardSerial())
          break;
      }


      if(!hasKey)         // Reject if key not found or timeout after 3 seconds
      {
        cardState = States::REJECT;
        if(debug) { Serial.println("PCD_Authenticate() failed: No default keys found during SETUP!"); }
        break;
      }

      startTick = getTick();
      cardState = States::WRITE;
      if(debug) { Serial.println("Passed preparations! Now writing ...");}
      break;
    }
    case States::WRITE:     
    {
      if(getTick() - startTick > 20)                // Reject if timeout exceeds 2 seconds.
      {
        cardState = States::REJECT;
        Serial.println("Timed out during WRITE.");
        break;
      }

      byte dataAddress = 2;
      String text = "";
      writeToTrailer(key.keyByte, debug);                     // Overwrite factory key with generated key

      // Write UID to database csv file
      registerUID(mfrc522->uid.uidByte, debug);
      if(debug) { Serial.println("Factory key overwritten!"); }
      
      text = !setupComplete ? "MASTER" : "";          // Text to write for first (master) key card.
      writeToDataBlock(dataAddress, text, debug);
      if(text != "")
      {
        Serial.printf("Write successful at address (%i)!\n", dataAddress);
        setupCompleted();                             // Permanently store completion status in preferences.
      }
      else
      {
        Serial.printf("Cleared data block at addres (%i)!\n", dataAddress);
      }

    
      cardState = States::IDLE;
      mfrc522->PICC_HaltA();
      mfrc522->PCD_StopCrypto1();  // Stop encryption on PCD
      if(debug) { Serial.println("Transitioning from WRITE to IDLE ..."); }
      break;
    }
    case States::READ:      
    {
      if(getTick() - startTick > 20)                // Reject if timeout exceeds 2 seconds.
      {
        cardState = States::REJECT;
        Serial.println("Timed out during READ.");
        break;
      }

      byte dataAddress = 2;
      String readData = readDataBlock(dataAddress, debug);
      if(debug) { Serial.printf("Datablock %i: %s\n", dataAddress, readData); }

      // For master key cards, change permissions to register new cards
      if(readData == "MASTER")
      {
        Serial.println("Master card detected!");
        registerMode = !registerMode;
        Serial.printf("Register Mode: %s\n", registerMode ? "TRUE" : "FALSE");
        cardState = States::IDLE;
        mfrc522->PICC_HaltA();       // Halt PICC
        mfrc522->PCD_StopCrypto1();  // Stop encryption on PCD
        break;
      }

      // For other cards with master key card privileges, deregister the key card from database (NOTE: CARD HAS ALREADY BEEN DETECTED ON LIST)
      if(registerMode)
      {
        factoryResetCard(debug);
        mfrc522->PICC_HaltA();       // Halt PICC
        mfrc522->PCD_StopCrypto1();  // Stop encryption on PCD
        cardState = States::IDLE;       
        break;
      }
            
      startTick = getTick();
      // Check in/out process using unique tokens/hash respective of its UID
      if(readData == String(uidHash(mfrc522->uid.uidByte)))     // If it already contains the hash/token, return the token by clearing the data at the address
      {
        cardState = States::EXIT;
      }
      else if(readData.length() == 0)                           // If no data at address, give the key card its unique hash/token
      {
        cardState = States::ENTER;
      }
      else
      {
        if(debug)
        {
          Serial.println("Invalid data block values!");
          Serial.printf("Read Data: %s\n", readData);
        }
        cardState = States::REJECT;
      }
      break;
        
    }
    case States::ENTER:
    {
      if(getTick() - startTick > 30)                // Reject if timeout exceeds 3 seconds.
      {
        cardState = States::REJECT;
        Serial.println("Timed out during ENTER.");
        break;
      }
      digitalWrite(redPin, LOW);
      digitalWrite(greenPin, HIGH);
      byte dataAddress = 2;
      int hashValue = uidHash(mfrc522->uid.uidByte);
      writeToDataBlock(dataAddress, String(hashValue), debug);
      cardState = States::IDLE;       
      mfrc522->PICC_HaltA();       // Halt PICC
      mfrc522->PCD_StopCrypto1();  // Stop encryption on PCD
      playSuccess(speakerPin);
      break;
    }
    case States::EXIT:
    {
      if(getTick() - startTick > 30)                // Reject if timeout exceeds 3 seconds.
      {
        cardState = States::REJECT;
        Serial.println("Timed out during EXIT.");
        break;
      }
      digitalWrite(redPin, LOW);
      digitalWrite(greenPin, HIGH);
      byte dataAddress = 2;
      clearBlock(dataAddress, debug);
      cardState = States::IDLE;       
      mfrc522->PICC_HaltA();       // Halt PICC
      mfrc522->PCD_StopCrypto1();  // Stop encryption on PCD
      playSuccess(speakerPin);
      break;
    }

    case States::REJECT:
    {
      Serial.println("Card has been REJECTED!");
      digitalWrite(redPin, HIGH);
      digitalWrite(greenPin, LOW);
      cardState = States::IDLE;
      mfrc522->PICC_HaltA();       // Halt PICC
      mfrc522->PCD_StopCrypto1();  // Stop encryption on PCD
      playDeclined(speakerPin);
      break;
    }
  }
}

/*****************************************************************************/

// Code by Miguel Balboa from MFRC522 library examples: 
// https://github.com/miguelbalboa/rfid

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
bool try_key(MFRC522::MIFARE_Key *key, bool debug)     
{
  bool result = false;
  byte buffer[18];
  byte block = 0;
  MFRC522::StatusCode status;

  Serial.println("Authenticating using key A...");
  status = mfrc522->PCD_Authenticate(MFRC522::PICC_Command::PICC_CMD_MF_AUTH_KEY_A, block, key, &(mfrc522->uid));
  if (status != MFRC522::StatusCode::STATUS_OK) {
    if(debug)
    {
      Serial.print("Try Key Method: PCD_Authenticate() failed: ");
      Serial.println(mfrc522->GetStatusCodeName(status));
    }
    return result;
  }

  // Read block
  byte byteCount = sizeof(buffer);
  status = mfrc522->MIFARE_Read(block, buffer, &byteCount);
  if (status != MFRC522::StatusCode::STATUS_OK) {
    if(debug)
    {
      Serial.print("Try Key Method: MIFARE_Read() failed: ");
      Serial.println(mfrc522->GetStatusCodeName(status));
    }
  }
  else {
      // Successful read
      result = true;
      if(debug)
      {
        Serial.print("Success with key:");
        dump_byte_array((*key).keyByte, MFRC522::MIFARE_Misc::MF_KEY_SIZE);
        Serial.println();
        // Dump block data
        Serial.print("Block "); Serial.print(block); Serial.print(F(":"));
        dump_byte_array(buffer, 16);
        Serial.println();
      }
  }
  Serial.println();
  return result;
}
/*****************************************************************************/

void initPrefs()
{
  Serial.println("Loading preferrences ...");
  prefs.begin("rfid", false);
  // UNCOMMENT TO RESET
  // prefs.clear();
  // return;                                    
  byte keyData[MFRC522::MIFARE_Misc::MF_KEY_SIZE];
  size_t len = prefs.getBytesLength("key");

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
  Serial.println("Loaded preferrences successfully!");
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
      Serial.println("Creating UID database ...");
  }
  file.close();
  Serial.println("UID database exists!");
}


int uidHash(byte* uidByte)
{
  int hash = 0;
  for(int i = 0; i < mfrc522->uid.size; i++)
  {
      hash = (hash * 31) + uidByte[i];
  }
  return abs(hash);
}

int uidIndex(byte* uidByte)
{
  int index = uidHash(uidByte) % TABLESIZE;
  return index;
}

// AI Assisted -> Converting byte to string format: https://chatgpt.com/share/6a217013-8f6c-83ec-a6c1-23764d53952a
String uidToString(byte* uid)
{
  String result = "";
  for(byte i = 0; i < mfrc522->uid.size; i++)
  {
    if(uid[i] < 0x10)               // If hexadecimal range from 0-9, add zero prefix
    {
      result += "0";
    }
    result += String(uid[i], HEX);
  }
  result.toUpperCase();             // Case sensitive for comparisons
  return result;
}

bool findUID(byte* uid, bool debug)
{
  String convertedUID = uidToString(uid);
  String line = readLine(LittleFS, uidFile, uidIndex(uid), true);
  if(debug)
  {
    Serial.printf("UID on card: %s\n", convertedUID);
    Serial.printf("UID in database: %s\n", line);
  }
  return convertedUID == line;
}

void registerUID(byte* uid, bool debug)
{
    // Note that once index is found need to multiply by uid length to get cursor position (including new line)
    File file = LittleFS.open(uidFile, "r+");
    int index = uidIndex(uid);
    int position = index * 9;                       // Each line is length of 9 = 8 digits + new line key
    file.seek(position);                            // Move cursor to respective hash position
    String line = file.readStringUntil('\n');       // Read only the 8 digits
    if(line == "00000000")                          // If all zeros, it is empty (preformatted upon successful trailer overwrite)
    {
      file.seek(position);                          // Reposition to the hash position
      // file.write(uidToString(uid), 8);
      file.print(uidToString(uid));                 // Overwrite the 8 digits
      if(debug) { Serial.println("UID registered to database successfully!"); }
    }
    else
    {
      if(debug) { Serial.printf("Unable to overwrite existing UID: %s\n", line); }
    }
    file.close();
}


void deregisterUID(byte* uid, bool debug)
{
    File file = LittleFS.open(uidFile, "r+");       // Read and write the file
    int index = uidIndex(uid);
    int position = index * 9;                       // Each line is length of 9 = 8 digits + new line key
    file.seek(position);                            // Move cursor to respective hash position
    String line = file.readStringUntil('\n');       // Read only the 8 digits
    if(line != "00000000")                          // If it is not zeros, then the UID has been written prior
    {
        file.seek(position);                        // Reposition to the hash position
        // file.write((const uint8_t*)"00000000", 8);
        file.print("00000000");                     // Reset by overwriting with zeros
        if(debug) { Serial.println("UID has been deregistered!"); }
    }
    file.close();
}

void writeToTrailer(byte* key, bool debug)
{
    byte trailerAddress = 3;
    byte readBlock[18] = {0};
    byte readSize = sizeof(readBlock);
    
    // Overwrite address with new key, keeping other elements the same
    mfrc522->MIFARE_Read(trailerAddress, readBlock, &readSize);
    memcpy(readBlock, key, MFRC522::MIFARE_Misc::MF_KEY_SIZE);

    // Write key to trailer address
    MFRC522::StatusCode status = mfrc522->MIFARE_Write(trailerAddress, readBlock, readSize - 2);
    if(status != MFRC522::StatusCode::STATUS_OK)
    {
      if(debug)
      {
        Serial.print("MIFARE_Write() failed: ");
        Serial.println("Failed to overwrite trailer address!");
        Serial.println(mfrc522->GetStatusCodeName(status));
      }
      return;
    }

    if(debug)
    {
      Serial.println("Successfully overwridden trailer address!");
      dump_byte_array(readBlock, readSize);
      Serial.println();
    }
}

void clearBlock(byte address, bool debug)
{
    byte writeBlock[16] = {0};
    byte writeSize = sizeof(writeBlock);

    // Overwrite existing block at address with a zero block
    MFRC522::StatusCode status = mfrc522->MIFARE_Write(address, writeBlock, writeSize);
    if(status != MFRC522::StatusCode::STATUS_OK)
    {
      if(debug)
      {
        Serial.print("MIFARE_Write() failed: ");
        Serial.printf("Datablock [%i] failed to clear!\n", address);
        Serial.println(mfrc522->GetStatusCodeName(status));
      }
      return;
    }

    if(debug) { Serial.printf("Datablock [%i] cleared successfully!\n", address); }
}

void writeToDataBlock(byte address, String data, bool debug)
{
    byte writeBlock[16] = {0};
    if(data.length() >= sizeof(writeBlock))
    {
      if(debug) { Serial.println("Data exceeded write block max size! "); }
      return;
    }

    memcpy(writeBlock, data.c_str(), data.length() + 1);        // Copy string data to the write block
    // Overwrite data block at address with the new write block containing the string data
    MFRC522::StatusCode status = mfrc522->MIFARE_Write(address, writeBlock, sizeof(writeBlock));
    if(status != MFRC522::StatusCode::STATUS_OK)
    {
      if(debug)
      {
        Serial.print("MIFARE_Write() failed: ");
        Serial.printf("Datablock [%i] failed to write!\n", address);
        Serial.println(mfrc522->GetStatusCodeName(status));
      }
      return;
    }
    
    if(debug) { Serial.printf("Wrote to Datablock [%i] successfully!\n", address); }
    
}

String readDataBlock(byte address, bool debug)
{
    byte buffer[18] = {0};
    byte blockSize = sizeof(buffer);

    // Read block at the specific address
    MFRC522::StatusCode status = mfrc522->MIFARE_Read(address, buffer, &blockSize);
    if(status != MFRC522::StatusCode::STATUS_OK)
    {
      if(debug)
      {
        Serial.print("MIFARE_Read() failed: ");
        Serial.println(mfrc522->GetStatusCodeName(status));
      }
      return "";
    }

    buffer[16] = '\0';      // Null character for end of string
    return String((char*)buffer);
}


// Can only be used post-authentication
void factoryResetCard(bool debug)
{
    Serial.println("Resetting key card to defaults!");
    deregisterUID(mfrc522->uid.uidByte, debug);
    clearBlock(2, debug);
    writeToTrailer(knownKeys[0], debug);
    mfrc522->PICC_HaltA();       // Halt PICC
    mfrc522->PCD_StopCrypto1();  // Stop encryption on PCD
}




