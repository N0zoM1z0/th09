#include "AnmManager.hpp"
#include "AnmVmLifecycle.hpp"
#include "AnmMatrixRuntime.hpp"

#include <stddef.h>

// Use the genuine AnmLoadedSprite owner, also used by Enemy OnUpdate,
// rather than a second 0x44-byte loader projection.
typedef char AnmSetSpriteLoadedSpriteSizeIs44[
    (sizeof(AnmLoadedSprite) == 0x44 &&
     offsetof(AnmLoadedSprite, textureHeight18) == 0x18 &&
     offsetof(AnmLoadedSprite, textureWidth1C) == 0x1C &&
     offsetof(AnmLoadedSprite, heightPx) == 0x30 &&
     offsetof(AnmLoadedSprite, widthPx) == 0x34 &&
     offsetof(AnmLoadedSprite, scaleX38) == 0x38 &&
     offsetof(AnmLoadedSprite, scaleY3C) == 0x3C) ? 1 : -1];

// The 189-byte original SetSprite writes the same native AnmVm owned by
// the 9776-byte exact ExecuteScript interpreter and the ANM lifecycle.
typedef char AnmSetSpriteNativeVmOffsets[
    (sizeof(AnmVm) == 0x2A4 &&
     offsetof(AnmVm, spriteSize) == 0x28 &&
     offsetof(AnmVm, matrix1) == 0x130 &&
     offsetof(AnmVm, matrix2) == 0x170 &&
     offsetof(AnmVm, matrix3) == 0x1B0 &&
     offsetof(AnmVm, anmFile) == 0x204 &&
     offsetof(AnmVm, activeSpriteIndex) == 0x214 &&
     offsetof(AnmVm, loadedSprite) == 0x224) ? 1 : -1];

struct AnmSetSpriteLoadedView
{
    unsigned char unknown00[4];
    void *rawData04;
    unsigned char unknown08[4];
    AnmLoadedSprite *sprites0C;
    unsigned char unknown10[8];
    int numberEntriesToBeLoaded18;
};

typedef char AnmSetSpriteLoadedRawDataAt04[
    (offsetof(AnmSetSpriteLoadedView, rawData04) == 0x04) ? 1 : -1];
typedef char AnmSetSpriteLoadedSpritesAt0C[
    (offsetof(AnmSetSpriteLoadedView, sprites0C) == 0x0C) ? 1 : -1];
typedef char AnmSetSpriteLoadedPendingAt18[
    (offsetof(AnmSetSpriteLoadedView, numberEntriesToBeLoaded18) == 0x18) ? 1 : -1];

int AnmLoaded::SetSprite(AnmVm *vm, int spriteIndex)
{
    AnmSetSpriteLoadedView *self =
        reinterpret_cast<AnmSetSpriteLoadedView *>(this);
    AnmVm *target = vm;

    if (self->rawData04 == NULL || self->numberEntriesToBeLoaded18 != 0)
        return -1;

    target->activeSpriteIndex = (short)spriteIndex;
    target->anmFile = this;
    target->loadedSprite = &self->sprites0C[spriteIndex];
    target->spriteSize.x = target->loadedSprite->widthPx;
    target->spriteSize.y = target->loadedSprite->heightPx;

    // Keep the actual 0x00401300 AnmMatrixRuntime method identity while the
    // storage is the native 0x40-byte interpreter matrix.
    reinterpret_cast<AnmMatrixRuntime *>(&target->matrix1)->SetIdentity();
    reinterpret_cast<AnmMatrixRuntime *>(&target->matrix3)->SetIdentity();

    target->matrix1.m[0][0] = target->spriteSize.x / 256.0f;
    target->matrix1.m[1][1] = target->spriteSize.y / 256.0f;
    target->matrix3.m[0][0] =
        target->loadedSprite->scaleX38 /
        target->loadedSprite->textureWidth1C * target->spriteSize.x;
    target->matrix3.m[1][1] =
        target->loadedSprite->scaleY3C /
        target->loadedSprite->textureHeight18 * target->spriteSize.y;
    target->matrix2 = target->matrix1;
    return 0;
}
