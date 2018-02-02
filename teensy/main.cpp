#include <stdint.h>
#include "debug.h"
#include "uart.h"
#include "printf.h"

void __attribute__((section(".mysection"))) ramtest(void)
{
    debug_led_hcf(1);
}

class a
{
    public:
        a(void)
        {
            dbprint("creating a\n");
        }

        ~a(void)
        {
            dbprint("destroying a\n");
        }

        void use(void)
        {
            dbprint("using a\n");
        }
};

a mclass;
int main (void)
{
    uint32_t j;
    mclass.use();

    while(1)
    {
        debug_led(1);
        //printf("hello %d world\n", 123456);
        dbprint("hey world\n");
        debug_led(0);
        for (j = 0; j < 0x1FFFFF; ++j);
    }

    return 0;
}
