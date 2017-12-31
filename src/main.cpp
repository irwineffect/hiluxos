#include <Arduino.h>
#include "pin_defs.h"
#include "utility.h"
//#include <Adafruit_GFX.h>
//#include <Adafruit_ILI9341.h>
#include "ILI9341_driver.h"
//#include <TouchScreen.h>

ILI9341_driver display = ILI9341_driver();
//Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS_PIN, TFT_DATA_NCOMMAND_PIN, TFT_RST_PIN);
//TouchScreen ts = TouchScreen(TOUCH_XP_PIN, TOUCH_YP_PIN, TOUCH_XN_PIN,
        //TOUCH_YN_PIN, 300);

void setup()
{
    //setup_pins();
    Serial.begin(9600);
    //tft.begin();
    //tft.fillScreen(ILI9341_BLUE);
    Serial.println("serial initialized");
    Serial.println("setting up display");
    display.begin(TFT_CS_PIN, TFT_DATA_NCOMMAND_PIN, TFT_RST_PIN);
    Serial.println("display initialized");
    delay(1000);
    Serial.println("filling screen");
    display.fill_screen(Color::black);
    Serial.println("setup complete");
}

void loop()
{
    delay(1000);
    display.fill_screen(Color::blue);
    delay(1000);
    display.fill_screen(Color::green);

}
