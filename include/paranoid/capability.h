#ifndef PARANOID_CAPABILITY_H
#define PARANOID_CAPABILITY_H

#include <paranoid/types.h>

typedef enum
{
    CAPABILITY_LAPTOP_LINK = 0,
    CAPABILITY_RADIO_RECEIVE = 1,
    CAPABILITY_USB_DATA = 2,
    CAPABILITY_WIFI = 3,
    CAPABILITY_BLUETOOTH = 4,
    CAPABILITY_MODEM = 5,
    CAPABILITY_COUNT = 6
} capability_t;

void capability_init(void);

int capability_grant(capability_t capability, u64 duration_seconds);
int capability_is_active(capability_t capability);

u64 capability_remaining_seconds(capability_t capability);
u64 capability_active_count(void);
u64 capability_seal(void);

const char *capability_name(capability_t capability);
const char *capability_state_name(void);

void capability_print_status(void);

#endif
