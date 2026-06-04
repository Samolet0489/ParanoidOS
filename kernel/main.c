#include <paranoid/log.h>
#include <paranoid/panic.h>
#include <paranoid/string.h>
#include <paranoid/format.h>

static void run_kernel_tests(void)
{
    char buffer[16];
    char dec_buffer[21];
    char hex_buffer[19];

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

    k_u64_to_dec(12345, dec_buffer);

    if (k_strcmp(dec_buffer, "12345") != 0)
    {
        panic("k_u64_to_dec failed");
    }

    k_u64_to_hex(0x40080000, hex_buffer);

    if (k_strcmp(hex_buffer, "0x0000000040080000") != 0)
    {
        panic("k_u64_to_hex failed");
    }
}

void kernel_main(void)
{
    log_info("ParanoidOS booted");
    log_info("Security mode: paranoid learning build");
    log_info("Milestone 5: number formatting loaded");

    run_kernel_tests();

    log_info("Kernel self-tests passed");

    log_info_dec("Example decimal value", 12345);
    log_info_hex("Kernel load address", 0x40080000);

    log_warn("No scheduler loaded");
    log_warn("No memory protection loaded");
    log_warn("No userspace loaded");

    while (1)
    {
        __asm__ volatile("wfe");
    }
}
