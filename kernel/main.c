#include <paranoid/boot.h>
#include <paranoid/panic.h>

void kernel_main(void)
{
    boot_run();

    panic("boot_run returned unexpectedly");
}
