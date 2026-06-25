// Humidity sensor: https://randomnerdtutorials.com/esp32-dht11-dht22-temperature-humidity-sensor-arduino-ide/

#include "DHT11.h"

namespace {
  DHT* dht;
  unsigned long startTick = 0;
}

void initDHT11(uint8_t dhtPin)
{
  dht = new DHT(dhtPin, DHTTYPE);
  dht->begin();
}

float getTemperature(bool debug)
{
  float t = dht->readTemperature();
  if (isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return 0;
  }
  if(debug) { Serial.printf("Temperature: %.1f'C\n", t); }
  return t;
}

float getHumidity(bool debug)
{
  float h = dht->readHumidity();
  if (isnan(h)) {
    Serial.println("Failed to read from DHT sensor!");
    return 0;
  }
  if(debug) { Serial.printf("Humidity: %.1f%%\n", h); }
  return h;
}

void selectColour(float temp, bool debug)
{
  // Check dangers first before approaching safety
  // Danger if below -10 or above 43 C
  if(temp < (float)Temperature_Range::FREEZING || temp > (float)Temperature_Range::HOT)
  {
    if(debug) { Serial.println("Colour code: RED"); }
    // currentColour = ColourCode::RED;
    tempColour = ColourCode::RED;
  }
  // Warning if below 10 or above 37 C
  else if(temp < (float)Temperature_Range::COLD || temp > (float)Temperature_Range::WARM)
  {
    if(debug) { Serial.println("Colour code: YELLOW"); }
    // currentColour = ColourCode::YELLOW;
    tempColour = ColourCode::YELLOW;
  }

  // Safe range: 11-36 C
  else
  {
    if(debug) { Serial.println("Colour code: GREEN"); }
    // currentColour = ColourCode::GREEN;
    tempColour = ColourCode::GREEN;
  }
}

void processDHT(bool humidityON, bool tempON, int interval, bool debug)
{
  if(getTick() - startTick >= interval)
  {
    startTick = getTick();
    if(humidityON)
    {
      float h = getHumidity(debug);
    }

    if(tempON)
    {
      float t = getTemperature(debug);
      selectColour(t, debug);
    }

  }
}