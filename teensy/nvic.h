#ifndef NVIC_H
#define NVIC_H

#include <cstdint>

class nvic
{
    public:
        nvic(void) {}

        // Convienence function for enabling interrupts for all peripherals
        void enable_interrupts(void);

        // Enable a single interrupt
        void enable_interrupt(uint8_t irq, uint8_t enable);

        // Interrupt Set Enable
        uint32_t *const ISER =
            reinterpret_cast<uint32_t*>(0xE000E100);

        // Interrupt Clear Enable
        uint32_t *const ICER =
            reinterpret_cast<uint32_t*>(0xE000E180);

        // Interrupt Set Pending
        uint32_t *const ISPR =
            reinterpret_cast<uint32_t*>(0xE000E200);

        // Interrupt Clear Pending
        uint32_t *const ICPR =
            reinterpret_cast<uint32_t*>(0xE000E280);

        // Interrupt Active Bit
        uint32_t *const IABR =
            reinterpret_cast<uint32_t*>(0xE000E300);

        // Interrupt Priority
        uint32_t *const IPR =
            reinterpret_cast<uint32_t*>(0xE000E400);

        // Software Interrupt Trigger
        uint32_t *const STIR =
            reinterpret_cast<uint32_t*>(0xE0000EF00);
};


extern nvic nvic_control;

#endif
