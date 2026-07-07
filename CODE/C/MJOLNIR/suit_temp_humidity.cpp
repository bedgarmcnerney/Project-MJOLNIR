#include "DHT.h"
#include "hardware.h"

#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

float temp = 0;
float hum = 0;

void suit_temp_setup() {
  dht.begin();
}

// if temp and humidity are 0, sensor is is disconnected
void suit_temp_loop() {
  float newHum = dht.readHumidity();
  float newTempC = dht.readTemperature();

  if (isnan(newHum) || isnan(newTempC)) {
    hum = 0;
    temp = 0;
    return;
  }

  hum = newHum;
  temp = (newTempC * 9.0 / 5.0) + 32.0;
}

float suit_temp() {
  return temp;
}

float humidity() {
  return hum;
}