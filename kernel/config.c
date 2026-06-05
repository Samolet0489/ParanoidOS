#include <paranoid/config.h>
#include <paranoid/log.h>
#include <paranoid/cmdline.h>

static const char *enabled_text(int value)
{
    if (value)
    {
        return "enabled";
    }

    return "disabled";
}

int config_debug_enabled(void)
{
    return cmdline_bool_enabled("debug", PARANOID_CONFIG_DEBUG);
}

int config_self_tests_enabled(void)
{
    return cmdline_bool_enabled("self_tests", PARANOID_CONFIG_SELF_TESTS);
}

int config_network_enabled(void)
{
    return cmdline_bool_enabled("network", PARANOID_CONFIG_NETWORK);
}

int config_userspace_enabled(void)
{
    return cmdline_bool_enabled("userspace", PARANOID_CONFIG_USERSPACE);
}

int config_panic_on_warn_enabled(void)
{
    return cmdline_bool_enabled("panic_on_warn", PARANOID_CONFIG_PANIC_ON_WARN);
}

void config_print_info(void)
{
    log_info("Boot configuration");

    log_info_pair("Debug mode", enabled_text(config_debug_enabled()));
    log_info_pair("Self tests", enabled_text(config_self_tests_enabled()));
    log_info_pair("Networking", enabled_text(config_network_enabled()));
    log_info_pair("Userspace", enabled_text(config_userspace_enabled()));
    log_info_pair("Panic on warning", enabled_text(config_panic_on_warn_enabled()));
}
