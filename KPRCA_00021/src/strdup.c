#include <libcgc.h>
#include <string.h>
#include <stdlib.h>
#include "strdup.h"

char* __strdup(char *s)
{
  int sz = 0;
  if (s)
  {
    sz = __strlen(s) + 1;
    char *d = __malloc(sz);
    if (d)
    {
      __memset(d, 0, sz);
      __memcpy(d, s, sz);
      return d;
    }
  }
  return NULL;
}
