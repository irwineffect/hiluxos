#ifndef INTERRUPT_TIMER_H
#define INTERRUPT_TIMER_H

#include <cstdint>

class interrupt_timer
{
    public:
        interrupt_timer(uint32_t base_addr) :
            regs(reinterpret_cast<struct register_map *>(base_addr)) {}

        void init(uint32_t value, uint8_t interrupt_enable,
                  void(*_user_callback)(void));
        void set_value(uint32_t value);
        uint32_t get_value(void);
        void enable_interrupt(uint8_t enable);
        void enable_timer(uint8_t enable);

        void register_callback(void(*_user_callback)(void));

        void callback(void);
    private:
        struct register_map
        {
            uint32_t LDVAL;      // 0x00 Load Value
            uint32_t CVAL;       // 0x04 Current Value
            uint32_t CTRL;       // 0x08 Control
            uint32_t FLG;       //  0x0C Flags
        };

        uint32_t *const main_control_reg =
            reinterpret_cast<uint32_t*>(0x40037000);

        struct register_map *const regs;

        void (*user_callback)(void);
};


extern interrupt_timer interrupt_timer_0,
                       interrupt_timer_1,
                       interrupt_timer_2,
                       interrupt_timer_3;



extern "C" void ISR_interrupt_timer_0(void);
extern "C" void ISR_interrupt_timer_1(void);
extern "C" void ISR_interrupt_timer_2(void);
extern "C" void ISR_interrupt_timer_3(void);

#endif
