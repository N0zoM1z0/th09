#include "AnmManager.hpp"
#include "AnmManagerQuadCacheView.hpp"

#include <stddef.h>
#include <windows.h>
#include <d3d8.h>

// The exact 31-byte ReleaseVertexBuffer leaf consumes the same native
// +0x12890 D3D vertex buffer as ANM primitive setup and 3D rendering.
typedef char AnmManagerVertexBufferAt12890[
    (offsetof(AnmManagerQuadCacheView, quadVertexBuffer) == 0x12890) ? 1 : -1];

void AnmManager::ReleaseVertexBuffer()
{
    AnmManagerQuadCacheView *manager =
        reinterpret_cast<AnmManagerQuadCacheView *>(this);
    if (manager->quadVertexBuffer != NULL)
    {
        manager->quadVertexBuffer->Release();
        manager->quadVertexBuffer = NULL;
    }
}
