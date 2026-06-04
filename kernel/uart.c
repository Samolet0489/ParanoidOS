#include <paranoid/uart.h>

#define UART0 ((volatile unsigned char *)0x09000000)

void uart_putc(char c)
{
    *UART0 = c;
}

void uart_puts(const char *text)
{
    while (*text)
    {
        uart_putc(*text);
        text++;
    }
}
