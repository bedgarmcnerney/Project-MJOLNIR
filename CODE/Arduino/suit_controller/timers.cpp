#include <Arduino.h>
#include "timers.h"

static uint32_t timers[4] = {0,0,0,0};

void timer_setup(void){
  for (uint8_t i = 0; i<sizeof(timers); i++){
    timers[i] = 0;
  }
}

void reset_timer(int8_t timer_number) {
  timers[timer_number] = millis();
}

uint32_t get_timer_value(int8_t timer_number){
  return millis() - timers[timer_number];
}

// bool timer_elapsed_ms(uint32_t interval_ms) {
//   return get_timer_ms() >= interval_ms;
// } 