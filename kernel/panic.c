#include <paranoid/panic.h>
#include <paranoid/uart.h>
#include <paranoid/boot_phase.h>

void panic(const char *message)
{
    uart_puts("\n");
    uart_puts("================================\n");
    uart_puts("KERNEL PANIC\n");
    uart_puts("================================\n");

    uart_puts("Boot phase: ");
    uart_puts(boot_phase_name());
    uart_puts("\n");

    uart_puts("Message: ");
    uart_puts(message);
    uart_puts("\n");

    uart_puts("System halted.\n");

    while (1)
    {
        __asm__ volatile("wfe");
    }
}
