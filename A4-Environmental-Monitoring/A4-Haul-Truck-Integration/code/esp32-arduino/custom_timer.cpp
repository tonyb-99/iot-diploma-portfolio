#include "custom_timer.h"

// unsigned long startTick = 0;

namespace{
  unsigned long prevTime = 0;
  unsigned long tick = 0;
  unsigned long lastTick = -1;
}

void initCustomTimer(bool debug)
{
  if (millis() - prevTime >= 100) {
    prevTime = millis();
    tick++;
  }

  if(lastTick != tick)
  {
    lastTick = tick;
    if(debug)
    {
      if(tick != 0 && tick % 10 == 0)
      {
        Serial.print("Time: ");
        Serial.print(tick / 10);
        Serial.println(" sec");
      } 
    }
  }
}

unsigned long getPrevTime()
{
  return prevTime;
}

unsigned long getTick()
{
  return tick;
}

unsigned long getLastTick()
{
  return lastTick;
}
