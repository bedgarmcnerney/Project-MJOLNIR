#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h"
#include "hardware.h"

MAX30105 particleSensor;


const byte RATE_SIZE = 4;
byte rates[RATE_SIZE];
byte rateSpot = 0;
byte rateCount = 0;

long lastBeat = 0;

float beatsPerMinute = 0;
uint8_t beatAvg = 0;

uint8_t pulse_available = 0; // 0 = sensor offline, 1 = sensor working
uint8_t finger_status = 0;   // 0 = no finger, 1 = finger detected

void reset_pulse_reading()
{
  beatsPerMinute = 0;
  beatAvg = 0;
  rateSpot = 0;
  rateCount = 0;
  lastBeat = 0;

  for (byte x = 0; x < RATE_SIZE; x++)
  {
    rates[x] = 0;
  }
}

void pulse_setup()
{
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST))
  {
    pulse_available = 0;
    finger_status = 0;
    reset_pulse_reading();
    return; // do NOT freeze suit
  }

  pulse_available = 1;
  finger_status = 0;
  reset_pulse_reading();

  particleSensor.setup();
  particleSensor.setPulseAmplitudeRed(0x1F);
  particleSensor.setPulseAmplitudeIR(0x1F);
  particleSensor.setPulseAmplitudeGreen(0);
}

void pulse_loop()
{
  if (pulse_available == 0)
  {
    finger_status = 0;
    reset_pulse_reading();
    return;
  }

  long irValue = particleSensor.getIR();

  if (irValue < 50000)
  {
    finger_status = 0;
    reset_pulse_reading();
    return;
  }

  finger_status = 1;

  if (checkForBeat(irValue))
  {
    long currentTime = millis();

    // Ignore the first detected beat after reset because lastBeat is not valid yet
    if (lastBeat == 0)
    {
      lastBeat = currentTime;
      return;
    }

    long delta = currentTime - lastBeat;
    lastBeat = currentTime;

    beatsPerMinute = 60.0 / (delta / 1000.0);

    if (beatsPerMinute < 245 && beatsPerMinute > 30)
    {
      rates[rateSpot++] = (byte)beatsPerMinute;
      rateSpot %= RATE_SIZE;

      if (rateCount < RATE_SIZE)
      {
        rateCount++;
      }

      uint16_t sum = 0;
      for (byte x = 0; x < rateCount; x++)
      {
        sum += rates[x];
      }

      beatAvg = sum / rateCount;
    }
  }
}

uint8_t pulse()
{
  if (pulse_available == 0)
  {
    return PULSE_OFFLINE; // Sensor is offline ERROR
  }
  else if (finger_status == 0)
  {
    return PULSE_NO_FINGER; // Finger not detected
  }
  else
  {
    if (beatAvg == 0)
    {
      return PULSE_CALIBRATING; // Calibrating
    }
    else
    {
      return beatAvg; // Acceptable measurement
    }
  }
}

uint8_t pulse_finger_status()
{
  return finger_status;
}

uint8_t pulse_sensor_status()
{
  return pulse_available;
}