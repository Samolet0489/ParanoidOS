#ParanoidOS

ParanoidOS is a tiny ARM64 operating system built from scratch as a learning project.

## Design theme

Security-first, minimal, paranoid by default.

## Rules
- No network by default.
- No hidden background services.
- No dynamic loading at the beginning.
- No userspace until the kernel is understandable.
- No permissions by accident.
- Every feature starts disabled.
- Small code is better than magic code.

## First target

QEMU ARM64 virt machine.

## Future target

Samsung Galaxy A32 5G SM-A326B/DS as an experimental learning device.
