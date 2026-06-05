#include <paranoid/cpu_info.h>
#include <paranoid/cpu.h>
#include <paranoid/types.h>
#include <paranoid/log.h>

void cpu_info_print(void)
{
    u64 current_el = cpu_current_el();
    u64 mpidr = cpu_mpidr();

    log_info("CPU information");
    log_info_dec("Current exception level", current_el);
    log_info_hex("CPU MPIDR register", mpidr);
}
