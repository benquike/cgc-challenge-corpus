#include <stdlib.h>
#include <string.h>

char *__strdup(const char *str)
{
    size_t n = __strlen(str) + 1;
    char *dup = __malloc(n);
    if (dup == NULL)
        return NULL;
    __memcpy(dup, str, n);
    return dup;
}
