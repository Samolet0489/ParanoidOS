#include <paranoid/capability.h>
#include <paranoid/timer.h>
#include <paranoid/log.h>
#include <paranoid/audit.h>
#include <paranoid/types.h>

static const char *capability_names[CAPABILITY_COUNT] =
{
    "Laptop link",
    "Radio receive",
    "USB data",
    "Wi-Fi",
    "Bluetooth",
    "Modem"
};

static u8 capability_active[CAPABILITY_COUNT];
static u64 capability_expiry_seconds[CAPABILITY_COUNT];
static int capability_initialized;

static int capability_valid(capability_t capability)
{
    return (int)capability >= 0 && capability < CAPABILITY_COUNT;
}

static void capability_remove_expired(void)
{
    u64 now;
    u64 index;

    if (!capability_initialized)
    {
        return;
    }

    now = timer_uptime_seconds();

    for (index = 0; index < CAPABILITY_COUNT; index++)
    {
        if (capability_active[index] &&
            now >= capability_expiry_seconds[index])
        {
            capability_active[index] = 0;
            capability_expiry_seconds[index] = 0;
            audit_record(
                "Capability expired",
                capability_name((capability_t)index));
        }
    }
}

void capability_init(void)
{
    u64 index;

    audit_init();

    for (index = 0; index < CAPABILITY_COUNT; index++)
    {
        capability_active[index] = 0;
        capability_expiry_seconds[index] = 0;
    }

    capability_initialized = 1;
    audit_record("Boot state", "Sealed Mode");
}

int capability_grant(capability_t capability, u64 duration_seconds)
{
    u64 now;

    if (!capability_initialized ||
        !capability_valid(capability) ||
        duration_seconds == 0)
    {
        return 0;
    }

    capability_remove_expired();

    now = timer_uptime_seconds();

    capability_active[capability] = 1;
    capability_expiry_seconds[capability] = now + duration_seconds;

    audit_record("Capability granted", capability_name(capability));

    return 1;
}

int capability_is_active(capability_t capability)
{
    if (!capability_valid(capability))
    {
        return 0;
    }

    capability_remove_expired();

    return capability_active[capability] != 0;
}

u64 capability_remaining_seconds(capability_t capability)
{
    u64 now;

    if (!capability_is_active(capability))
    {
        return 0;
    }

    now = timer_uptime_seconds();

    return capability_expiry_seconds[capability] - now;
}

u64 capability_active_count(void)
{
    u64 index;
    u64 count = 0;

    capability_remove_expired();

    for (index = 0; index < CAPABILITY_COUNT; index++)
    {
        if (capability_active[index])
        {
            count++;
        }
    }

    return count;
}

u64 capability_seal(void)
{
    u64 index;
    u64 revoked = 0;

    for (index = 0; index < CAPABILITY_COUNT; index++)
    {
        if (capability_active[index])
        {
            revoked++;
            audit_record(
                "Capability revoked",
                capability_name((capability_t)index));
        }

        capability_active[index] = 0;
        capability_expiry_seconds[index] = 0;
    }

    audit_record("Sealed Mode restored", "all capabilities denied");

    return revoked;
}

const char *capability_name(capability_t capability)
{
    if (!capability_valid(capability))
    {
        return "Unknown";
    }

    return capability_names[capability];
}

const char *capability_state_name(void)
{
    if (capability_active_count() == 0)
    {
        return "sealed";
    }

    return "temporary leases active";
}

void capability_print_status(void)
{
    u64 index;

    log_info("Capability leases");
    log_info_pair("Capability state", capability_state_name());

    for (index = 0; index < CAPABILITY_COUNT; index++)
    {
        capability_t capability = (capability_t)index;

        if (capability_is_active(capability))
        {
            log_info_pair(capability_name(capability), "active");
            log_info_dec(
                "Lease seconds remaining",
                capability_remaining_seconds(capability));
        }
        else
        {
            log_info_pair(capability_name(capability), "denied");
        }
    }
}
