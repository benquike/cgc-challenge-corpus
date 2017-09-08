/*
 * Copyright (C) Chris Eagle
 * Copyright (C) Narf Industries <info@narfindustries.com>
 *
 * Permission is hereby granted, __free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

// this is the libc from EAGLE_00005/pov_1/
// the only change I've made is to define fprintf() in e_libc.c
// this is only used when debugging.
#ifndef E_LIBC_H
#define E_LIBC_H

// size_t __strlen(const char *str);
// char *__strcpy(char *dst, const char *src);
// int __strcmp(const char *s1, const char *s2);
// int __memcmp(const char *s1, const char *s2, unsigned int len);
// char *__memcpy(char *s1, const char *s2, unsigned int len);
// char *__strchr(const char *s, int c);
// int __printf(const char *format, ...);

// int toupper(int c);
// int tolower(int c);

// int transmit_all(int fd, const void *buf, const size_t size);
// int receive_all(int fd, char *buf, const size_t size);

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

// int  fgetc(__FILE *);
// int  getc(__FILE *);
// int  getchar(void);

// char *fgets(char *, int, __FILE *);
// int __fread(void *, size_t, size_t, __FILE *);

// int ferror(__FILE *stream);
// int feof(__FILE *stream);

int fprintf(__FILE * stream, const char *format, ...);

#endif
