#include <paranoid/panic.h>
#include <paranoid/uart.h>

void panic(const char *message)
{
    uart_puts("\n");
    uart_puts("================================\n");
    uart_puts("KERNEL PANIC\n");
    uart_puts("================================\n");
    uart_puts(message);
    uart_puts("\nSystem halted.\n");

    while (1)
    {
        __asm__ volatile("wfe");
    }
}
