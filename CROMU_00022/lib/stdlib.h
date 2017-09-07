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
#include <stdarg.h>
#include <libcgc.h>

#define INUSE_FLAG 1
#define FREE_FLAG 2
#define F32_PRECISION       0.00001


typedef struct _heap_block_header {
	size_t remaining_size;
	struct _heap_block_header *next;
	char data[1];
} heap_block_header;


typedef struct _heap_header {
	size_t size;
	char flags;
} heap_header;

typedef struct _heap_metadata {
	size_t mem_commit;
	size_t mem_free;
	size_t mem_inuse;
	heap_block_header *blocks;
} heap_metadata;

//void *__calloc(size_t count, size_t size);
//void __free(void *ptr);
//void *__malloc(size_t size);



int __isspace( int c );
int __isdigit( int c );
int __isnan( double val );
int __isinf( double val );
double __atof(const char *str);
int __atoi(const char *str);
int __islower( int c );
int __isupper( int c );
int __isalpha( int c );
int __isalnum( int c );
int __memcpy( void *dest, void *src, size_t n);

char *__strcpy( char *dest, char *src );
char *__strncpy( char *, const char *, size_t );
int __putc( int );
//int __printf( const char *fmt, ... );
void __bzero( void *, size_t );
void *__memset(void *, int, size_t);
int __strcmp( const char *, const char * );
char *__strncat( char *, const char *, size_t );
size_t __receive_until( char *, char, size_t );
size_t receive_until_flush( char *, char , size_t);
size_t __strcat( char *, char* );
size_t __strlen( char * );
size_t __itoa( char *, size_t, size_t );
void __puts( char *t );
char *__strchr(const char *, int);
char *__strtok(char *, const char *);
ssize_t __write( const void *, size_t );
char *__strdup( char * );

int __putc( int c );

void int_to_str( int val, char *buf );
void int_to_hex( unsigned int val, char *buf );
void float_to_str( double val, char *buf, int precision );
int vprintf( const char *fmt, va_list arg );
int vsprintf( char *str, const char *fmt, va_list arg );
int __printf( const char *fmt, ... );
int sprintf( char *str, const char *fmt, ... );
#endif // __STDLIB_H__
