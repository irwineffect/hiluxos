#include <Arduino.h>
#include "pin_defs.h"
#include "utility.h"
#include "ILI9341_driver.h"
#include <TouchScreen.h>

ILI9341_driver display = ILI9341_driver();
TouchScreen ts = TouchScreen(TOUCH_XP_PIN, TOUCH_YP_PIN, TOUCH_XN_PIN,
        TOUCH_YN_PIN, 300);

char buff[32];
class Time
{
    public:
        Time(void)
        {
            seconds = 0;
            minutes = 0;
            hours   = 0;
        }

        void update(void)
        {
            seconds++;
            if (seconds >= 60)
            {
                seconds = 0;
                minutes++;

                if (minutes >= 60)
                {
                    minutes = 0;
                    hours++;

                    if (hours >= 24)
                    {
                        hours = 0;
                    }
                }
            }
        }

        void show(ILI9341_driver &display)
        {
            display.reset_cursor();
            display.set_text_scale(6);
            if (hours < 10)
            {
                display.putc('0');
            }
            display.puts(itoa(hours, buff, 10));
            display.putc(':');
            if (minutes < 10)
            {
                display.putc('0');
            }
            display.puts(itoa(minutes, buff, 10));
            display.set_text_scale(2);
            if (seconds < 10)
            {
                display.putc('0');
            }
            display.puts(itoa(seconds, buff, 10));
        }

    private:
        uint8_t hours,
                minutes,
                seconds;
};

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

    display.set_text_color(Color::green);
}

uint16_t x = 0, y = 0;
uint16_t prev_x = 0, prev_y = 0;
uint8_t boxsize = 10;
Time time = Time();




void loop()
{

    time.show(display);
    time.update();
    delay(1000);


    //display.reset_cursor();
    //display.set_text_scale(x);
    //display.puts("10\n45\r");
    //x += 1;
    //delay (1000);
    //delay(100);
    //display.puts("hello worlds!\n");
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
