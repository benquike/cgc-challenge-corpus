
extern "C"
{
#include <libcgc.h>
#include <stdlib.h>
}

void call_inits( void )
{
    size_t asize, n;
    __printf("ctors called");
}
