CROSS = aarch64-linux-gnu-

CC = $(CROSS)gcc
LD = $(CROSS)ld
OBJCOPY = $(CROSS)objcopy

CFLAGS = -Wall -Wextra -ffreestanding -nostdlib -nostartfiles -mgeneral-regs-only -O2 -Iinclude -fno-builtin -fno-stack-protector -fno-asynchronous-unwind-tables -fno-unwind-tables $(BUILD_DEFINES)

BUILD_DIR = build
TARGET_DIR = targets/qemu-virt-aarch64

VERSION = 0.0.13
TARGET_NAME = qemu-virt-aarch64
ARCH_NAME = aarch64
BUILD_MODE = debug
GIT_COMMIT = $(shell git rev-parse --short HEAD 2>/dev/null || echo unknown)

BUILD_DEFINES = \
	-DPARANOID_VERSION=\"$(VERSION)\" \
	-DPARANOID_TARGET=\"$(TARGET_NAME)\" \
	-DPARANOID_ARCH=\"$(ARCH_NAME)\" \
	-DPARANOID_BUILD_MODE=\"$(BUILD_MODE)\" \
	-DPARANOID_GIT_COMMIT=\"$(GIT_COMMIT)\"

KERNEL_C = kernel/main.c kernel/uart.c kernel/panic.c kernel/string.c kernel/log.c kernel/format.c kernel/exception.c kernel/stack.c kernel/build.c
KERNEL_OBJS = $(patsubst kernel/%.c,$(BUILD_DIR)/%.o,$(KERNEL_C))

ARCH_C = arch/aarch64/cpu.c
ARCH_C_OBJS = $(patsubst arch/aarch64/%.c,$(BUILD_DIR)/aarch64_%.o,$(ARCH_C))

ARCH_ASM = arch/aarch64/exception.S
ARCH_ASM_OBJS = $(patsubst arch/aarch64/%.S,$(BUILD_DIR)/aarch64_%.o,$(ARCH_ASM))

ARCH_OBJS = $(ARCH_C_OBJS) $(ARCH_ASM_OBJS)

all: $(BUILD_DIR)/kernel.img

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/boot.o: boot/boot.S | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c boot/boot.S -o $(BUILD_DIR)/boot.o

$(BUILD_DIR)/%.o: kernel/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/aarch64_%.o: arch/aarch64/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/aarch64_%.o: arch/aarch64/%.S | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/kernel.elf: $(BUILD_DIR)/boot.o $(KERNEL_OBJS) $(ARCH_OBJS) $(TARGET_DIR)/linker.ld
	$(LD) -T $(TARGET_DIR)/linker.ld $(BUILD_DIR)/boot.o $(KERNEL_OBJS) $(ARCH_OBJS) -o $(BUILD_DIR)/kernel.elf

$(BUILD_DIR)/kernel.img: $(BUILD_DIR)/kernel.elf
	$(OBJCOPY) -O binary $(BUILD_DIR)/kernel.elf $(BUILD_DIR)/kernel.img

run: $(BUILD_DIR)/kernel.img
	qemu-system-aarch64 -M virt -cpu cortex-a53 -nographic -kernel $(BUILD_DIR)/kernel.img

clean:
	rm -rf $(BUILD_DIR)
