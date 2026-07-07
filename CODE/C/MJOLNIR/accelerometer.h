#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#include <stdint.h>

void accelerometer_setup(void);
void accelerometer_loop(void);
char* convert_int16_to_str(int16_t i);
uint8_t get_action();
uint8_t get_gesture_enable_state();

#endif