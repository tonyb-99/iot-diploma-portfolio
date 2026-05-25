// Datasheet: https://friendlywire.com/projects/ne555-servo-safe/SG90-datasheet.pdf
// Servo for esp32: https://randomnerdtutorials.com/esp32-servo-motor-web-server-arduino-ide/

#include "SG90-Servo.h"

Servo sg90;
int pos = 0;

void initServo(uint8_t servoPin)
{
  sg90.attach(servoPin);
  Serial.println("Servo initialised!");
}

void rotate(int angle, int duration, bool debug)
{
  if(angle >= 0 && angle <= 180)
  {
    bool clockwise = angle > pos;
    if(clockwise)
    {
      for(; pos <= angle; pos++)
      {
        sg90.write(pos);
        delay(duration);
      }
    }
    else
    {
      for(; pos >=angle; pos--)
      {
        sg90.write(pos);
        delay(duration);
      }
    }

    if(debug)
    {
      Serial.printf("Rotatiing from %i' to %i'\n", pos, angle);
    }
    
  }
}