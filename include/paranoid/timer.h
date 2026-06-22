#ifndef PARANOID_TIMER_H
#define PARANOID_TIMER_H

#include <paranoid/types.h>

void timer_init(void);

u64 timer_frequency(void);
u64 timer_ticks(void);

u64 timer_uptime_seconds(void);
u64 timer_uptime_milliseconds(void);

#endif
