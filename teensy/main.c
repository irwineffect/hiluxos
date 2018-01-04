int main (void)
{
    unsigned int i;

    while(1)
    {
        (*(volatile unsigned int *)0x400FF080) = 0x20;
        for (i = 0; i < 0xFFFF; ++i);
        (*(volatile unsigned int *)0x400FF080) = 0x00;
        for (i = 0; i < 0x2FFFFF; ++i);
    }
}
