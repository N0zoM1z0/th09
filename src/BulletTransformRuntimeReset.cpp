#include "BulletManager.hpp"
#include <stddef.h>

struct BulletTransformResetView
{
    unsigned char unknown000[0xDB4];
    unsigned int activeTransformFlagsDB4;
    unsigned int transformFlagsDB8;
    unsigned char unknownDBC[0x18];
    int transformIndexDD4;
    BulletTransformRecord transformsDD8[18];

    void ResetTransformRuntime();
};

typedef char BulletTransformResetActiveAtDB4[
    (offsetof(BulletTransformResetView, activeTransformFlagsDB4) == 0xDB4) ? 1 : -1];
typedef char BulletTransformResetFlagsAtDB8[
    (offsetof(BulletTransformResetView, transformFlagsDB8) == 0xDB8) ? 1 : -1];
typedef char BulletTransformResetIndexAtDD4[
    (offsetof(BulletTransformResetView, transformIndexDD4) == 0xDD4) ? 1 : -1];
typedef char BulletTransformResetRecordsAtDD8[
    (offsetof(BulletTransformResetView, transformsDD8) == 0xDD8) ? 1 : -1];

void BulletTransformResetView::ResetTransformRuntime()
{
    transformIndexDD4 = 0;
    transformFlagsDB8 = 0;
    activeTransformFlagsDB4 = 0;
    for (int i = 0; i < 18; ++i)
        transformsDD8[i].kind = 0;
}
