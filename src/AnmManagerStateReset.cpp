#include "AnmManager.hpp"
#include "AnmManagerRenderCacheView.hpp"

#include <stddef.h>

// The ANM debug counters are native members at +0x0C..+0x18; the same
// allocation's +0x12888 camera flag belongs to the shared render cache.
typedef char AnmManagerNativeFrameDebugAndCamera[
    (offsetof(AnmManager, scriptsStartedThisFrame0C) == 0x0C &&
     offsetof(AnmManager, scriptsExecutedThisFrame10) == 0x10 &&
     offsetof(AnmManager, renderStateChangesThisFrame14) == 0x14 &&
     offsetof(AnmManager, flushesThisFrame18) == 0x18 &&
     offsetof(AnmManagerRenderCacheView, cameraMode) == 0x12888) ? 1 : -1];

void AnmManager::ResetFrameDebugInfo()
{
    // Preserve the original store ordering: +0x10, +0x14, +0x0C, +0x18.
    scriptsExecutedThisFrame10 = 0;
    renderStateChangesThisFrame14 = 0;
    scriptsStartedThisFrame0C = 0;
    flushesThisFrame18 = 0;
}

void AnmManager::ClearCameraSettings()
{
    reinterpret_cast<AnmManagerRenderCacheView *>(this)->cameraMode = 0xff;
}
