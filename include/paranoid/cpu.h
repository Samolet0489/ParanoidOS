#ifndef PARANOID_CPU_H
#define PARANOID_CPU_H

#include <paranoid/types.h>

u64 cpu_current_el(void);
u64 cpu_mpidr(void);

#endif
