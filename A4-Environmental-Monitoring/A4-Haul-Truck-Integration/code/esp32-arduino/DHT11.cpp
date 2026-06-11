// Humidity sensor: https://randomnerdtutorials.com/esp32-dht11-dht22-temperature-humidity-sensor-arduino-ide/

#include "DHT11.h"

namespace {
  DHT* dht;
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