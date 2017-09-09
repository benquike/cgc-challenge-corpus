extern "C" {
    #include <libcgc.h>
    #include <stdlib.h>
};

extern void terminate(int);

extern "C" void __cxa_pure_virtual()
{
    __printf("Virtual Function....\n");
}

void *operator new(unsigned int size)
{
    return __malloc(size);
}

void *operator new[](unsigned int size)
{
    return ::operator new(size);
}

void operator delete(void *p) throw()
{
    __free(p);
}

void operator delete[](void *p) throw()
{
    ::operator delete(p);
}

