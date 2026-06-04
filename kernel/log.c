#include <paranoid/log.h>
#include <paranoid/uart.h>
#include <paranoid/panic.h>

static void log_write(const char *level, const char *message)
{
    uart_puts("[");
    uart_puts(level);
    uart_puts("] ");
    uart_puts(message);
    uart_puts("\n");
}

void log_info(const char *message)
{
    log_write("INFO", message);
}

void log_warn(const char *message)
{
    log_write("WARN", message);
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
