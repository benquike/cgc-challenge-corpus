#ifndef STRING_H_
#define STRING_H_

#include <stddef.h>

extern void *__memcpy(void *dest, const void *src, size_t n);
extern void *memmove(void *dest, const void *src, size_t n);
extern void *__memset(void *dest, int c, size_t n);
extern int __cgc_memcmp(void *s1, const void *s2, size_t n);
extern void *memchr(const void *s, int c, size_t n);

extern size_t __strlen(const char *s);
extern char *__strcpy(char *dest, const char *src);
extern char *__strncpy(char *dest, const char *src, size_t n);
extern char *__strchr(const char *s, int c);
extern char *strsep(char **stringp, const char *delim);
extern int __strcmp(const char *s1, const char *s2);

#endif /* !STRING_H_ */
