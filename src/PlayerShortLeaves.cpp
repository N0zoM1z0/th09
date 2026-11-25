#include <stddef.h>
#include "AnmManager.hpp"
#include "PlayerCollisionRegionCreate.hpp"
#include "PlayerLifecycleView.hpp"

void PlayerAnmVmView::SetBlendModeAdditive()
{
    flagsWord = (flagsWord & ~0x20u) | 0x10u;
}

void PlayerAnmVmView::SetBlendModeNormal()
{
    flagsWord &= ~0x30u;
}

// The Player preload leaf reads the original ANM loader's +4 file pointer;
// both owners use the same native layout.
typedef char PlayerAnmRawDataAt04[
    (offsetof(AnmLoaded, rawData04) == 0x04) ? 1 : -1];

void *PlayerAnmManagerRawView::GetAnmRawData(int anmIdx)
{
    return GetAnm(anmIdx)->rawData04;
}

void PlayerCollisionRegionCreateRegionView::Deactivate()
{
    active3C = 0;
}
