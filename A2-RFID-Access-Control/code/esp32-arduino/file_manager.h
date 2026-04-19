#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <Arduino.h>
#include "LittleFS.h"

void initLittleFS();
String readFile(fs::FS &fs, const char * path, bool debug = false);
void writeFile(fs::FS &fs, const char * path, const char * message);
bool deleteFile(fs::FS &fs, const char *path); 
#endif