#ifndef UART_H
#define UART_H

#include <cstdint>

class uart
{
    public:


        uart(uint32_t base_addr) :
            regs(reinterpret_cast<struct register_map*>(base_addr)) {}

        void init();
        void putc(const char c);
        void puts(const char *s);

    private:
        struct register_map
        {
            uint8_t BDH;      // 0x00 baud rate high
            uint8_t BDL;      // 0x01 baud rate low
            uint8_t C1;       // 0x02 control 1
            uint8_t C2;       // 0x03 control 2
            uint8_t S1;       // 0x04 status 1
            uint8_t S2;       // 0x05 status 2
            uint8_t C3;       // 0x06 control 3
            uint8_t data;     // 0x07 data register
            uint8_t pad_0[8]; // 0x08-0x0F padding
            uint8_t PFIFO;    // 0x10 FIFO params
            uint8_t CFIFO;    // 0x11 FIFO control
            uint8_t SFIFO;    // 0x12 FIFO status
            uint8_t TWFIFO;   // 0x13 FIFO transmit watermark
            uint8_t TCFIFO;   // 0x14 FIFO transmit count
            uint8_t RWFIFO;   // 0x15 FIFO receive watermark
            uint8_t RCFIFO;   // 0x16 FIFO receive count
        };
        struct register_map *const regs;
};


extern uart uart_0;

#endif
