#include <paranoid/string.h>

u64 k_strlen(const char *text)
{
    u64 length = 0;

    while (text[length] != '\0')
    {
        length++;
    }

    return length;
}

void *k_memset(void *dest, u8 value, u64 count)
{
    u8 *ptr = dest;

    for (u64 i = 0; i < count; i++)
    {
        ptr[i] = value;
    }

    return dest;
}

void *k_memcpy(void *dest, const void *src, u64 count)
{
    u8 *dest_ptr = dest;
    const u8 *src_ptr = src;

    for (u64 i = 0; i < count; i++)
    {
        dest_ptr[i] = src_ptr[i];
    }

    return dest;
}

int k_strcmp(const char *left, const char *right)
{
    u64 i = 0;

    while (left[i] != '\0' && right[i] != '\0')
    {
        if (left[i] != right[i])
        {
            return left[i] - right[i];
        }

        i++;
    }

    return left[i] - right[i];
}
