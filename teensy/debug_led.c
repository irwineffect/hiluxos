#include "debug_led.h"
#include <stdint.h>

// Prepare the debug LED for use
void debug_led_setup(void)
{
    // Configure the LED pin
    (*(volatile uint32_t*)0x4004B014) = 0x00000143;
    // Set the LED pin (PTC 5) to output
    (*(volatile uint32_t*)0x400FF094) = 0x00000020;
}

// Turn on or off the debug LED
void debug_led(uint8_t state)
{
    (*(volatile uint32_t*)0x400FF080) = state ? 0x20 : 0;
}

// hcf: halt and catch fire
// This function never returns, it just sits and blinks the LED
void debug_led_hcf(uint8_t code)
{
    uint8_t i;
    uint32_t j;
    while(1)
    {
        for (i=0; i < code; ++i)
        {
            debug_led(1);
            for (j = 0; j < 0xFFFF; ++j);
            debug_led(0);
            for (j = 0; j < 0xCFFFF; ++j);
        }

        for (j = 0; j < 0x3FFFFF; ++j);
    }
}
