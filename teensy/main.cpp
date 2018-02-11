#include <cstdint>
#include "debug.h"
#include "uart.h"
#include "printf.h"
#include "spi.h"
#include "clocks.h"

int main (void)
{
   clock_control.enable_peripherals();
   spi_0.init();

    while(1)
    {
        debug_led(1);
        printf("in main while loop\n");
        debug_led(0);
        for (uint32_t j = 0; j < 0x1FFFFF; ++j);
    }

    return 0;
}
