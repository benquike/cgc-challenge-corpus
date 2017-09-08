#ifndef STRING_H
#define STRING_H

#include <libcgc.h>
#include "inttypes.h"

size_t __strncpy(char* dst, size_t len, const char* src);

size_t __strcpy(char* dst, const char* src);

int __strcmp(const char* l, const char* r);

char* strstr(char* haystack, const char* needle);

char* __strchr(char* haystack, char needle);

char* strrstr(char* haystack, const char* needle);

char* strrchr(char* haystack, char needle);

size_t __strlen(const char* str);

uint32_t strToUint32(const char* str);
#endif//STRING_H
