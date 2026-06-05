#include <paranoid/build.h>
#include <paranoid/log.h>

#ifndef PARANOID_VERSION
#define PARANOID_VERSION "unknown"
#endif

#ifndef PARANOID_TARGET
#define PARANOID_TARGET "unknown"
#endif

#ifndef PARANOID_ARCH
#define PARANOID_ARCH "unknown"
#endif

#ifndef PARANOID_BUILD_MODE
#define PARANOID_BUILD_MODE "unknown"
#endif

#ifndef PARANOID_GIT_COMMIT
#define PARANOID_GIT_COMMIT "unknown"
#endif

void build_print_info(void)
{
    log_info_pair("Version", PARANOID_VERSION);
    log_info_pair("Target", PARANOID_TARGET);
    log_info_pair("Architecture", PARANOID_ARCH);
    log_info_pair("Build mode", PARANOID_BUILD_MODE);
    log_info_pair("Git commit", PARANOID_GIT_COMMIT);
}
