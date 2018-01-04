.syntax unified

.section ".vectors"
.long _estack  // IRQ  0: Inital Stack Pointer
.long _startup // IRQ  1: Inital Program Counter
.long _halt    // IRQ  2: Non-maskable Interrupt (NMI)
.long _halt    // IRQ  3: Hard Fault
.long _halt    // IRQ  4: MemManage Fault
.long _halt    // IRQ  5: Bus Fault
.long _halt    // IRQ  6: Usage Fault
.long _halt    // IRQ  7:
.long _halt    // IRQ  8:
.long _halt    // IRQ  9:
.long _halt    // IRQ 10:
.long _halt    // IRQ 11: Supervisor Call (SVCall)
.long _halt    // IRQ 12:
.long _halt    // IRQ 13:
.long _halt    // IRQ 14:
.long _halt    // IRQ 15: System Tick Timer (SysTick)


.section ".flashconfig"
.long 0xFFFFFFFF
.long 0xFFFFFFFF
.long 0xFFFFFFFF
.long 0xFFFFFFFE

.section ".startup","x",%progbits
.thumb_func // Not sure why, but this is needed
_startup:

    // First, disable the watchdog timer
    cpsid i // Disable interrupts

    // unlock watchdog by writing 0xC520 followed by 0xD928
    ldr r6, =0x4005200E
    ldr r0, =0xC520
    strh r0, [r6]
    ldr r0, =0xD928
    strh r0, [r6]

    // Disable watchdog
    ldr r6, =0x40052000
    ldr r0, =0x01D2
    strh r0, [r6]

    cpsie i // Enable interrupts

    // Enable system clock on all GPIO ports
    ldr r6, =0x40048038
    ldr r0, =0x00043F82
    str r0, [r6]

    // Configure the LED pin
    ldr r6, =0x4004B014
    ldr r0, =0x00000143
    str r0, [r6]

    // Set the LED pin (PTC 5) to output
    ldr r6, =0x400FF094
    ldr r0, =0x20
    str r0, [r6]

    ldr fp, =_estack // necessary?

    // Zero out .data section. Not Strictly necessary, but I want to know if
    // my data loading code below is broken, because it appears the
    // bootloader sets up the RAM when programming, but of course the RAM is
    // reset on power cycle. Clearing it out first hopefully should uncover
    // this bug if it happens

    ldr r5, =_sdata
    ldr r0, =0
    ldr r7, =_edata
    b data_clear_skip
    data_clear_loop:
        str r0, [r5]
        add r5, r5, #1
        data_clear_skip:
        cmp r5, r7
        bne data_clear_loop

    // load .data section from flash into RAM
    ldr r6, =_sflashdata //src
    ldr r5, =_sdata //dest
    ldr r7, =_edata //end
    b data_load_skip
    data_load_loop:
        ldr r0, [r6]
        str r0, [r5]
        add r5, r5, #1
        add r6, r6, #1
        data_load_skip:
        cmp r5, r7
        bne data_load_loop

    // zero out .bss section
    ldr r5, =_sbss
    ldr r0, =0
    ldr r7, =_ebss
    b bss_clear_skip
    bss_clear_loop:
        str r0, [r5]
        add r5, r5, #1
        bss_clear_skip:
        cmp r5, r7
        bne bss_clear_loop

    // Turn on LED
    //ldr r6, =0x400FF080
    //ldr r0, =0x20
    //str r0, [r6]

    bl main
    b _halt // Shouldn't ever get here

//loop:
//    bl led_on
//    bl delay
//    bl led_off
//    bl delay
//    bl delay
//    bl delay
//    bl delay
//    bl delay
//    b loop
//
//led_off:
//    ldr r6, =0x400FF080
//    ldr r0, =0x0
//    str r0, [r6]
//    mov pc, r14
//
//led_on:
//    ldr r6, =0x400FF080
//    ldr r0, =0x20
//    str r0, [r6]
//    mov pc, r14
//
//delay:
//    ldr r1, =0xA62A0
//    delay_loop:
//        sub r1, r1, #1
//        cmp r1, #0
//        bne delay_loop
//        mov pc, r14

_halt:
    b _halt

.end
