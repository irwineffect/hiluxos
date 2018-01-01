#include <Arduino.h>
#include "pin_defs.h"
#include "utility.h"
#include "ILI9341_driver.h"
#include <TouchScreen.h>

ILI9341_driver display = ILI9341_driver();
TouchScreen ts = TouchScreen(TOUCH_XP_PIN, TOUCH_YP_PIN, TOUCH_XN_PIN,
        TOUCH_YN_PIN, 300);

void setup()
{
    Serial.begin(9600);
    Serial.println("serial initialized");
    Serial.println("setting up display");
    display.begin(TFT_CS_PIN, TFT_DATA_NCOMMAND_PIN, TFT_RST_PIN);
    Serial.println("display initialized");
    Serial.println("filling screen");
    display.fill_screen(Color::black);
    Serial.println("setup complete");

    //display.set_cursor_home(300, 0);
    display.puts("hello worlds! 0\r");
    delay(1000);
    display.puts("hello worlds! 1\r");
    delay(1000);
    display.puts("hello worlds! 2\r");
    delay(1000);
    display.puts("hello worlds! 3\n");
    delay(1000);
}

uint16_t x = 0, y = 0;
uint16_t prev_x = 0, prev_y = 0;
uint32_t time = 0;
uint8_t boxsize = 10;
void loop()
{

    delay(100);
    display.puts("hello worlds!\n");
//    prev_x = x;
//    prev_y = y;
//    x += boxsize;
//    if (x > display.WIDTH-1)
//    {
//        x = 0;
//        y += boxsize;
//        if (y > display.HEIGHT-1)
//        {
//            y = 0;
//        }
//    }
//    display.draw_rectangle(Color::green, x, y, boxsize, boxsize);
//    display.draw_rectangle(Color::black, prev_x, prev_y, boxsize, boxsize);
//    delay(100);
}
