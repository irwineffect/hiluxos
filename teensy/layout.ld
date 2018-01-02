MEMORY {
    FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 512K
    RAM (rwx)  : ORIGIN = 0x1FFF0000, LENGTH = 64K
}

SECTIONS {
    . = 0x00000000;
    .text : {
        KEEP(*(.vectors))
        . = 0x400;
        KEEP(*(.flashconfig*))
        *(.startup)
        *(.text)
    } > FLASH

    _estack = ORIGIN(RAM) + LENGTH(RAM); /* stack pointer start */
}
