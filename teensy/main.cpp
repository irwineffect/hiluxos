#include <cstdint>
#include "debug.h"
#include "uart.h"
#include "printf.h"
#include "spi.h"
#include "clocks.h"
#include "interrupt_timer.h"
#include "nvic.h"

uint8_t led_state = 0;
void timer_callback(void)
{
    debug_led(led_state);
    led_state = !led_state;
    printf("timer callback\n");
}

int main (void)
{
    clock_control.enable_peripherals();
    uart_0.init();
    spi_0.init();
    interrupt_timer_0.init(0x5FFFFFF, 1, &timer_callback);
    interrupt_timer_0.enable_timer(1);

    nvic_control.enable_interrupts();
    while(1)
    {
        //debug_led(1);
        printf("in main while loop\n");
        //printf("timer: %d\n", interrupt_timer_0.get_value());
        //debug_led(0);
        for (uint32_t j = 0; j < 0x1FFFFF; ++j);
    }

    return 0;
}

