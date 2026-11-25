#include "PlayerShotInitializeView.hpp"
#include "EffectManager.hpp"

struct EffectFixedSlotRecordView;
struct EffectFixedSlotManagerView
{
    unsigned char unknown00[0x30];
    Effect *effects30;
    EffectFixedSlotRecordView *GetFixedSlotEffect(int slot);
};

struct PlayerShotSpawn6DescriptorView;
struct PlayerShotSpawn6ShotView;
struct PlayerShotSpawn6PlayerView;

int __fastcall PlayerShotSpawnCallbackType6(
    PlayerShotSpawn6PlayerView *player,
    PlayerShotSpawn6ShotView *shot,
    int value,
    PlayerShotSpawn6DescriptorView *entry)
{
    PlayerShtDescriptorView *nativeEntry =
        reinterpret_cast<PlayerShtDescriptorView *>(entry);
    if (value == nativeEntry->fireInterval)
    {
        PlayerShotCtorView *nativeShot =
            reinterpret_cast<PlayerShotCtorView *>(shot);
        reinterpret_cast<PlayerLifecycleView *>(player)->InitializeShot(
            nativeShot, nativeEntry);
        EffectFixedSlotManagerView *effectManager =
            reinterpret_cast<EffectFixedSlotManagerView *>(
                reinterpret_cast<PlayerLifecycleView *>(player)
                    ->sideState->effectManager0C);
        reinterpret_cast<EffectFloat3 &>(nativeShot->position2A4) =
            reinterpret_cast<Effect *>(
                effectManager->GetFixedSlotEffect(5))->position;
        nativeShot->angle450 = -1.5707964f;
        nativeShot->velocity43C.x = 0.0f;
        nativeShot->velocity43C.y = -nativeEntry->speed18;
        return 1;
    }
    return 0;
}
