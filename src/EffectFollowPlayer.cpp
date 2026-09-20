#include "EffectManager.hpp"

struct EffectFollowPlayerView
{
    unsigned char unknown000[0x1B88];
    EffectFloat3 position1B88;
};

struct EffectFollowSideStateView
{
    unsigned char unknown00[0x04];
    EffectFollowPlayerView *player04;
};

int __fastcall EffectFollowPlayerUpdate(Effect *effect)
{
    EffectFollowSideStateView *side =
        reinterpret_cast<EffectFollowSideStateView *>(effect->manager->sideState);
    effect->position = side->player04->position1B88;
    return 1;
}
