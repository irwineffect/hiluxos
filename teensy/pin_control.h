#ifndef PIN_CONTROL_H
#define PIN_CONTROL_H

#include <cstdint>

typedef struct
{
    uint32_t pins[32];
} PIN_CONTROL_REGISTER_MAP;

typedef struct
{
    uint32_t PDOR; // 0x00 data output
    uint32_t PSOR; // 0x04 set output
    uint32_t PCOR; // 0x08 clear output
    uint32_t PTOR; // 0x0C toggle output
    uint32_t PDIR; // 0x10 data input
    uint32_t PDDR; // 0x14 data direction register
} GPIO_REGISTER_MAP;

typedef struct
{
    PIN_CONTROL_REGISTER_MAP *pin_control_regs;
    GPIO_REGISTER_MAP *gpio_control_regs;
} PORT;

extern const PORT port_a,
                  port_b,
                  port_c,
                  port_d,
                  port_e;

void pin_set_mux(const PORT *port, uint8_t pin, uint8_t mux);
void pin_set_drive_strength(const PORT *port, uint8_t pin, uint8_t drive);
void pin_set_data_direction(const PORT *port, uint8_t pin, uint8_t out_nin);
uint8_t pin_read(const PORT *port, uint8_t pin);
void pin_write(const PORT *port, uint8_t pin, uint8_t state);

#endif
