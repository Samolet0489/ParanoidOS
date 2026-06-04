#define UART0 ((volatile unsigned char *)0x09000000)

static void uart_putc(char c)
{
    *UART0 = c;
}

static void uart_puts(const char *text)
{
    while (*text)
    {
        uart_putc(*text);
        text++;
    }
}

void kernel_main(void)
{
    uart_puts("ParanoidOS booted.\n");
    uart_puts("Security mode: paranoid learning build.\n");
    uart_puts("No network. No userspace. No mercy.\n");

    while (1)
    {
        __asm__ volatile("wfe");
    }
}
