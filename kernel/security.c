#include <paranoid/security.h>
#include <paranoid/cmdline.h>
#include <paranoid/log.h>

static security_mode_t current_security_mode = SECURITY_MODE_NORMAL;

void security_init(void)
{
    if (cmdline_has_key_value("security", "paranoid"))
    {
        current_security_mode = SECURITY_MODE_PARANOID;
        return;
    }

    if (cmdline_has_key_value("security", "strict"))
    {
        current_security_mode = SECURITY_MODE_STRICT;
        return;
    }

    current_security_mode = SECURITY_MODE_NORMAL;
}

security_mode_t security_current_mode(void)
{
    return current_security_mode;
}

const char *security_mode_name(void)
{
    if (current_security_mode == SECURITY_MODE_PARANOID)
    {
        return "paranoid";
    }

    if (current_security_mode == SECURITY_MODE_STRICT)
    {
        return "strict";
    }

    return "normal";
}

int security_strict_or_higher(void)
{
    return current_security_mode == SECURITY_MODE_STRICT ||
           current_security_mode == SECURITY_MODE_PARANOID;
}

int security_paranoid_enabled(void)
{
    return current_security_mode == SECURITY_MODE_PARANOID;
}

void security_print_info(void)
{
    log_info("Security profile");
    log_info_pair("Security mode", security_mode_name());

    if (current_security_mode == SECURITY_MODE_NORMAL)
    {
        log_info_pair("Profile policy", "normal checks");
        return;
    }

    if (current_security_mode == SECURITY_MODE_STRICT)
    {
        log_info_pair("Profile policy", "warnings are fatal");
        return;
    }

    log_info_pair("Profile policy", "maximum paranoia");
}
