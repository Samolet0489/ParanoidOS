#include <paranoid/timer.h>

static u64 timer_start_ticks;
static u64 timer_hz;

static u64 timer_read_counter(void)
{
    u64 value;

    __asm__ volatile("mrs %0, cntpct_el0" : "=r"(value));

    return value;
}

static u64 timer_read_frequency(void)
{
    u64 value;

    __asm__ volatile("mrs %0, cntfrq_el0" : "=r"(value));

    return value;
}

static u64 timer_elapsed_ticks(void)
{
    return timer_read_counter() - timer_start_ticks;
}

void timer_init(void)
{
    timer_hz = timer_read_frequency();
    timer_start_ticks = timer_read_counter();
}

u64 timer_frequency(void)
{
    return timer_hz;
}

u64 timer_ticks(void)
{
    return timer_read_counter();
}

u64 timer_uptime_seconds(void)
{
    if (timer_hz == 0)
    {
        return 0;
    }

    return timer_elapsed_ticks() / timer_hz;
}

u64 timer_uptime_milliseconds(void)
{
    u64 elapsed;
    u64 remainder;

    if (timer_hz == 0)
    {
        return 0;
    }

    elapsed = timer_elapsed_ticks();
    remainder = elapsed % timer_hz;

    return (remainder * 1000ULL) / timer_hz;
}
