#include <paranoid/uart.h>
#include <paranoid/panic.h>
#include <paranoid/string.h>

static void run_kernel_tests(void)
{
    char buffer[16];

    if (k_strlen("ParanoidOS") != 10)
    {
        panic("k_strlen failed");
    }

    k_memset(buffer, 0, sizeof(buffer));
    k_memcpy(buffer, "safe", 5);

    if (k_strcmp(buffer, "safe") != 0)
    {
        panic("k_memcpy or k_strcmp failed");
    }

    if (k_strcmp("abc", "abc") != 0)
    {
        panic("k_strcmp equality failed");
    }

    if (k_strcmp("abc", "abd") >= 0)
    {
        panic("k_strcmp ordering failed");
    }
}

void kernel_main(void)
{
    uart_puts("ParanoidOS booted.\n");
    uart_puts("Security mode: paranoid learning build.\n");
    uart_puts("Milestone 3: kernel string library loaded.\n");

    run_kernel_tests();

    uart_puts("Kernel self-tests passed.\n");

    while (1)
    {
        __asm__ volatile("wfe");
    }
}
