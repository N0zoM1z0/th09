#include "AnmManager.hpp"
#include <d3d8.h>
#include <stddef.h>

struct AnmManagerReleaseSurfacesView
{
    unsigned char unknown000[0x123FC];
    IDirect3DSurface8 *surfaces123FC[32];
};

typedef char AnmReleaseSurfacesPrimaryAt123FC[
    (offsetof(AnmManagerReleaseSurfacesView, surfaces123FC) == 0x123FC) ? 1 : -1];

void AnmManager::ReleaseSurfaces()
{
    AnmManagerReleaseSurfacesView *view =
        reinterpret_cast<AnmManagerReleaseSurfacesView *>(this);
    for (int i = 0; i < 32; ++i)
    {
        if (view->surfaces123FC[i] != 0)
        {
            view->surfaces123FC[i]->Release();
            view->surfaces123FC[i] = 0;
        }
    }
}
