#ifndef STDLIB_H_
#define STDLIB_H_

#include <libcgc.h>
#include <stdarg.h>
#include <stddef.h>

#define __isinf(x) __builtin_isinf(x)
#define __isnan(x) __builtin_isnan(x)

extern int fdprintf(int fd, const char *fmt, ...);
extern int sprintf(char *s, const char *fmt, ...);
#define __printf(...) fdprintf(STDOUT, __VA_ARGS__)
extern int vfdprintf(int fd, const char *fmt, va_list ap);

long __strtol(const char *str, char **endptr, int base);
unsigned long __strtoul(const char *str, char **endptr, int base);

extern void *__malloc(size_t size);
extern void *__calloc(size_t nmemb, size_t size);
extern void *__realloc(void *ptr, size_t size);
extern void __free(void *ptr);
extern size_t malloc_size(void *ptr);
int send_n_bytes(int fd, size_t n, char *buf);

static void __exit(int ret)
{
    _terminate(ret);
}

#endif /* !STDLIB_H_ */
