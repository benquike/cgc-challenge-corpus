#ifndef STDIO_H_
#define STDIO_H_

#include <libpov.h>
#include "stdarg.h"

typedef struct __cgc_FILE __cgc_FILE;

extern __cgc_FILE * const stdin;
extern __cgc_FILE * const stdout;
extern __cgc_FILE * const stderr;

int __cgc_printf(const char *fmt, ...);
int __libpov_fprintf(__cgc_FILE *stream, const char *fmt, ...);
int __libpov_sprintf(char *str, const char *fmt, ...);

int __libpov_vprintf(const char *fmt, va_list ap);
int __libpov_vfprintf(__cgc_FILE *stream, const char *fmt, va_list ap);
int __libpov_vsprintf(char *str, const char *fmt, va_list ap);

ssize_t __cgc_fread(void *ptr, size_t size, __cgc_FILE *stream);
ssize_t __cgc_freaduntil(char *str, size_t size, char term, __cgc_FILE *stream);
ssize_t __cgc_fwrite(const void *ptr, size_t size, __cgc_FILE *stream);
int __cgc_fflush(__cgc_FILE *stream);

void *__libpov_memmove(void *dst, const void *src, size_t n);

#endif
