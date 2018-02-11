# hiluxos
hiluxos is a project to build an information display console for my car, using
a [Teensy 3.5](https://www.pjrc.com/store/teensy35.html) ARM development board
and a [2.4 inch color touchscreen
display](https://www.adafruit.com/product/2478).

I'm trying to do most everything from scratch as a learning project for
embedded C++ and ARM development.

This is still a major work in progress, I'm working on bringing up the basic
hardware peripherals on the ARM chip. Once basic screen control is running,
initial high-level features planned are a clock, voltage monitoring, and
temperature sensing.

## Filesystem orgainization
- `teensy/`: The main project.
- `nano/`: Arduino platformio project focused on learning how to control the
  display.
- `reset/`: A simple UART bridge and resetting module using an Arduino nano to
  allow headless development on the teensy.

## Progress
- [x] Initial bootup from assembly to C/C++
- [x] GPIO control
- [x] UART
- [ ] SPI
- [ ] RTC
- [ ] ADC
- [ ] Sleep mode (to reduce power consumption when car is off, if necessary)
