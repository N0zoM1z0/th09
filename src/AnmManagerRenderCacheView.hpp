#pragma once

#include "AnmManager.hpp"
#include "Float3.hpp"
#include <stddef.h>
#include <d3d8.h>

// Target-bound shared projection of the one native ANM manager render cache.
// The inherited views cover the same real allocation, not extra objects.
struct AnmManagerRenderCacheView
{
    unsigned int mixColor;
    int useMixColor;
    unsigned char unknown000008[0x1C - 0x08];
    Float2 screenShakeOffset;
    unsigned char unknown000024[0x12880 - 0x24];
    IDirect3DTexture8 *currentTexture;
    unsigned char currentBlendMode;
    unsigned char currentColorOp;
    unsigned char currentVertexShader;
    unsigned char disableZWrite;
    unsigned char cameraMode;
    unsigned char unknown12889[0x1288C - 0x12889];
};

typedef char AnmManagerNativeCacheFields[
    (offsetof(AnmManagerRenderCacheView, mixColor) == 0x000 &&
     offsetof(AnmManagerRenderCacheView, useMixColor) == 0x004 &&
     offsetof(AnmManagerRenderCacheView, screenShakeOffset) == 0x1C &&
     offsetof(AnmManagerRenderCacheView, currentTexture) == 0x12880 &&
     offsetof(AnmManagerRenderCacheView, currentBlendMode) == 0x12884 &&
     offsetof(AnmManagerRenderCacheView, currentColorOp) == 0x12885 &&
     offsetof(AnmManagerRenderCacheView, currentVertexShader) == 0x12886 &&
     offsetof(AnmManagerRenderCacheView, disableZWrite) == 0x12887 &&
     offsetof(AnmManagerRenderCacheView, cameraMode) == 0x12888 &&
     sizeof(AnmManagerRenderCacheView) == 0x1288C) ? 1 : -1];
