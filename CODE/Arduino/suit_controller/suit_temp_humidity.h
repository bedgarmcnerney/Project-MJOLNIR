#ifndef SUIT_TEMP_HUMIDITY_H
#define SUIT_TEMP_HUMIDITY_H

#include <stdint.h>

void suit_temp_setup();
void suit_temp_loop();
float suit_temp();
float humidity();

#endif