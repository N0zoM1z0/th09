#include "AnmManager.hpp"
#include "AsciiManager.hpp"
#include "GameConfiguration.hpp"
#include "GameManagerRuntimeView.hpp"
#include "PlayerLifecycleView.hpp"
#include "PlayStats.hpp"

#include <stddef.h>

struct FrontResultBonusView
{
    unsigned char unknown00000[0x08];
    AnmLoaded *frontAnm08;
    unsigned char unknown0000C[0x1095C - 0x0C];
    int updateState1095C;
    int drawCounter10960;
    int side10964;
    unsigned char unknown10968[0x11154 - 0x10968];
    AnmVm resultVms11154[5];
    int bonus11E88[7];

    void InitializeResultBonus();
};

typedef char FrontResultVmAt11154[
    (offsetof(FrontResultBonusView, resultVms11154) == 0x11154) ? 1 : -1];
typedef char FrontResultBonusAt11E88[
    (offsetof(FrontResultBonusView, bonus11E88) == 0x11E88) ? 1 : -1];
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

void FrontResultBonusView::InitializeResultBonus()
{
    this->frontAnm08->ExecuteAnmIdx(&this->resultVms11154[0], 63);
    this->frontAnm08->ExecuteAnmIdx(&this->resultVms11154[3], 66);

    int side = this->side10964;
    this->drawCounter10960 = 0;
    this->updateState1095C = 2;

    this->bonus11E88[0] =
        100000 * g_GameManager.sides[side].player04->header24.state84;

    {
        PlayerLifecycleView *player =
            g_GameManager.sides[side].player04;
        if (player->ownerState30410.maxState08 < 1000)
            this->bonus11E88[1] =
                10000 *
                g_GameManager.sides[side].player04->ownerState30410.maxState08;
        else
            this->bonus11E88[1] = 9999990;
    }

    if (g_GameManager.sides[side].player04->header24.state8C > 66)
        this->bonus11E88[2] = 9999990;
    else
        this->bonus11E88[2] =
            150000 * g_GameManager.sides[side].player04->header24.state8C;

    if (g_GameManager.sides[side].player04->header24.state90 > 33)
        this->bonus11E88[3] = 9999990;
    else
        this->bonus11E88[3] =
            300000 * g_GameManager.sides[side].player04->header24.state90;

    if (g_GameManager.sides[side].player04->header24.state94 > 33)
        this->bonus11E88[4] = 9999990;
    else
        this->bonus11E88[4] =
            300000 * g_GameManager.sides[side].player04->header24.state94;

    if (g_GameManager.field0FC == 8)
    {
        if (g_GameManager.sides[side].state->value00 >= 10.0f)
        {
            this->bonus11E88[5] = 99999990;
        }
        else
        {
            this->bonus11E88[5] =
                static_cast<int>(
                    g_GameManager.sides[side].state->value00) *
                10000000;
        }
    }
    else
    {
        this->bonus11E88[5] = 0;
    }

    int totalBonus = this->bonus11E88[2];
    totalBonus += this->bonus11E88[5];
    totalBonus += this->bonus11E88[3];
    totalBonus += this->bonus11E88[4];
    totalBonus += this->bonus11E88[1];
    totalBonus += this->bonus11E88[0];
    this->bonus11E88[6] = totalBonus;

    if ((g_GameManager.IsGameMode0() || g_GameManager.IsGameMode1()) &&
        side == 0)
    {
        g_PlayStatsRecord.SetCharacterUnlocked0(
            g_GameManager.sides[1].shotType20);

        if (g_GameManager.field0FC == 8)
        {
            this->frontAnm08->ExecuteAnmIdx(&this->resultVms11154[1], 64);
            g_GameManager.sides[0].state->phase10 = 9999;
        }

        if (g_GameConfiguration->valueAC > 0 &&
            g_GameManager.IsGameMode0())
        {
            this->frontAnm08->ExecuteAnmIdx(&this->resultVms11154[2], 65);
            this->bonus11E88[6] = 0;
        }
    }

    g_GameManager.sides[this->side10964].AddScore(this->bonus11E88[6]);
}
