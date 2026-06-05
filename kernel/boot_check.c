#include <paranoid/boot_check.h>
#include <paranoid/log.h>
#include <paranoid/panic.h>
#include <paranoid/config.h>

void boot_check_report(const char *name, const char *status, boot_check_severity_t severity)
{
    if (severity == BOOT_CHECK_INFO)
    {
        log_info_pair(name, status);
        return;
    }

    if (severity == BOOT_CHECK_WARN)
    {
        log_warn_pair(name, status);
        return;
    }

    log_error_pair(name, status);
    panic("Fatal boot check failed");
}

void boot_check_run_all(void)
{
    log_info("Structured boot checks");

    boot_check_report("Scheduler", "unavailable", BOOT_CHECK_WARN);
    boot_check_report("Memory protection", "unavailable", BOOT_CHECK_WARN);

    if (config_network_enabled())
    {
        boot_check_report("Networking", "enabled", BOOT_CHECK_INFO);
    }
    else
    {
        boot_check_report("Networking", "disabled by policy", BOOT_CHECK_INFO);
    }

    if (config_userspace_enabled())
    {
        boot_check_report("Userspace", "enabled", BOOT_CHECK_INFO);
    }
    else
    {
        boot_check_report("Userspace", "disabled by policy", BOOT_CHECK_INFO);
    }

    if (config_panic_on_warn_enabled())
    {
        boot_check_report("Warning policy", "panic on warning", BOOT_CHECK_INFO);
    }
    else
    {
        boot_check_report("Warning policy", "warnings are non-fatal", BOOT_CHECK_INFO);
    }
}
