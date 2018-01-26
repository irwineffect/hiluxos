#include "uart.h"
#include <stdint.h>

const UART uart_0 = {(void*) 0x4006A000},
           uart_1 = {(void*) 0x4006B000};

void uart_setup(UART *u)
{
    // Set the baud rate;
    const uint16_t baud_rate = 0;
    u->reg->BDH = (baud_rate >> 8) & 0x1F;
    u->reg->BDL = baud_rate & 0xFF;

    // Turn on the transmitter
    u->reg->C2 |= (1 << 3);

    // Turn on the receiver
    //u->reg->C2 |= (1 << 2);

    // Setup pins
    if (u == &uart_0)
    {
        // mux PTB16 (pin 0) to alt3 (UART0_RX)
        // mux PTB17 (pin 1) to alt3 (UART0_TX)
    }
}

void uart_putc(UART *u, char c)
{
    while((u->reg->S1 & (1 << 7)) != 0);
    u->reg->data = c;
}

void uart_prints(UART *u, char *s)
{
    while(*s != '\0')
    {
        uart_putc(u, *s);
        ++s;
    }
}
