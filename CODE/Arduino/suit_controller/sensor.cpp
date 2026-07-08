#include "sensor.h"
#include <Arduino.h>
#include <stdint.h>
#include "hardware.h"


float duration, distance;
uint8_t enable = 0;
uint8_t last_enable_pin_state = LOW;
uint8_t threat_detected = 0;

void ultrasonic_sensor_setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(ULTRASONIC_ENABLE_PIN, INPUT);
  digitalWrite(TRIG_PIN, LOW);
}

  // Sensor disarmed = 0
  // Armed = 1

uint8_t rear_guard_enable_state() {
  return enable;
}

  // Threat Not detected = 0
  // Threat Detected = 1

uint8_t ultrasonic_sensor_loop() {
  
  uint8_t current_enable_pin_state = digitalRead(ULTRASONIC_ENABLE_PIN);
  // Arm only when the pin first changes from LOW to HIGH
  if (last_enable_pin_state == LOW && current_enable_pin_state == HIGH) {
    enable = 1;
    //Serial.println("SENSOR_ARMED");
  }

  last_enable_pin_state = current_enable_pin_state;

  if (enable == 0) {
    return 0;
  }

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH, 10000);

  if (duration == 0) {
    return 0;
  }

  float distance = (duration * 0.0343) / 2;

  if (distance <= 50) {
    enable = 0;

    //Serial.println(distance);
    threat_detected = 1;
    //Serial.println("Ultrasonic disarmed");
  }
  else {
    threat_detected = 0;
  }
  return threat_detected;
}


