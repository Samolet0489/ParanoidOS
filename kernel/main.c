#include <paranoid/uart.h>

void kernel_main(void)
{
    uart_puts("ParanoidOS booted.\n");
    uart_puts("Security mode: paranoid learning build.\n");
    uart_puts("Milestone 2 complete: UART driver loaded.\n");

    while (1)
    {
        __asm__ volatile("wfe");
    }
}
