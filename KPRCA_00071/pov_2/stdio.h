#ifndef STDIO_H_
#define STDIO_H_

#include <libpov.h>

typedef struct __FILE __FILE;

extern __FILE * const stdin;
extern __FILE * const stdout;
extern __FILE * const stderr;

int __printf(const char *fmt, ...);
int fprintf(__FILE *stream, const char *fmt, ...);
int sprintf(char *str, const char *fmt, ...);

int vprintf(const char *fmt, va_list ap);
int vfprintf(__FILE *stream, const char *fmt, va_list ap);
int vsprintf(char *str, const char *fmt, va_list ap);

ssize_t __fread(void *ptr, size_t size, __FILE *stream);
ssize_t freaduntil(char *str, size_t size, char term, __FILE *stream);
ssize_t __fwrite(const void *ptr, size_t size, __FILE *stream);
int fflush(__FILE *stream);

void *memmove(void *dst, const void *src, size_t n);

#endif
