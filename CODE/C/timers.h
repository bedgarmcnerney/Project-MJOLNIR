#ifndef TIMERS_H
#define TIMERS_H

#include <stdint.h>

void timer_setup(void);
void reset_timer(int8_t timer_number);
uint32_t get_timer_value(int8_t timer_number);
//bool timer_elapsed_ms(uint32_t interval_ms);

#endif