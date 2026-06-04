CROSS = aarch64-linux-gnu-

CC = $(CROSS)gcc
LD = $(CROSS)ld
OBJCOPY = $(CROSS)objcopy

CFLAGS = -Wall -Wextra -ffreestanding -nostdlib -nostartfiles -mgeneral-regs-only -O2 -Iinclude

BUILD_DIR = build
TARGET_DIR = targets/qemu-virt-aarch64

KERNEL_C = kernel/main.c kernel/uart.c kernel/panic.c
KERNEL_OBJS = $(patsubst kernel/%.c,$(BUILD_DIR)/%.o,$(KERNEL_C))

all: $(BUILD_DIR)/kernel.img

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/boot.o: boot/boot.S | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c boot/boot.S -o $(BUILD_DIR)/boot.o

$(BUILD_DIR)/%.o: kernel/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/kernel.elf: $(BUILD_DIR)/boot.o $(KERNEL_OBJS) $(TARGET_DIR)/linker.ld
	$(LD) -T $(TARGET_DIR)/linker.ld $(BUILD_DIR)/boot.o $(KERNEL_OBJS) -o $(BUILD_DIR)/kernel.elf

$(BUILD_DIR)/kernel.img: $(BUILD_DIR)/kernel.elf
	$(OBJCOPY) -O binary $(BUILD_DIR)/kernel.elf $(BUILD_DIR)/kernel.img

run: $(BUILD_DIR)/kernel.img
	qemu-system-aarch64 -M virt -cpu cortex-a53 -nographic -kernel $(BUILD_DIR)/kernel.img

clean:
	rm -rf $(BUILD_DIR)
