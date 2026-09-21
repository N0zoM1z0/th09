#include "AnmManager.hpp"
#include "CardAttack.hpp"
#include "EnemyManager.hpp"

#include <stddef.h>
#include <string.h>

struct CardAttackStartPlayerView
{
    unsigned char unknown00000[0xA0];
    int valueA0;
    int valueA4;
    unsigned char unknown0A8[0xB0 - 0xA8];
    int valueB0;
    int valueB4;
    int valueB8;
    unsigned char unknown0BC[0x303F0 - 0xBC];
    int tableValue303F0;
};

struct CardAttackStartStateView
{
    unsigned char unknown00[0x1C];
    int field1C;
    int field20;
};

struct CardAttackStartFrontAuxView
{
    AnmLoaded *sideAnm00;
    AnmLoaded *optionalAnm04;
    int unknown08;
    AnmVm vms0C[2];
    int unknown554;
};

struct CardAttackStartFrontSideView
{
    unsigned char unknown0000[0xA678];
    CardAttackStartFrontAuxView auxA678;
};

struct CardAttackStartSideView
{
    CardAttackStartStateView *state00;
    CardAttackStartPlayerView *player04;
    unsigned char unknown08[8];
    EnemyManagerView *enemyManager10;
    void *subsystem5_14;
    CardAttackStartFrontSideView *frontSide18;
    unsigned char unknown1C[0x1C];
};
typedef char CardAttackStartSideSizeIs38[
    (sizeof(CardAttackStartSideView) == 0x38) ? 1 : -1];

struct CardAttackStartValueCCView
{
    unsigned char unknown00[0x1C];
    int value1C;

    void SubtractSaturating(int amount);
};

struct CardAttackStartGameManagerView
{
    unsigned char unknown000[0xB0];
    CardAttackStartValueCCView valueCCB0;
    unsigned char unknown0D0[0x134 - 0xD0];
    union
    {
        unsigned int flags134;
        struct
        {
            unsigned int lowFlags134 : 11;
            unsigned int transitionBits134 : 2;
            unsigned int highFlags134 : 19;
        };
    };
};

typedef char CardAttackStartValueCCAtB0[
    (offsetof(CardAttackStartGameManagerView, valueCCB0) == 0xB0) ? 1 : -1];
typedef char CardAttackStartFlagsAt134[
    (offsetof(CardAttackStartGameManagerView, flags134) == 0x134) ? 1 : -1];

typedef int (*CardAttackBeginCallback)(CardAttack *cardAttack);

struct CardAttackCallbackRow
{
    CardAttackBeginCallback begin;
    CardAttackUpdateCallback update;
    CardAttackDrawCallback draw;
    CardAttackEndCallback end;
};

struct CardAttackStartSoundPlayerView
{
    void PlaySoundByIdx(int soundIndex, int pan);
};

extern CardAttackStartGameManagerView g_GameManager;
extern CardAttackStartSoundPlayerView g_SoundPlayer;
extern CardAttackCallbackRow g_CardAttackCallbackRows[];

struct CardAttackStartView
{
    int sideIndex00;
    CardAttackStartSideView *sideState04;
    CardAttackStartSideView *otherSideState08;
    ChainElem *calcChain0C;
    ChainElem *drawChain10;
    AnmLoaded *presentationAnm14;
    char text18[0x80];
    ZunTimer timer98;
    int primaryPresentationCounterA4;
    int secondaryPresentationCounterA8;
    AnmVm mainVms0AC[2];
    AnmVm secondaryVms5F4[5];
    CardAttackUpdateCallback primaryUpdate1328;
    CardAttackDrawCallback primaryDraw132C;
    CardAttackEndCallback primaryEnd1330;
    CardAttackUpdateCallback secondaryUpdate1334;
    CardAttackDrawCallback secondaryDraw1338;
    CardAttackEndCallback secondaryEnd133C;
    unsigned char unknown1340[8];
    int primaryValue1348;
    int primaryRow134C;
    int primaryClass1350;
    int secondaryValue1354;
    int secondaryRow1358;
    int secondaryClass135C;
    void *extra1360;
    void *extra1364;

    void StartAttack(
        int callbackRow, int attackClass, int value, const char *text);
};

typedef char CardAttackStartPrimaryAt1328[
    (offsetof(CardAttackStartView, primaryUpdate1328) == 0x1328) ? 1 : -1];
typedef char CardAttackStartTailAt1348[
    (offsetof(CardAttackStartView, primaryValue1348) == 0x1348) ? 1 : -1];
typedef char CardAttackStartSizeIs1368[
    (sizeof(CardAttackStartView) == 0x1368) ? 1 : -1];

void CardAttackStartView::StartAttack(
    int callbackRow, int attackClass, int value, const char *text)
{
    int displayValue;
    if (attackClass < 2)
        displayValue = otherSideState08->player04->valueA0;
    else
        displayValue = otherSideState08->player04->valueA4;

    g_GameManager.valueCCB0.SubtractSaturating(500);

    if (attackClass == 2)
    {
        if (sideState04->enemyManager10->FindActiveEnemyBySideCategory(3) != 0)
            return;

        if (otherSideState08->enemyManager10->FindActiveEnemyBySideCategory(3) != 0)
        {
            EnemyView *enemy =
                otherSideState08->enemyManager10->FindActiveEnemyBySideCategory(3);
            enemy->flags337C |= 0x100u;
            ++otherSideState08->player04->valueB8;
        }
        ++otherSideState08->player04->valueB4;

        if (secondaryUpdate1334 != 0)
        {
            secondaryEnd133C(reinterpret_cast<CardAttack *>(this));
            secondaryUpdate1334 = 0;
            secondaryDraw1338 = 0;
            secondaryEnd133C = 0;
        }

        secondaryRow1358 = callbackRow;
        secondaryValue1354 = value;
        secondaryPresentationCounterA8 = 1;
        secondaryClass135C = 2;
        strcpy(text18, text);

        g_CardAttackCallbackRows[secondaryRow1358].begin(
            reinterpret_cast<CardAttack *>(this));
        secondaryUpdate1334 =
            g_CardAttackCallbackRows[secondaryRow1358].update;
        secondaryDraw1338 =
            g_CardAttackCallbackRows[secondaryRow1358].draw;
        secondaryEnd133C =
            g_CardAttackCallbackRows[secondaryRow1358].end;

        if (otherSideState08->player04->valueA4 < 16)
            ++otherSideState08->player04->valueA4;
        if (sideState04->player04->valueA4 < 16)
            ++sideState04->player04->valueA4;

        g_SoundPlayer.PlaySoundByIdx(53, 0);
    }
    else
    {
        if (primaryUpdate1328 != 0)
        {
            primaryEnd1330(reinterpret_cast<CardAttack *>(this));
            primaryUpdate1328 = 0;
            primaryDraw132C = 0;
            primaryEnd1330 = 0;
        }

        primaryRow134C = callbackRow;
        primaryPresentationCounterA4 = 1;
        primaryValue1348 = value;
        primaryClass1350 = attackClass;
        strcpy(text18, text);
        ++otherSideState08->player04->valueB0;

        g_CardAttackCallbackRows[primaryRow134C].begin(
            reinterpret_cast<CardAttack *>(this));
        primaryUpdate1328 =
            g_CardAttackCallbackRows[primaryRow134C].update;
        primaryDraw132C =
            g_CardAttackCallbackRows[primaryRow134C].draw;
        primaryEnd1330 =
            g_CardAttackCallbackRows[primaryRow134C].end;

        if (otherSideState08->player04->valueA0 < 16)
            ++otherSideState08->player04->valueA0;

        g_SoundPlayer.PlaySoundByIdx(14, 0);
    }

    presentationAnm14->SetAndExecuteScriptIdx(&secondaryVms5F4[0], 30);
    presentationAnm14->SetAndExecuteScriptIdx(&secondaryVms5F4[1], 31);
    presentationAnm14->SetAndExecuteScriptIdx(&secondaryVms5F4[2], 32);
    presentationAnm14->SetAndExecuteScriptIdx(&secondaryVms5F4[3], 33);
    presentationAnm14->SetAndExecuteScriptIdx(&secondaryVms5F4[4], 34);

    presentationAnm14->SetSprite(
        &secondaryVms5F4[3], displayValue / 10 + 25);
    presentationAnm14->SetSprite(
        &secondaryVms5F4[4], displayValue % 10 + 25);
    presentationAnm14->SetSprite(
        &secondaryVms5F4[2], attackClass + 21);

    g_GameManager.transitionBits134 |=
        1u << sideIndex00;
    timer98 = 0;

    sideState04->state00->field1C = 1;
    sideState04->state00->field20 = 30;
    otherSideState08->state00->field1C = 2;
    otherSideState08->state00->field20 = 30;

    if (sideIndex00 == 1)
    {
        otherSideState08->frontSide18->auxA678.sideAnm00->ExecuteAnmIdx(
            &otherSideState08->frontSide18->auxA678.vms0C[0],
            otherSideState08->player04->tableValue303F0);
        otherSideState08->frontSide18->auxA678.sideAnm00->ExecuteAnmIdx(
            &otherSideState08->frontSide18->auxA678.vms0C[1],
            otherSideState08->player04->tableValue303F0 + 1);
    }
    else
    {
        otherSideState08->frontSide18->auxA678.sideAnm00->ExecuteAnmIdx(
            &otherSideState08->frontSide18->auxA678.vms0C[0],
            otherSideState08->player04->tableValue303F0 + 2);
        otherSideState08->frontSide18->auxA678.sideAnm00->ExecuteAnmIdx(
            &otherSideState08->frontSide18->auxA678.vms0C[1],
            otherSideState08->player04->tableValue303F0 + 3);
    }
}
