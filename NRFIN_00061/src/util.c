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
#include "util.h"
#include "malloc.h"
#include "strtok.h"
#include "atoi.h"
#include "memcpy.h"
#include "cbstdio.h"
#include "strncmp.h"
#include "comms.h"
#include <libcgc.h>

Attribute* attributes;

/**
* Zeroize and deallocate all memory used for attributes
*
* @return None
*/
void clearAttributes() {
	Attribute* attr_ptr;
	void* next_attr;
	for(attr_ptr=attributes; attr_ptr != NULL; attr_ptr = next_attr) {
		next_attr = attr_ptr->next;
		attr_ptr->value = 0;
		attr_ptr->key = 0;
		attr_ptr->next = 0;
		__free(attr_ptr);
	}
	attributes = NULL;
}

/**
* Parse the body of the requesting message and create key/value 
* pairs for each attribute
* 
* @param body The body of the requesting message containing the attributes
*
* @return None
*/
void initializeAttributes(char* body) {
	char *key, *value;
	Attribute *new_attr;
	size_t size;

	clearAttributes();

	key = __strtok(body, "=");
	do {
		value = __strtok(0, ";");
		if(!(new_attr = __malloc(sizeof(Attribute))))
			_terminate(1);
		__bzero((char *)new_attr, sizeof(Attribute));
		size = __strlen(key);
		if(!(new_attr->key = __malloc(size+1)))
			_terminate(1);
		__bzero(new_attr->key, size+1);
		__memcpy(new_attr->key, key, size);
		size = __strlen(value);
		if(!(new_attr->value = __malloc(size+1)))
			_terminate(1);
		__bzero(new_attr->value, size+1);
		__memcpy(new_attr->value, value, size);
		new_attr->next = attributes;
		attributes = new_attr;
	} while((key = __strtok(0, "=")));
}

/**
* Send an error response message to requestor
* 
* @param response The response message to send
*
* @return None
*/
void sendErrorResponse(const char* response) {
	char* buffer;
	size_t bytes;
	int ret;

	if(!(buffer = __malloc(sizeof(RESPONSE_HDR)+__strlen(response)+1)))
		_terminate(1);

	__bzero(buffer, sizeof(RESPONSE_HDR)+__strlen(response)+1);
	__sprintf(buffer, "!X=!X?", RESPONSE_HDR, response);
	if((ret = transmit_all(STDOUT, buffer, __strlen(buffer)))) 
		_terminate(1);

	__free(buffer);
}

/**
* Store the value of the string attribute with name into buffer_ptr
* 
* @param buffer_prt The buffer to store the value
* @param name The name of the attribute 
*
* @return None
*/
void getStringAttribute(char** buffer_ptr, const char* name) {
	Attribute* attr_ptr;
	size_t size, size1, size2;

	for(attr_ptr=attributes; attr_ptr != NULL; attr_ptr = attr_ptr->next) {
		size1 = __strlen(attr_ptr->key);
		size2 = __strlen(name);
		size = size1 > size2 ? size1 : size2;
		if(!strncmp(attr_ptr->key, name, size)) {
			*buffer_ptr = attr_ptr->value;
			return;
		}
	}

	return;
}

/**
* Store the value of the integer attribute with name into buffer_ptr
* 
* @param buffer_prt The buffer to store the value
* @param name The name of the attribute 
*
* @return None
*/
void getIntegerAttribute(unsigned int* int_ptr, const char* name) {
	char* buffer=NULL;

	getStringAttribute((char **) &buffer, name);
	if(buffer)
		*int_ptr = __atoi(buffer);
	else
		*int_ptr = 0;

}
