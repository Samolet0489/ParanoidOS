#include <paranoid/exception.h>
#include <paranoid/cpu.h>
#include <paranoid/log.h>
#include <paranoid/panic.h>

extern char exception_vector_table;

#define ESR_EC_SHIFT 26
#define ESR_EC_MASK  0x3F
#define ESR_IL_SHIFT 25
#define ESR_IL_MASK  0x1
#define ESR_ISS_MASK 0x1FFFFFF

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

static const char *esr_exception_class_name(u64 ec)
{
    switch (ec)
    {
        case 0x00:
            return "Unknown reason";
        case 0x01:
            return "Trapped WFI or WFE instruction";
        case 0x03:
            return "Trapped MCR or MRC access";
        case 0x04:
            return "Trapped MCRR or MRRC access";
        case 0x05:
            return "Trapped MCR or MRC access";
        case 0x06:
            return "Trapped LDC or STC access";
        case 0x07:
            return "Access to SIMD or floating-point";
        case 0x0E:
            return "Illegal execution state";
        case 0x11:
            return "SVC instruction in AArch32";
        case 0x15:
            return "SVC instruction in AArch64";
        case 0x18:
            return "Trapped MSR, MRS or system instruction";
        case 0x20:
            return "Instruction abort from lower EL";
        case 0x21:
            return "Instruction abort from same EL";
        case 0x22:
            return "PC alignment fault";
        case 0x24:
            return "Data abort from lower EL";
        case 0x25:
            return "Data abort from same EL";
        case 0x26:
            return "SP alignment fault";
        case 0x2C:
            return "Floating-point exception in AArch64";
        case 0x2F:
            return "SError interrupt";
        case 0x30:
            return "Breakpoint from lower EL";
        case 0x31:
            return "Breakpoint from same EL";
        case 0x32:
            return "Software step from lower EL";
        case 0x33:
            return "Software step from same EL";
        case 0x34:
            return "Watchpoint from lower EL";
        case 0x35:
            return "Watchpoint from same EL";
        case 0x38:
            return "BKPT instruction";
        case 0x3C:
            return "BRK instruction";
        default:
            return "Unknown ESR exception class";
    }
}

static void decode_esr(u64 esr)
{
    u64 ec = (esr >> ESR_EC_SHIFT) & ESR_EC_MASK;
    u64 il = (esr >> ESR_IL_SHIFT) & ESR_IL_MASK;
    u64 iss = esr & ESR_ISS_MASK;

    log_info_dec("ESR exception class", ec);
    log_info_hex("ESR exception class hex", ec);
    log_info("ESR class name follows");
    log_info(esr_exception_class_name(ec));

    log_info_dec("ESR instruction length", il);
    log_info_hex("ESR ISS field", iss);
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

    decode_esr(esr);

    panic("Unhandled CPU exception");
}
