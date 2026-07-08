#ifndef PULSE_OXIMETER_H
#define PULSE_OXIMETER_H

#include <stdint.h>

void pulse_setup();
void pulse_loop();
uint8_t pulse();
uint8_t pulse_finger_status();
uint8_t pulse_sensor_status();

#endif