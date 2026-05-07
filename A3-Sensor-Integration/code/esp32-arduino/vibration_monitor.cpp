#include "vibration_monitor.h"

Adafruit_MPU6050 mpu;
sensors_event_t a, g, temp;

namespace {
  float offsetX;
  float offsetY;
  float offsetZ;
}

void initVibrationMonitor(byte accelerometer, byte gyro, byte bandwidth, bool debug)
{
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_10_HZ);

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

    Serial.println("");
  }
}

void calibrateA(bool debug)
{
  const int sampleSize = 100;
  float sumX = 0;
  float sumY = 0;
  float sumZ = 0;

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

void sensorUpdate()
{
  mpu.getEvent(&a, &g, &temp);
}

float getAcceleration_x()
{
  return a.acceleration.x - offsetX;
}

float getAcceleration_y()
{
  return a.acceleration.y - offsetY;
}

float getAcceleration_z()
{
  return a.acceleration.z - offsetZ;
}

















