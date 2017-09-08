#include <string.h>

char *__strcat(char *dest, const char *src)
{
  __strcpy(dest + __strlen(dest), src);
  return dest;
}
