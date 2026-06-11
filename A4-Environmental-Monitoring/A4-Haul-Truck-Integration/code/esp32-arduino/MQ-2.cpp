// Assisted with AI: https://chatgpt.com/share/6a1273ad-4c5c-83ec-9596-9190cc3ebc09
// MQ-2 Sensor: https://randomnerdtutorials.com/guide-for-mq-2-gas-smoke-sensor-with-arduino/
// MQ-2 Datasheet: https://www.mouser.com/datasheet/2/321/605-00008-MQ-2-Datasheet-370464.pdf

#include "MQ-2.h"

namespace{
  uint8_t mq2Pin_A;
  uint8_t mq2Pin_D;
  float defaultRo = 5;
  float rS_air;
  float rS_gas;
  float rO;
  const float gasThreshold = 7;
  volatile bool gasDetected = false;
}

// For instantaneous detection. State will change if analogue read decreases.
void IRAM_ATTR gasISR()
{
  gasDetected = true;
}

void initMQ2(uint8_t analogPin)
{
  mq2Pin_A = analogPin;
  pinMode(mq2Pin_A, INPUT);
  Serial.println("Preparing MQ2 sensor (~ 20 seconds)");
  delay(20 * 1000); // Delay 2 minutes to warm up sensor
}

void initMQ2ISR(uint8_t digitalPin)
{
  mq2Pin_D = digitalPin;
  attachInterrupt(digitalPinToInterrupt(mq2Pin_D), gasISR, FALLING);
  Serial.println("MQ2 ISR initialised.");
}

void calibrateMQ2(bool debug)
{
  float sum = 0;
  int size = 5;
  for(int i = 0; i < size; i++)
  {
    sum += analogRead(mq2Pin_A);
    delay(2000);
  }
  float avgRead = sum / size;

  /******************AI Assisted********************/
  // Find Rs & Ro of air
  float voltage = avgRead / 4095 * 3.3;
  voltage *= 1.5;
  rS_air = 5 - voltage;
  rS_air /= voltage;

  rO = rS_air / 9.8;
  /*************************************************/

  if(rO < 0.65 * defaultRo || rO > 1.35 * defaultRo)
  {
    rO = defaultRo;
    rS_air = rO * 9.8;
    if(debug)
    {
      Serial.println("Calibration not within range. Using default values ...");
    }
  }

  if(debug)
  {
    Serial.printf("Analog value = %f\n", avgRead);
    Serial.printf("Rs / Ro = %.2f / %.2f = %.2f\n", rS_air, rO, rS_air / rO);
  }
}
  
float gasResistanceRatio()
{
  return rS_gas / rO;
}

bool presenceOfGas()
{
  return gasDetected;
}


void detectGas(bool debug)
{
  float avgRead = 0;
  int sampleSize = 5;
  for(int i = 0; i < sampleSize; i++)
  {
    avgRead += analogRead(mq2Pin_A);
    delay(200);     // 5 times per second
  }
  avgRead /= sampleSize;

  /******************AI Assisted********************/
  // Find Rs of gas in the air.
  float voltage = avgRead / 4095 * 3.3;
  voltage *= 1.5;
  rS_gas = 5 - voltage;
  rS_gas /= voltage;
  /*************************************************/

  if(debug)
  {
    Serial.printf("Analog value = %f\n", avgRead);
    Serial.printf("Rs / Ro = %.2f / %.2f = %.2f\n", rS_gas, rO, rS_gas / rO);
  }
  checkGasLevel(debug);
}

 
void checkGasLevel(bool debug)
{
  if(gasResistanceRatio() < gasThreshold)
  {
    gasDetected = true;
  }
  else
  {
    gasDetected = false;
  }


  /**********************WIP**********************/
  if(gasDetected)
  {
    // Play alert and warning
  }
  /***********************************************/

  if(debug)
  {
    Serial.printf("Gas detected: %s\n", gasDetected ? "TRUE" : "FALSE");
  }
}