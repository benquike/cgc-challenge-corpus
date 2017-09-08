#ifndef CGC_MALLOC_H
#define CGC_MALLOC_H

void *__calloc(size_t nmemb, size_t size);
void  __free(void *ptr);
void *__malloc(size_t size);
void *__realloc(void *ptr, size_t size);
#endif
