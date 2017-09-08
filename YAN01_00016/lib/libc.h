#ifndef LIBC_H
#define LIBC_H

#include <libcgc.h>

#define PAGE_SIZE 4096

/**
 * Transmits all len bytes from the buffer buf to the file descriptor
 *   or terminates in error
**/
void transmit_all(int fd, const void* buf, size_t len);

/**
 * Receives all len bytes from the buffer buf to the file descriptor
 *   or terminates in error
**/
void receive_all(int fd, void* buf, size_t len);

/**
 * Receives data from the buffer either until the delim is seen or until 
 *   the buffer is full.
 * Since this function adds a NULL character at the end, we will actually
 *   only __read len-1 chars.
 * Returns the number of characters __read (to include the delim). Note that
 *  the user must check to see that buf[count-1] is actually delim, and not
 *  because it just ended due to the max size
**/
size_t __receive_until(int fd, char* buf, size_t len, char delim);

//string functions
int __strcmp(const char* s1, const char* s2);

size_t strcpyUntil(char* dst, const char* src, char delim);
static inline size_t __strcpy(char* dst, const char* src)
{
  return (strcpyUntil(dst, src, '\0'));
}


static inline size_t __strlen(const char* str)
{
  size_t i = 0; 
  if (str == NULL)
  {
    return (0);
  }
  for (i = 0; str[i] != '\0'; i++);
  return (i);
}

static inline void fprintStr(int fd, const char* str)
{
  if (str == NULL)
  {
    return;
  }

  transmit_all(fd, str, __strlen(str));
}

#endif//LIBC_H
