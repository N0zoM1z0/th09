#include <stddef.h>

struct PlayerAnmVmBlendModeView
{
    unsigned char unknown000[0x1F8];
    unsigned int flags1F8;

    void SetBlendModeAdditive();
    void SetBlendModeNormal();
};

typedef char PlayerBlendFlagsAt1F8[
    (offsetof(PlayerAnmVmBlendModeView, flags1F8) == 0x1F8) ? 1 : -1];

void PlayerAnmVmBlendModeView::SetBlendModeAdditive()
{
    flags1F8 = (flags1F8 & ~0x20u) | 0x10u;
}

void PlayerAnmVmBlendModeView::SetBlendModeNormal()
{
    flags1F8 &= ~0x30u;
}

class AnmLoaded;

struct PlayerAnmLoadedRawView
{
    unsigned char unknown00[4];
    void *rawData04;
};

typedef char PlayerAnmRawDataAt04[
    (offsetof(PlayerAnmLoadedRawView, rawData04) == 0x04) ? 1 : -1];

struct PlayerAnmManagerRawView
{
    AnmLoaded *GetAnm(int anmIdx);
    void *GetAnmRawData(int anmIdx);
};

void *PlayerAnmManagerRawView::GetAnmRawData(int anmIdx)
{
    return reinterpret_cast<PlayerAnmLoadedRawView *>(GetAnm(anmIdx))->rawData04;
}

struct PlayerCollisionRegionRuntimeView
{
    unsigned char unknown00[0x3C];
    unsigned char active3C;
    unsigned char unknown3D[3];
    int delay40;

    void Deactivate();
};

typedef char PlayerCollisionRegionLeafSizeIs44[
    (sizeof(PlayerCollisionRegionRuntimeView) == 0x44) ? 1 : -1];
typedef char PlayerCollisionRegionActiveAt3C[
    (offsetof(PlayerCollisionRegionRuntimeView, active3C) == 0x3C) ? 1 : -1];

void PlayerCollisionRegionRuntimeView::Deactivate()
{
    active3C = 0;
}
