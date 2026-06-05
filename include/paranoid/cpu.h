#ifndef PARANOID_CPU_H
#define PARANOID_CPU_H

#include <paranoid/types.h>

u64 cpu_current_el(void);
u64 cpu_mpidr(void);

void cpu_set_vbar(u64 address);
u64 cpu_get_vbar(void);

u64 cpu_stack_pointer(void);

#endif
