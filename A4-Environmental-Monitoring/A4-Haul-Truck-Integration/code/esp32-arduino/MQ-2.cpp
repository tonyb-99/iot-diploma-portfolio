#include "MQ-2.h"

namespace{
  uint8_t mq2Pin;
  float avgAir;
  float avgGas;
  float rO;
  bool gasDetected = false;
}


void initMQ2(uint8_t analogPin)
{
  mq2Pin = analogPin;
  pinMode(mq2Pin, INPUT);
  Serial.println("Preparing MQ2 sensor (~ 20 seconds)");
  delay(20 * 1000); // Delay 2 minutes to warm up sensor
}


void calibrateMQ2(bool debug)
{
  float sum = 0;
  int size = 5;
  for(int i = 0; i < size; i++)
  {
    sum += analogRead(mq2Pin);
    delay(2000);
  }
  avgAir = sum / size;
  avgAir /= 4095;
  if(debug)
  {
    Serial.printf("Initial Gas volume: %.1f%%\n", avgAir);
  }
}
  
float stableAir()
{
  return avgAir;
}

float gasVolume()
{
  return avgGas;
}


bool presenceOfGas()
{
  return gasDetected;
}

void detectGas(bool debug)
{
  avgGas = 0;
  int sampleSize = 5;
  for(int i = 0; i < sampleSize; i++)
  {
    avgGas += analogRead(mq2Pin);
    delay(200);     // 5 times per second
  }
  avgGas /= sampleSize;
  Serial.printf("Analog value = %f\n", avgGas);
  avgGas /= 4095;
  if(debug)
  {
    Serial.printf("Gas volume: %.1f%%\n", avgGas);
  }
  checkGasLevel(debug);
}

 
void checkGasLevel(bool debug)
{
  if(avgGas > 0.4)
  {
    gasDetected = true;
  }
  else
  {
    gasDetected = false;
  }

  if(debug)
  {
    Serial.printf("Gas detected: %s\n", gasDetected ? "TRUE" : "FALSE");
  }
}