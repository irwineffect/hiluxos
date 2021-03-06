#ifndef ILI9341_DRIVER_H
#define ILI9341_DRIVER_H

#include <Arduino.h>
#include <SPI.h>

// for some reason Arduino is #defining putc as a macro?
#undef putc

namespace Color
{
    enum : uint32_t
    {
        black  = 0x000000,
        blue   = 0x0000FF,
        green  = 0x00FF00,
        red    = 0xFF0000,

        white  = 0xFFFFFF
    };
}

// Pixels are 18 bits large; 6 each for red, green, and blue. The most
// significant 2 bits are not used and are ignored.
class Pixel
{
    public:

        // Some default colors
        //enum class Color : uint32_t
        //{
        //    black  = 0x000000,
        //    blue   = 0x0000FF,
        //    green  = 0x00FF00,
        //    red    = 0xFF0000,

        //    white  = 0xFFFFFF
        //};
        Pixel(void)
        {
            red = 0;
            green = 0;
            blue = 0;
        }

        Pixel(uint8_t _red, uint8_t _green, uint8_t _blue)
        {
            red = _red & 0x3F;
            green = _green & 0x3F;
            blue = _blue & 0x3F;
        }

        // RGB Input Format:
        // Each color uses the first six bits of each byte, stored in the order
        // RGB. The MSB is not used.
        // -------------------------------------------------------------------------
        // |7|6|5|4|3|2|1|0|  |7|6|5|4|3|2|1|0| |7|6|5|4|3|2|1|0|  |7|6|5|4|3|2|1|0|
        // | | | | | | | | |  | | |R|R|R|R|R|R| | | |G|G|G|G|G|G|  | | |B|B|B|B|B|B|
        // -------------------------------------------------------------------------
        Pixel(uint32_t rgb)
        {
            red = (rgb >> 16) & 0x3F;
            green = (rgb >> 8) & 0x3F;
            blue = rgb & 0x3F;
        }

        // Internal storage for the pixel
        uint8_t red,
                green,
                blue;
};

class ILI9341_driver
{
    public:
        // System functions
        ILI9341_driver(void);
        void begin(uint8_t _cs, uint8_t _data_ncommand, uint8_t _rst);
        void sleep(void);
        void wake(void);
        void hardware_reset(void);
        void read_command(uint8_t command, uint8_t data[], uint8_t len);

        // Drawing functions
        void draw_pixel(Pixel color, uint16_t x, uint16_t y);
        void fill_screen(Pixel color);
        void draw_hline(Pixel color, uint16_t x1, uint16_t x2, uint16_t y);
        void draw_vline(Pixel color, uint16_t y1, uint16_t y2, uint16_t x);
        void draw_line(Pixel color, uint16_t x1, uint16_t y1, uint16_t x2,
                uint16_t y2);
        void draw_rectangle(Pixel color, uint16_t x, uint16_t y, uint16_t w,
                uint16_t h);
        void draw_circle(Pixel color, uint16_t x, uint16_t y, uint16_t r);
        void draw_char(Pixel color, char c, uint16_t x, uint16_t y, Pixel bg);

        // text manipulation functions
        void set_cursor_start(uint16_t x, uint16_t y);
        void set_cursor_end(uint16_t x, uint16_t y);
        void set_cursor(uint16_t x, uint16_t y);
        void set_text_color(Pixel color);
        void set_text_bg_color(Pixel color);
        void set_text_scale(uint8_t scale);
        void putc(char c);
        void puts(const char c[]);
        void reset_cursor(void);

        static const uint16_t WIDTH  = 320,
                              HEIGHT = 240;

        static const uint32_t NUM_PIXELS_IN_SCREEN = 0x12C00;

        static const uint8_t CHARACTER_WIDTH  = 8,
                             CHARACTER_HEIGHT = 8;

    private:
        void write_command(uint8_t command, uint8_t data[], uint8_t len,
                uint8_t bulk_transfer=0);
        void write_pixels(Pixel data[], uint32_t len, uint8_t bulk_transfer=0,
                uint8_t repeat_pixel=0);
        void begin_spi(void);
        void end_spi(void);
        inline void write_window(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
        {
            uint8_t addr_buff[4];

            addr_buff[0] = (x1 >> 8) & 0xFF;
            addr_buff[1] = x1 & 0xFF;
            addr_buff[2] = (x2 >> 8) & 0xFF;
            addr_buff[3] = x2 & 0xFF;
            write_command((uint8_t)Command::CASET, addr_buff, 4, 1);

            addr_buff[0] = (y1 >> 8) & 0xFF;
            addr_buff[1] = y1 & 0xFF;
            addr_buff[2] = (y2 >> 8) & 0xFF;
            addr_buff[3] = y2 & 0xFF;
            write_command((uint8_t)Command::PASET, addr_buff, 4, 1);
        }

        inline void fastio(volatile uint8_t *port, uint8_t mask, uint8_t state)
        {
            if (state == 1)
            {
                *port |= mask;
            }
            else
            {
                *port &= ~mask;
            }
        }

        //pins
        uint8_t chip_select_pin,
                data_ncommand_pin,
                reset_pin;
        volatile uint8_t *chip_select_port, *data_ncommand_port, *reset_port;
        uint8_t chip_select_mask, data_ncommand_mask, reset_mask;

        // struct for holding spi settings
        SPISettings spi_settings;

        // text manipulation variables
        uint16_t
            // The current cursor position.
            cursor_xpos,
            cursor_ypos,

            // The "home" position of the cursor, text begins drawing here.
            cursor_start_xpos,
            cursor_start_ypos,

            // The limit position of the cursor, once text reaches this
            // location, further characters will be drawn starting from the
            // home position.
            cursor_end_xpos,
            cursor_end_ypos;

        Pixel text_color, text_bg_color;

        uint16_t text_scale;

        // Command set
        enum class Command : uint8_t
        {
            NOP      = 0x00,
            PWCTR1   = 0xC0,
            PWCTR2   = 0xC1,
            VMCTR1   = 0xC5,
            VMCTR2   = 0xC7,
            MADCTL   = 0x36,
            VSCRSADD = 0x37,
            PIXSEL   = 0x3A,
            FRMCTR1  = 0xB1,
            DFUNCTR  = 0xB6,
            GAMMASET = 0x26,
            GMCTRP1  = 0xE0,
            GMCTRN1  = 0xE1,
            SLPOUT   = 0x11,
            SLPIN    = 0x10,
            DISPON   = 0x29,
            CASET    = 0x2A,
            PASET    = 0x2B,
            RAMWR    = 0x2C,
            RAMRD    = 0x2E
        };
};

#endif
