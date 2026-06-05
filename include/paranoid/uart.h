#ifndef PARANOID_UART_H
#define PARANOID_UART_H

void uart_putc(char c);
void uart_puts(const char *text);

int uart_has_char(void);
char uart_getc(void);

#endif
