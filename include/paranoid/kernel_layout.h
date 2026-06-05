#ifndef PARANOID_KERNEL_LAYOUT_H
#define PARANOID_KERNEL_LAYOUT_H

#include <paranoid/types.h>

u64 kernel_layout_start(void);
u64 kernel_layout_end(void);
u64 kernel_layout_size(void);

u64 kernel_layout_bss_start(void);
u64 kernel_layout_bss_end(void);
u64 kernel_layout_bss_size(void);

void kernel_layout_print_info(void);

#endif
