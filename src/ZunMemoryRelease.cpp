#include "ZunMemory.hpp"

#include <stdlib.h>

void *ZunMemory::Alloc(size_t size, const char *debugText)
{
    return malloc(size);
}

void ZunMemory::Free(void *ptr)
{
    free(ptr);
}

void *ZunMemory::AddToRegistry(void *ptr, size_t size, char *name)
{
    return ptr;
}
