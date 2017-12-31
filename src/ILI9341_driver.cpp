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

    chip_select_port = portOutputRegister(digitalPinToPort(_cs));
    chip_select_mask = digitalPinToBitMask(_cs);

    data_ncommand_port = portOutputRegister(digitalPinToPort(_data_ncommand));
    data_ncommand_mask = digitalPinToBitMask(_data_ncommand);

    reset_port = portOutputRegister(digitalPinToPort(_rst));
    reset_mask = digitalPinToBitMask(_rst);

    //Serial.println("ports/masks:");
    //Serial.print((uint8_t)chip_select_port); Serial.print("\t"); Serial.println(chip_select_mask, BIN);
    //Serial.print((uint8_t)data_ncommand_port); Serial.print("\t"); Serial.println(data_ncommand_mask, BIN);
    //Serial.print((uint8_t)reset_port); Serial.print("\t"); Serial.println(reset_mask, BIN);


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
    spi_settings = SPISettings(8000000, MSBFIRST, SPI_MODE0);
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
    Serial.println("filling screen with: ");
    Serial.print("  red: 0x"); Serial.println(color.red, HEX);
    Serial.print("  green: 0x"); Serial.println(color.green, HEX);
    Serial.print("  blue: 0x"); Serial.println(color.blue, HEX);

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
    write_pixels(&color, 0x12C00, 1, 1);
    write_command((uint8_t)Command::NOP, 0, 0, 1);
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

    Serial.println("write_command()");
    Serial.print("  command: 0x"); Serial.println(command, HEX);
    // send the command byte, while the data/ncommand pin is low
    //Serial.print("pin_state: "); Serial.println(digitalRead(data_ncommand_pin));
    fastio(data_ncommand_port, data_ncommand_mask, 0);
    //Serial.print("pin_state: "); Serial.println(digitalRead(data_ncommand_pin));
    SPDR = command;
    while(!(SPSR & _BV(SPIF)));
    //Serial.print("pin_state: "); Serial.println(digitalRead(data_ncommand_pin));
    fastio(data_ncommand_port, data_ncommand_mask, 1);
    //Serial.print("pin_state: "); Serial.println(digitalRead(data_ncommand_pin));

    //tranfer any parameters associated with the command
    for (uint8_t i=0; i < len; ++i)
    {
        Serial.print("  data: 0x"); Serial.println(data[i], HEX);
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
    Serial.println("write_pixels()");
    Serial.print("  red: 0x"); Serial.println(data[0].red, HEX);
    Serial.print("  green: 0x"); Serial.println(data[0].green, HEX);
    Serial.print("  blue: 0x"); Serial.println(data[0].blue, HEX);
    Serial.print("  len: 0x"); Serial.println(len, HEX);
    Serial.print("  repeat_pixel: "); Serial.println(repeat_pixel);

    // send the command byte, while the data/ncommand pin is low
    fastio(data_ncommand_port, data_ncommand_mask, 0);
    SPDR = static_cast<uint8_t>(Command::RAMWR);
    while(!(SPSR & _BV(SPIF)));
    fastio(data_ncommand_port, data_ncommand_mask, 1);

    Serial.println("writing pixels");
    // Write out the pixels
    if (repeat_pixel == 1)
    {
        for (uint32_t i = 0; i < len; ++i)
        {
            //Serial.println(i);
            SPDR = data[0].red << 2;
            while(!(SPSR & _BV(SPIF)));

            SPDR = data[0].green << 2;
            while(!(SPSR & _BV(SPIF)));

            SPDR = data[0].blue << 2;
            while(!(SPSR & _BV(SPIF)));
        }
    }
    else
    {
        for (uint32_t i = 0; i < len; ++i)
        {
            SPDR = data[i].red;
            while(!(SPSR & _BV(SPIF)));

            SPDR = data[i].green;
            while(!(SPSR & _BV(SPIF)));

            SPDR = data[i].blue;
            while(!(SPSR & _BV(SPIF)));
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
    Serial.println("begin_spi()");
    //Serial.print("pin_state: "); Serial.println(digitalRead(chip_select_pin));
    fastio(chip_select_port, chip_select_mask, 0);
    //Serial.print("pin_state: "); Serial.println(digitalRead(chip_select_pin));
    SPI.beginTransaction(spi_settings);
}

void ILI9341_driver::end_spi(void)
{
    //Serial.print("pin_state: "); Serial.println(digitalRead(chip_select_pin));
    fastio(chip_select_port, chip_select_mask, 1);
    //Serial.print("pin_state: "); Serial.println(digitalRead(chip_select_pin));
    SPI.endTransaction();
    Serial.println("end_spi()");
}
