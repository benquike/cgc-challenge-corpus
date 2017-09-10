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

#include "string.h"

void *
__libpov_memcpy(void *destination, const void *source, size_t num)
{
    unsigned char *destination_ = destination;
    const unsigned char *source_ = source;
    while (num--)
        *destination_++ = *source_++;
    return destination;
}

void *
memmove(void *destination, const void *source, size_t num)
{
    unsigned char *destination_ = destination;
    const unsigned char *source_ = source;
    while (num--)
        if (destination < source)
            *destination_++ = *source_++;
        else
            destination_[num] = source_[num];
    return destination;
}

void *
__libpov_memset(void *ptr, int value, size_t num)
{
    unsigned char *ptr_ = ptr;
    while (num--)
        *ptr_++ = (unsigned char)value;
    return ptr;
}

size_t
__libpov_strlen(const char *str)
{
    size_t ret = 0;
    while (*str++)
        ret++;
    return ret;
}

size_t
strnlen(const char *str, size_t num)
{
    size_t ret = 0;
    while (num-- && *str++)
        ret++;
    return ret;
}

char *
__libpov_strcpy(char *destination, const char *source)
{
    size_t i = 0;
    for (; source[i]; i++)
        destination[i] = source[i];
    destination[i] = '\0';
    return destination;
}

char *
__cgc_strncpy(char *destination, const char *source, size_t num)
{
    size_t i = 0;
    for (; i < num && source[i]; i++)
        destination[i] = source[i];
    for (; i < num; i++)
        destination[i] = '\0';
    return destination;
}

char *
__cgc_strcat(char *destination, const char *source)
{
    __libpov_strcpy(destination + __libpov_strlen(destination), source);
    return destination;
}

char *
__cgc_strncat(char *destination, const char *source, size_t num)
{
    __cgc_strncpy(destination + __libpov_strlen(destination), source, num);
    destination[num] = '\0';
    return destination;
}

int
__libpov_memcmp(const void *ptr1, const void *ptr2, size_t num)
{
    const unsigned char *ptr1_ = ptr1;
    const unsigned char *ptr2_ = ptr2;

    while (num--)
        if (*ptr1_ != *ptr2_)
            return *ptr1_ - *ptr2_;
        else
            ptr1_++, ptr2_++;

    return 0;
}

int
__libpov_strcmp(const char *str1, const char *str2)
{
    for (; *str1 && *str2 && *str1 == *str2; str1++, str2++)
        ;
    return *(const unsigned char *)str1 - *(const unsigned char *)str2;
}

int
__libpov_strncmp(const char *str1, const char *str2, size_t num)
{
    for (; --num && *str1 && *str2 && *str1 == *str2; str1++, str2++)
        ;
    return *(const unsigned char *)str1 - *(const unsigned char *)str2;
}

void *
memchr(void *ptr, int value, size_t num)
{
    unsigned char *ptr_ = ptr;
    while (num--)
        if (*ptr_ == (unsigned char)value)
            return ptr_;
        else
            ptr_++;

    return NULL;
}

char *
__libpov_strchr(char *str, int character)
{
    while (*str)
        if (*str == (char)character)
            return str;
        else
            str++;

    return NULL;
}

char *
strpbrk(char *str1, const char *str2)
{
    char *ret = NULL;
    while (*str1)
        if (__libpov_strchr((char *)str2, *str1))
            return str1;
        else
            str1++;

    return NULL;
}

char *
strrchr(char *str, int character)
{
    str += __libpov_strlen(str) - 1;
    while (*str)
        if (*str == (char)character)
            return str;
        else
            str--;

    return NULL;
}

char *
strstr(char *str1, const char *str2)
{
    size_t str2_len;

    str2_len = __libpov_strlen(str2);
    while (*str1)
        if (__libpov_strncmp(str1, str2, str2_len) == 0)
            return str1;
        else
            str1++;

    return NULL;
}

size_t
strcspn(const char *str1, const char *str2)
{
    const char *match;

    if ((match = strpbrk((char *)str1, str2)) == NULL)
        return __libpov_strlen(str1);
    else
        return match - str1;
}

size_t
strspn(const char *str1, const char *str2)
{
    size_t ret = 0;
    while (*str1 && __libpov_strchr((char *)str2, *str1))
        ret++;
    return ret;
}

char *
__cgc_strtok(char *str, const char *delimiters)
{
    char *ret;
    static char *last = NULL;

    if (str == NULL && (last == NULL || __libpov_strlen(last) == 0))
        return NULL;

    if (last == NULL || __libpov_strlen(last) == 0)
        last = str;

    while (__libpov_strchr((char *)delimiters, *last) != NULL)
        last++;

    ret = last;
    if ((last = strpbrk(last, delimiters)) != NULL)
        *last++ = '\0';

    return ret;
}

