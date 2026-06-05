#ifndef PARANOID_SECURITY_H
#define PARANOID_SECURITY_H

typedef enum
{
    SECURITY_MODE_NORMAL = 0,
    SECURITY_MODE_STRICT = 1,
    SECURITY_MODE_PARANOID = 2
} security_mode_t;

void security_init(void);
void security_print_info(void);

security_mode_t security_current_mode(void);
const char *security_mode_name(void);

int security_strict_or_higher(void);
int security_paranoid_enabled(void);

#endif
