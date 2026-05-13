#include "MQ-2.h"

namespace{
  uint8_t mq2Pin;
  float avgAir;
  float minGasFactor = 1.4;
}


void initMQ2(uint8_t analogPin)
{
  mq2Pin = analogPin;
  pinMode(mq2Pin, INPUT);
  delay(120 * 1000); // Delay 2 minutes to warm up sensor
}

void initMQ2Interrupt()
{

}

void calibrate()
{
  float sum = 0;
  int size = 5
  for(int i = 0; i < size; i ++)
  {
    sum += analogRead(mqPin);
    delay(2000);
  }
  avgAir = sum / size;
}
  

void detectGas()
{
  float gasProportion = analogRead(mqPin);
  if(gasProportion > airAvg * minGasFactor)
  {

  }
  else
  {
    
  }
}