#include <paranoid/panic.h>
#include <paranoid/uart.h>
#include <paranoid/boot_phase.h>
#include <paranoid/cpu.h>
#include <paranoid/kernel_layout.h>
#include <paranoid/format.h>
#include <paranoid/types.h>
#include <paranoid/capability.h>
#include <paranoid/audit.h>

static void panic_puts(const char *text)
{
    uart_puts(text);
}

static void panic_print_hex(const char *label, u64 value)
{
    char buffer[19];

    k_u64_to_hex(value, buffer);

    panic_puts(label);
    panic_puts(": ");
    panic_puts(buffer);
    panic_puts("\n");
}

static void panic_print_dec(const char *label, u64 value)
{
    char buffer[21];

    k_u64_to_dec(value, buffer);

    panic_puts(label);
    panic_puts(": ");
    panic_puts(buffer);
    panic_puts("\n");
}

static void panic_print_context(void)
{
    panic_puts("\nPanic context:\n");

    panic_puts("Boot phase: ");
    panic_puts(boot_phase_name());
    panic_puts("\n");

    panic_print_dec("Current exception level", cpu_current_el());
    panic_print_hex("Current stack pointer", cpu_stack_pointer());

    panic_print_hex("Kernel start", kernel_layout_start());
    panic_print_hex("Kernel end", kernel_layout_end());
    panic_print_hex("BSS start", kernel_layout_bss_start());
    panic_print_hex("BSS end", kernel_layout_bss_end());
}

void panic(const char *message)
{
    u64 revoked;

    audit_record("Panic Mode", "entered");
    revoked = capability_seal();
    panic_puts("\n");
    panic_puts("================================\n");
    panic_puts("KERNEL PANIC\n");
    panic_puts("================================\n");

    panic_puts("Panic Mode: temporary capabilities revoked\n");
    panic_print_dec("Revoked capability leases", revoked);

    panic_print_context();

    panic_puts("\nMessage: ");
    panic_puts(message);
    panic_puts("\n");

    panic_puts("System halted.\n");

    while (1)
    {
        __asm__ volatile("wfe");
    }
}
