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
#include <libcgc.h>
#include <stdlib.h>
#include <stdint.h>

int __memcpy( void *dest, void *src, size_t n )
{
        size_t index = 0;

        while ( index < n ) {
                ((char*)dest)[index] = ((char*)src)[index];
                index++;
        }

        return index;
}

int __islower( int c )
{
        if ( c >= 0x61 && c <= 0x7a )
                return 1;
        else
                return 0;
}

int __isupper( int c )
{
        if ( c >= 0x41 && c <= 0x5a )
                return 1;
        else
                return 0;
}

int __isalpha( int c )
{
        return __islower( c ) | __isupper( c );
}

int __isalnum( int c )
{
        return __isalpha( c ) | __isdigit( c );
}

int __isspace( int c )
{
    if ( c == ' ' ||
         c == '\t' ||
         c == '\n' ||
         c == '\v' ||
         c == '\f' ||
         c == '\r' )
        return 1;
    else
        return 0;
}

int __isdigit( int c )
{
    if ( c >= '0' && c <= '9' )
        return 1;
    else
        return 0;
}

int __isnan( double val )
{
    return __builtin_isnan( val );
}

int __isinf( double val )
{
    return __builtin_isinf( val );
}

double __atof(const char* str)
{
    if ( str == NULL )
        return 0.0;

    double val = 0.0;
    double scale = 0.1;
    int sign = 1;
    int part;

    // Skip whitespace
    while ( __isspace( str[0] ) )
        str++;

    part = 0; // First part (+/-/./number is acceptable)

    while( str[0] != '\0' )
    {
        if ( str[0] == '-' )
        {
            if ( part != 0 )
                return 0.0;

            sign = -1;
            part++;
        }
        else if ( str[0] == '+' )
        {
            if ( part != 0 )
                return 0.0;

            part++;
        }
        else if ( str[0] == '.' )
        {
            if ( part == 0 || part == 1 )
                part = 2;
            else
                return 0.0;
        }
        else if ( __isdigit( *str ) )
        {
            if ( part == 0 || part == 1 )
            {
                // In integer part
                part = 1;
                val = (val * 10.0) + (*str - '0');
            }
            else if ( part == 2 )
            {
                val += ((*str - '0') * scale);
                scale /= 10.0;
            }
            else
            {
                // part invalid
                return 0.0;
            }
        }
        else
            break;

        str++;
    }

    return (sign * val);
}


int __atoi(const char* str)
{
    if ( str == NULL )
        return 0;

    int integer_part = 0;
    int sign = 1;
    int part;
    int digit_count = 0;

    // Skip whitespace
    while ( __isspace( str[0] ) )
        str++;

    part = 0; // First part (+/-/number is acceptable)

    while( str[0] != '\0' )
    {
        if ( str[0] == '-' )
        {
            if ( part != 0 )
                return 0;

            sign = -1;
            part++;
        }
        else if ( str[0] == '+' )
        {
            if ( part != 0 )
                return 0;

            part++;
        }
        else if ( __isdigit( *str ) )
        {
            if ( part == 0 || part == 1 )
            {
                // In integer part
                part = 1;
                integer_part = (integer_part * 10) + (*str - '0');

                digit_count++;

                if ( digit_count == 10 )
                    break;
            }
            else
            {
                // part invalid
                return 0;
            }
        }
        else
            break;

        str++;
    }

    return (sign * integer_part);
}

char *__strcpy( char *dest, char *src )
{
    size_t i;

    for ( i = 0; ; i++ )
    {
        if ( src[i] == '\0' )
            break;

        dest[i] = src[i];
    }
    dest[i] = '\0';

    return (dest);
}

char *__strncpy( char *dest, const char *src, size_t n )
{
    size_t i;

    for ( i = 0; i < n && src[i] != '\0'; i++)
        dest[i] = src[i];
    for ( ; i < n; i++)
        dest[i] = '\0';

    return (dest);
}

void __bzero( void *buff, size_t len )
{
    size_t index = 0;
    unsigned char *c = buff;

    if ( buff == NULL ) {
        goto end;
    }

    if ( len == 0 ) {
        goto end;
    }

    for ( index = 0; index < len; index++ ) {
        c[index] = 0x00;
    }

end:
    return;
}

void *__memset(void *s, int c, size_t n)
{
    unsigned char *t = (unsigned char *)s;
    while (--n)
        t[n] = (unsigned char)c;
    t[n] = (unsigned char)c;
    return(s);
}

int __strcmp( const char *s1, const char *s2 )
{
    while ( *s1 && (*s1 == *s2) )
    {
      s1++,s2++;
    }
    return (*(const unsigned char *)s1 - *(const unsigned char *)s2);
}

char *__strncat ( char *dest, const char *src, size_t n )
{
    size_t dest_len = __strlen(dest);
    size_t i;

    if (dest == NULL || src == NULL)
    {
      return(dest);
    }
    for (i = 0; i < n && src[i] != '\0'; i++)
    {
      dest[dest_len+i] = src[i];
    }
    dest[dest_len+i] = '\0';

    return(dest);
}

size_t __receive_until( char *dst, char delim, size_t max )
{
    size_t len = 0;
    size_t rx = 0;
    char c = 0;

    while( len < max ) {
        dst[len] = 0x00;

        if ( receive( STDIN, &c, 1, &rx ) != 0 ) {
            len = 0;
            goto end;
        }

	if (rx == 0) {
		len = 0;
		goto end;
	}

        if ( c == delim ) {
            goto end;
        }

        dst[len] = c;
        len++;
    }
end:
    return len;
}

size_t __strcat( char *dest, char*src )
{
    size_t length = 0;
    size_t start = 0;

    if ( dest == NULL || src == NULL) {
        goto end;
    }

    start = __strlen( dest );

    for ( ; src[length] != 0x00 ; start++, length++ ) {
        dest[start] = src[length];
    }

    length = start;
end:
    return length;
}

size_t __strlen( char * str )
{
    size_t length = 0;

    if ( str == NULL ) {
        goto end;
    }

    while ( str[length] ) { length++; }

end:
    return length;
}

size_t __itoa( char *out, size_t val, size_t max )
{
    size_t length = 0;
    size_t end = 0;
    size_t temp = 0;

    if ( out == NULL ) {
        goto end;
    }

    // Calculate the needed length
    temp = val;
    do {
        end++;
        temp /= 10;
    } while ( temp );

    // ensure we have enough room
    if ( end >= max ) {
        goto end;
    }

    length = end;

    // Subtract one to skip the null
    end--;

    do {
        out[end] = (val % 10) + 0x30;
        val /= 10;
        end--;
    } while ( val );

    out[length] = 0x00;
end:
    return length;
}

void __puts( char *t )
{
    size_t size;
    size_t total_sent = 0;
    size_t len;

    if (!t) {
       return;
    }

    len = __strlen(t);

    while (total_sent < len) {
        if (transmit(STDOUT, t+total_sent, len-total_sent, &size) != 0) {
            return;
        }
        total_sent += size;
    }
    size = 0;
    while (size != 1) {
        if (transmit(STDOUT, "\n", 1, &size) != 0) {
            return;
        }
    }
}

char *__strchr(const char *s, int c) {
	while (*s != '\0') {
		if (*s == c) {
			return((char *)s);
		}
		s++;
	}
	if (*s == c) {
		return((char *)s);
	}
	return(NULL);
}

char *token = NULL;
char *prev_str = NULL;
unsigned int prev_str_len = 0;
char *prev_str_ptr = NULL;
char *__strtok(char *str, const char *delim) {
	char *start;
	char *end;
	char *t;
	int i;

	// invalid input
	if (delim == NULL) {
		return(NULL);
	}
	
	// called on existing string
	if (str == NULL) {
		if (prev_str == NULL) {
			return(NULL);
		}
		// already parsed through end of original str
		if (prev_str_ptr >= prev_str+prev_str_len) {
			return(NULL);
		}
	} else {
		// called with new string, so __free the old one
		if (prev_str) {
			deallocate(prev_str, prev_str_len);
			prev_str = NULL;
			prev_str_len = 0;
			prev_str_ptr = NULL;
		}
	}

	// not been called before, so make a copy of the string
	if (prev_str == NULL) {
		if (__strlen(str) > 4096) {
			// too big
			return(NULL);
		} 
		prev_str_len = __strlen(str);
		if (allocate(prev_str_len, 0, (void *)&prev_str)) {
			return(NULL);
		}
		__strcpy(prev_str, str);
		prev_str_ptr = prev_str;
	}

	str = prev_str_ptr;

	// make sure the string isn't starting with a delimeter
	while (__strchr(delim, str[0]) && str < prev_str+prev_str_len) {
		str++;
	}
	if (str >= prev_str+prev_str_len) {
		return(NULL);
	}

	// find the earliest next delimiter
	start = str;
	end = str+__strlen(str);
	for (i = 0; i < __strlen((char *)delim); i++) {
		if ((t = __strchr(start, delim[i]))) {
			if (t != NULL && t < end) {
				end = t;
			}
		}
	}
	
	// populate the new token
	token = start;
	*end = '\0';

	prev_str_ptr = end+1;

	return(token);
}

ssize_t __write( const void *buf, size_t count )
{
	size_t size;
	size_t total_sent = 0;

	if (!buf) {
		return(0);
	}

	while (total_sent < count) {
		if (transmit(STDOUT, buf+total_sent, count-total_sent, &size) != 0) {
			return(total_sent);
		}
		total_sent += size;
	}	

	return(total_sent);

}

char *__strdup(char *s) 
{
        char *retval;

        if (!s) {
                return(NULL);
        }

        if (allocate(__strlen(s)+1, 0, (void *)&retval)) {
                return(NULL);
        }

        __bzero(retval, __strlen(s)+1);
        __strcpy(retval, s);

        return(retval);
}

heap_metadata *heap_manager = NULL;


void *__calloc(size_t count, size_t size) {
    void *ret;
    ret = __malloc(size * count);
    __memset(ret, 0, size * count);
    return ret;
}

void __free(void *ptr) {
    heap_header *chunkHeader;
    heap_block_header *blockHead;

    chunkHeader = (heap_header*)(((char*)ptr)-sizeof(heap_header));
    chunkHeader->flags = FREE_FLAG;
    blockHead = (heap_block_header *)((int)&ptr & 0xfffff000);
    blockHead->remaining_size+=chunkHeader->size;
    return;
}

void *__malloc(size_t size) {
    heap_block_header *blockHead;
    if (heap_manager == NULL) {
        void *mallocPtr;
        //this is our first allocation.
        allocate(4096, 0, &mallocPtr);
        heap_manager = mallocPtr;
        heap_manager->mem_commit = 4096;
        heap_manager->mem_inuse = sizeof(heap_manager);
        heap_manager->mem_free = 4096-heap_manager->mem_inuse;
        allocate(4096, 0, (void *)&heap_manager->blocks);
        __memset(heap_manager->blocks, 0, 4096);
        blockHead = (heap_block_header *)heap_manager->blocks;
        blockHead->remaining_size = 4096-sizeof(heap_block_header);
        blockHead->next = NULL;
    }
    blockHead = (heap_block_header *)heap_manager->blocks;
    if(size > blockHead->remaining_size) {
        allocate(4096, 0, (void *)&blockHead->next);
        if(blockHead->next == NULL) {
            __puts("Not enough space available to allocate more heap.  Failure.");
            _terminate(-1);
        }
        blockHead = blockHead->next;
        blockHead->remaining_size = 4096-sizeof(heap_block_header);
    } else {
        heap_header *chunkHeader;
        blockHead->remaining_size-=size;
        chunkHeader = (heap_header *)blockHead->data;

        while((chunkHeader->flags & INUSE_FLAG) && (chunkHeader->size < size+sizeof(heap_header)))
            chunkHeader = (heap_header *)(((void *)(chunkHeader)+sizeof(heap_header)) + chunkHeader->size);
        chunkHeader->size = size;
        chunkHeader->flags = INUSE_FLAG;
        return (char *)chunkHeader+sizeof(heap_header);
    }
    return 0;
}


