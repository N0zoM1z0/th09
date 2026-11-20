#include "EffectManager.hpp"
#include "PlayerLifecycleView.hpp"

typedef char EffectFollowPlayerPositionOffset[
    (offsetof(PlayerLifecycleView, position1B88) == 0x1B88 &&
     sizeof(PlayerPositionView) == sizeof(EffectFloat3)) ? 1 : -1];

typedef char EffectFollowPlayerSideOwnerOffset[
    (offsetof(PlayerSideStateView, player04) == 0x04) ? 1 : -1];

int __fastcall EffectFollowPlayerUpdate(Effect *effect)
{
    PlayerSideStateView *side =
        reinterpret_cast<PlayerSideStateView *>(effect->manager->sideState);
    PlayerLifecycleView *player = side->player04;
    effect->position = reinterpret_cast<const EffectFloat3 &>(
        player->position1B88);
    return 1;
}
