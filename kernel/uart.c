#include <paranoid/uart.h>
#include <paranoid/types.h>

#define UART0_BASE 0x09000000UL

#define UART_DR ((volatile u32 *)(UART0_BASE + 0x00))
#define UART_FR ((volatile u32 *)(UART0_BASE + 0x18))

#define UART_FR_RXFE (1 << 4)
#define UART_FR_TXFF (1 << 5)

void uart_putc(char c)
{
    if (c == '\n')
    {
        uart_putc('\r');
    }

    while ((*UART_FR & UART_FR_TXFF) != 0)
    {
    }

    *UART_DR = (u32)c;
}

void uart_puts(const char *text)
{
    while (*text)
    {
        uart_putc(*text);
        text++;
    }
}

int uart_has_char(void)
{
    return ((*UART_FR & UART_FR_RXFE) == 0);
}

char uart_getc(void)
{
    while (!uart_has_char())
    {
        __asm__ volatile("wfe");
    }

    return (char)(*UART_DR & 0xFF);
}
