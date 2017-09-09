#ifndef STDDEF_H_
#define STDDEF_H_

#define PAGE_SIZE 4096

typedef long int ptrdiff_t;

#ifndef _LIBCGC_H
typedef long unsigned int size_t;

#ifndef __ssize_t_defined
typedef long int ssize_t;
# define __ssize_t_defined
#endif

    #undef NULL
    #define NULL 0
#endif

#define offsetof(TYPE, MEMBER) __builtin_offsetof (TYPE, MEMBER)

#endif /* !STDDEF_H_ */
