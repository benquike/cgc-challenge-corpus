/*
 * Copyright (C) Narf Industries <info@narfindustries.com>
 *
 * Permission is hereby granted, __libpov_free of charge, to any person obtaining a
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

#ifndef STDIO_H_
#define STDIO_H_

#include <libcgc.h>

#include "stdlib.h"

typedef struct __cgc_FILE __cgc_FILE;
extern __cgc_FILE *stdin, *stdout, *stderr;

/** Is this file in __cgc_read or __cgc_write mode, mutually exclusive */
enum mode {
    /** Read mode */
    READ,
    /** Write mode */
    WRITE
};

/**
 * Read exactly size bytes from fd into ptr.
 *
 * @param fd The file descriptor to __cgc_read from
 * @param ptr The output buffer
 * @param size The size to __cgc_read
 * @return size on success, else EXIT_FAILURE
 */
ssize_t read_all(int fd, void *ptr, size_t size);

/**
 * Read at most size bytes from fd into ptr, stopping on delim.
 *
 * @param fd The file descriptor to __cgc_read from
 * @param ptr The output buffer
 * @param delim The byte to stop on
 * @param size The size to __cgc_read
 * @return size on success, else EXIT_FAILURE
 */
ssize_t read_until(int fd, void *ptr, unsigned char delim, size_t size);

/**
 * Write exactly size bytes from ptr to fd.
 *
 * @param fd The file descriptor to __cgc_write to
 * @param ptr The input buffer
 * @param size The size to __cgc_write
 * @return size on success, else EXIT_FAILURE
 */
ssize_t write_all(int fd, const void *ptr, size_t size);

/**
 * Flush buffer from __cgc_FILE pointer to its fd.
 *
 * @param stream The __cgc_FILE to flush
 * @return EXIT_SUCCESS on success, else EXIT_FAILURE
 */
int __cgc_fflush(__cgc_FILE *stream);

/**
 * Read exactly size bytes from __cgc_FILE pointer into ptr, buffered.
 *
 * BUG: Currently buffering unimplemented.
 *
 * @param ptr The output buffer
 * @param size The size to __cgc_read
 * @param stream The __cgc_FILE pointer to __cgc_read from
 * @return size on success, else EXIT_FAILURE
 */
ssize_t __cgc_fread(void *ptr, size_t size, __cgc_FILE *stream);

/**
 * Read at most size bytes from __cgc_FILE pointer into ptr, stopping on delim, buffered.
 *
 * BUG: Currently buffering unimplemented.
 *
 * @param ptr The output buffer
 * @param delim The byte to stop on
 * @param size The size to __cgc_read
 * @param stream The __cgc_FILE pointer to __cgc_read from
 * @return size on success, else EXIT_FAILURE
 */
ssize_t fread_until(void *ptr, unsigned char delim, size_t size, __cgc_FILE *stream);

/**
 * Write size bytes from ptr into __cgc_FILE pointer, buffered.
 *
 * BUG: Currently buffering unimplemented.
 *
 * @param ptr The input buffer
 * @param size The size to __cgc_write
 * @param stream The __cgc_FILE pointer to __cgc_write to
 * @return size on success, else EXIT_FAILURE
 */
ssize_t __cgc_fwrite(const void *ptr, size_t size, __cgc_FILE *stream);

/**
 * Get a character from __cgc_FILE pointer, buffered.
 *
 * BUG: Currently buffering unimplemented.
 *
 * @param stream The __cgc_FILE pointer to __cgc_read from
 * @return The value of the character __cgc_read, or EXIT_FAILURE
 */
int __cgc_fgetc(__cgc_FILE *stream);

/**
 * Write a character to __cgc_FILE pointer, buffered.
 *
 * BUG: Currently buffering unimplemented.
 *
 * @param stream The __cgc_FILE pointer to __cgc_write to
 * @return EXIT_SUCCESS on success, else EXIT_FAILURE
 */
int __cgc_fputc(int character, __cgc_FILE *stream);

/**
 * Get a character from stdin, buffered.
 *
 * BUG: Currently buffering unimplemented.
 *
 * @return The value of the character __cgc_read, or EXIT_FAILURE
 */
#define getc() __cgc_fgetc(stdin)

/**
 * Write a character to stdout, buffered.
 *
 * BUG: Currently buffering unimplemented.
 *
 * @return EXIT_SUCCESS on success, else EXIT_FAILURE
 */
#define __cgc_putc(character) __cgc_fputc(character, stdout)

/**
 * Print a formatted string to stdout, taking parameters from a va_list,
 *      buffered.
 *
 * BUG: Currently buffering unimplemented.
 *
 * @param format The format string to use
 * @param arg The va_list to retrieve values from
 * @return The number of bytes written on success, else EXIT_FAILURE
 */
int __libpov_vprintf(const char *format, va_list arg);

/**
 * Print a formatted string to a __cgc_FILE pointer, taking parameters from a va_list,
 *      buffered.
 *
 * BUG: Currently buffering unimplemented.
 *
 * @param stream The __cgc_FILE pointer to __cgc_write to
 * @param format The format string to use
 * @param arg The va_list to retrieve values from
 * @return The number of bytes written on success, else EXIT_FAILURE
 */
int __libpov_vfprintf(__cgc_FILE *stream, const char *format, va_list arg);

/**
 * Write a formatted string to a buffer, taking parameters from a va_list.
 *
 * @param s The buffer to __cgc_write to
 * @param format The format string to use
 * @param arg The va_list to retrieve values from
 * @return The number of bytes written on success, else EXIT_FAILURE
 */
int __libpov_vsprintf(char *s, const char *format, va_list arg);

/**
 * Write a formatted string to a buffer, writing at most num bytes, taking
 *      parameters from a va_list.
 *
 * @param s The buffer to __cgc_write to
 * @param num The maximum number of bytes to __cgc_write
 * @param format The format string to use
 * @param arg The va_list to retrieve values from
 * @return The number of bytes written on success, else EXIT_FAILURE
 */
int vsnprintf(char *s, size_t num, const char *format, va_list arg);

/**
 * Read formatted input from stdin, taking parameters from a va_list,
 *      buffered.
 *
 * UNIMPLEMENTED
 *
 * @param format The format string to use
 * @param arg The va_list to __cgc_write values to
 * @return The number of bytes __cgc_read on success, else EXIT_FAILURE
 */
int vscanf(const char *format, va_list arg);

/**
 * Read formatted input from a __cgc_FILE pointer, taking parameters from a va_list,
 *      buffered.
 *
 * UNIMPLEMENTED
 *
 * @param stream The file pointer to __cgc_read from
 * @param format The format string to use
 * @param arg The va_list to __cgc_write values to
 * @return The number of bytes __cgc_read on success, else EXIT_FAILURE
 */
int vfscanf(__cgc_FILE *stream, const char *format, va_list arg);

/**
 * Read formatted input from a string, taking parameters from a va_list.
 *
 * UNIMPLEMENTED
 *
 * @param s The string to __cgc_read from
 * @param format The format string to use
 * @param arg The va_list to __cgc_write values to
 * @return The number of bytes __cgc_read on success, else EXIT_FAILURE
 */
int vsscanf(char *s, const char *format, va_list arg);

/**
 * Print a formatted string to stdout, buffered.
 *
 * BUG: Currently buffering unimplemented.
 *
 * @param format The format string to use
 * @return The number of bytes written on success, else EXIT_FAILURE
 */
int __cgc_printf(const char *format, ...);

/**
 * Print a formatted string to a __cgc_FILE pointer, buffered.
 *
 * BUG: Currently buffering unimplemented.
 *
 * @param stream The __cgc_FILE pointer to __cgc_write to
 * @param format The format string to use
 * @return The number of bytes written on success, else EXIT_FAILURE
 */
int __libpov_fprintf(__cgc_FILE *stream, const char *format, ...);

/**
 * Write a formatted string to a buffer.
 *
 * @param s The buffer to __cgc_write to
 * @param format The format string to use
 * @return The number of bytes written on success, else EXIT_FAILURE
 */
int __libpov_sprintf(char *s, const char *format, ...);

/**
 * Write a formatted string to a buffer, writing at most num bytes.
 *
 * @param s The buffer to __cgc_write to
 * @param num The maximum number of bytes to __cgc_write
 * @param format The format string to use
 * @return The number of bytes written on success, else EXIT_FAILURE
 */
int snprintf(char *s, size_t num, const char *format, ...);

/**
 * Read formatted input from stdin, buffered.
 *
 * UNIMPLEMENTED
 *
 * @param format The format string to use
 * @return The number of bytes __cgc_read on success, else EXIT_FAILURE
 */
int scanf(const char *format, ...);

/**
 * Read formatted input from a __cgc_FILE pointer, buffered.
 *
 * UNIMPLEMENTED
 *
 * @param stream The __cgc_FILE pointer to __cgc_read from
 * @param format The format string to use
 * @return The number of bytes __cgc_read on success, else EXIT_FAILURE
 */
int fscanf(const char *format, ...);

/**
 * Read formatted input from a string.
 *
 * UNIMPLEMENTED
 *
 * @param s The string to __cgc_read from
 * @param format The format string to use
 * @return The number of bytes __cgc_read on success, else EXIT_FAILURE
 */
int sscanf(char *s, const char *format, ...);

#endif /* STDIO_H_ */

