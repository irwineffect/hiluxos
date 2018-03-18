#include "rtc.h"
#include <cstdint>

// create instances
rtc rtc_control;

void rtc::init()
{
    // check if the RTC is already enabled. If it is not running, start it.
    if ((regs->SR & (1 << 4)) == 0)
    {
        regs->CR |= (1 << 8); // enable the 32 kHz oscillator
        // wait for oscillator to stabilize

        regs->TSR = 0; // initialize the seconds register
        regs->SR |= (1 << 4); // enable the RTC seconds counter
    }
}


uint32_t rtc::get_time_raw(void)
{
    return regs->TSR;
}

void rtc::set_time_raw(uint32_t new_time)
{
    // disable the seconds counter
    regs->SR &= ~(1 << 4);

    // set the time
    regs->TSR = new_time;

    // enable the seconds counter
    regs->SR |= (1 << 4);
}


struct rtc::time rtc::get_time(void)
{
    struct time current_time;

    uint32_t raw_time = get_time_raw();

    current_time.seconds = raw_time % 60;
    raw_time /= 60;

    current_time.minutes = raw_time % 60;
    raw_time /= 60;

    current_time.minutes = raw_time % 24;

    return current_time;
}

void rtc::set_time(struct rtc::time new_time)
{
    uint32_t raw_time;

    raw_time = new_time.hours * 60 * 60 +
               new_time.minutes * 60 +
               new_time.seconds;

    set_time_raw(raw_time);
}
