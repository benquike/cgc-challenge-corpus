#ifndef _EXTERNAL_API_H
#define _EXTERNAL_API_H

#include <libcgc.h>
#include "cgc_types.h"
#include "printf.h"

#define assert(x) \
    do { if (!(x)) { __printf(STDERR, "fail: " #x " at %s (%u)\n", __FILE__, __LINE__); _terminate(1); } } while(0)

char *__strchr(const char *s, int c);
char *__strdup(const char *src);
char *__strtok(char *str, const char *sep);
int __memcmp(const void *buf1, const void *buf2, size_t size);
int read_until(int fd, char *buf, unsigned int size, char endchar);
int __strcmp(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, size_t n);
int transmit_all(int fd, const char *str, size_t size);
int transmit_str(int fd, char *str);
size_t receive_all(int fd, char *buf, const size_t size);
size_t strcspn(const char *s1, const char *s2);
size_t strlcat(char *dst, const char *src, const size_t size);
size_t strlcpy(char *dst, const char *src, const size_t size);
size_t __strlen(const char *s);
size_t strspn(const char *s1, const char *s2);
void *__calloc(size_t count, size_t size);
void err(unsigned int id, char *str); // __attribute__((noreturn));
void *__memcpy(void *dst, const void *src, size_t size);
void *__memset(void *b, int c, size_t len);
void __sleep(int amount);

#endif /* _EXTERNAL_API_H */
