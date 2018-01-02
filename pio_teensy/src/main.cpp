#include <Arduino.h>

void setup(void)
{
    pinMode(13, OUTPUT);
    digitalWrite(13, LOW);
}

void loop(void)
{
    delay(1000);
    digitalWrite(13, HIGH);
    delay(250);
    digitalWrite(13, LOW);
}
