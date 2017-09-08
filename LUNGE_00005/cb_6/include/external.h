#ifndef _EXTERNAL_H
#define _EXTERNAL_H

#include "cgc_types.h"

#define transmit(a, b, c) __write(a, b, c)
#define receive(a, b, c) __read(a, b, c)
// #define terminate(a) _exit(a)

extern ssize_t __read(int fildes, void *buf, size_t nbyte);
extern ssize_t __write(int fildes, const void *buf, size_t nbyte);
// extern void _exit(int status);

extern void *__malloc(size_t size);
extern void __free(void *ptr);

#endif /* _EXTERNAL_H */
