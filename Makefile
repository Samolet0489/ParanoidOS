CROSS = aarch64-linux-gnu-

CC = $(CROSS)gcc
LD = $(CROSS)ld
OBJCOPY = $(CROSS)objcopy

CFLAGS = -Wall -Wextra -ffreestanding -nostdlib -nostartfiles -mgeneral-regs-only -O2

BUILD_DIR = build
TARGET_DIR = targets/qemu-virt-aarch64

all: $(BUILD_DIR)/kernel.img

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/boot.o: boot/boot.S | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c boot/boot.S -o $(BUILD_DIR)/boot.o

$(BUILD_DIR)/main.o: kernel/main.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c kernel/main.c -o $(BUILD_DIR)/main.o

$(BUILD_DIR)/kernel.elf: $(BUILD_DIR)/boot.o $(BUILD_DIR)/main.o $(TARGET_DIR)/linker.ld
	$(LD) -T $(TARGET_DIR)/linker.ld $(BUILD_DIR)/boot.o $(BUILD_DIR)/main.o -o $(BUILD_DIR)/kernel.elf

$(BUILD_DIR)/kernel.img: $(BUILD_DIR)/kernel.elf
	$(OBJCOPY) -O binary $(BUILD_DIR)/kernel.elf $(BUILD_DIR)/kernel.img

run: $(BUILD_DIR)/kernel.img
	qemu-system-aarch64 -M virt -cpu cortex-a53 -nographic -kernel $(BUILD_DIR)/kernel.img

clean:
	rm -rf $(BUILD_DIR)
