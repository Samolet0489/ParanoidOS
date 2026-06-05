#include <paranoid/stack.h>
#include <paranoid/types.h>
#include <paranoid/log.h>
#include <paranoid/panic.h>

extern char stack_bottom;
extern char stack_top;

#define STACK_GUARD_SIZE 64
#define STACK_GUARD_PATTERN 0xA5

void stack_init_guard(void)
{
    u8 *guard = (u8 *)&stack_bottom;

    for (u64 i = 0; i < STACK_GUARD_SIZE; i++)
    {
        guard[i] = STACK_GUARD_PATTERN;
    }
}

void stack_check_guard(void)
{
    u8 *guard = (u8 *)&stack_bottom;

    for (u64 i = 0; i < STACK_GUARD_SIZE; i++)
    {
        if (guard[i] != STACK_GUARD_PATTERN)
        {
            panic("Stack guard corrupted");
        }
    }
}

void stack_print_info(void)
{
    u64 bottom = (u64)&stack_bottom;
    u64 top = (u64)&stack_top;
    u64 size = top - bottom;

    log_info_hex("Stack bottom", bottom);
    log_info_hex("Stack top", top);
    log_info_dec("Stack size bytes", size);
    log_info_dec("Stack guard size bytes", STACK_GUARD_SIZE);
}
