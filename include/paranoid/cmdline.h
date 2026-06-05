#ifndef PARANOID_CMDLINE_H
#define PARANOID_CMDLINE_H

void cmdline_init(const char *line);
const char *cmdline_get(void);
void cmdline_print_info(void);

int cmdline_has_key_value(const char *key, const char *value);
int cmdline_bool_enabled(const char *key, int default_value);

#endif
