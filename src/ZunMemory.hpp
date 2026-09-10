#pragma once

#include <stddef.h>

class ZunMemory
{
  public:
    void *AddToRegistry(void *ptr, size_t size, char *name);
};

extern ZunMemory g_ZunMemory;
