#include "debug.h"
#include "pin_control.h"
#include "uart.h"
#include <stdint.h>

// Prepare the debug LED and UART port for use
void debug_init(void)
{
    // Configure the LED pin
    pin_set_mux(&port_c, 5, 0b001);
    pin_set_drive_strength(&port_c, 5, 1);
    pin_set_data_direction(&port_c, 5, 1);

    // Configure UART serial port
    uart_setup(&uart_0);

    dbprint("\ndbprint initialized\n");
}

void dbprint(const char *s)
{
    uart_prints(&uart_0, s);
}

// Turn on or off the debug LED
void debug_led(uint8_t state)
{
    //(*(volatile uint32_t*)0x400FF080) = state ? 0x20 : 0;
    pin_write(&port_c, 5, (state ? 1 : 0));
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
