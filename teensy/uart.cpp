#include "uart.h"
#include <cstdint>
#include "pin_control.h"
#include "debug.h"

// create instances
uart uart_0(0x4006A000);

void uart::init()
{
    // Module clock appears to be ~21Mhz.
    // To calculate divider value:
    // divider = 21Mhz/16/desired_baud_rate
    // Set the baud rate divider to 11 for 115200 baud.
    const uint16_t baud_rate_divider = 11;

    regs->BDH = (baud_rate_divider >> 8) & 0x1F;
    regs->BDL = baud_rate_divider & 0xFF;

    // Turn on the transmitter
    regs->C2 |= (1 << 3);

    // Turn on the receiver
    //u->reg->C2 |= (1 << 2);


    // Setup pins
    //if (u == &uart_0)
    {
        // mux PTB16 (pin 0) to alt3 (UART0_RX)
        //pin_set_mux(&port_b, 16, 3);
        //pin_set_data_direction(&port_b, 16, 0);

        // mux PTB17 (pin 1) to alt3 (UART0_TX)
        pin_set_mux(&port_b, 17, 3);
        pin_set_drive_strength(&port_b, 17, 1);
        pin_set_data_direction(&port_b, 17, 1);
    }
    return;
}

void uart::putc(const char c)
{
    while((regs->S1 & (1 << 7)) == 0);
    regs->data = c;
}

void uart::puts(const char *s)
{
    while(*s != '\0')
    {
        putc(*s);
        ++s;
    }
}
