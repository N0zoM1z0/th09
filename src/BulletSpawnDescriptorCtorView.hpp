#pragma once

#include <stddef.h>

// The independently exact 0x0040D500 constructor owns this 0x214-byte carrier.
// Consumers project the established BulletSpawnDescriptor field layout over it.
struct BulletSpawnDescriptorCtorView
{
    unsigned char unknown000[0x204];
    int transformSound204;
    unsigned char unknown208[0x0C];

    BulletSpawnDescriptorCtorView();
};

typedef char BulletSpawnDescriptorCtorViewSizeIs214[
    (sizeof(BulletSpawnDescriptorCtorView) == 0x214) ? 1 : -1];
typedef char BulletSpawnDescriptorCtorSoundAt204[
    (offsetof(BulletSpawnDescriptorCtorView, transformSound204) == 0x204) ? 1 : -1];
