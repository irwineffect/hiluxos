#include <Arduino.h>
#include "pin_defs.h"

void setup_pins(void)
{
    //pinMode(SPI_SCK_PIN, OUTPUT);
    //digitalWrite(SPI_SCK_PIN, LOW);

    //pinMode(SPI_MISO_PIN, INPUT);
    //digitalWrite(SPI_MISO_PIN, LOW);

    //pinMode(SPI_MOSI_PIN, OUTPUT);
    //digitalWrite(SPI_MOSI_PIN, LOW);

    pinMode(TFT_CS_PIN, OUTPUT);
    digitalWrite(TFT_CS_PIN, HIGH);

    //pinMode(TFT_DATA_NCOMMAND_PIN, OUTPUT);
    //digitalWrite(TFT_DATA_NCOMMAND_PIN, LOW);

    pinMode(TFT_RST_PIN, OUTPUT);
    digitalWrite(TFT_RST_PIN, HIGH);

    pinMode(TFT_BACKLIGHT_PIN, OUTPUT);
    digitalWrite(TFT_BACKLIGHT_PIN, HIGH);

    //pinMode(SD_CS_PIN, OUTPUT);
    //digitalWrite(SD_CS_PIN, HIGH);

    //pinMode(TOUCH_YP_PIN, INPUT);
    //digitalWrite(TOUCH_YP_PIN, LOW);

    //pinMode(TOUCH_YN_PIN, INPUT);
    //digitalWrite(TOUCH_YN_PIN, LOW);

    //pinMode(TOUCH_XP_PIN, INPUT);
    //digitalWrite(TOUCH_XP_PIN, LOW);

    //pinMode(TOUCH_XN_PIN, INPUT);
    //digitalWrite(TOUCH_XN_PIN, LOW);
}
