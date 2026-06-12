#include "vibration_monitor.h"
#include "file_manager.h"


#define CS_PIN 5


const bool debug = true;
bool hasCalibrated = false;
unsigned long currentTime = 0;
unsigned long prevTime = 0;
unsigned long tick = 0;
unsigned long lastTick = -1;
unsigned long startTick = 0;
unsigned long duration = 0;
bool isAbnormal = false;

// https://randomnerdtutorials.com/esp32-how-to-log-data/
// https://randomnerdtutorials.com/esp32-microsd-card-arduino/
// https://esp32io.com/tutorials/esp32-sd-card
// https://randomnerdtutorials.com/esp32-mpu-6050-accelerometer-gyroscope-arduino/
// https://randomnerdtutorials.com/flash-upload-micropython-firmware-esp32-esp8266/
// https://controllerstech.com/mpu6050-arduino-tutorial/
// https://dsa-explorer-hub.vercel.app/algorithm/low-pass-filter
// https://maker.pro/arduino/tutorial/how-to-clean-up-noisy-sensor-data-with-a-moving-average-filter

// Calculate offset
// Calculate moving average using low pass filter: ideal for stable values as sensor is stationary.
// Peak ground acceleration (PGA): https://en.wikipedia.org/wiki/Peak_ground_acceleration
// Threshold +/- 0.02 g, unstable.


void setup(void) 
{
  Serial.begin(115200);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  initSDReader(CS_PIN, debug);
  SD_FileCheck(fileData, fileName, debug);
  writeFile(SD, fileName.c_str(), headings.c_str());
  initVibrationMonitor(MPU6050_RANGE_2_G, MPU6050_RANGE_250_DEG, MPU6050_BAND_260_HZ);    // High freq for fast readings, no smoothing.
  // initVibrationInterrupt(MPU6050_HIGHPASS_0_63_HZ, 10);      // Picks up slow vibrations
  initVibrationInterrupt(MPU6050_HIGHPASS_5_HZ, 10);            // Picks fast moving vibrations
  delay(100);
}

void loop() 
{
  currentTime = millis();
  if(currentTime - prevTime > 10)
  {
    prevTime = currentTime;
    tick++;
  }

  if(lastTick != tick)
  {
    lastTick = tick;
    if(tick % 100 == 0)
    {
      Serial.printf("Duration: %i (sec) ...\n", duration);
      duration++;
    }
  }

  // Interrupt on abrupt movement
  if(mpu.getMotionInterruptStatus())
  {
    Serial.println("Anomaly detected!");
    isAbnormal = true;
    sensorUpdate(true);
    appendFile(SD, fileName.c_str(), exportString().c_str());
    hasCalibrated = false;
    return;
  }

  // Calibrate upon initialisation and after interrupts
  if(!hasCalibrated)
  {
    calibrateA(true);
    hasCalibrated = true;
  }

  isAbnormal = false;

  // Measure vibration and export every ~ 0.5s
  if(tick % 50 == 0)
  {
    sensorUpdate(true);
    appendFile(SD, fileName.c_str(), exportString().c_str());
  }
  
}


String timestamp(unsigned long ms)
{
  unsigned long totalSeconds = ms / 1000;

  unsigned int hrs = totalSeconds / 3600;
  unsigned int mins = (totalSeconds % 3600) / 60;
  unsigned int seconds = totalSeconds % 60;

  String display = "";

  // Hours
  if(hrs < 10)
  {
    display += "0";
  }
  display += String(hrs);
  display += ":";

  // Minutes
  if(mins < 10)
  {
    display += "0";
  }
  display += String(mins);
  display += ":";

  // Seconds
  if(seconds < 10)
  {
    display += "0";
  }
  display += String(seconds);

  return display;
}

String exportString()
{
  String output = "";
  output += timestamp(currentTime);
  // output += timestamp(tick * 10);   // Total ms = tick * 10ms. NOTE: TICK DOES NOT UPDATE DURING INTERRUPT.
  Serial.print(output);
  Serial.print(" ----> ");
  output += ", ";
  output += String(getRawAcceleration_x(), 3);
  output += ", ";
  output += String(getAvgAcceleration_x(), 3);
  output += ", ";
  output += String(getRawAcceleration_y(), 3);
  output += ", ";
  output += String(getAvgAcceleration_y(), 3);
  output += ", ";
  output += String(getRawAcceleration_z(), 3);
  output += ", ";
  output += String(getAvgAcceleration_z(), 3);
  output += ", ";
  output += String(isAbnormal);
  output += "\n";
  return output;
}


