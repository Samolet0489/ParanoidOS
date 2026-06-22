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

    . = ALIGN(4096);
    __text_start = .;

    .text :
    {
        KEEP(*(.text.boot))
        KEEP(*(.text.exceptions))
        *(.text*)
    } :text

    __text_end = .;

    . = ALIGN(4096);
    __rodata_start = .;

    .rodata :
    {
        *(.rodata*)
    } :rodata

    __rodata_end = .;

    . = ALIGN(4096);
    __data_start = .;

    .data :
    {
        *(.data*)
        *(.got)
        *(.got.*)
    } :data

    __data_end = .;

    . = ALIGN(4096);
    __bss_start = .;

    .bss (NOLOAD) :
    {
        *(.bss*)
        *(COMMON)
        . = ALIGN(4096);
    } :data

    __bss_end = .;

    . = ALIGN(4096);
    __stack_guard_start = .;

    .stack_guard (NOLOAD) :
    {
        . += 4096;
    } :data

    __stack_guard_end = .;
    stack_bottom = .;

    .stack (NOLOAD) :
    {
        . += 4096;
    } :data

    stack_top = .;

    . = ALIGN(4096);
    __page_tables_start = .;

    .page_tables (NOLOAD) :
    {
        . = ALIGN(4096);
        KEEP(*(.page_tables))
        . = ALIGN(4096);
    } :data

    __page_tables_end = .;
    __runtime_zero_end = .;
    __kernel_end = .;
}
