#include <paranoid/cpu.h>

u64 cpu_current_el(void)
{
    u64 value;

    __asm__ volatile("mrs %0, CurrentEL" : "=r"(value));

    return (value >> 2) & 0x3;
}

u64 cpu_mpidr(void)
{
    u64 value;

    __asm__ volatile("mrs %0, MPIDR_EL1" : "=r"(value));

    return value;
}

void cpu_set_vbar(u64 address)
{
    __asm__ volatile("msr VBAR_EL1, %0" : : "r"(address) : "memory");
    __asm__ volatile("isb");
}

u64 cpu_get_vbar(void)
{
    u64 value;

    __asm__ volatile("mrs %0, VBAR_EL1" : "=r"(value));

    return value;
}

u64 cpu_stack_pointer(void)
{
    u64 value;

    __asm__ volatile("mov %0, sp" : "=r"(value));

    return value;
}
