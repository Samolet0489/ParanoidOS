#ifndef PARANOID_CONFIG_H
#define PARANOID_CONFIG_H

#ifndef PARANOID_CONFIG_DEBUG
#define PARANOID_CONFIG_DEBUG 1
#endif

#ifndef PARANOID_CONFIG_SELF_TESTS
#define PARANOID_CONFIG_SELF_TESTS 1
#endif

#ifndef PARANOID_CONFIG_NETWORK
#define PARANOID_CONFIG_NETWORK 0
#endif

#ifndef PARANOID_CONFIG_USERSPACE
#define PARANOID_CONFIG_USERSPACE 0
#endif

#ifndef PARANOID_CONFIG_PANIC_ON_WARN
#define PARANOID_CONFIG_PANIC_ON_WARN 0
#endif

void config_print_info(void);

int config_debug_enabled(void);
int config_self_tests_enabled(void);
int config_network_enabled(void);
int config_userspace_enabled(void);
int config_panic_on_warn_enabled(void);

#endif
