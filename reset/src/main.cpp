#include <Arduino.h>
#include <SoftwareSerial.h>

uint8_t counter;
SoftwareSerial sbridge(3, 4); //RX, TX

void setup(void)
{
    Serial.begin(115200);
    Serial.println("resetter initialized");

    counter = 0;

    digitalWrite(2, HIGH);
    pinMode(2, OUTPUT);
    digitalWrite(2, HIGH);

    sbridge.begin(115200);
}

void reset_teensy(void)
{
    Serial.println("\n***resetting teensy***\n");
    digitalWrite(2, LOW);
    delay(250);
    digitalWrite(2, HIGH);
}

void loop(void)
{
    if (Serial.available())
    {
       uint8_t b = Serial.read();
       //sbridge.write(b);
       b = b - '0';
       //Serial.print("got ");
       //Serial.print(b);
       //Serial.print(", expected ");
       //Serial.print(counter);
       //Serial.println();
       if (b == counter)
       {
           ++counter;
       }
       else if (b == 0)
       {
           counter = 1;
       }
       else
       {
           counter = 0;
       }
       if (counter == 3)
       {
           reset_teensy();
           counter = 0;
       }
    }

    if (sbridge.available())
    {
        Serial.write(sbridge.read());
    }
}

