#ifndef PARANOID_MMU_H
#define PARANOID_MMU_H

#include <paranoid/types.h>

int mmu_init(void);
int mmu_is_enabled(void);

void mmu_print_info(void);

#endif
