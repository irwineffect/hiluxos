#include <stdint.h>
#include "debug_led.h"
#include "uart.h"

int main (void)
{
    uint32_t j;
    debug_led_setup();
    uart_setup(&uart_0);

    while(1)
    {
        debug_led(1);
        uart_prints(&uart_0, "hello\n");
        debug_led(0);
        for (j = 0; j < 0x1FFFFF; ++j);
    }

    return 0;
}
