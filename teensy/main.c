#include <stdint.h>
#include "debug.h"
#include "uart.h"

int main (void)
{
    uint32_t j;

    while(1)
    {
        debug_led(1);
        dbprint("hello world\n");
        debug_led(0);
        for (j = 0; j < 0x1FFFFF; ++j);
    }

    return 0;
}
