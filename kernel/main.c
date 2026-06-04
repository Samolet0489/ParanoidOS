#include <paranoid/log.h>
#include <paranoid/panic.h>
#include <paranoid/string.h>
#include <paranoid/format.h>
#include <paranoid/cpu.h>

extern char __kernel_start;
extern char __kernel_end;
extern char __bss_start;
extern char __bss_end;

static u64 bss_test_value;
static char bss_test_buffer[32];

static void run_kernel_tests(void)
{
    char buffer[16];
    char dec_buffer[21];
    char hex_buffer[19];

    if (bss_test_value != 0)
    {
        panic("BSS u64 value was not cleared");
    }

    for (u64 i = 0; i < sizeof(bss_test_buffer); i++)
    {
        if (bss_test_buffer[i] != 0)
        {
            panic("BSS buffer was not cleared");
        }
    }

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

static void print_boot_info(void)
{
    u64 current_el = cpu_current_el();
    u64 mpidr = cpu_mpidr();

    log_info_dec("Current exception level", current_el);
    log_info_hex("CPU MPIDR register", mpidr);

    log_info_hex("Kernel start address", (u64)&__kernel_start);
    log_info_hex("Kernel end address", (u64)&__kernel_end);
    log_info_hex("BSS start address", (u64)&__bss_start);
    log_info_hex("BSS end address", (u64)&__bss_end);

}

void kernel_main(void)
{
    log_info("ParanoidOS booted");
    log_info("Security mode: paranoid learning build");
    log_info("Milestone 7: BSS clearing during boot loaded");

    run_kernel_tests();

    log_info("Kernel self-tests passed");
    log_info("BSS section cleared successfully");

    print_boot_info();

    log_warn("No scheduler loaded");
    log_warn("No memory protection loaded");
    log_warn("No userspace loaded");

    while (1)
    {
        __asm__ volatile("wfe");
    }
}
