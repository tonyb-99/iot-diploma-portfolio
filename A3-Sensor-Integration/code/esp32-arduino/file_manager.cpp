#include "file_manager.h"

// Initialize LittleFS
void initLittleFS() {
  if (!LittleFS.begin(true)) {
    Serial.println("An error has occurred while mounting LittleFS");
  }
  Serial.println("LittleFS mounted successfully");
}

// Read File from LittleFS
String readFile(fs::FS &fs, const char * path, bool debug){
  Serial.printf("Reading file: %s\r\n", path); 

  File file = fs.open(path);
  if(!file || file.isDirectory()){
    Serial.println("- failed to open file for reading"); 
    return String();
  }
  
  if(debug) { Serial.printf("Read Output:\n---------------------\n"); }
  String fileContent = "";
  while (file.available()) {
    fileContent += file.readStringUntil('\n');  // read line-by-line
    fileContent += "\n"; // re-add newline since readStringUntil removes it
  }

  if(debug) { Serial.println(fileContent);  }
  file.close();
  return fileContent;
}

// Write file to LittleFS
void writeFile(fs::FS &fs, const char * path, const char * message){
  Serial.printf("Writing file: %s\r\n", path);

  File file = fs.open(path, FILE_WRITE);
  if(!file){
    Serial.println("- failed to open file for writing");
    return;
  }
  if(file.print(message)){
    Serial.println("- file written");
  } else {
    Serial.println("- write failed");
  }
}

// Delete file to LittleFS
void deleteFile(fs::FS &fs, const char *path) {
  Serial.printf("Deleting file: %s\r\n", path);
  if (fs.remove(path)) {
    Serial.println("- file deleted");
  } else {
    Serial.println("- delete failed");
  }
}
