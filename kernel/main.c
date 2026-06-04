#include <paranoid/log.h>
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
    log_info("ParanoidOS booted");
    log_info("Security mode: paranoid learning build");
    log_info("Milestone 4: kernel logger loaded");

    run_kernel_tests();

    log_info("Kernel self-tests passed");
    log_warn("No scheduler loaded");
    log_warn("No memory protection loaded");
    log_warn("No userspace loaded");

    while (1)
    {
        __asm__ volatile("wfe");
    }
}
