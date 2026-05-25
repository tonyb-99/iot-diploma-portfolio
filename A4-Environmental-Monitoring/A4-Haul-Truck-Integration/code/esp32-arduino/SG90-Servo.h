#ifndef SG90_SERVO_H
#define SG90_SERVO_H

#include <ESP32Servo.h>
#include <Arduino.h>

void initServo(uint8_t servoPin);
void rotate(int angle, int duration = 15, bool debug = false);

#endif