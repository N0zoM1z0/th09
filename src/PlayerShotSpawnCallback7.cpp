#include "PlayerShotInitializeView.hpp"
#include "PlayerEnemyTrackingNative.hpp"
#include "PlayerForwardAngleView.hpp"
#include <math.h>


int __fastcall PlayerShotSpawnCallbackType7(
    PlayerLifecycleView *player,
    PlayerShotCtorView *shot,
    int value,
    PlayerShtDescriptorView *entry)
{
    if (value == entry->fireInterval)
    {
        PlayerEnemyTrackingNativeView &tracking = player->tracking30364;
        float angle =
            reinterpret_cast<PlayerForwardAngleView *>(player)->AngleFromPlayerToPoint(
                reinterpret_cast<PlayerForwardAngleFloat3View *>(
                    &tracking.trackedX00)) +
            entry->angle14;

        if (tracking.trackedX00 < -144.0f)
            angle = -1.5707964f;

        reinterpret_cast<PlayerLifecycleView *>(player)->InitializeShot(
            reinterpret_cast<PlayerShotCtorView *>(shot),
            reinterpret_cast<PlayerShtDescriptorView *>(entry));

        shot->velocity43C.x = cosf(angle) * entry->speed18;
        shot->velocity43C.y = sinf(angle) * entry->speed18;
        shot->angle450 = angle;
        return 1;
    }

    return 0;
}
