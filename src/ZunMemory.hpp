#pragma once

#include <stddef.h>

class ZunMemory
{
  public:
    ZunMemory();
    ~ZunMemory();

    void *Alloc(size_t size, const char *debugText);
    void Free(void *ptr);
    void *AddToRegistry(void *ptr, size_t size, char *name);

  private:
    struct RegistryInfo
    {
        void *data;
        size_t size;
        char *name;
    };

    RegistryInfo *registry[0x1000];
    int bRegistryInUse;
};

typedef char ZunMemorySizeIs4004[
    (sizeof(ZunMemory) == 0x4004) ? 1 : -1];

extern ZunMemory g_ZunMemory;
