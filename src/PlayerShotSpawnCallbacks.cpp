#include "PlayerShotInitializeView.hpp"
#include "EffectManager.hpp"
struct PlayerShotSpawnPlayerView;
struct PlayerShotSpawnShotView;
struct PlayerShotSpawnDescriptorView;

int __fastcall PlayerShotSpawnCallbackType2(
    PlayerShotSpawnPlayerView *player,
    PlayerShotSpawnShotView *shot,
    int value,
    PlayerShotSpawnDescriptorView *entry)
{
    PlayerShtDescriptorView *nativeEntry =
        reinterpret_cast<PlayerShtDescriptorView *>(entry);
    if (value == nativeEntry->fireInterval)
    {
        PlayerShotCtorView *nativeShot =
            reinterpret_cast<PlayerShotCtorView *>(shot);
        reinterpret_cast<PlayerLifecycleView *>(player)->InitializeShot(
            nativeShot, nativeEntry);
        nativeShot->velocity43C.x = 0.0f;
        nativeShot->velocity43C.y = 0.0f;
        nativeShot->angle450 = nativeEntry->angle14;
        return 1;
    }

    return 0;
}

int __fastcall PlayerShotSpawnCallbackType4(
    PlayerShotSpawnPlayerView *player,
    PlayerShotSpawnShotView *,
    int value,
    PlayerShotSpawnDescriptorView *entry)
{
    PlayerLifecycleView *nativePlayer =
        reinterpret_cast<PlayerLifecycleView *>(player);
    if (value == reinterpret_cast<PlayerShtDescriptorView *>(entry)->fireInterval)
    {
        nativePlayer->sideState->effectManager0C->SpawnEffectInFixedSlot(
            TH09_EFFECT_PLAYER_SHOT_SPAWN,
            reinterpret_cast<const EffectFloat3 *>(&nativePlayer->position1B88),
            5,
            static_cast<unsigned int>(-1));
    }
    return 0;
}

int __fastcall PlayerShotSpawnCallbackType5(
    PlayerShotSpawnPlayerView *player,
    PlayerShotSpawnShotView *shot,
    int value,
    PlayerShotSpawnDescriptorView *entry)
{
    PlayerLifecycleView *nativePlayer =
        reinterpret_cast<PlayerLifecycleView *>(player);
    EffectFixedSlotManagerView *effectManager =
        reinterpret_cast<EffectFixedSlotManagerView *>(
            nativePlayer->sideState->effectManager0C);
    Effect *effect = reinterpret_cast<Effect *>(
        effectManager->GetFixedSlotEffect(5));

    if (effect->active == 0 &&
        value == reinterpret_cast<PlayerShtDescriptorView *>(entry)->fireInterval)
    {
        nativePlayer->InitializeShot(
            reinterpret_cast<PlayerShotCtorView *>(shot),
            reinterpret_cast<PlayerShtDescriptorView *>(entry));
        return 1;
    }

    return 0;
}
