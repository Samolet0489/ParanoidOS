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

void kernel_main(void)
{
    boot_phase_set("kernel entry");

    log_info("ParanoidOS booted");
    log_info("Security mode: paranoid learning build");
    log_info("Milestone 23: CPU info module cleanup loaded");

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

    boot_phase_set("stack setup");

    stack_init_guard();
    stack_check_guard();
    log_info("Stack guard initialized");

    stack_print_info();
    stack_print_usage();

    boot_phase_set("exception setup");

    exception_init();
    log_info("Exception handling initialized");

    boot_phase_set("boot info");
    cpu_info_print();
    kernel_layout_print_info();

    boot_phase_set("boot checks");
    boot_check_run_all();

    boot_phase_set("idle");
    log_info("Kernel entered idle state");

    while (1)
    {
        stack_check_guard();
        __asm__ volatile("wfe");
    }
}
