# ParanoidOS Project Progress Log

Purpose: This file is meant to be updated after every work session and pasted/uploaded into future ChatGPT chats so the assistant can continue the project without losing context.

Last updated: 2026-06-05

---

## 1. Project identity

Project name: ParanoidOS

Goal:
Build a tiny operating system from scratch as a learning project, with a strong security/paranoid design theme.

Main idea:
Start safely in an ARM64 emulator, learn OS fundamentals, then later use the Samsung A32 5G as an experimental learning device only after enough boot/kernel knowledge is built.

This is not intended to become a realistic secure daily-phone OS immediately. The phone is a sacrificial/learning device, while the user's actual daily phone is separate.

---

## 2. Hardware target / learning phone

Phone:
Samsung Galaxy A32 5G

Model:
SM-A326B/DS

Role:
Learning / experimentation device only.

Current decision:
Do not flash the phone yet. First build and test ParanoidOS in QEMU. Use the phone only for reconnaissance until the OS and boot concepts are understood better.

Important note:
The A32 5G is not a good supported target for GrapheneOS/CalyxOS/official LineageOS. It is useful here because the user accepts the risk and wants to learn.

---

## 3. Current development environment

Host OS:
Windows laptop

Terminal environments:
- PowerShell for Windows-side phone/ADB/GitHub/browser tasks
- WSL Ubuntu for OS development

Project path in WSL:
~/projects/ParanoidOS

Current WSL user shown in terminal:
root

Repository owner/account visible on GitHub:
Samolet0489

GitHub repo:
ParanoidOS

Branch:
main was recommended after renaming from master.

---

## 4. Tools installed / used

Expected Windows-side tools:
- Samsung Android USB Driver
- Android SDK Platform-Tools
- adb.exe
- GitHub in browser

Expected WSL-side tools:
- build-essential
- git
- make
- qemu-system-aarch64
- gcc-aarch64-linux-gnu
- binutils-aarch64-linux-gnu
- gdb-multiarch
- gh, if GitHub CLI authentication is needed

Important commands used/recommended:
sudo apt update
sudo apt install -y build-essential git make qemu-system-aarch64 gcc-aarch64-linux-gnu binutils-aarch64-linux-gnu gdb-multiarch

---

## 5. Project structure created

Expected structure:

ParanoidOS/
├── arch/
│   └── aarch64/
├── boot/
│   └── boot.S
├── build/
├── docs/
├── include/
├── kernel/
│   └── main.c
├── targets/
│   └── qemu-virt-aarch64/
│       └── linker.ld
├── tools/
├── .gitignore
├── Makefile
└── README.md

Note:
build/ contains generated files and should not be committed.

---

## 6. First OS milestone completed

Milestone:
Boot minimal ARM64 kernel in QEMU.

Target:
QEMU ARM64 virt machine

QEMU command used through Makefile:
qemu-system-aarch64 -M virt -cpu cortex-a53 -nographic -kernel build/kernel.img

Expected boot output:
ParanoidOS booted.
Security mode: paranoid learning build.
No network. No userspace. No mercy.

Earlier tutorial text may have used AlexOS, but the actual chosen project name is ParanoidOS.

---

## 7. Files created so far

### boot/boot.S

Purpose:
First code that runs. Sets up a stack and jumps to kernel_main.

Key behavior:
- Defines _start
- Loads stack_top into sp
- Calls kernel_main
- Hangs forever with wfe loop

### kernel/main.c

Purpose:
Minimal kernel entry point and UART printing.

Key behavior:
- Uses QEMU UART address 0x09000000
- Implements uart_putc
- Implements uart_puts
- Prints ParanoidOS boot messages
- Hangs forever with wfe loop

### targets/qemu-virt-aarch64/linker.ld

Purpose:
Linker script for QEMU ARM64 virt target.

Key behavior:
- Entry point: _start
- Load address: 0x40080000
- Defines .text, .rodata, .data, .bss

### Makefile

Purpose:
Builds and runs the kernel.

Key behavior:
- Uses aarch64-linux-gnu toolchain
- Builds boot.o and main.o
- Links kernel.elf
- Converts to kernel.img
- Runs QEMU

### .gitignore

Purpose:
Avoid committing generated build artifacts.

Expected contents:
build/
*.o
*.elf
*.img

### README.md

Purpose:
Documents ParanoidOS security-first learning theme.

---

## 8. Git history / GitHub status

Initial local commit was created successfully.

Original accidental commit message:
Boot minimal ARM65 kernel in QEMU

Correction recommended:
Boot minimal ARM64 kernel in QEMU

Git identity issue:
The first commit was made as:
root <root@DESKTOP-ACIGFQP.>

Then Git config was corrected:
git config --global user.name "Samolet0489"
git config --global user.email "alexsavovjr@gmail.com"

Typo encountered:
git config -- global user.email "alexsavovjr@gmail.com"

Correct form:
git config --global user.email "alexsavovjr@gmail.com"

Recommended amend command:
git commit --amend --author="Samolet0489 <alexsavovjr@gmail.com>" -m "Boot minimal ARM64 kernel in QEMU"

Generated build files were accidentally committed first:
- build/boot.o
- build/kernel.elf
- build/kernel.img
- build/main.o

Fix recommended:
Create .gitignore and remove build/ from Git tracking.

Commands:
cat > .gitignore << 'EOF'
build/
*.o
*.elf
*.img
EOF

git rm -r --cached build
git add .gitignore
git commit --amend --no-edit

GitHub push flow recommended:
git remote add origin https://github.com/Samolet0489/ParanoidOS.git
git branch -M main
git push -u origin main

User reported:
Everything from the Git/GitHub instructions seemed to work.

Current assumption:
ParanoidOS exists on GitHub under Samolet0489 and has the first minimal QEMU ARM64 kernel pushed. This should be verified with:
git remote -v
git branch
git log --oneline --decorate -3
git status

---

## 9. Phone reconnaissance status

Phone was plugged into laptop.

Recommended but not yet confirmed as completed:
Enable USB debugging:
Settings -> About phone -> Software information -> tap Build number 7 times -> Developer options -> USB debugging

Recommended ADB check:
C:\platform-tools\adb.exe devices

Recommended safe info dump location:
C:\a32-lab

Recommended safe info dump commands:
C:\platform-tools\adb.exe shell getprop > a32_getprop.txt
C:\platform-tools\adb.exe shell uname -a > a32_kernel.txt
C:\platform-tools\adb.exe shell cat /proc/cpuinfo > a32_cpuinfo.txt
C:\platform-tools\adb.exe shell cat /proc/meminfo > a32_meminfo.txt
C:\platform-tools\adb.exe shell df -h > a32_storage.txt
C:\platform-tools\adb.exe shell ls -l /dev/block/by-name > a32_partitions.txt

Status:
Not confirmed yet whether these ADB dumps were successfully created.

Rule:
Do not write to phone partitions yet.
Do not unlock bootloader yet.
Do not flash anything yet.

---

## 10. Current safety rules

Current rule:
Emulator first, phone later.

Do not do yet:
- No Odin/Heimdall flashing
- No bootloader unlock
- No vbmeta changes
- No partition writes
- No rooting
- No GSI flashing
- No random ROM flashing

Allowed now:
- QEMU work
- Git commits
- ADB read-only inspection
- Documentation
- Learning boot/kernel concepts

---

## 11. Current technical understanding

ParanoidOS currently runs as:
Bare-metal ARM64 code on QEMU virt machine.

It currently has:
- Assembly boot entry
- Stack setup
- C kernel_main
- UART output
- Infinite low-power wait loop

It does not yet have:
- Standard library
- Memory functions
- Logger module
- Panic system
- Exception vectors
- Interrupts
- Timer
- MMU
- Heap
- Scheduler
- Userspace
- Filesystem
- Phone boot support

---

## 12. Next recommended steps

Next session should start by verifying repo and build state:

cd ~/projects/ParanoidOS
git status
git remote -v
git log --oneline --decorate -3
make clean
make run

Then continue with the next OS milestone:

Milestone 2:
Create a tiny kernel library and panic system.

Planned changes:
- Move UART code out of kernel/main.c
- Add include/paranoid/types.h
- Add include/paranoid/uart.h
- Add kernel/uart.c
- Add include/paranoid/panic.h
- Add kernel/panic.c
- Add simple kernel_print / kernel_panic behavior
- Update Makefile to build multiple C files
- Commit as:
  Add UART driver and kernel panic output

Suggested future roadmap:
1. UART driver cleanup
2. Tiny string/memory functions: strlen, memcpy, memset, strcmp
3. Kernel logger: INFO/WARN/PANIC
4. Panic handler
5. Exception vector table
6. ARM64 exception level inspection
7. Timer interrupt
8. Simple physical memory map
9. Page tables / MMU
10. Simple task scheduler
11. Tiny shell
12. Android boot image research
13. Samsung A32 5G partition/firmware research
14. Controlled phone experiments only after strong emulator progress

---

## 13. How to use this file in a future ChatGPT chat

Paste or upload this file and say:

I want to continue ParanoidOS. Here is the current project progress log. Continue from the next recommended step.

The assistant should:
- Treat ParanoidOS as the active OS learning project.
- Use Samsung Galaxy A32 5G SM-A326B/DS as the future experimental device.
- Keep work emulator-first.
- Avoid flashing instructions until explicitly ready and backed by checks.
- Continue from Milestone 2 unless the repo state says otherwise.


## Milestone 4 completed 

Added a basic kernel logger.

New files:
- include/paranoid/log.h
- kernel/log.c

New functions:
- log_info()
- log_warn()
- log_error()
- log_panic()

main.c now uses the logger instead of calling uart_puts() directly.

Expected boot output now uses log levels:
[INFO], [WARN], [ERROR], [PANIC]

Commit:
Add kernel logger



## Milestone 5 completed

Added number formatting support.

New files:
- include/paranoid/format.h
- kernel/format.c

New functions:
- k_u64_to_dec()
- k_u64_to_hex()
- log_info_dec()
- log_info_hex()

The kernel can now print decimal values and full 64-bit hexadecimal values.

Example output:
[INFO] Example decimal value: 12345
[INFO] Kernel load address: 0x0000000040080000

Commit:
Add number and hex formatting


## Milestone 6 completed

Added CPU and boot information logging.

New files:
- include/paranoid/cpu.h
- arch/aarch64/cpu.c

Updated files:
- targets/qemu-virt-aarch64/linker.ld
- Makefile
- kernel/main.c

New functions:
- cpu_current_el()
- cpu_mpidr()

New linker symbols:
- __kernel_start
- __kernel_end
- __bss_start
- __bss_end

The kernel can now print basic ARM64 boot information, including the current exception level, MPIDR register, and kernel memory region addresses.

Commit:
Add CPU and boot information logging


## Milestone 8 completed

Added basic ARM64 exception handling.

New files:
- include/paranoid/exception.h
- kernel/exception.c
- arch/aarch64/exception.S

Updated files:
- include/paranoid/cpu.h
- arch/aarch64/cpu.c
- kernel/main.c
- Makefile

New behavior:
- Kernel installs an ARM64 exception vector table using VBAR_EL1
- Kernel prints the vector table address
- Kernel prints VBAR_EL1
- Kernel has a C exception_handler()
- Exception handler prints exception type, ESR_EL1, ELR_EL1, and FAR_EL1
- Optional test uses brk #0 through exception_trigger_test()

Commit:
Add basic ARM64 exception handling


## Milestone 9 completed

Added basic ESR_EL1 decoding for ARM64 exceptions.

Updated files:
- kernel/exception.c
- kernel/main.c

New behavior:
- Exception handler now extracts the ESR exception class
- Exception handler prints the exception class in decimal and hexadecimal
- Exception handler prints a human-readable exception class name
- Exception handler prints the instruction length bit
- Exception handler prints the ISS field

Important ESR fields:
- EC: Exception Class, bits [31:26]
- IL: Instruction Length, bit [25]
- ISS: Instruction Specific Syndrome, bits [24:0]

Tested with:
- brk #0 through exception_trigger_test()

Expected BRK result:
- ESR exception class: 60
- ESR exception class hex: 0x000000000000003C
- ESR class name: BRK instruction

Commit:
Decode ESR exception information


## Milestone 10 completed

Separated kernel ELF load segments to remove RWX permissions.

Updated files:
- targets/qemu-virt-aarch64/linker.ld
- Makefile
- kernel/main.c

New behavior:
- Linker script now defines PHDRS for text, rodata, and data segments
- .text is mapped Read + Execute
- .rodata is mapped Read only
- .data and .bss are mapped Read + Write
- The previous RWX linker warning is removed
- Compiler flags were improved for freestanding kernel development

Verification commands:
- aarch64-linux-gnu-readelf -l build/kernel.elf
- aarch64-linux-gnu-nm -n build/kernel.elf | grep -E "__kernel|__bss|_start|exception_vector_table|stack"

Expected result:
- No LOAD segment should have RWE permissions

Commit:
Separate kernel ELF segments


## Milestone 11 completed

Added a basic stack guard.

New files:
- include/paranoid/stack.h
- kernel/stack.c

Updated files:
- boot/boot.S
- kernel/main.c
- Makefile

New behavior:
- stack_bottom and stack_top are now exported from boot.S
- Kernel initializes a 64-byte guard pattern at the bottom of the stack
- Kernel checks the stack guard during boot
- Kernel checks the stack guard while idle
- Kernel prints stack bottom, stack top, stack size, and guard size

Security purpose:
This does not provide real memory protection yet, but it helps detect simple stack overflow corruption before the MMU is implemented.

Commit:
Add basic stack guard
