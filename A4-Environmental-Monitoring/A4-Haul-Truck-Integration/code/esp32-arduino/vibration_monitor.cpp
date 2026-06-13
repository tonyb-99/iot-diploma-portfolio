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

#include "vibration_monitor.h"

Adafruit_MPU6050 mpu;
sensors_event_t a, g, temp;

namespace {
  const String* pFileName = nullptr;
  const String headings = "timestamp, x_raw, x_filtered, y_raw, y_filtered, z_raw, z_filtered, IsAbnormal?\n";
  unsigned long startTick = 0;
  float offsetX = 0, offsetY = 0, offsetZ = 0;
  unsigned long count = 0;
  const int bufferSize = 5;
  float bufferX[5];
  float bufferY[5];
  float bufferZ[5];
  float avgX = 0, avgY = 0, avgZ = 0;
  String output = "";
  bool hasCalibrated = false;
  bool isAbnormal = false;
}

void createDataFile(fs::FS &fs, const String& fileName, bool debug)
{
  pFileName = &fileName;
  fileCheck(fs, *pFileName, debug);
  writeFile(fs, pFileName->c_str(), headings.c_str());
}

void initVibrationMonitor(mpu6050_accel_range_t accelerometer, mpu6050_gyro_range_t gyro, mpu6050_bandwidth_t bandwidth, bool debug)
{
  if (!mpu.begin(MPU6050_ALTADDR)) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }

  mpu.setAccelerometerRange(accelerometer);
  mpu.setGyroRange(gyro);
  mpu.setFilterBandwidth(bandwidth);

  Serial.println("");
  Serial.println("Accelerometer initiated ...");
  
  if(debug)
  {
    Serial.print("Accelerometer range set to: ");
    switch (mpu.getAccelerometerRange()) {
      case MPU6050_RANGE_2_G:
        Serial.println("+-2G");
        break;
      case MPU6050_RANGE_4_G:
        Serial.println("+-4G");
        break;
      case MPU6050_RANGE_8_G:
        Serial.println("+-8G");
        break;
      case MPU6050_RANGE_16_G:
        Serial.println("+-16G");
        break;
    }

    Serial.print("Gyro range set to: ");
    switch (mpu.getGyroRange()) {
      case MPU6050_RANGE_250_DEG:
        Serial.println("+- 250 deg/s");
        break;
      case MPU6050_RANGE_500_DEG:
        Serial.println("+- 500 deg/s");
        break;
      case MPU6050_RANGE_1000_DEG:
        Serial.println("+- 1000 deg/s");
        break;
      case MPU6050_RANGE_2000_DEG:
        Serial.println("+- 2000 deg/s");
        break;
    }

    Serial.print("Filter bandwidth set to: ");
    switch (mpu.getFilterBandwidth()) {
      case MPU6050_BAND_260_HZ:
        Serial.println("260 Hz");
        break;
      case MPU6050_BAND_184_HZ:
        Serial.println("184 Hz");
        break;
      case MPU6050_BAND_94_HZ:
        Serial.println("94 Hz");
        break;
      case MPU6050_BAND_44_HZ:
        Serial.println("44 Hz");
        break;
      case MPU6050_BAND_21_HZ:
        Serial.println("21 Hz");
        break;
      case MPU6050_BAND_10_HZ:
        Serial.println("10 Hz");
        break;
      case MPU6050_BAND_5_HZ:
        Serial.println("5 Hz");
        break;
    }
  }
}

// LSB = Least significant bit. 1 LSB = 0.002 g
void initVibrationInterrupt(mpu6050_highpass_t highpassBandwidth, float lsb, float duration)
{
  mpu.setHighPassFilter(highpassBandwidth);
  mpu.setMotionDetectionThreshold(lsb);           // 10 LSB recommended. Lower means higher sensitivity. Higher is less sensitivity.
  mpu.setMotionDetectionDuration(duration);
  mpu.setInterruptPinLatch(true);	                // Keep it latched.  Will turn off when reinitialized.
  mpu.setInterruptPinPolarity(true);
  mpu.setMotionInterrupt(true);
  Serial.println("Acc. Interrupt initiated ...");
}


// Calibrate the vibration sensor by sampling values to find the average offset
void calibrateA(bool debug)
{
  int sampleSize = 100;
  float sumX = 0, sumY = 0, sumZ = 0;

  for(int i = 0; i < sampleSize; i++)
  {
    mpu.getEvent(&a, &g, &temp);
    sumX += a.acceleration.x;
    sumY += a.acceleration.y;
    sumZ += a.acceleration.z;
    
    delay(10);
  }

  offsetX = sumX / sampleSize;
  offsetY = sumY / sampleSize;
  offsetZ = sumZ / sampleSize - 9.81;

  if(debug)
  {
    Serial.printf(
      "Offsets (100 samples): (x:%.3f, y:%.3f, z:%.3f)\n", 
      offsetX, 
      offsetY, 
      offsetZ
      );
  }
}

// Read sensor values and refine values.
void sensorUpdate(bool debug)
{
  mpu.getEvent(&a, &g, &temp);
  lowPassFiltering(debug);
}

float getRawAcceleration_x()
{
  return a.acceleration.x - offsetX;
}

float getRawAcceleration_y()
{
  return a.acceleration.y - offsetY;
}

float getRawAcceleration_z()
{
  return a.acceleration.z - offsetZ;
}


// This function calculates the average of a data point by averaging a subset of values prior to it.
void lowPassFiltering(bool debug)
{
  // Each time function is called, add values to buffer
  int index = count % bufferSize;
  float sumX = 0, sumY = 0, sumZ = 0;

  float rawX = getRawAcceleration_x();
  float rawY = getRawAcceleration_y();
  float rawZ = getRawAcceleration_z();

  bufferX[index] = rawX;
  bufferY[index] = rawY;
  bufferZ[index] = rawZ;
  // Begin averaging after first value recorded

  if(count == 0)
  {
    avgX = bufferX[index];
    avgY = bufferY[index];
    avgZ = bufferZ[index];
  }
  else if(count > 0 && count < bufferSize)
  {
    int samples = count + 1;
    for(int i = 0; i <= count; i++)
    {
        sumX += bufferX[i];
        sumY += bufferY[i];
        sumZ += bufferZ[i];
    }
    avgX = sumX / samples;
    avgY = sumY / samples;
    avgZ = sumZ / samples;
  }
  // Replace the older values from the start upon reaching the end of buffer
  else
  {
    for(int i = 0; i < bufferSize; i++)
    {
        sumX += bufferX[i];
        sumY += bufferY[i];
        sumZ += bufferZ[i];
    }
    avgX = sumX / bufferSize;
    avgY = sumY / bufferSize;
    avgZ = sumZ / bufferSize;
  }

  if(debug)
  {
    Serial.printf("(%i): Raw accelermoter values: (x=%.3f, y=%.3f, z=%.3f) ms^-2\n", count, rawX, rawY, rawZ);
    Serial.printf("(%i): Moving avg values: (x=%.3f, y=%.3f, z=%.3f) ms^-2\n", count, avgX, avgY, avgZ);
    Serial.println();
  }

  count++;
}

float getAvgAcceleration_x()
{
  return avgX;
}

float getAvgAcceleration_y()
{
  return avgY;
}

float getAvgAcceleration_z()
{
  return avgZ;
}


String exportString(bool debug)
{
  String output = "";
  output += getDate();
  output += " ";
  output += getTimestamp();
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
  if(debug) 
  { 
    Serial.print(headings);
    Serial.print(output); 
  }
  return output;
}

void measureVibrations(fs::FS &fs, bool debug)
{
  // Interrupt on abrupt movement
  if(mpu.getMotionInterruptStatus())
  {
    Serial.println("Anomaly detected!");
    isAbnormal = true;
    sensorUpdate(debug);
    appendFile(fs, pFileName->c_str(), exportString(debug).c_str());
    hasCalibrated = false;
    return;
  }

  // Calibrate upon initialisation and after interrupts
  if(!hasCalibrated)
  {
    calibrateA(debug);
    hasCalibrated = true;
  }

  isAbnormal = false;

  // Measure vibration and export every 10s
  if(getTick() - startTick >= 100)
  {
    startTick = getTick();
    sensorUpdate(debug);
    appendFile(fs, pFileName->c_str(), exportString(debug).c_str());
  }
}











