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

#include <libcgc.h>
#include <libc.h>

static char *last_strtok_str;

struct __FILE {
    int fd;
    unsigned char *buf;
    size_t bufsize;
    size_t bufpos;
};

static unsigned char stdin_buf[PAGE_SIZE];

static __FILE stdfiles[3] = {
    { STDIN, stdin_buf, 0, 0 },
    { STDOUT, NULL, 0, 0 },
    { STDERR, NULL, 0, 0 }
};

__FILE *stdin = &stdfiles[0];
__FILE *stdout = &stdfiles[1];
__FILE *stderr = &stdfiles[2];

static void *
__cgc_memchr(void *ptr, int value, size_t num)
{
    unsigned char *ptr_ = ptr;
    while (num--)
        if (*ptr_ == (unsigned char)value)
            return ptr_;
        else
            ptr_++;

    return NULL;
}

#define MIN(a, b) ((a) < (b) ? (a) : (b))

ssize_t
__fread(void *ptr, size_t size, __FILE *stream)
{
    ssize_t ret = 0;
    size_t bytes_rx, buffered, whole_chunks;
    unsigned char *ptr_ = ptr;

    if (size > SSIZE_MAX)// || stream->mode != READ)
        return EXIT_FAILURE;

    if (size == 0)
        return ret;

    // Copy remaining bytes
    if (stream->bufsize > 0) {
        buffered = MIN(size, stream->bufsize);

        __memcpy(ptr_, &stream->buf[stream->bufpos], buffered);
        stream->bufsize -= buffered;
        stream->bufpos = stream->bufsize ? stream->bufpos + buffered : 0;
        size -= buffered;
        ptr_ += buffered;
        ret += buffered;

        if (size == 0)
            return ret;

        stream->bufsize = 0;
        stream->bufpos = 0;
    }

    // Read whole chunks
    if (size >= PAGE_SIZE) {
        whole_chunks = size & ~(PAGE_SIZE - 1);

        if ((bytes_rx = readall(stream->fd, (char *)ptr_, whole_chunks)) != whole_chunks)
            return EXIT_FAILURE;

        size &= PAGE_SIZE - 1;
        ptr_ += whole_chunks;
        ret += whole_chunks;

        if (size == 0)
            return ret;
    }

//    // Read the remainder, attempting to overread to fill buffer but breaking
//    // once all of our data has been __read
//    if (!stream->buf && allocate_buffer(stream) != EXIT_SUCCESS)
//        return EXIT_FAILURE;

    while (stream->bufsize < size) {
        if (receive(stream->fd, &stream->buf[stream->bufpos + stream->bufsize],
                    PAGE_SIZE - stream->bufsize, &bytes_rx) != 0 ||
                bytes_rx == 0)
            return EXIT_FAILURE;

        stream->bufsize += bytes_rx;
    }

    if (stream->bufsize >= size) {
        buffered = MIN(size, stream->bufsize);

        __memcpy(ptr_, &stream->buf[stream->bufpos], buffered);
        stream->bufsize -= buffered;
        stream->bufpos = stream->bufsize ? stream->bufpos + buffered : 0;
        ret += buffered;
    }

    return ret;
}

ssize_t
fread_until(void *ptr, unsigned char delim, size_t size, __FILE *stream)
{
    ssize_t ret = 0;
    size_t buffered, bytes_rx;
    char *delim_ptr = NULL;
    unsigned char *ptr_ = ptr;

    // Remove upper bound check to enable pov
    if (size < 1)// || size > SSIZE_MAX || stream->mode != READ)
        return EXIT_FAILURE;

//    // Read the remainder, attempting to overread to fill buffer but breaking
//    // once all of our data has been __read
//    if (!stream->buf && allocate_buffer(stream) != EXIT_SUCCESS)
//        return EXIT_FAILURE;

    while (size - 1) {
        if (stream->buf && stream->bufsize > 0) {
            if ((delim_ptr = __cgc_memchr(&stream->buf[stream->bufpos], delim, stream->bufsize)))
                buffered = (unsigned char *)delim_ptr - &stream->buf[stream->bufpos] + 1;
            else
                buffered = stream->bufsize;

            buffered = MIN(size - 1, buffered);

            __memcpy(ptr_, &stream->buf[stream->bufpos], buffered);
            stream->bufsize -= buffered;
            stream->bufpos = stream->bufsize ? stream->bufpos + buffered : 0;
            size -= buffered;
            ptr_ += buffered;
            ret += buffered;

            if (size == 1 || delim_ptr) {
                if ((delim_ptr = __cgc_memchr(ptr, delim, ret)))
                    *delim_ptr = '\0';
                else
                    *ptr_ = '\0';

                return ret;
            }
        }

        if (receive(stream->fd, &stream->buf[stream->bufpos + stream->bufsize],
                    PAGE_SIZE - stream->bufsize, &bytes_rx) != 0 ||
                bytes_rx == 0)
            return EXIT_FAILURE;

        stream->bufsize += bytes_rx;
    }

    return EXIT_FAILURE;
}

size_t readline(int fd, char *buf, size_t s) {
    size_t i,read_;

    for (i=0; i < s; i+=read_) {
        read_ = 0;
        if (receive(fd, buf+i, 1, &read_))
            DIE(READFAIL);
        //EOF when we didn't expect
        if (!read_)
            DIE(READFAIL);
        if (*(buf+i) == NEWLINE)
            break;
    }
    
    if (*(buf+i) != NEWLINE)
        DIE(READFAIL);

    *(buf+i) = '\0';

    return i;
}

size_t readall(int fd, char *buf, size_t s) {
    size_t i,recvd = 0;

    for (i=0; i < s; i+=recvd) {
        if (receive(fd, buf+i, s-i, &recvd))
            DIE(READFAIL);
        if (!recvd)
            break;
    }

    return i;
}

size_t sendall(int fd, char *buf, size_t s) {
    size_t i,sent;

    for (i=0; i < s; i+=sent) 
        if (transmit(fd, buf+i, s-i, &sent))
            DIE(WRITEFAIL);
    
    return s;
}

void *__memset(void *s, int c, size_t n) {
    size_t i;

    for (i=0; i < n; i++)
        *((char *)s+i) = (char)c;

    return s;
}

int streq(char *s1, char *s2) {
    while (*s1++ == *s2++ && (*(s1-1)));

    return (*(s1-1) == *(s2-1));
}

int __strlen(const char *s) {
    const char *o = s;

    while(*s++);
    
    return s-o-1;
}

void __strcpy(char *s1, const char *s2) {
    while ((*s1++ = *s2++));
}

void __memcpy(void *dest, void *src, size_t len) {
    int i = 0;

    for (i = 0; i < len; i++)
        *((char*)dest+i) = *((char*)src+i);
}

unsigned int __atoi(char *s) {
    unsigned int res = 0;

    while(*s)
        res = res*10 + *s++ - '0';
        
    return res;
}

char * __strcat(char *dest, const char *src) {
    char *res = dest;

    while(*dest++);

    dest--;

    while(*src)
        *dest++ = *src++;

    *dest='\0';

    return res;
}

char *tohex(int val, char *s) {
    int i;

    for (i=7; i >= 0; i--) {
        s[7-i] = HEXC((val>>(i*4) & 0xf));
    }

    s[8] = '\0';
    return s;
}

void __sprintf(char *buf, const char *fmt, ...) {
    va_list argp;
    va_start(argp, fmt);
    __vsprintf(buf, fmt, argp);
    va_end(argp);
}

void __vsprintf(char *buf, const char *fmt, va_list argp) {
    char num[9] = {0};
    char *s, *p;
    int i;

    for (p = (char *)fmt; *p; p++) {
        if (*p != FMTCHAR) {
            *buf++ = *p;
            continue;
        }

        switch(*++p) {
            case 'b':
                //char buffer
                s = va_arg(argp, char *);
                __strcpy(buf, s);
                buf += __strlen(s);
                break;
            case 'i':
                //print hex
                i = va_arg(argp, int);
                tohex(i, num);
                __strcpy(buf, num);
                buf += __strlen(num);
                break;
            case FMTCHAR:
                *buf++ = *p;
                break;
        }
    }
}

void __printf(const char *fmt, ...) {
    va_list argp;
    va_start(argp, fmt);
    __vfdprintf(STDOUT, fmt, argp);
    va_end(argp);
}

void __fdprintf(int fd, const char *fmt, ...) {
    va_list argp;
    va_start(argp, fmt);
    __vfdprintf(fd, fmt, argp);
    va_end(argp);
}

void __vfdprintf(int fd, const char *fmt, va_list argp) {
    char hex[9];
    char *s, *p;
    int i;

    for (p = (char *)fmt; *p; p++) {
        if (*p != FMTCHAR) {
            sendall(fd, p, 1);
            continue;
        }

        switch(*++p) {
            case 'b':
                //char buffer
                s = va_arg(argp, char *);
                sendall(fd, s, __strlen(s));
                break;
            case 'h':
                //print hex
                i = va_arg(argp, int);
                tohex(i, hex);
                sendall(fd, hex, __strlen(hex));
                break;
            case FMTCHAR:
                sendall(fd, p, 1);
                break;
        }
    }
}

char *__strtok(char *s, char sep) {
    char *cur;

    if (s == NULL) {
        if (last_strtok_str != NULL)
            s = last_strtok_str;
        else
            return NULL;
    }

    cur = s;

    while (*cur != '\0' && *cur++ != sep);

    if (*cur == '\0') {
        last_strtok_str = NULL;
        return s;
    }

    *--cur = '\0';
    last_strtok_str = cur+1;
    return s;
}


int __memeq(void *b1, void *b2, size_t len) {
    int i;

    for (i=0; i < len; i++) {
        if (*(char *)b1 != *(char *)b2)
            return 0;
    }

    return 1;
}
