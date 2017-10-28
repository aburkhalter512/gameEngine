#include "util/string.h"

#include <string.h>
#include <stdlib.h>

char* copy_str(const char* src)
{
    size_t srcLen = strlen(src) + 1;
    char* copy = malloc(sizeof(char) * srcLen);
    return strcpy(copy, src);
}
