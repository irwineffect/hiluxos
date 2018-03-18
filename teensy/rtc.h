#ifndef RTC_H
#define RTC_H

#include <cstdint>

class rtc
{
    public:
        struct time
        {
            uint8_t hours;
            uint8_t minutes;
            uint8_t seconds;
        };

        rtc() {}

        void init();

        // pretty time manipulation
        struct time get_time(void);
        void set_time(struct time new_time);

        // raw time manipulation
        uint32_t get_time_raw(void);
        void set_time_raw(uint32_t new_time);

    private:
        struct register_map
        {
            uint32_t TSR;      // 0x00 Seconds
            uint32_t TPR;      // 0x04 prescaler
            uint32_t TAR;      // 0x08 alarm
            uint32_t TCR;      // 0x0C compensation
            uint32_t  CR;      // 0x10 control
            uint32_t  SR;      // 0x14 status
            uint32_t  LR;      // 0x18 lock
            uint32_t IER;      // 0x1C interrupt enable
            //uint8_t  pad_0[2018];  // 0x1D-0x7FF padding
            //uint32_t WAR;      // 0x800 write access
            //uint32_t RAR;      // 0x804 read access
        };

        struct register_map *const regs =
            reinterpret_cast<struct register_map *>(0x4003D000);
};


extern rtc rtc_control;

#endif
