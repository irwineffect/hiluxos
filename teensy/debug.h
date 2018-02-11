#ifndef DEBUG_LED_H
#define DEBUG_LED_H

#include <cstdint>


extern "C" void debug_init(void);
void debug_led(uint8_t state);
extern "C" void debug_led_hcf(uint8_t code);

#endif
