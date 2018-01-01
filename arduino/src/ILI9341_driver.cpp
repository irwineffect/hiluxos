#include <Arduino.h>
#include <SPI.h>
#include "ILI9341_driver.h"
#include "font8x8_basic.h"

// for some reason Arduino is #defining putc as a macro?
#undef putc

ILI9341_driver::ILI9341_driver(void)
{
}

void ILI9341_driver::begin(uint8_t _cs, uint8_t _data_ncommand, uint8_t _rst)
{
    chip_select_pin = _cs;
    data_ncommand_pin = _data_ncommand;
    reset_pin = _rst;

    chip_select_port = portOutputRegister(digitalPinToPort(_cs));
    chip_select_mask = digitalPinToBitMask(_cs);

    data_ncommand_port = portOutputRegister(digitalPinToPort(_data_ncommand));
    data_ncommand_mask = digitalPinToBitMask(_data_ncommand);

    reset_port = portOutputRegister(digitalPinToPort(_rst));
    reset_mask = digitalPinToBitMask(_rst);

    // Set pin modes and intitalize pin states
    pinMode(chip_select_pin, OUTPUT);
    digitalWrite(chip_select_pin, HIGH);
    pinMode(data_ncommand_pin, OUTPUT);
    digitalWrite(data_ncommand_pin, HIGH);
    pinMode(reset_pin, OUTPUT);
    digitalWrite(reset_pin, HIGH);

    // Reset the display
    hardware_reset();

    // Setup SPI bus
    spi_settings = SPISettings(6000000, MSBFIRST, SPI_MODE0);
    SPI.begin();

    // Magic configuration taken from
    // https://github.com/adafruit/Adafruit_ILI9341/
    // The weird syntax is using string literals stored in flash, converted to
    // uint8_t arrays to make code a little cleaner.
    begin_spi();
    write_command(0xEF, (uint8_t*)"\x03\x80\x02", 3, 1);
    write_command(0xCF, (uint8_t*)"\x00\xC1\x30", 3, 1);
    write_command(0xED, (uint8_t*)"\x64\x03\x12\x81", 4, 1);
    write_command(0xE8, (uint8_t*)"\x85\x00\x78", 3, 1);
    write_command(0xCB, (uint8_t*)"\x39\x2C\x00\x34\x02", 5, 1);
    write_command(0xF7, (uint8_t*)"\x20", 1, 1);
    write_command(0xEA, (uint8_t*)"\x00\x00", 2, 1);

    write_command((uint8_t)Command::PWCTR1, (uint8_t*)"\x23", 1, 1);
    write_command((uint8_t)Command::PWCTR2, (uint8_t*)"\x10", 1, 1);
    write_command((uint8_t)Command::VMCTR1, (uint8_t*)"\x3E\x28", 2, 1);
    write_command((uint8_t)Command::VMCTR2, (uint8_t*)"\x86", 1, 1);
    write_command((uint8_t)Command::MADCTL, (uint8_t*)"\x28", 1, 1);
    write_command((uint8_t)Command::VSCRSADD, (uint8_t*)"\x00\x00", 2, 1);
    write_command((uint8_t)Command::PIXSEL, (uint8_t*)"\x66", 1, 1);
    write_command((uint8_t)Command::FRMCTR1, (uint8_t*)"\x00\x18", 2, 1);
    write_command((uint8_t)Command::DFUNCTR, (uint8_t*)"\x08\x82\x27", 3, 1);
    write_command((uint8_t)Command::GAMMASET, (uint8_t*)"\x01", 1, 1);
    write_command((uint8_t)Command::GMCTRP1, (uint8_t*)"\x0F\x31\x2B\x0C\x0E\x08\x4E"
                "\xF1\x37\x07\x10\x03\x0E\x09\x00", 15, 1);
    write_command((uint8_t)Command::GMCTRN1, (uint8_t*)"\x00\x0E\x14\x03\x11\x07\x31"
                "\xC1\x48\x08\x0F\x0C\x31\x36\x0F", 15, 1);
    write_command((uint8_t)Command::SLPOUT, 0, 0, 1);
    delay(5); //must wait 5ms if waking from sleep
    write_command((uint8_t)Command::DISPON, 0, 0, 1);
    end_spi();


    // initialize text variables
    cursor_start_xpos = 0;
    cursor_start_ypos = 0;
    cursor_end_xpos = WIDTH-1;
    cursor_end_ypos = HEIGHT-1;
    cursor_xpos = 0;
    cursor_ypos = 0;
    text_color = Color::white;
    text_bg_color = Color::black;
    text_scale = 1;
}


void ILI9341_driver::sleep(void)
{
    write_command((uint8_t)Command::SLPIN, 0, 0);
    delay(120);
}

void ILI9341_driver::wake(void)
{
    write_command((uint8_t)Command::SLPOUT, 0, 0);
    delay(5);
}

void ILI9341_driver::draw_pixel(Pixel color, uint16_t x, uint16_t y)
{
    uint8_t addr_buff[4];

    begin_spi();

    // Set the address for the pixel
    addr_buff[0] = (x >> 8) & 0xFF;
    addr_buff[1] = x & 0xFF;
    addr_buff[2] = (x >> 8) & 0xFF;
    addr_buff[3] = x & 0xFF;
    write_command((uint8_t)Command::CASET, addr_buff, 4, 1);

    addr_buff[0] = (y >> 8) & 0xFF;
    addr_buff[1] = y & 0xFF;
    addr_buff[2] = (y >> 8) & 0xFF;
    addr_buff[3] = y & 0xFF;
    write_command((uint8_t)Command::PASET, addr_buff, 4, 1);

    // set the data for the pixel
    write_pixels(&color, 1, 1);

    end_spi();
}

void ILI9341_driver::fill_screen(Pixel color)
{
    uint8_t addr_buff[4];
    begin_spi();

    // Set the start/end addresss for the write window
    // 0x13F (319) for the screen width (320)
    addr_buff[0] = 0;
    addr_buff[1] = 0;
    addr_buff[2] = (0x13F >> 8) & 0xFF;
    addr_buff[3] = 0x13F & 0xFF;
    write_command((uint8_t)Command::CASET, addr_buff, 4, 1);

    // 0xEF (239) for the screen height (240)
    addr_buff[0] = 0;
    addr_buff[1] = 0;
    addr_buff[2] = (0xEF >> 8) & 0xFF;
    addr_buff[3] = 0xEF & 0xFF;
    write_command((uint8_t)Command::PASET, addr_buff, 4, 1);

    // set the data for the pixel
    write_pixels(&color, NUM_PIXELS_IN_SCREEN, 1, 1);
    end_spi();
}

void ILI9341_driver::draw_line(Pixel color, uint16_t x1, uint16_t y1, uint16_t
        x2, uint16_t y2)
{
}

void ILI9341_driver::draw_hline(Pixel color, uint16_t x1, uint16_t x2, uint16_t
        y)
{
    uint8_t addr_buff[4];

    // if x1 is greater than x2, swap them
    if (x1 > x2)
    {
        uint16_t tmp = x1;
        x1 = x2;
        x2 = tmp;
    }

    // calculate the number of pixels in the line
    uint16_t num_pixels = x2 - x1;

    begin_spi();

    // Set the address for the pixel
    addr_buff[0] = (x1 >> 8) & 0xFF;
    addr_buff[1] = x1 & 0xFF;
    addr_buff[2] = (x2 >> 8) & 0xFF;
    addr_buff[3] = x2 & 0xFF;
    write_command((uint8_t)Command::CASET, addr_buff, 4, 1);

    addr_buff[0] = (y >> 8) & 0xFF;
    addr_buff[1] = y & 0xFF;
    addr_buff[2] = (y >> 8) & 0xFF;
    addr_buff[3] = y & 0xFF;
    write_command((uint8_t)Command::PASET, addr_buff, 4, 1);

    // set the data for the pixels
    write_pixels(&color, num_pixels, 1, 1);

    end_spi();
}

void ILI9341_driver::draw_vline(Pixel color, uint16_t y1, uint16_t y2, uint16_t
        x)
{
    uint8_t addr_buff[4];

    // if y1 is greater than y2, swap them
    if (y1 > y2)
    {
        uint16_t tmp = y1;
        y1 = y2;
        y2 = tmp;
    }

    // calculate the number of pixels in the line
    uint16_t num_pixels = y2 - y1;

    begin_spi();

    // Set the address for the pixel
    addr_buff[0] = (x >> 8) & 0xFF;
    addr_buff[1] = x & 0xFF;
    addr_buff[2] = (x >> 8) & 0xFF;
    addr_buff[3] = x & 0xFF;
    write_command((uint8_t)Command::CASET, addr_buff, 4, 1);

    addr_buff[0] = (y1 >> 8) & 0xFF;
    addr_buff[1] = y1 & 0xFF;
    addr_buff[2] = (y2 >> 8) & 0xFF;
    addr_buff[3] = y2 & 0xFF;
    write_command((uint8_t)Command::PASET, addr_buff, 4, 1);

    // set the data for the pixels
    write_pixels(&color, num_pixels, 1, 1);

    end_spi();

}

void ILI9341_driver::draw_circle(Pixel color, uint16_t x, uint16_t y, uint16_t
        r)
{
}

void ILI9341_driver:: draw_rectangle(Pixel color, uint16_t x, uint16_t y,
        uint16_t w, uint16_t h)
{
    uint16_t x2 = x + w - 1;
    uint16_t y2 = y + h - 1;

    begin_spi();

    uint32_t num_pixels = (uint32_t) w * h;
    write_window(x, y, x2, y2);
    write_pixels(&color, num_pixels, 1, 1);

    end_spi();
}

void ILI9341_driver::draw_char(Pixel color, char c, uint16_t x, uint16_t y,
        Pixel bg)
{
    begin_spi();
    write_window(x, y, x+(CHARACTER_WIDTH*text_scale)-1,
            y+(CHARACTER_WIDTH*text_scale)-1);

    // send ram write command
    fastio(data_ncommand_port, data_ncommand_mask, 0);
    SPDR = static_cast<uint8_t>(Command::RAMWR);
    while(!(SPSR & _BV(SPIF)));
    fastio(data_ncommand_port, data_ncommand_mask, 1);

    Pixel *current_pixel;

    for (uint8_t i = 0; i < 8; ++i)
    {
       uint8_t current_byte = pgm_read_byte(&(font8x8_basic[(uint8_t)c][i]));
       for (uint8_t v_scale = 0; v_scale < text_scale; ++v_scale)
       {
           for (uint8_t j = 0; j < 8; ++j)
           {
               for (uint8_t h_scale = 0; h_scale < text_scale; ++h_scale)
               {
                    if (current_byte & (1 << j))
                    {
                        current_pixel = &color;
                    }
                    else
                    {
                        current_pixel = &bg;
                    }

                    SPDR = current_pixel->red << 2;
                    while(!(SPSR & _BV(SPIF)));

                    SPDR = current_pixel->green << 2;
                    while(!(SPSR & _BV(SPIF)));

                    SPDR = current_pixel->blue << 2;
                    while(!(SPSR & _BV(SPIF)));
               }
           }
       }
    }

    write_command((uint8_t)Command::NOP, 0, 0, 1);
    end_spi();
}

void ILI9341_driver::set_cursor_start(uint16_t x, uint16_t y)
{
    cursor_start_xpos = x;
    cursor_start_ypos = y;

    cursor_xpos = x;
    cursor_ypos = y;
}

void ILI9341_driver::set_cursor_end(uint16_t x, uint16_t y)
{
    cursor_end_xpos = x;
    cursor_end_ypos = y;
}

void ILI9341_driver::set_cursor(uint16_t x, uint16_t y)
{
    cursor_xpos = x;
    cursor_ypos = y;
}

void ILI9341_driver::reset_cursor(void)
{
    cursor_xpos = cursor_start_xpos;
    cursor_ypos = cursor_start_ypos;
}


void ILI9341_driver::set_text_color(Pixel color)
{
    text_color = color;
}

void ILI9341_driver::set_text_bg_color(Pixel color)
{
    text_bg_color = color;
}

void ILI9341_driver::set_text_scale(uint8_t scale)
{
    text_scale = scale;
}

void ILI9341_driver::putc(char c)
{
    switch (c)
    {
        case '\n':
            cursor_xpos = cursor_start_xpos;
            cursor_ypos += CHARACTER_HEIGHT*text_scale;

            if ((cursor_ypos + CHARACTER_HEIGHT*text_scale) >= cursor_end_ypos)
            {
                cursor_ypos = cursor_start_ypos;
            }
            break;
        case '\r':
            cursor_xpos = cursor_start_xpos;
            break;

        default:
            draw_char(text_color, c, cursor_xpos, cursor_ypos, text_bg_color);

            // Logic for advancing the cursor.
            cursor_xpos += CHARACTER_WIDTH*text_scale;
            if ((cursor_xpos + CHARACTER_WIDTH*text_scale) >= cursor_end_xpos)
            {
                cursor_xpos = cursor_start_xpos;
                cursor_ypos += CHARACTER_HEIGHT*text_scale;

                if ((cursor_ypos + CHARACTER_HEIGHT*text_scale) >= cursor_end_ypos)
                {
                    cursor_ypos = cursor_start_ypos;
                }
            }
            break;
    }
}

void ILI9341_driver::puts(const char c[])
{
    while (*c != '\0')
    {
        putc(*c);
        ++c;
    }
}


void ILI9341_driver::hardware_reset(void)
{
    digitalWrite(reset_pin, LOW);
    // reset must be pulled low for at least 10 microseconds
    delayMicroseconds(15);
    digitalWrite(reset_pin, HIGH);
    // must wait at least 120 milliseconds for display to accept commands
    delay(150);
}

void ILI9341_driver::write_command(uint8_t command, uint8_t data[], uint8_t
        len, uint8_t bulk_transfer)
{
    if (!bulk_transfer)
    {
        begin_spi();
    }

    // send the command byte, while the data/ncommand pin is low
    fastio(data_ncommand_port, data_ncommand_mask, 0);
    SPDR = command;
    while(!(SPSR & _BV(SPIF)));
    fastio(data_ncommand_port, data_ncommand_mask, 1);

    //tranfer any parameters associated with the command
    for (uint8_t i=0; i < len; ++i)
    {
        SPDR = data[i];
        while(!(SPSR & _BV(SPIF)));
    }

    if (!bulk_transfer)
    {
        end_spi();
    }
}

void ILI9341_driver::write_pixels(Pixel data[], uint32_t len, uint8_t
        bulk_transfer, uint8_t repeat_pixel)
{
    if (!bulk_transfer)
    {
        begin_spi();
    }

    // send the command byte, while the data/ncommand pin is low
    fastio(data_ncommand_port, data_ncommand_mask, 0);
    SPDR = static_cast<uint8_t>(Command::RAMWR);
    while(!(SPSR & _BV(SPIF)));
    fastio(data_ncommand_port, data_ncommand_mask, 1);

    // Write out the pixels
    uint32_t j = 0;
    for (uint32_t i = 0; i < len; ++i)
    {
        SPDR = data[j].red << 2;
        while(!(SPSR & _BV(SPIF)));

        SPDR = data[j].green << 2;
        while(!(SPSR & _BV(SPIF)));

        SPDR = data[j].blue << 2;
        while(!(SPSR & _BV(SPIF)));

        if (repeat_pixel == 0)
        {
            ++j;
        }
    }

    write_command((uint8_t)Command::NOP, 0, 0, 1);
    if (!bulk_transfer)
    {
        end_spi();
    }
}

void ILI9341_driver::begin_spi(void)
{
    fastio(chip_select_port, chip_select_mask, 0);
    SPI.beginTransaction(spi_settings);
}

void ILI9341_driver::end_spi(void)
{
    fastio(chip_select_port, chip_select_mask, 1);
    SPI.endTransaction();
}
