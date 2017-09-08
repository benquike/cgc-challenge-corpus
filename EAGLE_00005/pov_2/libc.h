#ifndef LIBC_H
#define LIBC_H

size_t __strlen(const char *str);
char *__strcpy(char *dst, const char *src);
int __strcmp(const char *s1, const char *s2);
int memcmp(const char *s1, const char *s2, unsigned int len);
char *__memcpy(char *s1, const char *s2, unsigned int len);
char *__strchr(const char *s, int c);
int __printf(const char *format, ...);

int toupper(int c);
int tolower(int c);

int transmit_all(int fd, const void *buf, const size_t size);
int receive_all(int fd, char *buf, const size_t size);

#define EOF                  -1

#ifndef NULL
#define NULL ((void*)0)
#endif

#define _FILE_STATE_OPEN  1
#define _FILE_STATE_ERROR 2
#define _FILE_STATE_EOF   4
#define _FILE_HAVE_LAST   8

struct _FILE;
typedef struct _FILE __FILE;

extern __FILE *stdin;
extern __FILE *stdout;
extern __FILE *stderr;

int  fgetc(__FILE *);
int  getc(__FILE *);
int  getchar(void);

char *fgets(char *, int, __FILE *);
int __fread(void *, size_t, size_t, __FILE *);

int ferror(__FILE *stream);
int feof(__FILE *stream);

int __printf(const char *format, ...);
int fprintf(__FILE * stream, const char *format, ...);
void receive_until_delim(int fd, char delim);

#endif
