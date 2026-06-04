.section .text.boot
.global _start

_start:
    ldr x0, =stack_top
    mov sp, x0

    bl clear_bss

    bl kernel_main

hang:
    wfe
    b hang

clear_bss:
    ldr x0, =__bss_start
    ldr x1, =__bss_end

clear_bss_loop:
    cmp x0, x1
    b.ge clear_bss_done

    str xzr, [x0], #8
    b clear_bss_loop

clear_bss_done:
    ret

.section .bss.stack
.align 12
stack_bottom:
    .skip 4096
stack_top:
