#include "AnmManager.hpp"

#include <stddef.h>

struct AnmLoadedGetSpriteLayout
{
    unsigned char unknown000[0x0C];
    AnmLoadedSprite *sprites;
};

typedef char AnmLoadedGetSpriteArrayAt0C[
    (offsetof(AnmLoadedGetSpriteLayout, sprites) == 0x0C) ? 1 : -1];

AnmLoadedSprite *AnmLoaded::GetSprite(int spriteIndex)
{
    AnmLoadedGetSpriteLayout *anm =
        reinterpret_cast<AnmLoadedGetSpriteLayout *>(this);
    return &anm->sprites[spriteIndex];
}
