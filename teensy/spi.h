#ifndef SPI_H
#define SPI_H

#include "printf.h"
#include <cstdint>

class spi
{
    public:
        spi(uint32_t base_addr) :
            regs(reinterpret_cast<struct register_map*>(base_addr)) {}

        void init();
        void send_byte(const uint8_t b);

    private:
        struct register_map
        {
            uint32_t MCR;       // 0x00 module configuration
            uint8_t  pad_0;     // 0x04-0x07 padding
            uint32_t TCR;       // 0x08 transfer control
            uint32_t CTAR0;     // 0x0C clock and transfer attributes 0
            uint32_t CTAR1;     // 0x10 clock and transfer attributes 1
            uint8_t  pad_1[24]; // 0x14-0x2B padding
            uint32_t SR;        // 0x2C status register
            uint32_t RSER;      // 0x30 DMA interrupt request select and enable
            uint32_t PUSHR;     // 0x34 push tx fifo
            uint32_t POPR;      // 0x38 pop rx fifo
            uint32_t TXFR[4];   // 0x3C transmit fifo introspection
            uint32_t pad_2[60]; // 0x40-0x7B padding
            uint32_t RXFR[4];   // 0x7C receive fifo introspection
        };

        struct register_map *const regs;
};

extern spi spi_0;

#endif
