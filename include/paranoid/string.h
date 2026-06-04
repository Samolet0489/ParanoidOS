#ifndef PARANOID_STRING_H
#define PARANOID_STRING_H

#include <paranoid/types.h>

u64 k_strlen(const char *text);
void *k_memset(void *dest, u8 value, u64 count);
void *k_memcpy(void *dest, const void *src, u64 count);
int k_strcmp(const char *left, const char *right);

#endif
