.section .text.exceptions
.balign 2048

.global exception_vector_table
.global exception_trigger_test
.extern exception_handler

exception_vector_table:
    b vector_sync_current_el_sp0
    .balign 128
    b vector_irq_current_el_sp0
    .balign 128
    b vector_fiq_current_el_sp0
    .balign 128
    b vector_serror_current_el_sp0
    .balign 128

    b vector_sync_current_el_spx
    .balign 128
    b vector_irq_current_el_spx
    .balign 128
    b vector_fiq_current_el_spx
    .balign 128
    b vector_serror_current_el_spx
    .balign 128

    b vector_sync_lower_el_aarch64
    .balign 128
    b vector_irq_lower_el_aarch64
    .balign 128
    b vector_fiq_lower_el_aarch64
    .balign 128
    b vector_serror_lower_el_aarch64
    .balign 128

    b vector_sync_lower_el_aarch32
    .balign 128
    b vector_irq_lower_el_aarch32
    .balign 128
    b vector_fiq_lower_el_aarch32
    .balign 128
    b vector_serror_lower_el_aarch32
    .balign 128

.macro HANDLE_EXCEPTION type
    mov x0, #\type
    mrs x1, ESR_EL1
    mrs x2, ELR_EL1
    mrs x3, FAR_EL1
    bl exception_handler

1:
    wfe
    b 1b
.endm

vector_sync_current_el_sp0:
    HANDLE_EXCEPTION 0

vector_irq_current_el_sp0:
    HANDLE_EXCEPTION 1

vector_fiq_current_el_sp0:
    HANDLE_EXCEPTION 2

vector_serror_current_el_sp0:
    HANDLE_EXCEPTION 3

vector_sync_current_el_spx:
    HANDLE_EXCEPTION 4

vector_irq_current_el_spx:
    HANDLE_EXCEPTION 5

vector_fiq_current_el_spx:
    HANDLE_EXCEPTION 6

vector_serror_current_el_spx:
    HANDLE_EXCEPTION 7

vector_sync_lower_el_aarch64:
    HANDLE_EXCEPTION 8

vector_irq_lower_el_aarch64:
    HANDLE_EXCEPTION 9

vector_fiq_lower_el_aarch64:
    HANDLE_EXCEPTION 10

vector_serror_lower_el_aarch64:
    HANDLE_EXCEPTION 11

vector_sync_lower_el_aarch32:
    HANDLE_EXCEPTION 12

vector_irq_lower_el_aarch32:
    HANDLE_EXCEPTION 13

vector_fiq_lower_el_aarch32:
    HANDLE_EXCEPTION 14

vector_serror_lower_el_aarch32:
    HANDLE_EXCEPTION 15

exception_trigger_test:
    brk #0
    ret
