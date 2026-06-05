#include <paranoid/stack.h>
#include <paranoid/types.h>
#include <paranoid/log.h>
#include <paranoid/panic.h>
#include <paranoid/cpu.h>

extern char stack_bottom;
extern char stack_top;

#define STACK_GUARD_SIZE 64
#define STACK_GUARD_PATTERN 0xA5

static void stack_validate_pointer(u64 sp)
{
    u64 bottom = (u64)&stack_bottom;
    u64 top = (u64)&stack_top;

    if (sp < bottom)
    {
        panic("Stack pointer below stack bottom");
    }

    if (sp > top)
    {
        panic("Stack pointer above stack top");
    }
}

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

u64 stack_current_used(void)
{
    u64 sp = cpu_stack_pointer();
    u64 top = (u64)&stack_top;

    stack_validate_pointer(sp);

    return top - sp;
}

u64 stack_current_free(void)
{
    u64 sp = cpu_stack_pointer();
    u64 bottom = (u64)&stack_bottom;

    stack_validate_pointer(sp);

    return sp - bottom;
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

void stack_print_usage(void)
{
    u64 sp = cpu_stack_pointer();

    stack_validate_pointer(sp);

    log_info_hex("Current stack pointer", sp);
    log_info_dec("Stack used bytes", stack_current_used());
    log_info_dec("Stack free bytes", stack_current_free());
}
