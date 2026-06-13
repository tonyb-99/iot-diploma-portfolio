#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <Arduino.h>
#include "FS.h"
// #include "SD.h"
// #include "SPI.h"
#include "LittleFS.h"

void initLittleFS();
// void initSDReader(uint8_t cs_pin, bool debug = false);
String readFile(fs::FS &fs, const char * path, bool debug = false);
String readLine(fs::FS &fs, const char * path, int index, bool debug = false);
void writeFile(fs::FS &fs, const char * path, const char * message);
void appendFile(fs::FS &fs, const char *path, const char *message);
void deleteFile(fs::FS &fs, const char *path); 
void fileCheck(fs::FS &fs, const String& fileName, bool debug);
#endif