#ifndef PARANOID_LOG_H
#define PARANOID_LOG_H

#include <paranoid/types.h>

void log_info(const char *message);
void log_warn(const char *message);
void log_error(const char *message);
void log_panic(const char *message);

void log_info_pair(const char *name, const char *value);
void log_info_dec(const char *message, u64 value);
void log_info_hex(const char *message, u64 value);

#endif
