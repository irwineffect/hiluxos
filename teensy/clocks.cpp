#include <cstdint>
#include "clocks.h"

clocks clock_control;

void clocks::enable_peripherals(void)
{
    // UART0
    clock_gating_regs->SCGC4 |= (1 << 10);

    // GPIO ports
    clock_gating_regs->SCGC5 |= (0b11111 << 9);

    // RTC
    clock_gating_regs->SCGC6 |= (1 << 29);

    // SPI0
    clock_gating_regs->SCGC6 |= (1 << 12);

    // Timers
    clock_gating_regs->SCGC6 |= (1 << 23);
}
