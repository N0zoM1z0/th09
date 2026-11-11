#include "AnmManager.hpp"

#include <stddef.h>

struct AnmLoadedGetSpriteLayout
{
    unsigned char unknown000[0x0C];
    AnmLoadedSprite *sprites;
};

typedef char AnmLoadedGetSpriteArrayAt0C[
    (offsetof(AnmLoadedGetSpriteLayout, sprites) == 0x0C) ? 1 : -1];

// Retail forms the physical sprite-record address by native unsigned
// DWORD byte arithmetic from the real loaded ANM sprite allocation.
AnmLoadedSprite *AnmLoaded::GetSprite(int spriteIndex)
{
    AnmLoadedGetSpriteLayout *anm =
        reinterpret_cast<AnmLoadedGetSpriteLayout *>(this);
    return reinterpret_cast<AnmLoadedSprite *>(
        reinterpret_cast<unsigned int>(anm->sprites) +
        static_cast<unsigned int>(spriteIndex) * sizeof(anm->sprites[0]));
}
