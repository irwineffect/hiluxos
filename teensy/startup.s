.syntax unified
.global _halt
.global _exit
.global __dso_handle
.global _init

.section ".vectors"
.long _estack                      //   0: Inital Stack Pointer
.long _startup                     //   1: Inital Program Counter
.long _nmi_handler                 //   2: Non-maskable Interrupt (NMI)
.long _hardfault_handler           //   3: Hard Fault
.long _memmanage_fault_handler     //   4: MemManage Fault
.long _bus_fault_handler           //   5: Bus Fault
.long _usage_fault_handler         //   6: Usage Fault
.long _halt                        //   7:
.long _halt                        //   8:
.long _halt                        //   9:
.long _halt                        //  10:
.long _svcall_handler              //  11: Supervisor Call (SVCall)
.long _halt                        //  12:
.long _halt                        //  13:
.long _halt                        //  14:
.long _systick_handler             //  15: System Tick Timer (SysTick)
.long _halt                        //  16: DMA  0 transfer complete
.long _halt                        //  17: DMA  1 transfer complete
.long _halt                        //  18: DMA  2 transfer complete
.long _halt                        //  19: DMA  3 transfer complete
.long _halt                        //  20: DMA  4 transfer complete
.long _halt                        //  21: DMA  5 transfer complete
.long _halt                        //  22: DMA  6 transfer complete
.long _halt                        //  23: DMA  7 transfer complete
.long _halt                        //  24: DMA  8 transfer complete
.long _halt                        //  25: DMA  9 transfer complete
.long _halt                        //  26: DMA 10 transfer complete
.long _halt                        //  27: DMA 11 transfer complete
.long _halt                        //  28: DMA 12 transfer complete
.long _halt                        //  29: DMA 13 transfer complete
.long _halt                        //  30: DMA 14 transfer complete
.long _halt                        //  31: DMA 15 transfer complete
.long _halt                        //  32:
.long _halt                        //  33:
.long _halt                        //  34:
.long _halt                        //  35:
.long _halt                        //  36:
.long _halt                        //  37:
.long _halt                        //  38:
.long _halt                        //  39:
.long _halt                        //  40:
.long _halt                        //  41:
.long _halt                        //  42:
.long _halt                        //  43:
.long _halt                        //  44:
.long _halt                        //  45:
.long _halt                        //  46:
.long _halt                        //  47:
.long _halt                        //  48:
.long _halt                        //  49:
.long _halt                        //  50:
.long _halt                        //  51:
.long _halt                        //  52:
.long _halt                        //  53:
.long _halt                        //  54:
.long _halt                        //  55:
.long _halt                        //  56:
.long _halt                        //  57:
.long _halt                        //  58:
.long _halt                        //  59:
.long _halt                        //  60:
.long _halt                        //  61:
.long _halt                        //  62:
.long _halt                        //  63:
.long ISR_interrupt_timer_0        //  64: Interrupt timer 0
.long ISR_interrupt_timer_1        //  65: Interrupt timer 1
.long ISR_interrupt_timer_2        //  66: Interrupt timer 2
.long ISR_interrupt_timer_3        //  67: Interrupt timer 3
.long _halt                        //  68:
.long _halt                        //  69:
.long _halt                        //  70:
.long _halt                        //  71:
.long _halt                        //  72:
.long _halt                        //  73:
.long _halt                        //  74:
.long _halt                        //  75:
.long _halt                        //  76:
.long _halt                        //  77:
.long _halt                        //  78:
.long _halt                        //  79:
.long _halt                        //  80:
.long _halt                        //  81:
.long _halt                        //  82:
.long _halt                        //  83:
.long _halt                        //  84:
.long _halt                        //  85:
.long _halt                        //  86:
.long _halt                        //  87:
.long _halt                        //  88:
.long _halt                        //  89:
.long _halt                        //  90:
.long _halt                        //  91:
.long _halt                        //  92:
.long _halt                        //  93:
.long _halt                        //  94:
.long _halt                        //  95:
.long _halt                        //  96:
.long _halt                        //  97:
.long _halt                        //  98:
.long _halt                        //  99:
.long _halt                        //  100:
.long _halt                        //  101:

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

    // Initialize C++ junk
    bl __libc_init_array

    // Jump to kernel main
    bl main
    b _halt // Shouldn't ever get here

_exit:
    b _halt
__dso_handle:
    b _halt
_init:
    mov pc, r14

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
