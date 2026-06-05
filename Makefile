CROSS = aarch64-linux-gnu-

CC = $(CROSS)gcc
LD = $(CROSS)ld
OBJCOPY = $(CROSS)objcopy

BUILD_DIR = build
TARGET_DIR = targets/qemu-virt-aarch64

VERSION = 0.1.00
TARGET_NAME = qemu-virt-aarch64
ARCH_NAME = aarch64
BUILD_MODE = debug
GIT_COMMIT = $(shell git rev-parse --short HEAD 2>/dev/null || echo unknown)

DEBUG ?= 1
SELF_TESTS ?= 1
NETWORK ?= 0
USERSPACE ?= 0
PANIC_ON_WARN ?= 0

CMDLINE ?= security=normal debug=1 self_tests=1 network=0 userspace=0 panic_on_warn=0

CONFIG_DEFINES = \
	-DPARANOID_CONFIG_DEBUG=$(DEBUG) \
	-DPARANOID_CONFIG_SELF_TESTS=$(SELF_TESTS) \
	-DPARANOID_CONFIG_NETWORK=$(NETWORK) \
	-DPARANOID_CONFIG_USERSPACE=$(USERSPACE) \
	-DPARANOID_CONFIG_PANIC_ON_WARN=$(PANIC_ON_WARN)

BUILD_DEFINES = \
	-DPARANOID_VERSION=\"$(VERSION)\" \
	-DPARANOID_TARGET=\"$(TARGET_NAME)\" \
	-DPARANOID_ARCH=\"$(ARCH_NAME)\" \
	-DPARANOID_BUILD_MODE=\"$(BUILD_MODE)\" \
	-DPARANOID_GIT_COMMIT=\"$(GIT_COMMIT)\" \
	-DPARANOID_CMDLINE='"$(CMDLINE)"' \
	$(CONFIG_DEFINES)

CFLAGS = -Wall -Wextra -Werror=implicit-function-declaration -ffreestanding -nostdlib -nostartfiles -mgeneral-regs-only -O2 -Iinclude -fno-builtin -fno-stack-protector -fno-asynchronous-unwind-tables -fno-unwind-tables $(BUILD_DEFINES)

KERNEL_C = kernel/main.c kernel/uart.c kernel/panic.c kernel/string.c kernel/log.c kernel/format.c kernel/exception.c kernel/stack.c kernel/build.c kernel/config.c kernel/boot_phase.c kernel/boot_check.c kernel/cmdline.c kernel/security.c kernel/security_policy.c kernel/selftest.c kernel/kernel_layout.c kernel/cpu_info.c kernel/boot.c kernel/shell.c
KERNEL_OBJS = $(patsubst kernel/%.c,$(BUILD_DIR)/kernel_%.o,$(KERNEL_C))

ARCH_C = arch/aarch64/cpu.c
ARCH_C_OBJS = $(patsubst arch/aarch64/%.c,$(BUILD_DIR)/aarch64_%.o,$(ARCH_C))

ARCH_ASM = arch/aarch64/exception.S
ARCH_ASM_OBJS = $(patsubst arch/aarch64/%.S,$(BUILD_DIR)/aarch64_%.o,$(ARCH_ASM))

BOOT_OBJ = $(BUILD_DIR)/boot_start.o
ARCH_OBJS = $(ARCH_C_OBJS) $(ARCH_ASM_OBJS)

all: $(BUILD_DIR)/kernel.img

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BOOT_OBJ): boot/boot.S | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c boot/boot.S -o $(BOOT_OBJ)

$(BUILD_DIR)/kernel_%.o: kernel/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/aarch64_%.o: arch/aarch64/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/aarch64_%.o: arch/aarch64/%.S | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/kernel.elf: $(BOOT_OBJ) $(KERNEL_OBJS) $(ARCH_OBJS) $(TARGET_DIR)/linker.ld
	$(LD) -T $(TARGET_DIR)/linker.ld $(BOOT_OBJ) $(KERNEL_OBJS) $(ARCH_OBJS) -o $(BUILD_DIR)/kernel.elf

$(BUILD_DIR)/kernel.img: $(BUILD_DIR)/kernel.elf
	$(OBJCOPY) -O binary $(BUILD_DIR)/kernel.elf $(BUILD_DIR)/kernel.img

run: $(BUILD_DIR)/kernel.img
	qemu-system-aarch64 -M virt -cpu cortex-a53 -nographic -kernel $(BUILD_DIR)/kernel.img

clean:
	rm -rf $(BUILD_DIR)
