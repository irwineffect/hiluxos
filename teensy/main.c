#include <stdint.h>
#include "debug_led.h"

int main (void)
{
    debug_led_setup();
    debug_led_hcf(5);

    return 0;
}
