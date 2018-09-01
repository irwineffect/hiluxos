#include "interrupt_timer.h"
#include <cstdint>

// create instances
interrupt_timer interrupt_timer_0(0x40037100),
                interrupt_timer_1(0x40037110),
                interrupt_timer_2(0x40037120),
                interrupt_timer_3(0x40037130);

void ISR_interrupt_timer_0(void)
{
    interrupt_timer_0.callback();
}

void ISR_interrupt_timer_1(void)
{
    interrupt_timer_1.callback();
}

void ISR_interrupt_timer_2(void)
{
    interrupt_timer_2.callback();
}

void ISR_interrupt_timer_3(void)
{
    interrupt_timer_3.callback();
}

void interrupt_timer::init(uint32_t value, uint8_t interrupt_enable,
        void(*_user_callback)(void))
{
    // enable the main module before doing anything else
    *main_control_reg &= ~(1 << 1);

    // clear the interrupt flag, just in case
    regs->FLG |= (1 << 1);

    // register the callback
    register_callback(_user_callback);

    // set the timer countdown period
    set_value(value);

    // forward interrupt enable
    enable_interrupt(interrupt_enable);
}

void interrupt_timer::set_value(uint32_t value)
{
    regs->LDVAL = value;
}

void interrupt_timer::enable_interrupt(uint8_t enable)
{
    if (enable == 0)
    {
        regs->CTRL &= ~(1 << 1);
    }
    else
    {
        regs->CTRL |= (1 << 1);
    }
}

void interrupt_timer::enable_timer(uint8_t enable)
{
    if (enable == 0)
    {
        regs->CTRL &= ~(1 << 0);
    }
    else
    {
        regs->CTRL |= (1 << 0);
    }
}

void interrupt_timer::register_callback(void(*_user_callback)(void))
{
    user_callback = _user_callback;
}

void interrupt_timer::callback(void)
{
    user_callback();
}
