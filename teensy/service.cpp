#include <cstdarg>
#include "uart.h"
#include "debug.h"
#include "printf.h"
#include "service.h"

// Hacky work-around for returning data from service handler, should try to
// pass by register.
static volatile uint32_t svc_return_code;

extern "C" void _service_handler(SVC_TYPE type, uint32_t a, uint32_t b, uint32_t c)
{
    printf("service call! %d\n", type);
    printf("a: %d  b: %d  c: %d\n", a, b, c);

    svc_return_code = 98;
}

uint32_t svc(SVC_TYPE type, uint32_t a, uint32_t b, uint32_t c)
{
    asm volatile("svc 0");

    return svc_return_code;
}
