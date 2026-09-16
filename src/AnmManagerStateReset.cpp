#include "AnmManager.hpp"

#include <stddef.h>

struct AnmManagerStateResetView
{
    unsigned char unknown000[0x0C];
    int scriptsExecutedThisFrame;
    int renderStateChangesThisFrame;
    int scriptsStartedThisFrame;
    int flushesThisFrame;
    unsigned char unknown01C[0x12888 - 0x01C];
    unsigned char cameraMode;
};

typedef char AnmManagerResetCameraModeAt12888[
    (offsetof(AnmManagerStateResetView, cameraMode) == 0x12888) ? 1 : -1];

void AnmManager::ResetFrameDebugInfo()
{
    AnmManagerStateResetView *manager =
        reinterpret_cast<AnmManagerStateResetView *>(this);
    manager->renderStateChangesThisFrame = 0;
    manager->scriptsStartedThisFrame = 0;
    manager->scriptsExecutedThisFrame = 0;
    manager->flushesThisFrame = 0;
}

void AnmManager::ClearCameraSettings()
{
    reinterpret_cast<AnmManagerStateResetView *>(this)->cameraMode = 0xff;
}
