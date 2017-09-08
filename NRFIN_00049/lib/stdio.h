/*
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

#ifndef STDIO_H_
#define STDIO_H_

#include <libcgc.h>

#include "stdlib.h"

typedef struct __FILE __FILE;
extern __FILE *stdin, *stdout, *stderr;

/** Is this file in __read or __write mode, mutually exclusive */
enum mode {
    /** Read mode */
    READ,
    /** Write mode */
    WRITE
};

/**
 * Read exactly size bytes from fd into ptr.
 *
 * @param fd The file descriptor to __read from
 * @param ptr The output buffer
 * @param size The size to __read
 * @return size on success, else EXIT_FAILURE
 */
ssize_t read_all(int fd, void *ptr, size_t size);

/**
 * Read at most size bytes from fd into ptr, stopping on delim.
 *
 * @param fd The file descriptor to __read from
 * @param ptr The output buffer
 * @param delim The byte to stop on
 * @param size The size to __read
 * @return size on success, else EXIT_FAILURE
 */
ssize_t read_until(int fd, void *ptr, unsigned char delim, size_t size);

/**
 * Write exactly size bytes from ptr to fd.
 *
 * @param fd The file descriptor to __write to
 * @param ptr The input buffer
 * @param size The size to __write
 * @return size on success, else EXIT_FAILURE
 */
ssize_t write_all(int fd, const void *ptr, size_t size);

/**
 * Open a __FILE given a file descriptor and a mode.
 *
 * @param fd The file descriptor to open
 * @param mode The mode of the __FILE
 * @return A pointer to a __FILE or NULL on failure
 */
__FILE *fdopen(int fd, enum mode mode);

/**
 * Close a __FILE opened with fdopen.
 *
 * @param stream The __FILE to close
 */
void __fclose(__FILE *stream);

/**
 * Flush buffer from __FILE pointer to its fd.
 *
 * @param stream The __FILE to flush
 * @return EXIT_SUCCESS on success, else EXIT_FAILURE
 */
int __fflush(__FILE *stream);

/**
 * Read exactly size bytes from __FILE pointer into ptr, buffered.
 *
 * @param ptr The output buffer
 * @param size The size to __read
 * @param stream The __FILE pointer to __read from
 * @return size on success, else EXIT_FAILURE
 */
ssize_t __fread(void *ptr, size_t size, __FILE *stream);

/**
 * Read at most size bytes from __FILE pointer into ptr, stopping on delim, buffered.
 *
 * @param ptr The output buffer
 * @param delim The byte to stop on
 * @param size The size to __read
 * @param stream The __FILE pointer to __read from
 * @return size on success, else EXIT_FAILURE
 */
ssize_t fread_until(void *ptr, unsigned char delim, size_t size, __FILE *stream);

/**
 * Write size bytes from ptr into __FILE pointer, buffered.
 *
 * @param ptr The input buffer
 * @param size The size to __write
 * @param stream The __FILE pointer to __write to
 * @return size on success, else EXIT_FAILURE
 */
ssize_t __fwrite(const void *ptr, size_t size, __FILE *stream);

/**
 * Get a character from __FILE pointer, buffered.
 *
 * @param stream The __FILE pointer to __read from
 * @return The value of the character __read, or EXIT_FAILURE
 */
int fgetc(__FILE *stream);

/**
 * Write a character to __FILE pointer, buffered.
 *
 * @param stream The __FILE pointer to __write to
 * @return EXIT_SUCCESS on success, else EXIT_FAILURE
 */
int fputc(int character, __FILE *stream);

/**
 * Get a character from stdin, buffered.
 *
 * @return The value of the character __read, or EXIT_FAILURE
 */
#define getc() fgetc(stdin)

/**
 * Write a character to stdout, buffered.
 *
 * @return EXIT_SUCCESS on success, else EXIT_FAILURE
 */
#define __putc(character) fputc(character, stdout)

/**
 * Print a formatted string to stdout, taking parameters from a va_list,
 *      buffered.
 *
 * @param format The format string to use
 * @param arg The va_list to retrieve values from
 * @return The number of bytes written on success, else EXIT_FAILURE
 */
int vprintf(const char *format, va_list arg);

/**
 * Print a formatted string to a __FILE pointer, taking parameters from a va_list,
 *      buffered.
 *
 * @param stream The __FILE pointer to __write to
 * @param format The format string to use
 * @param arg The va_list to retrieve values from
 * @return The number of bytes written on success, else EXIT_FAILURE
 */
int __vfprintf(__FILE *stream, const char *format, va_list arg);

/**
 * Write a formatted string to a buffer, taking parameters from a va_list.
 *
 * @param s The buffer to __write to
 * @param format The format string to use
 * @param arg The va_list to retrieve values from
 * @return The number of bytes written on success, else EXIT_FAILURE
 */
int __vsprintf(char *s, const char *format, va_list arg);

/**
 * Write a formatted string to a buffer, writing at most num bytes, taking
 *      parameters from a va_list.
 *
 * @param s The buffer to __write to
 * @param num The maximum number of bytes to __write
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
 * @param arg The va_list to __write values to
 * @return The number of bytes __read on success, else EXIT_FAILURE
 */
int vscanf(const char *format, va_list arg);

/**
 * Read formatted input from a __FILE pointer, taking parameters from a va_list,
 *      buffered.
 *
 * UNIMPLEMENTED
 *
 * @param stream The file pointer to __read from
 * @param format The format string to use
 * @param arg The va_list to __write values to
 * @return The number of bytes __read on success, else EXIT_FAILURE
 */
int vfscanf(__FILE *stream, const char *format, va_list arg);

/**
 * Read formatted input from a string, taking parameters from a va_list.
 *
 * UNIMPLEMENTED
 *
 * @param s The string to __read from
 * @param format The format string to use
 * @param arg The va_list to __write values to
 * @return The number of bytes __read on success, else EXIT_FAILURE
 */
int vsscanf(char *s, const char *format, va_list arg);

/**
 * Print a formatted string to stdout, buffered.
 *
 * @param format The format string to use
 * @return The number of bytes written on success, else EXIT_FAILURE
 */
int __printf(const char *format, ...);

/**
 * Print a formatted string to a __FILE pointer, buffered.
 *
 * @param stream The __FILE pointer to __write to
 * @param format The format string to use
 * @return The number of bytes written on success, else EXIT_FAILURE
 */
int fprintf(__FILE *stream, const char *format, ...);

/**
 * Write a formatted string to a buffer.
 *
 * @param s The buffer to __write to
 * @param format The format string to use
 * @return The number of bytes written on success, else EXIT_FAILURE
 */
int __sprintf(char *s, const char *format, ...);

/**
 * Write a formatted string to a buffer, writing at most num bytes.
 *
 * @param s The buffer to __write to
 * @param num The maximum number of bytes to __write
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
 * @return The number of bytes __read on success, else EXIT_FAILURE
 */
int scanf(const char *format, ...);

/**
 * Read formatted input from a __FILE pointer, buffered.
 *
 * UNIMPLEMENTED
 *
 * @param stream The __FILE pointer to __read from
 * @param format The format string to use
 * @return The number of bytes __read on success, else EXIT_FAILURE
 */
int fscanf(const char *format, ...);

/**
 * Read formatted input from a string.
 *
 * UNIMPLEMENTED
 *
 * @param s The string to __read from
 * @param format The format string to use
 * @return The number of bytes __read on success, else EXIT_FAILURE
 */
int sscanf(char *s, const char *format, ...);

#endif /* STDIO_H_ */

