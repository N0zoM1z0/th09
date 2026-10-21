#pragma once

#include <stddef.h>
#include "BulletManager.hpp"

// The independently exact 0x0040D500 constructor initializes this 0x214-byte
// carrier. Its offset-zero payload is the real Etama descriptor object;
// the carrier name and original C++ type spelling remain provisional.
struct BulletSpawnDescriptorCtorView
{
    BulletSpawnDescriptor descriptor;

    BulletSpawnDescriptorCtorView();
};

typedef char BulletSpawnDescriptorCtorViewSizeIs214[
    (sizeof(BulletSpawnDescriptorCtorView) == 0x214) ? 1 : -1];
typedef char BulletSpawnDescriptorCtorPayloadAt0[
    (offsetof(BulletSpawnDescriptorCtorView, descriptor) == 0) ? 1 : -1];
typedef char BulletSpawnDescriptorCtorSoundAt204[
    (offsetof(BulletSpawnDescriptorCtorView, descriptor) +
     offsetof(BulletSpawnDescriptor, transformSound) == 0x204) ? 1 : -1];
