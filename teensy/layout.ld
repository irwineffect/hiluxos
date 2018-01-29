MEMORY {
    FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 512K
    RAM (rwx)  : ORIGIN = 0x1FFF0000, LENGTH = 192K
}

SECTIONS {
    . = 0x00000000;
    .text : {
        KEEP(*(.vectors))
        . = 0x400;
        KEEP(*(.flashconfig*))
        *(.handlers*)
        *(.startup*)
        *(.text*)
        *(.rodata*)
        . = ALIGN(8);
    } > FLASH

    .data : {
        . = ALIGN(8);
        _sdata = .;
        *(.fastrun*)
        *(.data*)
        . = ALIGN(8);
        _edata = .;
    } > RAM AT > FLASH

    _sflashdata = LOADADDR(.data);

    .bss : {
        . = ALIGN(8);
        _sbss = .;
        __bss_start__ = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(8);
        _ebss = .;
        __bss_end__ = .;
    } > RAM

    _estack = ORIGIN(RAM) + LENGTH(RAM); /* stack pointer start */
}
