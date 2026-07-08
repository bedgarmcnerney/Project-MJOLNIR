#ifndef SENSOR_H
#define SENSOR_H

#include <stdint.h>

void ultrasonic_sensor_setup();
uint8_t ultrasonic_sensor_loop();
uint8_t rear_guard_enable_state();

#endif