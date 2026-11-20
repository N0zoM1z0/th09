#include <stddef.h>
#include "AnmManager.hpp"
#include "PlayerCollisionRegionCreate.hpp"

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

// The Player preload leaf reads the original ANM loader's +4 file pointer;
// both owners use the same native layout.
typedef char PlayerAnmRawDataAt04[
    (offsetof(AnmLoaded, rawData04) == 0x04) ? 1 : -1];

struct PlayerAnmManagerRawView
{
    AnmLoaded *GetAnm(int anmIdx);
    void *GetAnmRawData(int anmIdx);
};

void *PlayerAnmManagerRawView::GetAnmRawData(int anmIdx)
{
    return GetAnm(anmIdx)->rawData04;
}

// This leaf operates on the same complete 0x44-byte region record that
// creation, damage and bullet collision already own. The derived view keeps
// the original leaf's decorated member name without a second opaque layout.
struct PlayerCollisionRegionRuntimeView : PlayerCollisionRegionCreateRegionView
{
    void Deactivate();
};

typedef char PlayerCollisionRegionLeafSizeIs44[
    (sizeof(PlayerCollisionRegionRuntimeView) == 0x44) ? 1 : -1];
typedef char PlayerCollisionRegionActiveAt3C[
    (offsetof(PlayerCollisionRegionCreateRegionView, active3C) == 0x3C) ? 1 : -1];

void PlayerCollisionRegionRuntimeView::Deactivate()
{
    this->active3C = 0;
}
