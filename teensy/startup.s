.syntax unified
.global _halt
.global _exit

.section ".vectors"
.long _estack                  // IRQ  0: Inital Stack Pointer
.long _startup                 // IRQ  1: Inital Program Counter
.long _nmi_handler             // IRQ  2: Non-maskable Interrupt (NMI)
.long _hardfault_handler       // IRQ  3: Hard Fault
.long _memmanage_fault_handler // IRQ  4: MemManage Fault
.long _bus_fault_handler       // IRQ  5: Bus Fault
.long _usage_fault_handler     // IRQ  6: Usage Fault
.long _halt                    // IRQ  7:
.long _halt                    // IRQ  8:
.long _halt                    // IRQ  9:
.long _halt                    // IRQ 10:
.long _svcall_handler          // IRQ 11: Supervisor Call (SVCall)
.long _halt                    // IRQ 12:
.long _halt                    // IRQ 13:
.long _halt                    // IRQ 14:
.long _systick_handler         // IRQ 15: System Tick Timer (SysTick)

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

    // Initialize debug functionality
    bl debug_init

    // Jump to kernel main
    bl main
    b _halt // Shouldn't ever get here

_exit:
    b _halt

.thumb_func
_halt:
    // Configure the LED pin
    ldr r6, =0x4004B014
    ldr r0, =0x00000143
    str r0, [r6]

    // Set the LED pin (PTC 5) to output
    ldr r6, =0x400FF094
    ldr r0, =0x20
    str r0, [r6]

    ldr r6, =0x400FF080
    ldr r0, =0x00
    ldr r1, =0x20
    ldr r3, =0x2FFFF

halt_loop:
    str r1, [r6]
    bl halt_delay
    str r0, [r6]
    bl halt_delay
    b halt_loop

halt_delay:
    mov r4, r3
    sub r4, r4, #1
    cmp r4, #0
    bne [halt_delay+2]
    mov pc, r14

.end
