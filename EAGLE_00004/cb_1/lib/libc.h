#ifndef LIBC_H
#define LIBC_H

#define LONG_MAX 0x7FFFFFFF
#define LONG_MIN 0x80000000
#define ERANGE 20

unsigned int htonl(unsigned int hostlong);

size_t __strlen(const char *str);
char *__strcpy(char *dst, const char *src);
int __strcmp(const char *s1, const char *s2);
char *__strchr(const char *s, int c);
long __strtol(const char *str, char **endptr, int base);
char *strsep(char **stringp, const char *delim);

void *__memset(void *dst, int c, unsigned int n);
int __memcmp(const void *b1, const void *b2, size_t n);

int toupper(int c);
int tolower(int c);

int transmit_all(int fd, const char *buf, const size_t size);

extern int errno;

#define EOF                  -1

#ifndef NULL
#define NULL ((void*)0)
#endif

#define _FILE_STATE_OPEN  1
#define _FILE_STATE_ERROR 2
#define _FILE_STATE_EOF   4
#define _FILE_HAVE_LAST   8

typedef struct _FILE {
   int fd;
   int state;
   int last;
} __FILE;

extern __FILE *stdin;
extern __FILE *stdout;
extern __FILE *stderr;

int  fgetc(__FILE *);
int  getc(__FILE *);
int  getchar(void);

char *fgets(char *, int, __FILE *);

int ferror(__FILE *stream);
int feof(__FILE *stream);

int __printf(const char *format, ...);
int fprintf(__FILE * stream, const char *format, ...);
int snprintf(char *str, size_t size, const char *format, ...);

#endif
