#include "nvic.h"
#include <cstdint>

// create instance
nvic nvic_control;


void nvic::enable_interrupts()
{
    // Enable programmable interrupt timer 0.
    enable_interrupt(48, 1);
}

void nvic::enable_interrupt(uint8_t irq, uint8_t enable)
{
    uint8_t register_location;
    uint8_t bit_location;

    register_location = irq / 32;
    bit_location = irq % 32;

    if (enable == 0)
    {
        ICER[register_location] = (1 << bit_location);
    }
    else
    {
        ISER[register_location] = (1 << bit_location);
    }
}

