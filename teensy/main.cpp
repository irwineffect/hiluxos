#include <cstdint>
#include "debug.h"
#include "uart.h"
#include "printf.h"
#include "spi.h"
#include "clocks.h"
#include "interrupt_timer.h"
#include "nvic.h"
#include "rtc.h"
#include "service.h"

uint8_t led_state = 0;
void timer_callback(void)
{
    debug_led(led_state);
    led_state = !led_state;
    printf("timer callback\n");
    rtc::time time = rtc_control.get_time();
    printf("%u:%u:%u\n", time.hours, time.minutes, time.seconds);
}

int main (void)
{
    clock_control.enable_peripherals();
    uart_0.init();
    spi_0.init();
    rtc::time time = {.hours = 22, .minutes = 14, .seconds = 0};
    rtc_control.init();
    rtc_control.set_time(time);
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
        printf("result: %d\n\n",svc(static_cast<SVC_TYPE>(1) ,2, 7, 8));
    }

    return 0;
}

