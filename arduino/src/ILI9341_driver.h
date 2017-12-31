#ifndef ILI9341_DRIVER_H
#define ILI9341_DRIVER_H

#include <Arduino.h>
#include <SPI.h>

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
        void draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                Pixel color);
        void draw_circle(uint16_t x, uint16_t y, uint16_t r, Pixel color);

        static const uint16_t WIDTH  = 320,
                              HEIGHT = 240;

        static const uint32_t NUM_PIXELS_IN_SCREEN = 0x12C00;


    private:
        void write_command(uint8_t command, uint8_t data[], uint8_t len,
                uint8_t bulk_transfer=0);
        void write_pixels(Pixel data[], uint32_t len, uint8_t bulk_transfer=0,
                uint8_t repeat_pixel=0);
        void begin_spi(void);
        void end_spi(void);
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

        SPISettings spi_settings;

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
