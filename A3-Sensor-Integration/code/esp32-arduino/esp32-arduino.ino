#include "vibration_monitor.h"
#include "file_manager.h"


#define CS_PIN 5

File fileData;
const String fileName = "/vibration_data.csv";
const bool debug = true;

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

void setup(void) {
  Serial.begin(115200);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  initSDReader(CS_PIN, debug);
  SD_FileCheck(fileData, fileName, debug);
  initVibrationMonitor(MPU6050_RANGE_2_G, MPU6050_RANGE_250_DEG, MPU6050_BAND_10_HZ);
  delay(100);
}

void loop() {

  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  Serial.print("Acceleration X: ");
  Serial.print(a.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(a.acceleration.y);
  Serial.print(", Z: ");
  Serial.print(a.acceleration.z);
  Serial.println(" m/s^2");

  delay(500);
}
