#include <Arduino.h>
#include <SPI.h>
#include "ILI9341_driver.h"

ILI9341_driver::ILI9341_driver(void)
{
}

void ILI9341_driver::begin(uint8_t _cs, uint8_t _data_ncommand, uint8_t _rst)
{
    chip_select_pin = _cs;
    data_ncommand_pin = _data_ncommand;
    reset_pin = _rst;

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
    write_command(0xEF, (uint8_t*)F("\x03\x80\x02"), 3, 1);
    write_command(0xCF, (uint8_t*)F("\x00\xC1\x30"), 3, 1);
    write_command(0xED, (uint8_t*)F("\x64\x03\x12\x81"), 4, 1);
    write_command(0xE8, (uint8_t*)F("\x85\x00\x78"), 3, 1);
    write_command(0xCB, (uint8_t*)F("\x39\x2C\x00\x34\x02"), 5, 1);
    write_command(0xF7, (uint8_t*)F("\x20"), 1, 1);
    write_command(0xEA, (uint8_t*)F("\x00\x00"), 2, 1);

    write_command((uint8_t)Command::PWCTR1, (uint8_t*)F("\x23"), 1, 1);
    write_command((uint8_t)Command::PWCTR2, (uint8_t*)F("\x10"), 1, 1);
    write_command((uint8_t)Command::VMCTR1, (uint8_t*)F("\x3E\x28"), 2, 1);
    write_command((uint8_t)Command::VMCTR2, (uint8_t*)F("\x86"), 1, 1);
    write_command((uint8_t)Command::MADCTL, (uint8_t*)F("\x48"), 1, 1);
    write_command((uint8_t)Command::VSCRSADD, (uint8_t*)F("\x00\x00"), 2, 1);
    write_command((uint8_t)Command::PIXSEL, (uint8_t*)F("\x66"), 1, 1);
    write_command((uint8_t)Command::FRMCTR1, (uint8_t*)F("\x00\x18"), 2, 1);
    write_command((uint8_t)Command::DFUNCTR, (uint8_t*)F("\x08\x82\x27"), 3, 1);
    write_command((uint8_t)Command::GAMMASET, (uint8_t*)F("\x01"), 1, 1);
    write_command((uint8_t)Command::GMCTRP1, (uint8_t*)F("\x0F\x31\x2B\x0C\x0E\x08\x4E"
                "\xF1\x37\x07\x10\x03\x0E\x09\x00"), 15, 1);
    write_command((uint8_t)Command::GMCTRN1, (uint8_t*)F("\x00\x0E\x14\x03\x11\x07\x31"
                "\xC1\x48\x08\x0F\x0C\x31\x36\x0F"), 15, 1);
    write_command((uint8_t)Command::SLPOUT, 0, 0, 1);
    delay(5); //must wait 5ms if waking from sleep
    write_command((uint8_t)Command::DISPON, 0, 0, 1);
    end_spi();
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

void ILI9341_driver::draw_pixel(uint16_t x, uint16_t y, Pixel color)
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
    write_pixels(&color, 0, 1);

    end_spi();
}

void ILI9341_driver::fill_screen(Pixel color)
{
    uint8_t addr_buff[4];

    begin_spi();

    // Set the address for the pixel
    // 0x13F (320) is the screen width
    addr_buff[0] = 0;
    addr_buff[1] = 0;
    addr_buff[2] = (0x13F >> 8) & 0xFF;
    addr_buff[3] = 0x13F & 0xFF;
    write_command((uint8_t)Command::CASET, addr_buff, 4, 1);

    // 0xEF (240) is the screen height
    addr_buff[0] = 0;
    addr_buff[1] = 0;
    addr_buff[2] = (0xEF >> 8) & 0xFF;
    addr_buff[3] = 0xEF & 0xFF;
    write_command((uint8_t)Command::PASET, addr_buff, 4, 1);

    // set the data for the pixel
    write_pixels(&color, NUM_PIXELS_IN_SCREEN, 1, 1);

    end_spi();
}

void ILI9341_driver::draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t
        y2, Pixel color)
{
}

void ILI9341_driver::draw_circle(uint16_t x, uint16_t y, uint16_t r, Pixel
        color)
{
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
    digitalWrite(data_ncommand_pin, LOW);
    SPDR = command;
    while(!SPSR & _BV(SPIF));
    digitalWrite(data_ncommand_pin, HIGH);

    //tranfer any parameters associated with the command
    for (uint8_t i=0; i < len; ++i)
    {
        SPDR = data[i];
        while(!SPSR & _BV(SPIF));
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
    digitalWrite(data_ncommand_pin, LOW);
    SPDR = static_cast<uint8_t>(Command::RAMWR);
    while(!SPSR & _BV(SPIF));
    digitalWrite(data_ncommand_pin, HIGH);

    // Write out the pixels
    if (repeat_pixel == 1)
    {
        for (uint32_t i = 0; i < len; ++i)
        {
            SPDR = data[0].red;
            while(!SPSR & _BV(SPIF));

            SPDR = data[0].green;
            while(!SPSR & _BV(SPIF));

            SPDR = data[0].blue;
            while(!SPSR & _BV(SPIF));
        }
    }
    else
    {
        for (uint32_t i = 0; i < len; ++i)
        {
            SPDR = data[i].red;
            while(!SPSR & _BV(SPIF));

            SPDR = data[i].green;
            while(!SPSR & _BV(SPIF));

            SPDR = data[i].blue;
            while(!SPSR & _BV(SPIF));
        }
    }

    if (!bulk_transfer)
    {
        write_command((uint8_t)Command::NOP, 0, 0, 1);
        end_spi();
    }
}

void ILI9341_driver::begin_spi(void)
{
    digitalWrite(chip_select_pin, LOW);
    SPI.beginTransaction(spi_settings);
}

void ILI9341_driver::end_spi(void)
{
    digitalWrite(chip_select_pin, HIGH);
    SPI.endTransaction();
}
