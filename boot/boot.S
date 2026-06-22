.section .text.boot
.global _start

_start:
    ldr x0, =stack_top
    mov sp, x0

    bl clear_runtime_memory

    bl kernel_main

hang:
    wfe
    b hang

clear_runtime_memory:
    ldr x0, =__bss_start
    ldr x1, =__runtime_zero_end

clear_runtime_memory_loop:
    cmp x0, x1
    b.ge clear_runtime_memory_done

    str xzr, [x0], #8
    b clear_runtime_memory_loop

clear_runtime_memory_done:
    ret
