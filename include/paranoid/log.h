#ifndef PARANOID_LOG_H
#define PARANOID_LOG_H

void log_info(const char *message);
void log_warn(const char *message);
void log_error(const char *message);
void log_panic(const char *message);

#endif
