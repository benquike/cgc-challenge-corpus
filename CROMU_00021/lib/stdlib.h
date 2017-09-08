/*

Author: Jason Williams <jdw@cromulence.com>

Copyright (c) 2014 Cromulence LLC

Permission is hereby granted, __free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/
#ifndef __STDLIB_H__
#define __STDLIB_H__

#include <libcgc.h>
#include "malloc.h"

void __bzero( void *buff, size_t len );
char *__strchr(const char *s, int c);
void int_to_str( int val, char *buf );

char *__strtok(char *str, const char *delim);
char *__strdup(char *s);
int __isspace( int c );
int __isdigit( int c );
int __isnan( double val );
int __isinf( double val );
int tolower( int c );
int toupper( int c );
double __atof(const char *str);
int __atoi(const char *str);
int abs( int );

void __puts(char *str);
int __strcmp( char *str1, char *str2 );
char *__strcpy( char *dest, char *src );
char *__strncpy( char *dest, const char *src, size_t num );
size_t __strlen( const char *str );
int __printf( const char *fmt, ... );
int sprintf( char *str, const char *fmt, ... );
size_t getline( char *buffer, size_t len );
size_t __receive_until( char *, char, size_t );

void* __memcpy( void *dest, void *src, size_t numbytes );
void* __memset( void *dest, int value, size_t num );

#endif // __STDLIB_H__
