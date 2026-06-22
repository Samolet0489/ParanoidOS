#include <paranoid/mmu.h>
#include <paranoid/log.h>
#include <paranoid/types.h>

extern char __text_start;
extern char __text_end;
extern char __rodata_start;
extern char __rodata_end;
extern char __data_start;
extern char __data_end;
extern char __bss_start;
extern char __bss_end;
extern char __stack_guard_start;
extern char __stack_guard_end;
extern char stack_bottom;
extern char stack_top;
extern char __page_tables_start;
extern char __page_tables_end;

#define MMU_PAGE_SIZE 4096ULL
#define MMU_TABLE_ENTRIES 512ULL

#define MMU_UART_BASE 0x09000000ULL
#define MMU_RAM_BASE 0x40000000ULL
#define MMU_RAM_LIMIT 0x40200000ULL

#define MMU_ADDRESS_MASK 0x0000FFFFFFFFF000ULL

#define MMU_DESC_VALID (1ULL << 0)
#define MMU_DESC_TABLE_OR_PAGE (1ULL << 1)
#define MMU_DESC_ATTR_DEVICE (1ULL << 2)
#define MMU_DESC_AP_READ_ONLY (2ULL << 6)
#define MMU_DESC_SH_OUTER (2ULL << 8)
#define MMU_DESC_SH_INNER (3ULL << 8)
#define MMU_DESC_AF (1ULL << 10)
#define MMU_DESC_PXN (1ULL << 53)
#define MMU_DESC_UXN (1ULL << 54)

#define MMU_ATTR_NORMAL_RX     (MMU_DESC_AF | MMU_DESC_SH_INNER | MMU_DESC_UXN)

#define MMU_ATTR_NORMAL_RO_NX     (MMU_DESC_AF | MMU_DESC_SH_INNER | MMU_DESC_AP_READ_ONLY |      MMU_DESC_PXN | MMU_DESC_UXN)

#define MMU_ATTR_NORMAL_RW_NX     (MMU_DESC_AF | MMU_DESC_SH_INNER | MMU_DESC_PXN | MMU_DESC_UXN)

#define MMU_ATTR_DEVICE_RW_NX     (MMU_DESC_ATTR_DEVICE | MMU_DESC_AF | MMU_DESC_SH_OUTER |      MMU_DESC_PXN | MMU_DESC_UXN)

#define MMU_MAIR_NORMAL_WBWA 0xFFULL
#define MMU_MAIR_DEVICE_NGNRE 0x04ULL
#define MMU_MAIR_VALUE     ((MMU_MAIR_NORMAL_WBWA << 0) | (MMU_MAIR_DEVICE_NGNRE << 8))

#define MMU_TCR_T0SZ_48 (16ULL << 0)
#define MMU_TCR_IRGN0_WBWA (1ULL << 8)
#define MMU_TCR_ORGN0_WBWA (1ULL << 10)
#define MMU_TCR_SH0_INNER (3ULL << 12)
#define MMU_TCR_TG0_4KB (0ULL << 14)
#define MMU_TCR_EPD1 (1ULL << 23)
#define MMU_TCR_IPS_32BIT (0ULL << 32)

#define MMU_TCR_VALUE     (MMU_TCR_T0SZ_48 | MMU_TCR_IRGN0_WBWA | MMU_TCR_ORGN0_WBWA |      MMU_TCR_SH0_INNER | MMU_TCR_TG0_4KB | MMU_TCR_EPD1 |      MMU_TCR_IPS_32BIT)

#define MMU_SCTLR_M (1ULL << 0)

static u64 mmu_l0_table[MMU_TABLE_ENTRIES]
    __attribute__((aligned(MMU_PAGE_SIZE), section(".page_tables")));

static u64 mmu_l1_table[MMU_TABLE_ENTRIES]
    __attribute__((aligned(MMU_PAGE_SIZE), section(".page_tables")));

static u64 mmu_l2_low_table[MMU_TABLE_ENTRIES]
    __attribute__((aligned(MMU_PAGE_SIZE), section(".page_tables")));

static u64 mmu_l2_ram_table[MMU_TABLE_ENTRIES]
    __attribute__((aligned(MMU_PAGE_SIZE), section(".page_tables")));

static u64 mmu_l3_kernel_table[MMU_TABLE_ENTRIES]
    __attribute__((aligned(MMU_PAGE_SIZE), section(".page_tables")));

static u64 mmu_align_down(u64 value)
{
    return value & ~(MMU_PAGE_SIZE - 1ULL);
}

static u64 mmu_align_up(u64 value)
{
    return (value + MMU_PAGE_SIZE - 1ULL) & ~(MMU_PAGE_SIZE - 1ULL);
}

static u64 mmu_l1_index(u64 address)
{
    return (address >> 30) & 0x1FFULL;
}

static u64 mmu_l2_index(u64 address)
{
    return (address >> 21) & 0x1FFULL;
}

static u64 mmu_l3_index(u64 address)
{
    return (address >> 12) & 0x1FFULL;
}

static void mmu_clear_table(u64 *table)
{
    u64 index;

    for (index = 0; index < MMU_TABLE_ENTRIES; index++)
    {
        table[index] = 0;
    }
}

static u64 mmu_table_descriptor(const u64 *table)
{
    return ((u64)table & MMU_ADDRESS_MASK) |
           MMU_DESC_VALID |
           MMU_DESC_TABLE_OR_PAGE;
}

static u64 mmu_page_descriptor(u64 physical_address, u64 attributes)
{
    return (physical_address & MMU_ADDRESS_MASK) |
           MMU_DESC_VALID |
           MMU_DESC_TABLE_OR_PAGE |
           attributes;
}

static u64 mmu_block_descriptor(u64 physical_address, u64 attributes)
{
    return (physical_address & MMU_ADDRESS_MASK) |
           MMU_DESC_VALID |
           attributes;
}

static int mmu_map_range(u64 start, u64 end, u64 attributes)
{
    u64 address;

    if (end < start)
    {
        return 0;
    }

    if (start < MMU_RAM_BASE || end > MMU_RAM_LIMIT)
    {
        return 0;
    }

    address = mmu_align_down(start);
    end = mmu_align_up(end);

    while (address < end)
    {
        mmu_l3_kernel_table[mmu_l3_index(address)] =
            mmu_page_descriptor(address, attributes);

        address += MMU_PAGE_SIZE;
    }

    return 1;
}

static int mmu_setup_tables(void)
{
    mmu_clear_table(mmu_l0_table);
    mmu_clear_table(mmu_l1_table);
    mmu_clear_table(mmu_l2_low_table);
    mmu_clear_table(mmu_l2_ram_table);
    mmu_clear_table(mmu_l3_kernel_table);

    mmu_l0_table[0] = mmu_table_descriptor(mmu_l1_table);

    mmu_l1_table[mmu_l1_index(MMU_UART_BASE)] =
        mmu_table_descriptor(mmu_l2_low_table);

    mmu_l1_table[mmu_l1_index(MMU_RAM_BASE)] =
        mmu_table_descriptor(mmu_l2_ram_table);

    mmu_l2_low_table[mmu_l2_index(MMU_UART_BASE)] =
        mmu_block_descriptor(MMU_UART_BASE, MMU_ATTR_DEVICE_RW_NX);

    mmu_l2_ram_table[mmu_l2_index(MMU_RAM_BASE)] =
        mmu_table_descriptor(mmu_l3_kernel_table);

    if (!mmu_map_range(
        (u64)&__text_start,
        (u64)&__text_end,
        MMU_ATTR_NORMAL_RX))
    {
        return 0;
    }

    if (!mmu_map_range(
        (u64)&__rodata_start,
        (u64)&__rodata_end,
        MMU_ATTR_NORMAL_RO_NX))
    {
        return 0;
    }

    if (!mmu_map_range(
        (u64)&__data_start,
        (u64)&__data_end,
        MMU_ATTR_NORMAL_RW_NX))
    {
        return 0;
    }

    if (!mmu_map_range(
        (u64)&__bss_start,
        (u64)&__bss_end,
        MMU_ATTR_NORMAL_RW_NX))
    {
        return 0;
    }

    if (!mmu_map_range(
        (u64)&stack_bottom,
        (u64)&stack_top,
        MMU_ATTR_NORMAL_RW_NX))
    {
        return 0;
    }

    if (!mmu_map_range(
        (u64)&__page_tables_start,
        (u64)&__page_tables_end,
        MMU_ATTR_NORMAL_RW_NX))
    {
        return 0;
    }

    return 1;
}

static void mmu_enable_translation(void)
{
    u64 sctlr;

    __asm__ volatile("dsb sy" : : : "memory");

    __asm__ volatile(
        "msr mair_el1, %0"
        :
        : "r"(MMU_MAIR_VALUE)
        : "memory");

    __asm__ volatile(
        "msr tcr_el1, %0"
        :
        : "r"(MMU_TCR_VALUE)
        : "memory");

    __asm__ volatile(
        "msr ttbr0_el1, %0"
        :
        : "r"((u64)mmu_l0_table)
        : "memory");

    __asm__ volatile("dsb sy" : : : "memory");
    __asm__ volatile("tlbi vmalle1" : : : "memory");
    __asm__ volatile("dsb sy" : : : "memory");
    __asm__ volatile("isb");

    __asm__ volatile("mrs %0, sctlr_el1" : "=r"(sctlr));

    sctlr |= MMU_SCTLR_M;

    __asm__ volatile(
        "msr sctlr_el1, %0"
        :
        : "r"(sctlr)
        : "memory");

    __asm__ volatile("isb");
}

int mmu_is_enabled(void)
{
    u64 sctlr;

    __asm__ volatile("mrs %0, sctlr_el1" : "=r"(sctlr));

    return (sctlr & MMU_SCTLR_M) != 0;
}

int mmu_init(void)
{
    if (mmu_is_enabled())
    {
        return 1;
    }

    if (!mmu_setup_tables())
    {
        return 0;
    }

    mmu_enable_translation();

    return mmu_is_enabled();
}

void mmu_print_info(void)
{
    log_info("Memory protection");

    if (!mmu_is_enabled())
    {
        log_warn("MMU is disabled");
        return;
    }

    log_info_pair("MMU", "enabled");
    log_info_pair("Translation", "identity map, 4 KiB pages");
    log_info_pair("Code pages", "RX");
    log_info_pair("Read-only pages", "R, NX");
    log_info_pair("Data and BSS pages", "RW, NX");
    log_info_pair("Stack pages", "RW, NX");
    log_info_pair("Stack guard page", "unmapped");
    log_info_pair("UART mapping", "device memory, RW, NX");

    log_info_hex("Stack guard start", (u64)&__stack_guard_start);
    log_info_hex("Stack guard end", (u64)&__stack_guard_end);
    log_info_hex("Page tables start", (u64)&__page_tables_start);
    log_info_hex("Page tables end", (u64)&__page_tables_end);
}
