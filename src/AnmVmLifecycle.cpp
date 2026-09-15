#include "AnmVmLifecycle.hpp"

#include <string.h>

AnmVmBase::AnmVmBase()
{
}

AnmVm::AnmVm()
{
    memset(this, 0, sizeof(AnmVm));
    *reinterpret_cast<short *>(reinterpret_cast<unsigned char *>(this) + 0x214) = -1;
}
