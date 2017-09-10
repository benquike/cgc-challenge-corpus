#ifndef STDIO_H_
#define STDIO_H_

#include <libpov.h>

typedef __builtin_va_list va_list;

typedef struct __FILE __FILE;

extern __FILE * const stdin;
extern __FILE * const stdout;
extern __FILE * const stderr;

int __printf(const char *fmt, ...);
int __cgc_fprintf(__FILE *stream, const char *fmt, ...);
int __cgc_sprintf(char *str, const char *fmt, ...);

int __cgc_vprintf(const char *fmt, va_list ap);
int __cgc_vfprintf(__FILE *stream, const char *fmt, va_list ap);
int __cgc_vsprintf(char *str, const char *fmt, va_list ap);

ssize_t __fread(void *ptr, size_t size, __FILE *stream);
ssize_t __cgc_freaduntil(char *str, size_t size, char term, __FILE *stream);
ssize_t __fwrite(const void *ptr, size_t size, __FILE *stream);
int __cgc_fflush(__FILE *stream);

void *memmove(void *dst, const void *src, size_t n);

#endif
