.syntax unified
.global _svcall_handler

// All interrupt handlers go here
.section ".handlers","x",%progbits
.thumb_func
_svcall_handler:
    bl debug_led_setup
    ldr r0, =0x02
    bl debug_led_hcf

.end
