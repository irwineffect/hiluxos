#ifndef CLOCKS_H
#define CLOCKS_H

class clocks
{
    public:
        clocks(void) {};

        void enable_peripherals(void);

    private:
        struct clock_gating_reg_map
        {
            uint32_t SCGC1;
            uint32_t SCGC2;
            uint32_t SCGC3;
            uint32_t SCGC4;
            uint32_t SCGC5;
            uint32_t SCGC6;
            uint32_t SCGC7;
        };

        struct clock_gating_reg_map *const clock_gating_regs =
            reinterpret_cast<struct clock_gating_reg_map*>(0x40048028);

};

extern clocks clock_control;

#endif
