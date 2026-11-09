#include "AnmManager.hpp"
#include "AsciiManager.hpp"
#include "GameConfiguration.hpp"
#include "GameManagerRuntimeView.hpp"
#include "PlayerLifecycleView.hpp"
#include "PlayStats.hpp"
#include "FrontFactoryConstructView.hpp"

#include <stddef.h>

// The result score uses the real native Player and side allocations.
typedef char FrontResultRealNativePlayerScores[
    (offsetof(PlayerLifecycleView, header24) + offsetof(PlayerHeaderStateView, state84) == 0xA8 &&
     offsetof(PlayerLifecycleView, header24) + offsetof(PlayerHeaderStateView, state8C) == 0xB0 &&
     offsetof(PlayerLifecycleView, header24) + offsetof(PlayerHeaderStateView, state90) == 0xB4 &&
     offsetof(PlayerLifecycleView, header24) + offsetof(PlayerHeaderStateView, state94) == 0xB8 &&
     offsetof(PlayerLifecycleView, ownerState30410) + offsetof(PlayerOwnerStateView, maxState08) == 0x30418 &&
     sizeof(AsciiGameManagerSideView) == 0x38 &&
     offsetof(AsciiGameManagerSideView, player04) == 0x04 &&
     offsetof(AsciiGameManagerSideView, state) == 0x1C &&
     offsetof(AsciiGameSideRuntimeView, phase10) == 0x10) ? 1 : -1];

extern GameConfiguration *g_GameConfiguration;

void FrontFactoryConstructView::InitializeResultBonus()
{
    this->frontAnm08->ExecuteAnmIdx(&this->resultVms10968[3], 63);
    this->frontAnm08->ExecuteAnmIdx(&this->resultVms10968[6], 66);

    int side = this->transitionSide10964;
    this->transitionFrame10960 = 0;
    this->transitionVmCount1095C = 2;

    this->resultValues11E88[0] =
        100000 * g_GameManager.sides[side].player04->header24.state84;

    {
        PlayerLifecycleView *player =
            g_GameManager.sides[side].player04;
        if (player->ownerState30410.maxState08 < 1000)
            this->resultValues11E88[1] =
                10000 *
                g_GameManager.sides[side].player04->ownerState30410.maxState08;
        else
            this->resultValues11E88[1] = 9999990;
    }

    if (g_GameManager.sides[side].player04->header24.state8C > 66)
        this->resultValues11E88[2] = 9999990;
    else
        this->resultValues11E88[2] =
            150000 * g_GameManager.sides[side].player04->header24.state8C;

    if (g_GameManager.sides[side].player04->header24.state90 > 33)
        this->resultValues11E88[3] = 9999990;
    else
        this->resultValues11E88[3] =
            300000 * g_GameManager.sides[side].player04->header24.state90;

    if (g_GameManager.sides[side].player04->header24.state94 > 33)
        this->resultValues11E88[4] = 9999990;
    else
        this->resultValues11E88[4] =
            300000 * g_GameManager.sides[side].player04->header24.state94;

    if (g_GameManager.field0FC == 8)
    {
        if (g_GameManager.sides[side].state->value00 >= 10.0f)
        {
            this->resultValues11E88[5] = 99999990;
        }
        else
        {
            this->resultValues11E88[5] =
                static_cast<int>(
                    g_GameManager.sides[side].state->value00) *
                10000000;
        }
    }
    else
    {
        this->resultValues11E88[5] = 0;
    }

    int totalBonus = this->resultValues11E88[2];
    totalBonus += this->resultValues11E88[5];
    totalBonus += this->resultValues11E88[3];
    totalBonus += this->resultValues11E88[4];
    totalBonus += this->resultValues11E88[1];
    totalBonus += this->resultValues11E88[0];
    this->resultValues11E88[6] = totalBonus;

    if ((g_GameManager.IsGameMode0() || g_GameManager.IsGameMode1()) &&
        side == 0)
    {
        g_PlayStatsRecord.SetCharacterUnlocked0(
            g_GameManager.sides[1].shotType20);

        if (g_GameManager.field0FC == 8)
        {
            this->frontAnm08->ExecuteAnmIdx(&this->resultVms10968[4], 64);
            g_GameManager.sides[0].state->phase10 = 9999;
        }

        if (g_GameConfiguration->valueAC > 0 &&
            g_GameManager.IsGameMode0())
        {
            this->frontAnm08->ExecuteAnmIdx(&this->resultVms10968[5], 65);
            this->resultValues11E88[6] = 0;
        }
    }

    g_GameManager.sides[this->transitionSide10964].AddScore(this->resultValues11E88[6]);
}
