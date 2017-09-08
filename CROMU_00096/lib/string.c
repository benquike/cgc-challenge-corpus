/*

Copyright (c) 2015 Cromulence LLC

Authors: Cromulence <cgc@cromulence.com>

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
#include <string.h>
#include <stdint.h>

size_t ___strlen( const char *str )
{
	size_t len = 0;
	while ( *str++ != '\0' )
		len++;

	return len;
}

void __bzero(void *s, size_t n) {
        while (n) {
                ((char *)s)[--n] = '\0';
        }
        ((char *)s)[n] = '\0';
}

void *__memset( void *ptr, int value, size_t num )
{
	void *ptr_temp = ptr;
	uint8_t set_value_byte = (uint8_t)value;
	uint32_t set_value_dword = (set_value_byte << 24) | (set_value_byte << 16) | (set_value_byte << 8) | set_value_byte;

	while ( num >= 4 )
	{
		*((uint32_t*)ptr) = set_value_dword;	
		ptr+=4;
		num-=4;	
	}

	while ( num > 0 )
	{
		*((uint8_t*)ptr++) = set_value_byte;	
		num--;
	}

	return (ptr_temp);
}

char *__strchr(char *s, int c) {
	uint32_t i;

	if (!s) {
		return(NULL);
	}
	for (i = 0; i < ___strlen(s); i++) {
		if (s[i] == c) {
			return(s+i);
		}
	}

	return(NULL);

}

char *strstr( char *str, char *sub, size_t len ) 
{
	size_t index = 0;
	size_t stlen = 0;

	if ( str == NULL || sub == NULL || len == 0 ) {
		return NULL;
	}

	stlen = ___strlen( sub );

	while ( index < ( len - ( stlen-1)  ) ) {
		if ( str[index] == sub[0] ) {
			if ( strncmp( str + index, sub, stlen ) == 0 ) {
				return str + index;
			}
		} 
		index++;
	}

	return NULL;
}


char *StrtokNext = NULL;
char *__strtok(char *str, char *sep) {
	uint32_t i, j;
	uint32_t str_len;
	char *tok;

	if (!sep) {
		return(NULL);
	}

	if (!str) {
		if (!StrtokNext) {
			return(NULL);
		} else {
			str = StrtokNext;
		}
	}

	// deal with any leading sep chars
	while (__strchr(sep, *str) && *str != '\0') {
		str++;
	}
	if (*str == '\0') {
		StrtokNext = NULL;
		return(NULL);
	}

	str_len = ___strlen(str);
	for (i = 0; i < str_len; i++) {
		if (__strchr(sep, str[i])) {
			// found a sep character
			str[i] = '\0';
			// see if there are any subsequent tokens
			for (j = i+1; j < str_len; j++) {
				if (__strchr(sep, str[j])) {
					// found one
					str[j] = '\0';
				} else {
					// no more tokens
					StrtokNext = str+j;
					return(str);
				}
			}
			if (j == str_len) {
				StrtokNext = NULL;
			}
			return(str);
		}
	}

	// made it to the end of the string without any new tokens
	StrtokNext = NULL;
	return(str);
}

int __strcmp(const char *s1, const char *s2) {

	if (s1 && !s2) {
		return(1);
	}
	if (!s1 && s2) {
		return(-1);
	}
	if (!s1 && !s2) {
		return(0);
	}

	while (*s1 != '\0' && *s2 != '\0') {
		if (*s1 > *s2) {
			return(1);
		}
		if (*s1 < *s2) {
			return(-1);
		}
		s1++;
		s2++;
	}
	if (*s1 != '\0' && *s2 == '\0') {
		return(1);
	}
	if (*s1 == '\0' && *s2 != '\0') {
		return(-1);
	}
	return(0);

}

int strncmp(const char *s1, const char *s2, size_t n) {

	if (s1 && !s2) {
		return(1);
	}
	if (!s1 && s2) {
		return(-1);
	}
	if (!s1 && !s2) {
		return(0);
	}

	while (*s1 != '\0' && *s2 != '\0') {
		if (*s1 > *s2) {
			return(1);
		}
		if (*s1 < *s2) {
			return(-1);
		}
		s1++;
		s2++;
		n--;
		if (n == 0)
			return (0);
	}
	if (*s1 != '\0' && *s2 == '\0') {
		return(1);
	}
	if (*s1 == '\0' && *s2 != '\0') {
		return(-1);
	}
	return(0);

}


char *__strcat(char *restrict s1, const char *restrict s2) {
	uint32_t i,j;

	if (!s1 || !s2) {
		return(NULL);
	}

	for (i = ___strlen(s1), j = 0; j < ___strlen(s2); i++, j++) {
		s1[i] = s2[j];
	}
	s1[i] = '\0';

	return(s1);
}


char *__strncat(char *s1, char *s2, size_t n)
{
	uint32_t i,j;

	if (!s1 || !s2) {
		return(NULL);
	}

	for (i = ___strlen(s1), j = 0; j < ___strlen(s2); i++, j++) {
		if (j >= n)
		{
			break;
		}
		s1[i] = s2[j];
	}
	s1[i] = '\0';

	return(s1);
}

int memcmp( const void *s1, const void *s2, size_t n )
{
	for ( size_t pos = 0; pos < n; pos++ )
	{
		uint8_t val1 = ((uint8_t*)s1)[pos];
		uint8_t val2 = ((uint8_t*)s2)[pos];

		if ( val1 < val2 )
			return (-1);
		else if ( val1 > val2 )
			return (1);
	}

	return (0);
}
