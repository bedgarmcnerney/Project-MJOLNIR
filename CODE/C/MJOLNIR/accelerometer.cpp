// (c) Michael Schoeffler 2017, http://www.mschoeffler.de

#include <Arduino.h>
#include <Wire.h>
#include <math.h>
#include "accelerometer.h"
#include "timers.h"
#include "hardware.h"

#define GESTURE_DELAY_TIMER 0
#define GESTURE_DELAY_MS 1000
#define MOTION_THRESHOLD 5000

const int MPU_ADDR = 0x68;

// Raw accelerometer values
static int32_t accelerometer_x = 0;
static int32_t accelerometer_y = 0;
static int32_t accelerometer_z = 0;

// Previous accelerometer values
static int32_t previous_x = 0;
static int32_t previous_y = 0;
static int32_t previous_z = 0;

// Motion values
static int32_t x_motion = 0;
static int32_t y_motion = 0;
static int32_t z_motion = 0;

// Smoothed motion values
static int32_t x_filtered = 0;
static int32_t y_filtered = 0;
static int32_t z_filtered = 0;

static bool first_read = true;

// This stores the last real gesture.
// This is the important part that prevents GEST from constantly going back to 0.
static uint8_t last_action = 0;

int16_t gyro_x, gyro_y, gyro_z;
int16_t temperature;

char tmp_str[7];

char* convert_int16_to_str(int16_t i) {
  sprintf(tmp_str, "%6d", i);
  return tmp_str;
}

static int16_t read_mpu_word() {
  return (int16_t)((Wire.read() << 8) | Wire.read());
}

void accelerometer_setup() {
  Wire.begin();

  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B); // PWR_MGMT_1 register
  Wire.write(0);    // wake up MPU-6050
  Wire.endTransmission(true);

  pinMode(GESTURE_ENABLE_PIN, INPUT);

  reset_timer(GESTURE_DELAY_TIMER);
}

uint8_t get_gesture_enable_state() {
  return digitalRead(GESTURE_ENABLE_PIN);
}

void accelerometer_loop() {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);

  if (Wire.endTransmission(false) != 0) {
    x_motion = 0;
    y_motion = 0;
    z_motion = 0;
    return;
  }

  uint8_t bytesRead = Wire.requestFrom(MPU_ADDR, 14, true);

  if (bytesRead < 14) {
    x_motion = 0;
    y_motion = 0;
    z_motion = 0;
    return;
  }

  int32_t new_x = read_mpu_word();
  int32_t new_y = read_mpu_word();
  int32_t new_z = read_mpu_word();

  temperature = read_mpu_word();

  gyro_x = read_mpu_word();
  gyro_y = read_mpu_word();
  gyro_z = read_mpu_word();

  if (first_read) {
    accelerometer_x = new_x;
    accelerometer_y = new_y;
    accelerometer_z = new_z;

    previous_x = new_x;
    previous_y = new_y;
    previous_z = new_z;

    x_motion = 0;
    y_motion = 0;
    z_motion = 0;

    x_filtered = 0;
    y_filtered = 0;
    z_filtered = 0;

    first_read = false;
    return;
  }

  previous_x = accelerometer_x;
  previous_y = accelerometer_y;
  previous_z = accelerometer_z;

  accelerometer_x = new_x;
  accelerometer_y = new_y;
  accelerometer_z = new_z;

  x_motion = previous_x - accelerometer_x;
  y_motion = previous_y - accelerometer_y;
  z_motion = previous_z - accelerometer_z;

  // Simple smoothing filter
  x_filtered = (x_filtered * 3 + x_motion) / 4;
  y_filtered = (y_filtered * 3 + y_motion) / 4;
  z_filtered = (z_filtered * 3 + z_motion) / 4;
}

uint8_t get_action() {
  // If gesture control is disabled, keep returning the last screen/action.
  // This prevents the Pi from snapping back to default.
  if (digitalRead(GESTURE_ENABLE_PIN) != HIGH) {
    return last_action;
  }

  // Cooldown to prevent one motion from triggering repeatedly
  if (get_timer_value(GESTURE_DELAY_TIMER) <= GESTURE_DELAY_MS) {
    return last_action;
  }

  int32_t x = abs(x_filtered);
  int32_t z = abs(z_filtered);

  // X-axis gesture must be clearly stronger than Z-axis motion
  if (x > MOTION_THRESHOLD && x > (z * 3 / 2)) {
    reset_timer(GESTURE_DELAY_TIMER);

    if (x_filtered >= 0) {
      last_action = 1;
    } else {
      last_action = 2;
    }

    return last_action;
  }

  // Z-axis gesture must be clearly stronger than X-axis motion
  if (z > MOTION_THRESHOLD && z > (x * 3 / 2)) {
    reset_timer(GESTURE_DELAY_TIMER);

    if (z_filtered >= 0) {
      last_action = 3;
    } else {
      last_action = 4;
    }

    return last_action;
  }

  // No new gesture happened, so keep the previous one.
  return last_action;
}