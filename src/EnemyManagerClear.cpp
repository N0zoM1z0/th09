#include "AsciiManager.hpp"
#include "EclRunRemote.inl"
#include "EclRunState.inl"

#include <stddef.h>

namespace
{
struct EnemyManagerClearLayout
{
    unsigned char unknown0000[0x31C];
    int sideIndex31C;
    unsigned char unknown0320[0x5758 - 0x320];
    Th09EclRunState::EnemyStateView enemies5758[128];
};

typedef char EnemyManagerClearSideAt31C[
    (offsetof(EnemyManagerClearLayout, sideIndex31C) == 0x31C) ? 1 : -1];
typedef char EnemyManagerClearEnemiesAt5758[
    (offsetof(EnemyManagerClearLayout, enemies5758) == 0x5758) ? 1 : -1];
}

extern AsciiManager g_AsciiManager;

int Th09EclRunRemote::ManagerClearView::KillAllNonBossEnemies(
    int transitionValue, int startingValue)
{
    EnemyManagerClearLayout *manager =
        reinterpret_cast<EnemyManagerClearLayout *>(this);
    int totalValue = startingValue;
    int popupValue = 2000;
    Th09EclRunState::EnemyStateView *enemy = manager->enemies5758;

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
            enemy->worldPosition2DD4 =
                enemy->position2D74 + enemy->positionOffset2D80;

            g_AsciiManager.CreateScorePopup(
                manager->sideIndex31C,
                &enemy->worldPosition2DD4,
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
                    Th09EclRunState::TrailSampleView *sample =
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
            reinterpret_cast<Th09EclRunState::ManagerStateView *>(this)
                ->InitializeSubroutine(
                    &enemy->mainContext07F4,
                    enemy->deathCallbackSubId2D2E);
            enemy->deathCallbackSubId2D2E = -1;
        }
    }

    return totalValue;
}
