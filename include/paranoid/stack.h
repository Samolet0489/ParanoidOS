#ifndef PARANOID_STACK_H
#define PARANOID_STACK_H

#include <paranoid/types.h>

void stack_init_guard(void);
void stack_check_guard(void);
void stack_print_info(void);
void stack_print_usage(void);

u64 stack_current_used(void);
u64 stack_current_free(void);

#endif
