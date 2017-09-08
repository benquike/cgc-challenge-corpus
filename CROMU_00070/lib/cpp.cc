
extern "C"
{
#include <libcgc.h>
#include <malloc.h>
}

void *operator new( unsigned int alloc_size )
{
    return (void *)__malloc( alloc_size );
}

void *operator new[]( unsigned int alloc_size )
{
    return (void *)__malloc( alloc_size );
}

void operator delete( void *ptr ) throw()
{
    __free( ptr );
}

void operator delete[]( void *ptr ) throw()
{
    __free( ptr );
}
