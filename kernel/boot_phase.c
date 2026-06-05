#include <paranoid/boot_phase.h>
#include <paranoid/log.h>

static const char *current_boot_phase = "early boot";

void boot_phase_set(const char *phase)
{
    current_boot_phase = phase;
}

const char *boot_phase_name(void)
{
    return current_boot_phase;
}

void boot_phase_print(void)
{
    log_info_pair("Boot phase", current_boot_phase);
}
