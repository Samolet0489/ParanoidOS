#include <paranoid/log.h>
#include <paranoid/uart.h>
#include <paranoid/panic.h>
#include <paranoid/format.h>
#include <paranoid/config.h>

static void log_write(const char *level, const char *message)
{
    uart_puts("[");
    uart_puts(level);
    uart_puts("] ");
    uart_puts(message);
    uart_puts("\n");
}

static void log_write_value(const char *level, const char *message, const char *value)
{
    uart_puts("[");
    uart_puts(level);
    uart_puts("] ");
    uart_puts(message);
    uart_puts(": ");
    uart_puts(value);
    uart_puts("\n");
}

static void log_handle_warning_policy(void)
{
    if (config_panic_on_warn_enabled())
    {
        panic("Warning treated as fatal");
    }
}

void log_info(const char *message)
{
    log_write("INFO", message);
}

void log_warn(const char *message)
{
    log_write("WARN", message);
    log_handle_warning_policy();
}

void log_error(const char *message)
{
    log_write("ERROR", message);
}

void log_panic(const char *message)
{
    log_write("PANIC", message);
    panic(message);
}

void log_info_pair(const char *name, const char *value)
{
    log_write_value("INFO", name, value);
}

void log_warn_pair(const char *name, const char *value)
{
    log_write_value("WARN", name, value);
    log_handle_warning_policy();
}

void log_error_pair(const char *name, const char *value)
{
    log_write_value("ERROR", name, value);
}

void log_info_dec(const char *message, u64 value)
{
    char buffer[21];

    k_u64_to_dec(value, buffer);
    log_write_value("INFO", message, buffer);
}

void log_info_hex(const char *message, u64 value)
{
    char buffer[19];

    k_u64_to_hex(value, buffer);
    log_write_value("INFO", message, buffer);
}
