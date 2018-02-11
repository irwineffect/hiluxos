#include <stdint.h>
#include "spi.h"

namespace SPI
{
    const SPI spi_0 = {reinterpret_cast<register_map*>(0x4002C000)};

    void setup(const SPI *s)
    {
        s->reg->MCR = (1 << 31) | (1 << 13) | (1 << 12);


    }

    void send_byte(const SPI *s, const uint8_t b)
    {

    }

};
