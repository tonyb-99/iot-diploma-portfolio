#ifndef VIBRATION_MONITOR_H
#define VIBRATION_MONTIOR_H

#include "clock.h"
#include "custom_timer.h"
#include "file_manager.h"
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Arduino.h>
#include <Wire.h>

extern Adafruit_MPU6050 mpu;
extern sensors_event_t a, g, temp;
// extern bool isAbnormal;

void initSDStorage(uint8_t sdPin, const String& fileName, bool debug = false);
void initVibrationMonitor(mpu6050_accel_range_t accelerometer, mpu6050_gyro_range_t gyro, mpu6050_bandwidth_t bandwidth, bool debug = false);
void initVibrationInterrupt(mpu6050_highpass_t highpassBandwidth, float lsb, float duration = 20);
void calibrateA(bool debug = false);
void sensorUpdate(bool debug = false);
float getRawAcceleration_x();
float getRawAcceleration_y();
float getRawAcceleration_z();
void lowPassFiltering(bool debug = false);
float getAvgAcceleration_x();
float getAvgAcceleration_y();
float getAvgAcceleration_z();
String getRawStringData();
String getFilteredStringData();
String exportString();
void measureVibrations(bool debug = false);

#endif