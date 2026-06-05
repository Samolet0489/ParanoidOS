#include <paranoid/cmdline.h>
#include <paranoid/string.h>
#include <paranoid/types.h>
#include <paranoid/log.h>

#ifndef PARANOID_CMDLINE
#define PARANOID_CMDLINE ""
#endif

static const char *current_cmdline = PARANOID_CMDLINE;

static int is_space(char c)
{
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

static int token_matches_key_value(const char *token, u64 token_len, const char *key, const char *value)
{
    u64 key_len = k_strlen(key);
    u64 value_len = k_strlen(value);

    if (token_len != key_len + 1 + value_len)
    {
        return 0;
    }

    for (u64 i = 0; i < key_len; i++)
    {
        if (token[i] != key[i])
        {
            return 0;
        }
    }

    if (token[key_len] != '=')
    {
        return 0;
    }

    for (u64 i = 0; i < value_len; i++)
    {
        if (token[key_len + 1 + i] != value[i])
        {
            return 0;
        }
    }

    return 1;
}

void cmdline_init(const char *line)
{
    if (line == (const char *)0)
    {
        current_cmdline = "";
        return;
    }

    current_cmdline = line;
}

const char *cmdline_get(void)
{
    return current_cmdline;
}

void cmdline_print_info(void)
{
    if (k_strlen(current_cmdline) == 0)
    {
        log_info_pair("Kernel command line", "<empty>");
        return;
    }

    log_info_pair("Kernel command line", current_cmdline);
}

int cmdline_has_key_value(const char *key, const char *value)
{
    const char *text = current_cmdline;
    u64 i = 0;

    while (text[i] != '\0')
    {
        while (is_space(text[i]))
        {
            i++;
        }

        u64 start = i;

        while (text[i] != '\0' && !is_space(text[i]))
        {
            i++;
        }

        u64 len = i - start;

        if (len > 0 && token_matches_key_value(&text[start], len, key, value))
        {
            return 1;
        }
    }

    return 0;
}

int cmdline_bool_enabled(const char *key, int default_value)
{
    if (cmdline_has_key_value(key, "1") ||
        cmdline_has_key_value(key, "true") ||
        cmdline_has_key_value(key, "yes") ||
        cmdline_has_key_value(key, "on") ||
        cmdline_has_key_value(key, "enabled"))
    {
        return 1;
    }

    if (cmdline_has_key_value(key, "0") ||
        cmdline_has_key_value(key, "false") ||
        cmdline_has_key_value(key, "no") ||
        cmdline_has_key_value(key, "off") ||
        cmdline_has_key_value(key, "disabled"))
    {
        return 0;
    }

    return default_value;
}
