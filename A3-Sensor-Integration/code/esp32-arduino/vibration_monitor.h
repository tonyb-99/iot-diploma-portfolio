#ifndef VIBRATION_MONITOR_H
#define VIBRATION_MONTIOR_H

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Arduino.h>
#include <Wire.h>

extern Adafruit_MPU6050 mpu;

void initVibrationMonitor(byte accelerometer, byte gyro, byte bandwidth, bool debug = false);

#endif