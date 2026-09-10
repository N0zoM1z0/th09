#pragma once

#include <stddef.h>

class ZunMemory
{
  public:
    void *Alloc(size_t size, const char *debugText);
    void *AddToRegistry(void *ptr, size_t size, char *name);
};

extern ZunMemory g_ZunMemory;
