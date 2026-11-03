#include "AsciiManager.hpp"
#include "EclRunRemote.inl"
#include "EclRunState.inl"

#include <stddef.h>

extern AsciiManager g_AsciiManager;

// The ABI owner is still provisional, but both manager and its 129 Enemy
// records use the independently established native allocations.  A mismatch
// between ECL and native field coordinates must fail this source build.
typedef char EnemyClearNativeLayout[
    (sizeof(EnemyManagerView) == sizeof(EnemyManagerCoreView) &&
     sizeof(EnemyView) == sizeof(EnemyCoreView) &&
     offsetof(EnemyManagerView, enemies5758) ==
         offsetof(EnemyManagerCoreView, enemies5758) &&
     offsetof(EnemyManagerView, sideIndex31C) ==
         offsetof(EnemyManagerCoreView, sideIndex31C) &&
     sizeof(((EnemyManagerView *)0)->enemies5758) ==
         sizeof(((EnemyManagerCoreView *)0)->enemies5758) &&
     offsetof(EnemyView, trailSamples33E8) ==
         offsetof(EnemyCoreView, trailSamples33E8) &&
     offsetof(EnemyView, deathCallbackSubId2D2E) ==
         offsetof(EnemyCoreView, deathCallbackSubId2D2E)) ? 1 : -1];

int EnemyManagerCoreView::KillAllNonBossEnemies(
    int transitionValue, int startingValue)
{
    EnemyManagerView *manager = reinterpret_cast<EnemyManagerView *>(this);
    int totalValue = startingValue;
    int popupValue = 2000;
    EnemyView *enemy = manager->enemies5758;

    for (int i = 0; i < 128; ++i, ++enemy)
    {
        if ((enemy->primaryFlags337C & 1) == 0 ||
            (enemy->primaryFlags337C & 2) != 0 ||
            (enemy->secondaryFlags3380 & 8) != 0)
        {
            continue;
        }

        enemy->life2E48 = 0;

        if ((enemy->primaryFlags337C &
             Th09EclRunState::ENEMY_STATE_SPECIAL_INTERACTION) != 0)
        {
            reinterpret_cast<Float3 &>(enemy->worldPosition2DD4) =
                reinterpret_cast<const Float3 &>(enemy->position2D74) +
                reinterpret_cast<const Float3 &>(enemy->positionOffset2D80);

            g_AsciiManager.CreateScorePopup(
                manager->sideIndex31C,
                reinterpret_cast<Float3 *>(&enemy->worldPosition2DD4),
                popupValue,
                popupValue >= transitionValue ? -256 : -1);

            totalValue += popupValue;
            popupValue += 30;
            if (popupValue > transitionValue)
                popupValue = transitionValue;

            if (enemy->trailFlags53A0 != 0)
            {
                int sampleIndex = 0;
                if (sampleIndex < enemy->trailHistoryLength53A2)
                {
                    EnemyTrailSampleView *sample =
                        enemy->trailSamples33E8;
                    do
                    {
                        g_AsciiManager.CreateScorePopup(
                            manager->sideIndex31C,
                            reinterpret_cast<Float3 *>(&sample->position00),
                            popupValue,
                            popupValue >= transitionValue ? -256 : -1);

                        totalValue += popupValue;
                        popupValue += 30;
                        if (popupValue > transitionValue)
                            popupValue = transitionValue;

                        sampleIndex += 6;
                        sample += 6;
                    }
                    while (sampleIndex < enemy->trailHistoryLength53A2);
                }
            }
        }

        if (enemy->deathCallbackSubId2D2E >= 0)
        {
            manager->primaryEclManager000.InitializeSubroutine(
                    &enemy->mainContext07F4,
                    enemy->deathCallbackSubId2D2E);
            enemy->deathCallbackSubId2D2E = -1;
        }
    }

    return totalValue;
}
