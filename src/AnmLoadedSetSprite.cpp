#include "AnmManager.hpp"
#include "AnmMatrixRuntime.hpp"

#include <stddef.h>

struct AnmSetSpriteFloat2
{
    float x;
    float y;
};

struct AnmSetSpriteLoadedSpriteView
{
    unsigned char unknown00[0x18];
    float textureHeight18;
    float textureWidth1C;
    unsigned char unknown20[0x10];
    float spriteHeight30;
    float spriteWidth34;
    float scaleX38;
    float scaleY3C;
    unsigned char unknown40[4];
};

typedef char AnmSetSpriteLoadedSpriteSizeIs44[
    (sizeof(AnmSetSpriteLoadedSpriteView) == 0x44) ? 1 : -1];

struct AnmSetSpriteVmView
{
    unsigned char unknown000[0x28];
    AnmSetSpriteFloat2 spriteSize28;
    unsigned char unknown030[0x130 - 0x30];
    AnmMatrixRuntime matrix1;
    AnmMatrixRuntime matrix2;
    AnmMatrixRuntime matrix3;
    unsigned char unknown1F0[0x204 - 0x1F0];
    AnmLoaded *anmFile204;
    unsigned char unknown208[0x214 - 0x208];
    short activeSpriteIndex214;
    unsigned char unknown216[0x224 - 0x216];
    AnmSetSpriteLoadedSpriteView *loadedSprite224;
    unsigned char unknown228[0x2A4 - 0x228];
};

typedef char AnmSetSpriteVmSizeIs2A4[
    (sizeof(AnmSetSpriteVmView) == 0x2A4) ? 1 : -1];
typedef char AnmSetSpriteVmMatrix1At130[
    (offsetof(AnmSetSpriteVmView, matrix1) == 0x130) ? 1 : -1];
typedef char AnmSetSpriteVmMatrix2At170[
    (offsetof(AnmSetSpriteVmView, matrix2) == 0x170) ? 1 : -1];
typedef char AnmSetSpriteVmMatrix3At1B0[
    (offsetof(AnmSetSpriteVmView, matrix3) == 0x1B0) ? 1 : -1];
typedef char AnmSetSpriteVmAnmFileAt204[
    (offsetof(AnmSetSpriteVmView, anmFile204) == 0x204) ? 1 : -1];
typedef char AnmSetSpriteVmActiveSpriteAt214[
    (offsetof(AnmSetSpriteVmView, activeSpriteIndex214) == 0x214) ? 1 : -1];
typedef char AnmSetSpriteVmLoadedSpriteAt224[
    (offsetof(AnmSetSpriteVmView, loadedSprite224) == 0x224) ? 1 : -1];

struct AnmSetSpriteLoadedView
{
    unsigned char unknown00[4];
    void *rawData04;
    unsigned char unknown08[4];
    AnmSetSpriteLoadedSpriteView *sprites0C;
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
    AnmSetSpriteVmView *target =
        reinterpret_cast<AnmSetSpriteVmView *>(vm);

    if (self->rawData04 == NULL || self->numberEntriesToBeLoaded18 != 0)
        return -1;

    target->activeSpriteIndex214 = (short)spriteIndex;
    target->anmFile204 = this;
    target->loadedSprite224 = &self->sprites0C[spriteIndex];
    target->spriteSize28.x = target->loadedSprite224->spriteWidth34;
    target->spriteSize28.y = target->loadedSprite224->spriteHeight30;

    target->matrix1.SetIdentity();
    target->matrix3.SetIdentity();

    target->matrix1.m[0][0] = target->spriteSize28.x / 256.0f;
    target->matrix1.m[1][1] = target->spriteSize28.y / 256.0f;
    target->matrix3.m[0][0] =
        target->loadedSprite224->scaleX38 /
        target->loadedSprite224->textureWidth1C * target->spriteSize28.x;
    target->matrix3.m[1][1] =
        target->loadedSprite224->scaleY3C /
        target->loadedSprite224->textureHeight18 * target->spriteSize28.y;
    target->matrix2 = target->matrix1;
    return 0;
}
