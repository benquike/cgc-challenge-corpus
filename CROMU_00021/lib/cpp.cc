
extern "C"
{
#include <libcgc.h>
#include <malloc.h>
}

#if defined(_CGC_EMU)
#include <new>
#endif

void *operator new( unsigned int alloc_size ) throw(std::bad_alloc)
{
    return (void *)__malloc( alloc_size );
}

void *operator new[]( unsigned int alloc_size ) throw(std::bad_alloc)
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
