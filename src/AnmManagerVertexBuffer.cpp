#include "AnmManager.hpp"

#include <stddef.h>
#include <windows.h>
#include <d3d8.h>

struct AnmManagerVertexBufferView
{
    unsigned char unknown000[0x12890];
    IDirect3DVertexBuffer8 *vertexBuffer;
};

typedef char AnmManagerVertexBufferAt12890[
    (offsetof(AnmManagerVertexBufferView, vertexBuffer) == 0x12890) ? 1 : -1];

void AnmManager::ReleaseVertexBuffer()
{
    AnmManagerVertexBufferView *manager =
        reinterpret_cast<AnmManagerVertexBufferView *>(this);
    if (manager->vertexBuffer != NULL)
    {
        manager->vertexBuffer->Release();
        manager->vertexBuffer = NULL;
    }
}
