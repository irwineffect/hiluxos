#include "pin_control.h"
#include <cstdint>

const PORT port_a = {reinterpret_cast<PIN_CONTROL_REGISTER_MAP*>(0x40049000),
                     reinterpret_cast<GPIO_REGISTER_MAP*>(0x400FF000)},

           port_b = {reinterpret_cast<PIN_CONTROL_REGISTER_MAP*>(0x4004A000),
                     reinterpret_cast<GPIO_REGISTER_MAP*>(0x400FF040)},

           port_c = {reinterpret_cast<PIN_CONTROL_REGISTER_MAP*>(0x4004B000),
                     reinterpret_cast<GPIO_REGISTER_MAP*>(0x400FF080)},

           port_d = {reinterpret_cast<PIN_CONTROL_REGISTER_MAP*>(0x4004C000),
                     reinterpret_cast<GPIO_REGISTER_MAP*>(0x400FF0C0)},

           port_e = {reinterpret_cast<PIN_CONTROL_REGISTER_MAP*>(0x4004D000),
                     reinterpret_cast<GPIO_REGISTER_MAP*>(0x400FF100)};

void pin_set_mux(const PORT *port, uint8_t pin, uint8_t mux)
{
    // valid values for pin is 0-31d
    // valid values for mux is 0-111b
    if (pin >= 32 || mux > 0b111)
    {
        // probably should hcf?
        return;
    }

    // first clear the mux value, then set it
    port->pin_control_regs->pins[pin] &= ~(0b111 << 8);
    port->pin_control_regs->pins[pin] |= (mux << 8);
}

void pin_set_drive_strength(const PORT *port, uint8_t pin, uint8_t
        drive)
{
    // valid values for pin is 0-31d
    if (pin >= 32)
    {
        // probably should hcf?
        return;
    }

    if (drive == 0)
    {
        port->pin_control_regs->pins[pin] &= ~(1 << 6);
    }
    else if (drive == 1)
    {
        port->pin_control_regs->pins[pin] |= (1 << 6);
    }
    else
    {
        // probably should hcf?
        return;
    }
}

void pin_set_data_direction(const PORT *port, uint8_t pin, uint8_t out_nin)
{
    // valid values for pin is 0-31d
    if (pin >= 32)
    {
        // probably should hcf?
        return;
    }

    //valid values for out_nin is 0,1
    if (out_nin == 0)
    {
        port->gpio_control_regs->PDDR &= ~(1 << pin);
    }
    else if (out_nin == 1)
    {
        port->gpio_control_regs->PDDR |= (1 << pin);
    }
    else
    {
        // probably should hcf?
        return;
    }
}

uint8_t pin_read(const PORT *port, uint8_t pin)
{
    // valid values for pin is 0-31d
    if (pin >= 32)
    {
        // probably should hcf?
        return 0;
    }

    if (port->gpio_control_regs->PDIR & (1 << pin))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void pin_write(const PORT *port, uint8_t pin, uint8_t state)
{
    // valid values for pin is 0-31d
    if (pin >= 32)
    {
        // probably should hcf?
        return;
    }

    if (state == 0)
    {
        port->gpio_control_regs->PCOR = (1 << pin);
    }
    else if (state == 1)
    {
        port->gpio_control_regs->PSOR = (1 << pin);
    }
    else
    {
        // probably should hcf?
        return;
    }
}
