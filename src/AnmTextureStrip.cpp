#include "AnmManager.hpp"

#include <stddef.h>

struct AnmStripVmView
{
    unsigned char unknown000[0x30];
    float uvScrollX30;
    float uvScrollY34;
    unsigned char unknown038[0x1F0 - 0x38];
    unsigned long color1F0;
    unsigned char unknown1F4[0x224 - 0x1F4];
    AnmLoadedSprite *loadedSprite224;
};

struct AnmStripVertex
{
    float x;
    float y;
    float z;
    float rhw;
    unsigned long diffuse;
    float u;
    float v;
};
typedef char AnmStripVertexSizeIs1C[
    (sizeof(AnmStripVertex) == 0x1C) ? 1 : -1];

int AnmManager::InitializeHorizontalTextureStrip(
    AnmVm *vmBase,
    VertexTex1DiffuseXyzrhw *verticesBase,
    int vertexCount)
{
    AnmStripVmView *vm = reinterpret_cast<AnmStripVmView *>(vmBase);
    AnmStripVertex *vertices =
        reinterpret_cast<AnmStripVertex *>(verticesBase);
    if (vertexCount < 3)
        return -1;

    float x = vm->loadedSprite224->uvEndX + vm->uvScrollX30;
    float xSpan =
        vm->loadedSprite224->uvEndX - vm->loadedSprite224->uvStartX;
    float y = vm->loadedSprite224->uvStartY + vm->uvScrollY34;
    AnmStripVertex *vertex = vertices;
    float step = xSpan / ((vertexCount + 1) / 2 - 1);
    int i = 0;
    float currentX = x;
    for (; i < vertexCount;
         i += 2, vertex += 2, currentX -= step)
    {
        vertex->u = currentX;
        vertex->v = y;
        vertex->diffuse = vm->color1F0;
        vertex->rhw = 1.0f;
    }

    y = vm->loadedSprite224->uvEndY + vm->uvScrollY34;
    vertex = vertices + 1;
    i = 1;
    currentX = x;
    for (; i < vertexCount;
         i += 2, vertex += 2, currentX -= step)
    {
        vertex->u = currentX;
        vertex->v = y;
        vertex->diffuse = vm->color1F0;
        vertex->rhw = 1.0f;
    }
    return 0;
}

int AnmManager::InitializeVerticalTextureStrip(
    AnmVm *vmBase,
    VertexTex1DiffuseXyzrhw *verticesBase,
    int vertexCount)
{
    AnmStripVmView *vm = reinterpret_cast<AnmStripVmView *>(vmBase);
    AnmStripVertex *vertices =
        reinterpret_cast<AnmStripVertex *>(verticesBase);
    if (vertexCount < 3)
        return -1;

    float y = vm->loadedSprite224->uvEndY + vm->uvScrollY34;
    float ySpan =
        vm->loadedSprite224->uvEndY - vm->loadedSprite224->uvStartY;
    float x = vm->loadedSprite224->uvStartX + vm->uvScrollX30;
    AnmStripVertex *vertex = vertices;
    float step = ySpan / ((vertexCount + 1) / 2 - 1);
    int i = 0;
    float currentY = y;
    for (; i < vertexCount;
         i += 2, vertex += 2, currentY -= step)
    {
        vertex->v = currentY;
        vertex->u = x;
        vertex->diffuse = vm->color1F0;
        vertex->rhw = 1.0f;
    }

    x = vm->loadedSprite224->uvEndX + vm->uvScrollX30;
    vertex = vertices + 1;
    i = 1;
    currentY = y;
    for (; i < vertexCount;
         i += 2, vertex += 2, currentY -= step)
    {
        vertex->v = currentY;
        vertex->u = x;
        vertex->diffuse = vm->color1F0;
        vertex->rhw = 1.0f;
    }
    return 0;
}
