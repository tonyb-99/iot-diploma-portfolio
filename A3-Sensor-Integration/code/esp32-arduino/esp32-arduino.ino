#include "vibration_monitor.h"
#include "file_manager.h"


#define CS_PIN 5

File fileData;
const String fileName = "/vibration_data.csv";
const bool debug = true;
bool hasCalibrated = false;

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


void setup(void) {
  Serial.begin(115200);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  //initSDReader(CS_PIN, debug);
  //SD_FileCheck(fileData, fileName, debug);
  initVibrationMonitor(MPU6050_RANGE_2_G, MPU6050_RANGE_250_DEG, MPU6050_BAND_260_HZ);    // High freq for fast readings, no smoothing.
  initVibrationInterrupt(MPU6050_HIGHPASS_0_63_HZ, 10);
  delay(100);
}

void loop() {

  if(!hasCalibrated)
  {
    calibrateA(true);
    hasCalibrated = true;
    delay(2000);
  }
  if(mpu.getMotionInterruptStatus())
  {
    sensorUpdate(true);
  }
  delay(10);
}
