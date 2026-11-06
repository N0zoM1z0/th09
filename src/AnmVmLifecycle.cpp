#include "AnmVmLifecycle.hpp"

#include <stddef.h>
#include <string.h>

// The original constructor writes the native VM sprite-index word at +0x214.
typedef char AnmVmNativeActiveSpriteIndexAt214[
    (offsetof(AnmVmBase, activeSpriteIndex) == 0x214) ? 1 : -1];

AnmVmBase::AnmVmBase()
{
}

AnmVm::AnmVm()
{
    memset(this, 0, sizeof(AnmVm));
    activeSpriteIndex = -1;
}
