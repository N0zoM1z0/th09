#include "AnmManager.hpp"
#include "AsciiManager.hpp"
#include "GameConfiguration.hpp"
#include "GameManagerMode.hpp"
#include "PlayStats.hpp"

#include <stddef.h>

struct FrontResultPlayerView
{
    unsigned char unknown00000[0xA8];
    int valueA8;
    unsigned char unknown000AC[0xB0 - 0xAC];
    int valueB0;
    int valueB4;
    int valueB8;
    unsigned char unknown000BC[0x30418 - 0xBC];
    int value30418;
};

struct FrontResultSideRuntimeView
{
    float value00;
    unsigned char unknown04[0x10 - 0x04];
    int value10;
};

struct FrontResultSideView
{
    unsigned char unknown00[0x04];
    FrontResultPlayerView *player04;
    unsigned char unknown08[0x1C - 0x08];
    FrontResultSideRuntimeView *runtime1C;
    int shotType20;
    unsigned char unknown24[0x38 - 0x24];

    void AddScore(int score);
};

struct FrontResultGameManagerLayout
{
    FrontResultSideView sides[2];
    unsigned char unknown070[0xFC - 0x70];
    int field0FC;
};

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

extern GameConfiguration *g_GameConfiguration;

static __inline FrontResultGameManagerLayout *FrontResultGameManager()
{
    return reinterpret_cast<FrontResultGameManagerLayout *>(&g_GameManager);
}

void FrontResultBonusView::InitializeResultBonus()
{
    this->frontAnm08->ExecuteAnmIdx(&this->resultVms11154[0], 63);
    this->frontAnm08->ExecuteAnmIdx(&this->resultVms11154[3], 66);

    int side = this->side10964;
    this->drawCounter10960 = 0;
    this->updateState1095C = 2;

    this->bonus11E88[0] =
        100000 * FrontResultGameManager()->sides[side].player04->valueA8;

    {
        FrontResultPlayerView *player =
            FrontResultGameManager()->sides[side].player04;
        if (player->value30418 < 1000)
            this->bonus11E88[1] = 10000 * player->value30418;
        else
            this->bonus11E88[1] = 9999990;
    }

    if (FrontResultGameManager()->sides[side].player04->valueB0 > 66)
        this->bonus11E88[2] = 9999990;
    else
        this->bonus11E88[2] =
            150000 * FrontResultGameManager()->sides[side].player04->valueB0;

    if (FrontResultGameManager()->sides[side].player04->valueB4 > 33)
        this->bonus11E88[3] = 9999990;
    else
        this->bonus11E88[3] =
            300000 * FrontResultGameManager()->sides[side].player04->valueB4;

    if (FrontResultGameManager()->sides[side].player04->valueB8 > 33)
        this->bonus11E88[4] = 9999990;
    else
        this->bonus11E88[4] =
            300000 * FrontResultGameManager()->sides[side].player04->valueB8;

    if (FrontResultGameManager()->field0FC == 8)
    {
        if (FrontResultGameManager()->sides[side].runtime1C->value00 < 10.0f)
        {
            this->bonus11E88[5] =
                static_cast<int>(
                    FrontResultGameManager()->sides[side].runtime1C->value00) *
                10000000;
        }
        else
        {
            this->bonus11E88[5] = 99999990;
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
            FrontResultGameManager()->sides[1].shotType20);

        if (FrontResultGameManager()->field0FC == 8)
        {
            this->frontAnm08->ExecuteAnmIdx(&this->resultVms11154[1], 64);
            FrontResultGameManager()->sides[0].runtime1C->value10 = 9999;
        }

        if (g_GameConfiguration->valueAC != 0 &&
            g_GameManager.IsGameMode0())
        {
            this->frontAnm08->ExecuteAnmIdx(&this->resultVms11154[2], 65);
            this->bonus11E88[6] = 0;
        }
    }

    FrontResultGameManager()->sides[this->side10964].AddScore(this->bonus11E88[6]);
}
