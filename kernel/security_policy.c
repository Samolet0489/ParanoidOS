#include <paranoid/security_policy.h>
#include <paranoid/security.h>
#include <paranoid/config.h>
#include <paranoid/log.h>

static const char *enabled_text(int value)
{
    if (value)
    {
        return "enabled";
    }

    return "disabled";
}

static const char *active_text(int value)
{
    if (value)
    {
        return "active";
    }

    return "inactive";
}

void security_policy_print_report(void)
{
    log_info("Security policy report");

    log_info_pair("Selected profile", security_mode_name());

    if (config_panic_on_warn_enabled())
    {
        log_info_pair("Warning handling", "fatal");
    }
    else
    {
        log_info_pair("Warning handling", "non-fatal");
    }

    log_info_pair("Self-tests policy", enabled_text(config_self_tests_enabled()));

    if (config_network_enabled())
    {
        log_info_pair("Networking policy", "allowed");
    }
    else
    {
        log_info_pair("Networking policy", "blocked");
    }

    if (config_userspace_enabled())
    {
        log_info_pair("Userspace policy", "allowed");
    }
    else
    {
        log_info_pair("Userspace policy", "blocked");
    }

    log_info_pair("Debug policy", enabled_text(config_debug_enabled()));
    log_info_pair("Strict policy", active_text(security_strict_or_higher()));
    log_info_pair("Paranoid policy", active_text(security_paranoid_enabled()));
}
