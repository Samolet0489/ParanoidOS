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

    __bss_start = .;

    .bss :
    {
        *(.bss*)
        *(COMMON)
    }

    __bss_end = .;

    __kernel_end = .;
}
