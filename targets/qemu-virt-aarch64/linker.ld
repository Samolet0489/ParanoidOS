ENTRY(_start)

PHDRS
{
    text PT_LOAD FLAGS(5);   /* Read + Execute */
    rodata PT_LOAD FLAGS(4); /* Read only */
    data PT_LOAD FLAGS(6);   /* Read + Write */
}

SECTIONS
{
    . = 0x40080000;

    __kernel_start = .;

    .text : ALIGN(4096)
    {
        KEEP(*(.text.boot))
        KEEP(*(.text.exceptions))
        *(.text*)
    } :text

    . = ALIGN(4096);

    .rodata :
    {
        *(.rodata*)
    } :rodata

    . = ALIGN(4096);

    .data :
    {
        *(.data*)
    } :data

    . = ALIGN(8);
    __bss_start = .;

    .bss (NOLOAD) :
    {
        *(.bss*)
        *(COMMON)
        . = ALIGN(8);
    } :data

    __bss_end = .;

    . = ALIGN(8);
    __kernel_end = .;
}
