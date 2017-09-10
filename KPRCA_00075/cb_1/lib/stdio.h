/*
 * Copyright (c) 2015 Kaprica Security, Inc.
 *
 * Permission is hereby granted, __free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */
#ifndef STDIO_H_
#define STDIO_H_

#include <stdlib.h>
#include <stdarg.h>

typedef struct __FILE __FILE;

extern __FILE * const stdin;
extern __FILE * const stdout;
extern __FILE * const stderr;

int __printf(const char *fmt, ...);
int fprintf(__FILE *stream, const char *fmt, ...);
int __sprintf(char *str, const char *fmt, ...);

int __cgc_vprintf(const char *fmt, va_list ap);
int __vfprintf(__FILE *stream, const char *fmt, va_list ap);
int __vsprintf(char *str, const char *fmt, va_list ap);

ssize_t __fread(void *ptr, size_t size, __FILE *stream);
ssize_t __freaduntil(char *str, size_t size, char term, __FILE *stream);
ssize_t __fwrite(const void *ptr, size_t size, __FILE *stream);
int __fflush(__FILE *stream);
void __fbuffered(__FILE *stream, int enable);
void __fxlat(__FILE *stream, const char *seed);
__FILE *__fopen(int fd, int mode);

#endif
