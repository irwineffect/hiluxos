#include <cstdarg>
#include "uart.h"
#include "debug.h"
#include "printf.h"

static const char *table = "0123456789ABCDEF";

static uart &printf_uart = uart_0;

void putc(const char c)
{
    printf_uart.putc(c);
}
void puts(const char *s)
{
    printf_uart.puts(s);
}

void rpu(uint32_t x, uint8_t base)
{
    char c;
    if (x)
    {
        c = table[x % base];
        rpu( x/base, base);
        putc(c);
    }
}

void put_unsigned_int(uint32_t u)
{
    if (u == 0)
    {
        putc('0');
    }
    else
    {
        rpu(u, 10);
    }
}

void put_hex(uint32_t u)
{
    if (u == 0)
    {
        putc('0');
    }
    else
    {
        rpu(u, 16);
    }
}
void put_signed_int(int32_t i)
{
    if ( i < 0 )
    {
        putc('-');
        i = -i;
    }
    put_unsigned_int(i);
}

void printf(const char* fmt, ...)
{
    const char *p = fmt;

    va_list args;
    va_start(args, fmt);
    while(*p)
    {
        if (*p != '%')
        {
            putc(*p);
        }
        else
        {
            ++p;
            switch(*p)
            {
                case '%':
                    putc('%');
                    break;
                case 's':
                    puts(va_arg(args, char*));
                    break;
                case 'u':
                    put_unsigned_int(va_arg(args, uint32_t));
                    break;
                case 'd':
                    put_signed_int(va_arg(args, int32_t));
                    break;
                case 'x':
                    put_hex(va_arg(args, uint32_t));
                    break;
                default:
                    //dbprint("error: unknown format specifier!\n");
                    debug_led_hcf(1);
                    break;
            }

        }
        ++p;
    }
    va_end(args);
}
