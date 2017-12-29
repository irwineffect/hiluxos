#include <Arduino.h>
#include "pin_defs.h"
#include "utility.h"
//#include <Adafruit_GFX.h>
//#include <Adafruit_ILI9341.h>
#include "ILI9341_driver.h"
#include <TouchScreen.h>

ILI9341_driver display = ILI9341_driver();
//TouchScreen ts = TouchScreen(TOUCH_XP_PIN, TOUCH_YP_PIN, TOUCH_XN_PIN,
        //TOUCH_YN_PIN, 300);

void setup()
{
    //setup_pins();
    Serial.begin(9600);
    display.begin(TFT_CS_PIN, TFT_DATA_NCOMMAND_PIN, TFT_RST_PIN);
    display.fill_screen(Color::black);
}

void loop()
{
    delay(1000);
    display.fill_screen(Color::blue);
    delay(1000);
    display.fill_screen(Color::green);
    Serial.println("got here");

}
