#pragma once

#include <stddef.h>

// The target-bound ANM manager screen/device-state prefix is shared by
// the background setters and Title's initial texture-cache invalidation.
// Original nominal C++ type names are not established by this view.
struct AnmManagerBackgroundView
{
    unsigned int mixColor;
    int useMixColor;
    unsigned char unknown008[0x12880 - 0x008];
    void *currentTexture;
    unsigned char unknown12884[0x12888 - 0x12884];
    unsigned char cameraMode;
};

typedef char AnmBackgroundCameraModeAt12888[
    (offsetof(AnmManagerBackgroundView, cameraMode) == 0x12888) ? 1 : -1];

typedef char AnmNativeCurrentTextureAt12880[
    (offsetof(AnmManagerBackgroundView, currentTexture) == 0x12880) ? 1 : -1];
