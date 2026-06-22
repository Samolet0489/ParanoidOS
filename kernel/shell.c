#include <paranoid/shell.h>
#include <paranoid/uart.h>
#include <paranoid/string.h>
#include <paranoid/log.h>
#include <paranoid/panic.h>
#include <paranoid/stack.h>
#include <paranoid/cpu_info.h>
#include <paranoid/kernel_layout.h>
#include <paranoid/build.h>
#include <paranoid/config.h>
#include <paranoid/security.h>
#include <paranoid/security_policy.h>
#include <paranoid/boot_phase.h>
#include <paranoid/boot_check.h>
#include <paranoid/timer.h>
#include <paranoid/mmu.h>
#include <paranoid/format.h>
#include <paranoid/types.h>

#define SHELL_BUFFER_SIZE 80

static void shell_print_prompt(void)
{
    uart_puts("\nparanoid> ");
}

static void shell_print_u64_line(const char *label, u64 value, const char *suffix)
{
    char number[21];

    k_u64_to_dec(value, number);

    uart_puts(label);
    uart_puts(number);
    uart_puts(suffix);
    uart_puts("\n");
}

static void shell_print_uptime(void)
{
    char seconds_text[21];
    u64 seconds = timer_uptime_seconds();
    u64 milliseconds = timer_uptime_milliseconds();

    k_u64_to_dec(seconds, seconds_text);

    uart_puts("Uptime: ");
    uart_puts(seconds_text);
    uart_puts(".");
    uart_putc('0' + (milliseconds / 100));
    uart_putc('0' + ((milliseconds / 10) % 10));
    uart_putc('0' + (milliseconds % 10));
    uart_puts(" seconds\n");
}

static void shell_read_line(char *buffer, u64 size)
{
    u64 index = 0;

    while (1)
    {
        char c = uart_getc();

        if (c == '\r' || c == '\n')
        {
            uart_puts("\n");
            buffer[index] = '\0';
            return;
        }

        if (c == 8 || c == 127)
        {
            if (index > 0)
            {
                index--;
                uart_puts("\b \b");
            }

            continue;
        }

        if (index + 1 < size)
        {
            buffer[index] = c;
            index++;
            uart_putc(c);
        }
    }
}

static void shell_help(void)
{
    uart_puts("Available commands:\n");
    uart_puts("  help      - show commands\n");
    uart_puts("  cpu       - print CPU information\n");
    uart_puts("  stack     - print stack information\n");
    uart_puts("  layout    - print kernel memory layout\n");
    uart_puts("  memory    - print runtime memory protection\n");
    uart_puts("  guard-test - prove the stack guard page blocks writes\n");
    uart_puts("  build     - print build information\n");
    uart_puts("  config    - print boot configuration\n");
    uart_puts("  security  - print security profile and policy\n");
    uart_puts("  phase     - print current boot phase\n");
    uart_puts("  checks    - run structured boot checks\n");
    uart_puts("  timer     - print generic timer information\n");
    uart_puts("  uptime    - print time since kernel boot\n");
    uart_puts("  panic     - trigger a manual kernel panic\n");
    uart_puts("  clear     - clear terminal\n");
}

static void shell_execute(const char *command)
{
    if (k_strlen(command) == 0)
    {
        return;
    }

    if (k_strcmp(command, "help") == 0)
    {
        shell_help();
        return;
    }

    if (k_strcmp(command, "cpu") == 0)
    {
        cpu_info_print();
        return;
    }

    if (k_strcmp(command, "stack") == 0)
    {
        stack_print_info();
        stack_print_usage();
        return;
    }

    if (k_strcmp(command, "layout") == 0)
    {
        kernel_layout_print_info();
        return;
    }

    if (k_strcmp(command, "memory") == 0)
    {
        mmu_print_info();
        return;
    }

    if (k_strcmp(command, "guard-test") == 0)
    {
        mmu_trigger_guard_test();
        return;
    }

    if (k_strcmp(command, "build") == 0)
    {
        build_print_info();
        return;
    }

    if (k_strcmp(command, "config") == 0)
    {
        config_print_info();
        return;
    }

    if (k_strcmp(command, "security") == 0)
    {
        security_print_info();
        security_policy_print_report();
        return;
    }

    if (k_strcmp(command, "phase") == 0)
    {
        boot_phase_print();
        return;
    }

    if (k_strcmp(command, "checks") == 0)
    {
        boot_check_run_all();
        return;
    }

    if (k_strcmp(command, "timer") == 0)
    {
        shell_print_u64_line("Timer frequency: ", timer_frequency(), " Hz");
        shell_print_u64_line("Timer ticks: ", timer_ticks(), "");
        return;
    }

    if (k_strcmp(command, "uptime") == 0)
    {
        shell_print_uptime();
        return;
    }

    if (k_strcmp(command, "panic") == 0)
    {
        panic("Manual panic from kernel shell");
    }

    if (k_strcmp(command, "clear") == 0)
    {
        uart_puts("\033[2J\033[H");
        return;
    }

    uart_puts("Unknown command. Type 'help'.\n");
}

void shell_run(void)
{
    char buffer[SHELL_BUFFER_SIZE];

    boot_phase_set("shell");

    uart_puts("\n");
    uart_puts("ParanoidOS interactive shell loaded.\n");
    uart_puts("Type 'help' for commands.\n");

    while (1)
    {
        stack_check_guard();
        shell_print_prompt();
        shell_read_line(buffer, sizeof(buffer));
        shell_execute(buffer);
    }
}
