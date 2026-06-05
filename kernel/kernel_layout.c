#include <paranoid/kernel_layout.h>
#include <paranoid/log.h>

extern char __kernel_start;
extern char __kernel_end;
extern char __bss_start;
extern char __bss_end;

u64 kernel_layout_start(void)
{
    return (u64)&__kernel_start;
}

u64 kernel_layout_end(void)
{
    return (u64)&__kernel_end;
}

u64 kernel_layout_size(void)
{
    return kernel_layout_end() - kernel_layout_start();
}

u64 kernel_layout_bss_start(void)
{
    return (u64)&__bss_start;
}

u64 kernel_layout_bss_end(void)
{
    return (u64)&__bss_end;
}

u64 kernel_layout_bss_size(void)
{
    return kernel_layout_bss_end() - kernel_layout_bss_start();
}

void kernel_layout_print_info(void)
{
    log_info("Kernel layout");

    log_info_hex("Kernel start address", kernel_layout_start());
    log_info_hex("Kernel end address", kernel_layout_end());
    log_info_dec("Kernel size bytes", kernel_layout_size());

    log_info_hex("BSS start address", kernel_layout_bss_start());
    log_info_hex("BSS end address", kernel_layout_bss_end());
    log_info_dec("BSS size bytes", kernel_layout_bss_size());
}
