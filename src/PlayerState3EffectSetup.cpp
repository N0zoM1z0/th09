#include "PlayerLifecycleView.hpp"
#include "EffectManager.hpp"
#include "ZunTimer.hpp"

// PlayerLifecycleView now includes the genuine collision-region producer
// declaration. Reuse its exact source-facing methods and 0xB0BC/+0xB100
// layout rather than redefining the same C++ classes in this source file.
typedef char PlayerState3NativeRegionProducer[
    (offsetof(PlayerLifecycleView, collisionRegionB0BC) ==
         offsetof(PlayerCollisionRegionCreateView, embeddedB0BC) &&
     offsetof(PlayerLifecycleView, activeB100) ==
         offsetof(PlayerCollisionRegionCreateView, activeB100)) ? 1 : -1];

Effect *PlayerLifecycleView::ConfigureState3Effect13()
{
    reinterpret_cast<PlayerCollisionRegionCreateView *>(this)->CreateCircleType1(
        reinterpret_cast<PlayerRegionPointView *>(&position1B88),
        0.0f, 4.0f, 0x30, 0);
    reinterpret_cast<PlayerCollisionRegionCreateView *>(this)->CreateCircleType4(
        reinterpret_cast<PlayerRegionPointView *>(&position1B88),
        0.0f, 4.0f, 2, 0x30, 0);
    SetUpdateState(3);
    timer303C8 = 0x30;
    return sideState->effectManager0C->SpawnEffect(
        13, reinterpret_cast<EffectFloat3 *>(&position1B88), 1, 0xFFFFFFFF);
}

Effect *PlayerLifecycleView::ConfigureState3Effect14()
{
    reinterpret_cast<PlayerCollisionRegionCreateView *>(this)->CreateCircleType1(
        reinterpret_cast<PlayerRegionPointView *>(&position1B88),
        0.0f, 4.0f, 0x40, 0);
    reinterpret_cast<PlayerCollisionRegionCreateView *>(this)->CreateCircleType4(
        reinterpret_cast<PlayerRegionPointView *>(&position1B88),
        0.0f, 4.0f, 2, 0x40, 0);
    SetUpdateState(3);
    timer303C8 = 0x40;
    return sideState->effectManager0C->SpawnEffect(
        14, reinterpret_cast<EffectFloat3 *>(&position1B88), 1, 0xFFFFFFFF);
}

Effect *PlayerLifecycleView::ConfigureState3Effect15()
{
    reinterpret_cast<PlayerCollisionRegionCreateView *>(this)->CreateCircleType1(
        reinterpret_cast<PlayerRegionPointView *>(&position1B88),
        0.0f, 3.5f, 0x80, 0);
    reinterpret_cast<PlayerCollisionRegionCreateView *>(this)->CreateCircleType4(
        reinterpret_cast<PlayerRegionPointView *>(&position1B88),
        0.0f, 3.5f, 2, 0x80, 0);
    SetUpdateState(3);
    timer303C8 = 0x80;
    return sideState->effectManager0C->SpawnEffect(
        15, reinterpret_cast<EffectFloat3 *>(&position1B88), 1, 0xFFFFFFFF);
}
