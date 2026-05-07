#ifndef VIBRATION_MONITOR_H
#define VIBRATION_MONTIOR_H

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Arduino.h>
#include <Wire.h>

extern Adafruit_MPU6050 mpu;
extern sensors_event_t a, g, temp;

void initVibrationMonitor(byte accelerometer, byte gyro, byte bandwidth, bool debug = false);
void calibrateA(bool debug = false);
void sensorUpdate();
float getAcceleration_x();
float getAcceleration_y();
float getAcceleration_z();


#endif