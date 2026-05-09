#ifndef VIBRATION_MONITOR_H
#define VIBRATION_MONTIOR_H

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Arduino.h>
#include <Wire.h>

extern Adafruit_MPU6050 mpu;
extern sensors_event_t a, g, temp;

void initVibrationMonitor(mpu6050_accel_range_t accelerometer, mpu6050_gyro_range_t gyro, mpu6050_bandwidth_t bandwidth, bool debug = false);
void initInterrupt(mpu6050_highpass_t highpassBandwidth, float lsb, float duration = 2);
void calibrateA(bool debug = false);
void sensorUpdate(bool debug = false);
float getRawAcceleration_x();
float getRawAcceleration_y();
float getRawAcceleration_z();
void lowPassFiltering(bool debug = false);
float getAvgAcceleration_x();
float getAvgAcceleration_y();
float getAvgAcceleration_z();

#endif