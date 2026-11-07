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
    unsigned char unknown000008[0x18 - 0x08];
    int flushesThisFrame;
    Float2 screenShakeOffset;
    // The source-facing 0x12024 cached world matrix and the ANM rendering
    // flags are slices of this same physical manager allocation. The original
    // byte cover is retained for independently unknown fields.
    union
    {
        unsigned char unknown000024[0x12880 - 0x24];
        struct
        {
            unsigned char unknown000024to12024[0x12024 - 0x24];
            D3DMATRIX cachedWorldMatrix12024;
            unsigned char unknown12064to12880[0x12880 - 0x12064];
        };
    };
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
     offsetof(AnmManagerRenderCacheView, flushesThisFrame) == 0x18 &&
     offsetof(AnmManagerRenderCacheView, screenShakeOffset) == 0x1C &&
     offsetof(AnmManagerRenderCacheView, cachedWorldMatrix12024) == 0x12024 &&
     offsetof(AnmManagerRenderCacheView, currentTexture) == 0x12880 &&
     offsetof(AnmManagerRenderCacheView, currentBlendMode) == 0x12884 &&
     offsetof(AnmManagerRenderCacheView, currentColorOp) == 0x12885 &&
     offsetof(AnmManagerRenderCacheView, currentVertexShader) == 0x12886 &&
     offsetof(AnmManagerRenderCacheView, disableZWrite) == 0x12887 &&
     offsetof(AnmManagerRenderCacheView, cameraMode) == 0x12888 &&
     sizeof(AnmManagerRenderCacheView) == 0x1288C) ? 1 : -1];
