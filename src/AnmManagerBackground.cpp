#include "AnmManager.hpp"

#include <stddef.h>

struct AnmManagerBackgroundView
{
    unsigned int mixColor;
    int useMixColor;
    unsigned char unknown008[0x12880 - 0x008];
    void *currentTexture;
    unsigned char unknown12884[0x12888 - 0x12884];
    unsigned char cameraMode;
};

typedef char AnmBackgroundCameraModeAt12888[
    (offsetof(AnmManagerBackgroundView, cameraMode) == 0x12888) ? 1 : -1];

static __inline AnmManagerBackgroundView *BackgroundManager(AnmManager *manager)
{
    return reinterpret_cast<AnmManagerBackgroundView *>(manager);
}

void AnmManager::SetCameraMode(int mode)
{
    BackgroundManager(this)->cameraMode = (unsigned char)mode;
}

void AnmManager::SetMixColorDefault()
{
    BackgroundManager(this)->useMixColor = 0;
    BackgroundManager(this)->mixColor = 0x80808080;
}

void AnmManager::SetMixColor(unsigned long color)
{
    BackgroundManager(this)->useMixColor = 1;
    BackgroundManager(this)->mixColor = color;
}

void AnmManager::Draw2DAndFlush(AnmVm *vm)
{
    this->Draw2D(vm);
    this->FlushVertexBuffer();
}

AnmLoaded *AnmManager::GetAnm(int anmIdx)
{
    return reinterpret_cast<AnmLoaded *>(
        reinterpret_cast<unsigned char *>(this) + 0x24 + anmIdx * 0x120);
}
