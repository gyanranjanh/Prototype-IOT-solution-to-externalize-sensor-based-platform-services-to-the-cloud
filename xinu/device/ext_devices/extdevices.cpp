#include <xinu_cpp.h>

void __cxa_pure_virtual() { while (1); }

void *malloc(uint32 size) {
    return (void *)getmem(size);
}

