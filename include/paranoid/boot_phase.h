#ifndef PARANOID_BOOT_PHASE_H
#define PARANOID_BOOT_PHASE_H

void boot_phase_set(const char *phase);
const char *boot_phase_name(void);
void boot_phase_print(void);

#endif
