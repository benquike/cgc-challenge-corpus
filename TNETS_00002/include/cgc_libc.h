#ifndef _EXTERNAL_API_H
#define _EXTERNAL_API_H

#include <libcgc.h>
#include "cgc_types.h"
#include "printf.h"

#define assert(x) \
    do { if (!(x)) { __printf("fail: " #x " at %s (%u)\n", __FILE__, __LINE__); } } while(0)

char *__strchr(const char *s, int c);
char *__strdup(const char *src);
char *__strtok(char *str, const char *sep);
int __strcmp(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, size_t n);
size_t strcspn(const char *s1, const char *s2);
size_t strlcat(char *dst, const char *src, const size_t size);
size_t strlcpy(char *dst, const char *src, const size_t size);
size_t __strlen(const char *s);
size_t strspn(const char *s1, const char *s2);
void *__calloc(size_t count, size_t size);
void *__memset(void *b, int c, size_t len);
void *__memcpy(void *dst, const void *src, size_t size);
void err(unsigned int id, char *str); // __attribute__((noreturn));
int transmit_str(int fd, const char *str);
int transmit_all(int fd, const char *str, size_t size);

#endif /* _EXTERNAL_API_H */
