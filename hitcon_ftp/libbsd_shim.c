#define _GNU_SOURCE

#include <stddef.h>
#include <string.h>

char *strnstr(const char *big, const char *little, size_t len)
{
    size_t needle_len;
    size_t i;

    if (!big || !little)
        return NULL;

    if (*little == '\0')
        return (char *)big;

    needle_len = strlen(little);
    if (needle_len > len)
        return NULL;

    for (i = 0; i + needle_len <= len && big[i] != '\0'; ++i)
    {
        if (big[i] == little[0] && memcmp(big + i, little, needle_len) == 0)
            return (char *)(big + i);
    }

    return NULL;
}
