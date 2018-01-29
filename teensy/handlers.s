.syntax unified

// export handler labels
.global _nmi_handler
.global _hardfault_handler
.global _memmanage_fault_handler
.global _bus_fault_handler
.global _usage_fault_handler
.global _svcall_handler
.global _systick_handler

// All interrupt handlers go here
.section ".handlers","x",%progbits

.thumb_func
_nmi_handler:
    bl debug_led_setup
    ldr r0, =2
    bl debug_led_hcf

.thumb_func
_hardfault_handler:
    bl debug_led_setup
    ldr r0, =3
    bl debug_led_hcf

.thumb_func
_memmanage_fault_handler:
    bl debug_led_setup
    ldr r0, =4
    bl debug_led_hcf

.thumb_func
_bus_fault_handler:
    bl debug_led_setup
    ldr r0, =5
    bl debug_led_hcf

.thumb_func
_usage_fault_handler:
    bl debug_led_setup
    ldr r0, =6
    bl debug_led_hcf

.thumb_func
_svcall_handler:
    bl debug_led_setup
    ldr r0, =11
    bl debug_led_hcf

.thumb_func
_systick_handler:
    bl debug_led_setup
    ldr r0, =15
    bl debug_led_hcf

.end
