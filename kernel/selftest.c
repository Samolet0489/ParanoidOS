#include <paranoid/selftest.h>
#include <paranoid/types.h>
#include <paranoid/panic.h>
#include <paranoid/string.h>
#include <paranoid/format.h>

static u64 bss_test_value;
static char bss_test_buffer[32];

void selftest_run_all(void)
{
    char buffer[16];
    char dec_buffer[21];
    char hex_buffer[19];

    if (bss_test_value != 0)
    {
        panic("BSS u64 value was not cleared");
    }

    for (u64 i = 0; i < sizeof(bss_test_buffer); i++)
    {
        if (bss_test_buffer[i] != 0)
        {
            panic("BSS buffer was not cleared");
        }
    }

    if (k_strlen("ParanoidOS") != 10)
    {
        panic("k_strlen failed");
    }

    k_memset(buffer, 0, sizeof(buffer));
    k_memcpy(buffer, "safe", 5);

    if (k_strcmp(buffer, "safe") != 0)
    {
        panic("k_memcpy or k_strcmp failed");
    }

    if (k_strcmp("abc", "abc") != 0)
    {
        panic("k_strcmp equality failed");
    }

    if (k_strcmp("abc", "abd") >= 0)
    {
        panic("k_strcmp ordering failed");
    }

    k_u64_to_dec(12345, dec_buffer);

    if (k_strcmp(dec_buffer, "12345") != 0)
    {
        panic("k_u64_to_dec failed");
    }

    k_u64_to_hex(0x40080000, hex_buffer);

    if (k_strcmp(hex_buffer, "0x0000000040080000") != 0)
    {
        panic("k_u64_to_hex failed");
    }
}
