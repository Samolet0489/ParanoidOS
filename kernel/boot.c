#include <paranoid/panic.h>
#include <paranoid/shell.h>
#include <paranoid/boot.h>
#include <paranoid/log.h>
#include <paranoid/exception.h>
#include <paranoid/stack.h>
#include <paranoid/build.h>
#include <paranoid/config.h>
#include <paranoid/boot_phase.h>
#include <paranoid/boot_check.h>
#include <paranoid/cmdline.h>
#include <paranoid/security.h>
#include <paranoid/security_policy.h>
#include <paranoid/selftest.h>
#include <paranoid/kernel_layout.h>
#include <paranoid/cpu_info.h>
#include <paranoid/timer.h>
#include <paranoid/mmu.h>

static void boot_run_self_tests(void)
{
    boot_phase_set("self-tests");

    if (config_self_tests_enabled())
    {
        selftest_run_all();
        log_info("Kernel self-tests passed");
    }
    else
    {
        log_warn("Kernel self-tests disabled");
    }

    log_info("BSS section cleared successfully");
}

static void boot_setup_stack(void)
{
    boot_phase_set("stack setup");

    stack_init_guard();
    stack_check_guard();
    log_info("Stack guard initialized");

    stack_print_info();
    stack_print_usage();
}

static void boot_setup_exceptions(void)
{
    boot_phase_set("exception setup");

    exception_init();
    log_info("Exception handling initialized");
}

static void boot_setup_timer(void)
{
    boot_phase_set("timer setup");

    timer_init();
    log_info("Generic timer initialized");
}

static void boot_setup_mmu(void)
{
    boot_phase_set("memory protection");

    if (!mmu_init())
    {
        panic("MMU setup failed");
    }

    log_info("MMU memory protection enabled");
}

static void boot_print_system_info(void)
{
    boot_phase_set("boot info");

    cpu_info_print();
    kernel_layout_print_info();
}

static void boot_run_checks(void)
{
    boot_phase_set("boot checks");
    boot_check_run_all();
}


static void boot_enter_shell(void)
{
    boot_phase_set("shell");
    log_info("Kernel entering interactive shell");

    shell_run();

    panic("shell_run returned unexpectedly");
}

void boot_run(void)
{
    boot_phase_set("kernel entry");

    log_info("ParanoidOS booted");
    log_info("Security mode: paranoid learning build");
    log_info("Milestone 29: guard-page fault proof loaded");

    boot_phase_print();

    boot_phase_set("build info");
    build_print_info();

    boot_phase_set("command line");
    cmdline_print_info();

    boot_phase_set("security profile");
    security_init();
    security_print_info();

    boot_phase_set("boot configuration");
    config_print_info();

    boot_phase_set("security policy report");
    security_policy_print_report();

    boot_run_self_tests();
    boot_setup_stack();
    boot_setup_exceptions();
    boot_setup_timer();
    boot_setup_mmu();
    boot_print_system_info();
    boot_run_checks();
    boot_enter_shell();
}
