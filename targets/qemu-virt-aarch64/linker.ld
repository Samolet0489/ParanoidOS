ENTRY(_start)

SECTIONS
{
    . = 0x40080000;

    __kernel_start = .;

    .text :
    {
        KEEP(*(.text.boot))
        *(.text*)
    }

    .rodata :
    {
        *(.rodata*)
    }

    .data :
    {
        *(.data*)
    }

    . = ALIGN(8);
    __bss_start = .;

    .bss (NOLOAD) :
    {
        *(.bss*)
        *(COMMON)
        . = ALIGN(8);
    }

    __bss_end = .;

    . = ALIGN(8);
    __kernel_end = .;
}
