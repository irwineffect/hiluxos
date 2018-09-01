.syntax unified

// export handler labels
.global _nmi_handler
.global _hardfault_handler
.global _memmanage_fault_handler
.global _bus_fault_handler
.global _usage_fault_handler
.global _svcall_handler
.global _systick_handler
.global _interrupt_timer_0_handler
.global _interrupt_timer_1_handler
.global _interrupt_timer_2_handler
.global _interrupt_timer_3_handler

// All interrupt handlers go here
.section ".handlers","x",%progbits

.thumb_func
_nmi_handler:
    bl debug_init
    ldr r0, =2
    bl debug_led_hcf

.thumb_func
_hardfault_handler:
    bl debug_init
    ldr r0, =3
    bl debug_led_hcf

.thumb_func
_memmanage_fault_handler:
    bl debug_init
    ldr r0, =4
    bl debug_led_hcf

.thumb_func
_bus_fault_handler:
    bl debug_init
    ldr r0, =5
    bl debug_led_hcf

.thumb_func
_usage_fault_handler:
    bl debug_init
    ldr r0, =6
    bl debug_led_hcf

.thumb_func
_svcall_handler:
    bl debug_init
    ldr r0, =11
    bl debug_led_hcf

.thumb_func
_systick_handler:
    bl debug_init
    ldr r0, =15
    bl debug_led_hcf

.thumb_func
_interrupt_timer_0_handler:
    //push lr to the stack
    STMFD sp, {lr}
    bl ISR_interrupt_timer_0
    LDMFD sp, {lr}
    mov pc, lr

.thumb_func
_interrupt_timer_1_handler:
    //push lr to the stack
    STMFD sp, {lr}
    bl ISR_interrupt_timer_1
    LDMFD sp, {lr}
    mov pc, lr

.thumb_func
_interrupt_timer_2_handler:
    //push lr to the stack
    STMFD sp, {lr}
    bl ISR_interrupt_timer_2
    LDMFD sp, {lr}
    mov pc, lr

.thumb_func
_interrupt_timer_3_handler:
    //push lr to the stack
    STMFD sp, {lr}
    bl ISR_interrupt_timer_3
    LDMFD sp, {lr}
    mov pc, lr


.end
