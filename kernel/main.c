#include <paranoid/uart.h>
#include <paranoid/panic.h>

void kernel_main(void)
{
    uart_puts("ParanoidOS booted.\n");
    uart_puts("Security mode: paranoid learning build.\n");
    uart_puts("Milestone 2: UART driver loaded.\n");

    panic("Intentional test panic.");
}
