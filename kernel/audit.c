#include <paranoid/audit.h>
#include <paranoid/timer.h>
#include <paranoid/log.h>
#include <paranoid/types.h>

#define AUDIT_CAPACITY 12

typedef struct
{
    u64 uptime_seconds;
    u64 uptime_milliseconds;
    const char *action;
    const char *detail;
} audit_event_t;

static audit_event_t audit_events[AUDIT_CAPACITY];
static u64 audit_total;
static int audit_ready;

void audit_init(void)
{
    u64 index;

    for (index = 0; index < AUDIT_CAPACITY; index++)
    {
        audit_events[index].uptime_seconds = 0;
        audit_events[index].uptime_milliseconds = 0;
        audit_events[index].action = 0;
        audit_events[index].detail = 0;
    }

    audit_total = 0;
    audit_ready = 1;
}

void audit_record(const char *action, const char *detail)
{
    u64 slot;

    if (!audit_ready)
    {
        return;
    }

    slot = audit_total % AUDIT_CAPACITY;

    audit_events[slot].uptime_seconds = timer_uptime_seconds();
    audit_events[slot].uptime_milliseconds = timer_uptime_milliseconds();
    audit_events[slot].action = action;
    audit_events[slot].detail = detail;

    audit_total++;
}

void audit_clear(void)
{
    u64 index;

    if (!audit_ready)
    {
        return;
    }

    for (index = 0; index < AUDIT_CAPACITY; index++)
    {
        audit_events[index].uptime_seconds = 0;
        audit_events[index].uptime_milliseconds = 0;
        audit_events[index].action = 0;
        audit_events[index].detail = 0;
    }

    audit_total = 0;
}

void audit_print(void)
{
    u64 retained;
    u64 first;
    u64 sequence;
    audit_event_t *event;

    log_info("Security audit log");

    if (!audit_ready || audit_total == 0)
    {
        log_info("No audit events recorded");
        return;
    }

    retained = audit_total;

    if (retained > AUDIT_CAPACITY)
    {
        retained = AUDIT_CAPACITY;
    }

    first = audit_total - retained;

    log_info_dec("Events retained", retained);

    for (sequence = first; sequence < audit_total; sequence++)
    {
        event = &audit_events[sequence % AUDIT_CAPACITY];

        log_info_dec("Audit event number", sequence + 1);
        log_info_pair("Action", event->action);
        log_info_pair("Detail", event->detail);
        log_info_dec("Uptime seconds", event->uptime_seconds);
        log_info_dec("Uptime milliseconds", event->uptime_milliseconds);
    }
}
