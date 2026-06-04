#include <paranoid/format.h>

void k_u64_to_dec(u64 value, char *buffer)
{
    char temp[21];
    int index = 0;
    int output_index = 0;

    if (value == 0)
    {
        buffer[0] = '0';
        buffer[1] = '\0';
        return;
    }

    while (value > 0)
    {
        temp[index] = '0' + (value % 10);
        value = value / 10;
        index++;
    }

    while (index > 0)
    {
        index--;
        buffer[output_index] = temp[index];
        output_index++;
    }

    buffer[output_index] = '\0';
}

void k_u64_to_hex(u64 value, char *buffer)
{
    const char *digits = "0123456789ABCDEF";

    buffer[0] = '0';
    buffer[1] = 'x';

    for (int i = 0; i < 16; i++)
    {
        int shift = (15 - i) * 4;
        u8 digit = (value >> shift) & 0xF;

        buffer[2 + i] = digits[digit];
    }

    buffer[18] = '\0';
}
