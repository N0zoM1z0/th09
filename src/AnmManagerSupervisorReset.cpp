#include "AnmManager.hpp"

#include <stddef.h>

struct AnmManagerSupervisorResetView
{
    unsigned char unknown000[0x1C];
    float screenShakeX;
    float screenShakeY;
};

typedef char AnmManagerSupervisorResetShakeAt1C[
    (offsetof(AnmManagerSupervisorResetView, screenShakeX) == 0x1C) ? 1 : -1];

void AnmManager::ResetForSupervisorFrame()
{
    this->ClearVertexShader();
    this->ClearSprite();
    this->ClearTexture();
    this->ClearColorOp();
    this->ClearBlendMode();
    this->ClearZWrite();
    this->ResetFrameDebugInfo();
    this->ClearCameraSettings();
    this->SetMixColorDefault();

    AnmManagerSupervisorResetView *manager =
        reinterpret_cast<AnmManagerSupervisorResetView *>(this);
    manager->screenShakeY = 0.0f;
    manager->screenShakeX = 0.0f;
}
