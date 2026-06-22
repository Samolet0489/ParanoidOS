#ifndef PARANOID_AUDIT_H
#define PARANOID_AUDIT_H

void audit_init(void);
void audit_record(const char *action, const char *detail);
void audit_clear(void);
void audit_print(void);

#endif
