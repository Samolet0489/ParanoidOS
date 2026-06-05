#include <paranoid/exception.h>
#include <paranoid/cpu.h>
#include <paranoid/log.h>
#include <paranoid/panic.h>

extern char exception_vector_table;

static const char *exception_type_name(u64 type)
{
    switch (type)
    {
        case 0:
            return "Current EL with SP0: synchronous";
        case 1:
            return "Current EL with SP0: IRQ";
        case 2:
            return "Current EL with SP0: FIQ";
        case 3:
            return "Current EL with SP0: SError";
        case 4:
            return "Current EL with SPx: synchronous";
        case 5:
            return "Current EL with SPx: IRQ";
        case 6:
            return "Current EL with SPx: FIQ";
        case 7:
            return "Current EL with SPx: SError";
        case 8:
            return "Lower EL AArch64: synchronous";
        case 9:
            return "Lower EL AArch64: IRQ";
        case 10:
            return "Lower EL AArch64: FIQ";
        case 11:
            return "Lower EL AArch64: SError";
        case 12:
            return "Lower EL AArch32: synchronous";
        case 13:
            return "Lower EL AArch32: IRQ";
        case 14:
            return "Lower EL AArch32: FIQ";
        case 15:
            return "Lower EL AArch32: SError";
        default:
            return "Unknown exception type";
    }
}

void exception_init(void)
{
    cpu_set_vbar((u64)&exception_vector_table);

    log_info_hex("Exception vector table address", (u64)&exception_vector_table);
    log_info_hex("VBAR_EL1", cpu_get_vbar());
}

void exception_handler(u64 type, u64 esr, u64 elr, u64 far)
{
    log_error("CPU exception caught");
    log_error(exception_type_name(type));

    log_info_dec("Exception type", type);
    log_info_hex("ESR_EL1", esr);
    log_info_hex("ELR_EL1", elr);
    log_info_hex("FAR_EL1", far);

    panic("Unhandled CPU exception");
}
