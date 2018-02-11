#include <stdint.h>
#include "spi.h"

// create instances
spi spi_0(0x4002C000);

void spi::init()
{
    // set master mode
    regs->MCR = (1 << 31);

    // disable tx/rx fifos
    regs->MCR |= (1 << 13) | (1 << 12);

    // TODO: Setup pins
}

void spi::send_byte(const uint8_t b)
{
    while((regs->SR & (1 << 25)) == 0);
    regs->PUSHR = b;
}

