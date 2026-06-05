#ifndef PARANOID_EXCEPTION_H
#define PARANOID_EXCEPTION_H

#include <paranoid/types.h>

void exception_init(void);
void exception_handler(u64 type, u64 esr, u64 elr, u64 far);
void exception_trigger_test(void);

#endif
