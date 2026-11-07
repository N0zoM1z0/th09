#pragma once

#include "AnmManagerRenderCacheView.hpp"
#include "VertexDiffuseXyzrhw.hpp"

#include <stddef.h>

// The 0x1288C..0x128E8 software-vertex/cache tail is the same native ANM
// allocation in primitive, projection, 3D and buffer-flush exact owners.
// Projection only reads the XYZ prefix; primitive draw also reads the last
// UV fields. C++03's distinct non-POD vertex classes must not form a union.
struct VertexTex1Xyz
{
    Float3 pos;
    Float2 textureUV;
};

struct AnmManagerQuadCacheView : AnmManagerRenderCacheView
{
    AnmLoadedSprite *currentSprite;
    IDirect3DVertexBuffer8 *quadVertexBuffer;
    VertexTex1Xyz untexturedVector[4];
    int spritesToDraw;
};

typedef char AnmManagerQuadCacheNativeOffsets[
    (sizeof(VertexTex1Xyz) == 0x14 &&
     offsetof(AnmManagerQuadCacheView, currentSprite) == 0x1288C &&
     offsetof(AnmManagerQuadCacheView, quadVertexBuffer) == 0x12890 &&
     offsetof(AnmManagerQuadCacheView, untexturedVector) == 0x12894 &&
     offsetof(AnmManagerQuadCacheView, spritesToDraw) == 0x128E4 &&
     sizeof(AnmManagerQuadCacheView) == 0x128E8) ? 1 : -1];
