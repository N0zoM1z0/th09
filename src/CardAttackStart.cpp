#include "AnmManager.hpp"
#include "CardAttackStartView.hpp"
#include "SoundPlayer.hpp"
#include "EnemyManager.hpp"
#include "FrontSide.hpp"
#include "Background.hpp"

#include <stddef.h>
#include <string.h>

// The Player header starts at +0x24. Its native state7C/state80 and
// state8C/state90/state94 are exactly the HUD/attack values at
// +0xA0/+0xA4 and +0xB0/+0xB4/+0xB8, respectively.
typedef char CardAttackStartNativePlayerHeader[
    (offsetof(PlayerLifecycleView, header24) == 0x24 &&
     offsetof(PlayerHeaderStateView, state7C) + 0x24 == 0xA0 &&
     offsetof(PlayerHeaderStateView, state80) + 0x24 == 0xA4 &&
     offsetof(PlayerHeaderStateView, state8C) + 0x24 == 0xB0 &&
     offsetof(PlayerHeaderStateView, state90) + 0x24 == 0xB4 &&
     offsetof(PlayerHeaderStateView, state94) + 0x24 == 0xB8 &&
     offsetof(PlayerLifecycleView, tableValue303F0) == 0x303F0) ? 1 : -1];

// The side's +0x00 Background owns both +0x1C/+0x20 transition words.
typedef char CardAttackStartNativeBackgroundTransition[
    (offsetof(Background, transitionMode) == 0x1C &&
     offsetof(Background, transitionFrames) == 0x20) ? 1 : -1];

// StartAttack reads the same full FrontSide HUD that exact OnDraw and
// OnUpdate own, including the 0x558-byte auxiliary VM region at +0xA678.
typedef char CardAttackStartNativeFrontAux[
    (offsetof(FrontSide, auxA678) == 0xA678 &&
     sizeof(FrontSideAuxView) == 0x558 &&
     offsetof(FrontSideAuxView, sideAnm00) == 0 &&
     offsetof(FrontSideAuxView, vms0C) == 0x0C) ? 1 : -1];

// The exact StartAttack uses the GameManager side record itself; only the
// Player pointer needs the view that exposes the +0xA0/+0x303F0 counters.
typedef char CardAttackStartNativeSideRecord[
    (sizeof(CardAttackStartSideView) == 0x38 &&
     offsetof(CardAttackStartSideView, background00) == 0 &&
     offsetof(CardAttackStartSideView, player04) == 0x04 &&
     offsetof(CardAttackStartSideView, enemyManager10) == 0x10 &&
     offsetof(CardAttackStartSideView, frontSide18) == 0x18) ? 1 : -1];

struct CardAttackStartValueCCView
{
    unsigned char unknown00[0x1C];
    int value1C;

    void SubtractSaturating(int amount);
};

// The original 0x20-byte value helper starts at the native GameManager
// valueB0, whose +0x1C member is precisely canonical GameManager.valueCC.
// Its original method name/ABI are retained without a second GameManager.
typedef char CardAttackStartNativeManagerFields[
    (offsetof(AsciiGameManagerView, valueB0) == 0xB0 &&
     offsetof(AsciiGameManagerView, valueCC) == 0xCC &&
     offsetof(AsciiGameManagerView, flags) == 0x134 &&
     sizeof(AsciiGameManagerView) == 0x390 &&
     offsetof(CardAttackStartValueCCView, value1C) == 0x1C) ? 1 : -1];

typedef int (*CardAttackBeginCallback)(CardAttack *cardAttack);

struct CardAttackCallbackRow
{
    CardAttackBeginCallback begin;
    CardAttackUpdateCallback update;
    CardAttackDrawCallback draw;
    CardAttackEndCallback end;
};

extern CardAttackCallbackRow g_CardAttackCallbackRows[];


void CardAttackStartView::StartAttack(
    int callbackRow, int attackClass, int value, const char *text)
{
    int displayValue;
    if (attackClass < 2)
        displayValue = startOtherSideState08->player04->header24.state7C;
    else
        displayValue = startOtherSideState08->player04->header24.state80;

    reinterpret_cast<CardAttackStartValueCCView *>(
        &g_GameManager.valueB0)->SubtractSaturating(500);

    if (attackClass == 2)
    {
        if (startSideState04->enemyManager10->FindActiveEnemyBySideCategory(3) != 0)
            return;

        if (startOtherSideState08->enemyManager10->FindActiveEnemyBySideCategory(3) != 0)
        {
            EnemyView *enemy =
                startOtherSideState08->enemyManager10->FindActiveEnemyBySideCategory(3);
            enemy->flags337C |= 0x100u;
            ++startOtherSideState08->player04->header24.state94;
        }
        ++startOtherSideState08->player04->header24.state90;

        if (secondaryUpdate1334 != 0)
        {
            secondaryEnd133C(this);
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
            this);
        secondaryUpdate1334 =
            g_CardAttackCallbackRows[secondaryRow1358].update;
        secondaryDraw1338 =
            g_CardAttackCallbackRows[secondaryRow1358].draw;
        secondaryEnd133C =
            g_CardAttackCallbackRows[secondaryRow1358].end;

        if (startOtherSideState08->player04->header24.state80 < 16)
            ++startOtherSideState08->player04->header24.state80;
        if (startSideState04->player04->header24.state80 < 16)
            ++startSideState04->player04->header24.state80;

        g_SoundPlayer.PlaySoundByIdx(53, 0);
    }
    else
    {
        if (primaryUpdate1328 != 0)
        {
            primaryEnd1330(this);
            primaryUpdate1328 = 0;
            primaryDraw132C = 0;
            primaryEnd1330 = 0;
        }

        primaryRow134C = callbackRow;
        primaryPresentationCounterA4 = 1;
        primaryValue1348 = value;
        primaryClass1350 = attackClass;
        strcpy(text18, text);
        ++startOtherSideState08->player04->header24.state8C;

        g_CardAttackCallbackRows[primaryRow134C].begin(
            this);
        primaryUpdate1328 =
            g_CardAttackCallbackRows[primaryRow134C].update;
        primaryDraw132C =
            g_CardAttackCallbackRows[primaryRow134C].draw;
        primaryEnd1330 =
            g_CardAttackCallbackRows[primaryRow134C].end;

        if (startOtherSideState08->player04->header24.state7C < 16)
            ++startOtherSideState08->player04->header24.state7C;

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

    startSideState04->background00->transitionMode = 1;
    startSideState04->background00->transitionFrames = 30;
    startOtherSideState08->background00->transitionMode = 2;
    startOtherSideState08->background00->transitionFrames = 30;

    if (sideIndex00 == 1)
    {
        startOtherSideState08->frontSide18->auxA678.sideAnm00->ExecuteAnmIdx(
            &startOtherSideState08->frontSide18->auxA678.vms0C[0],
            startOtherSideState08->player04->tableValue303F0);
        startOtherSideState08->frontSide18->auxA678.sideAnm00->ExecuteAnmIdx(
            &startOtherSideState08->frontSide18->auxA678.vms0C[1],
            startOtherSideState08->player04->tableValue303F0 + 1);
    }
    else
    {
        startOtherSideState08->frontSide18->auxA678.sideAnm00->ExecuteAnmIdx(
            &startOtherSideState08->frontSide18->auxA678.vms0C[0],
            startOtherSideState08->player04->tableValue303F0 + 2);
        startOtherSideState08->frontSide18->auxA678.sideAnm00->ExecuteAnmIdx(
            &startOtherSideState08->frontSide18->auxA678.vms0C[1],
            startOtherSideState08->player04->tableValue303F0 + 3);
    }
}
