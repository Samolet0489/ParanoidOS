#ifndef PARANOID_BOOT_CHECK_H
#define PARANOID_BOOT_CHECK_H

typedef enum
{
    BOOT_CHECK_INFO = 0,
    BOOT_CHECK_WARN = 1,
    BOOT_CHECK_FATAL = 2
} boot_check_severity_t;

void boot_check_report(const char *name, const char *status, boot_check_severity_t severity);
void boot_check_run_all(void);

#endif
