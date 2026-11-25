// Target-bound TH09 FrontSide HUD owner; OnUpdate's complete 2381-byte
// replay includes every field/call relocation under pinned VC7.1 O2/Ob1.
#include "FrontSide.hpp"
#define TH09_GAME_MANAGER_TYPES_ONLY
#include "GameManagerRuntimeView.hpp"
#undef TH09_GAME_MANAGER_TYPES_ONLY
#include "Supervisor.hpp"

#include "AnmManager.hpp"
#include "ZunMemory.hpp"

#include <new>
#include <stdlib.h>
#include <string.h>

// Each HUD VM is the shared 0x2A4-byte ANM object, rather than a
// second opaque 0x2A4-byte layout with duplicate scalar and color names.
typedef char FrontSideNativeVmMemberPositions[
    (sizeof(AnmVm) == 0x2A4 &&
     offsetof(AnmVm, scale) == 0x18 &&
     offsetof(AnmVm, color1Script) == 0x1F0 &&
     offsetof(AnmVm, flagsWord) == 0x1F8 &&
     offsetof(AnmVm, pos) == 0x208 &&
     offsetof(AnmVm, pos2) == 0x288) ? 1 : -1];

// The HUD reads its live counters and timers from the genuine Player.
// There is no independently allocated, padding-filled HUD Player view.
typedef char FrontSideNativePlayerFields[
    (offsetof(PlayerLifecycleView, header24) +
        offsetof(PlayerHeaderStateView, state80) == 0xA4 &&
     offsetof(PlayerLifecycleView, header24) +
        offsetof(PlayerHeaderStateView, state84) == 0xA8 &&
     offsetof(PlayerLifecycleView, scalar30384) == 0x30384 &&
     offsetof(PlayerLifecycleView, scalar30388) == 0x30388 &&
     offsetof(PlayerLifecycleView, ownerState30410) +
        offsetof(PlayerOwnerStateView, state04) == 0x30414 &&
     offsetof(PlayerLifecycleView, ownerState30410) +
        offsetof(PlayerOwnerStateView, maxValue10) == 0x30420 &&
     offsetof(PlayerLifecycleView, ownerState30410) +
        offsetof(PlayerOwnerStateView, timer14) == 0x30424 &&
     offsetof(PlayerLifecycleView, ownerState30410) +
        offsetof(PlayerOwnerStateView, timer20) == 0x30430) ? 1 : -1];

typedef AsciiGameManagerSideView FrontSideSideStateView;

typedef char FrontSideSideStateSizeIs38[
    (sizeof(FrontSideSideStateView) == 0x38) ? 1 : -1];
// Both HUD counters and the score word are genuine members of the shared
// GameManager per-side 0xA0 record; do not retain a smaller second view.
typedef char FrontSideNativeRuntimeLayout[
    (offsetof(FrontSideSideStateView, runtime) == 0x1C &&
     sizeof(AsciiGameSideRuntimeView) == 0xA0 &&
     offsetof(AsciiGameSideRuntimeView, value04) == 0x04 &&
     offsetof(AsciiGameSideRuntimeView, counter14) == 0x14) ? 1 : -1];

// The HUD and the rest of gameplay share the one 0x390-byte GameManager.
typedef char FrontSideNativeGameManagerFields[
    (sizeof(AsciiGameManagerView) == 0x390 &&
     offsetof(AsciiGameManagerView, sides) == 0 &&
     offsetof(AsciiGameManagerView, field0FC) == 0xFC &&
     offsetof(AsciiGameManagerView, field108) == 0x108 &&
     offsetof(AsciiGameManagerView, field10C) == 0x10C &&
     offsetof(AsciiGameManagerView, gameMode) == 0x118) ? 1 : -1];

// The periodic HUD indicators use the genuine 12-byte ZunTimer object;
// its integer getter and modulo operator have independent original owners.
typedef char FrontSideNativeTimerRoles[
    (sizeof(ZunTimer) == 0x0C &&
     offsetof(FrontSide, updateTimerABDC) == 0xABDC &&
     (offsetof(PlayerLifecycleView, ownerState30410) + offsetof(PlayerOwnerStateView, timer20)) == 0x30430) ? 1 : -1];

extern Chain g_Chain;
extern AsciiGameManagerView g_GameManager;

const char *g_FrontSideAuxPaths[3] = {
    "pl06_fc_s.anm",
    "pl06b_fc_s.anm",
    NULL,
};

// The HUD's VM groups, the GameManager side records, and the auxiliary
// resource paths are already declared with their native array layouts.

static int ReleaseOptionalAnm(FrontSide *frontSide)
{
    if (frontSide->auxA678.optionalAnm04 != NULL)
        g_AnmManager->ReleaseAnm(7);
    return 0;
}

int FrontSide::AddedCallback(FrontSide *frontSide)
{
    frontSide->frontAnmA668 = g_AnmManager->GetAnm(10);
    if (frontSide->frontAnmA668 == NULL)
        return -1;

    int sideIndex = frontSide->sideIndexA66C;
    frontSide->auxA678.sideAnm00 =
        g_AnmManager->GetAnm(static_cast<int>(
            static_cast<unsigned int>(sideIndex) + 5u));

    FrontSideSideStateView *side = frontSide->sideStateA670;
    if (sideIndex == 0 &&
        g_GameManager.field0FC == 8 && side->shotType20 == 6)
    {
        frontSide->auxA678.optionalAnm04 =
            g_AnmManager->PreloadAnm(
                7, (g_FrontSideAuxPaths[g_GameManager.sides[0].auxPathIndex30]));
        if (frontSide->auxA678.optionalAnm04 == NULL)
            return -1;
    }

    for (unsigned int i = 0; i < 11; ++i)
    {
        frontSide->frontAnmA668->ExecuteAnmIdx(&(frontSide->statusVms0000[i]), i + 7);
        // The visible HUD position pair is owned by that same genuine VM.
        (frontSide->statusVms0000[i]).pos2 =
            (frontSide->statusVms0000[i]).pos;
    }
    for (unsigned int i = 0; i < 5; ++i)
        frontSide->frontAnmA668->ExecuteAnmIdx(&(frontSide->stockVms1D0C[i]), i + 35);
    for (unsigned int i = 0; i < 7; ++i)
        frontSide->frontAnmA668->ExecuteAnmIdx(&(frontSide->gaugeVms2A40[i]), i + 52);
    for (unsigned int i = 0; i < 10; ++i)
        frontSide->frontAnmA668->ExecuteAnmIdx(&(frontSide->scoreVms3F60[i]), i + 20);
    frontSide->frontAnmA668->ExecuteAnmIdx(&frontSide->vm3CBC, 59);

    if (!g_GameManager.IsGameMode2())
    {
        for (unsigned int i = 0; i < 7; ++i)
            frontSide->frontAnmA668->ExecuteAnmIdx(&(frontSide->rankVms86AC[i]), i + 40);
    }
    for (unsigned int i = 0; i < 5; ++i)
        frontSide->frontAnmA668->ExecuteAnmIdx(&(frontSide->counterVms9928[i]), i + 47);

    frontSide->updateTimerABDC = 0;
    memset(&frontSide->vm59C8, 0, sizeof(frontSide->vm59C8));
    return 0;
}

int FrontSide::Reset()
{
    memset(this, 0, sizeof(FrontSide));
    return 0;
}

FrontSide *FrontSide::Create(int sideIndex)
{
    FrontSide *frontSide = static_cast<FrontSide *>(
        g_ZunMemory.AddToRegistry(new FrontSide, sizeof(FrontSide), "FrontSideInf"));
    frontSide->Reset();

    frontSide->sideStateA670 = &(g_GameManager.sides[sideIndex]);
    frontSide->sideIndexA66C = sideIndex;
    frontSide->otherSideStateA674 = &(g_GameManager.sides[static_cast<int>(1u - static_cast<unsigned int>(sideIndex))]);

    frontSide->calcChainA660 =
        g_Chain.CreateElem(reinterpret_cast<ChainCallback>(FrontSide::OnUpdate));
    frontSide->calcChainA660->arg = frontSide;
    frontSide->calcChainA660->addedCallback =
        reinterpret_cast<ChainLifetimeCallback>(FrontSide::AddedCallback);
    if (g_Chain.AddToCalcChain(frontSide->calcChainA660, static_cast<int>(static_cast<unsigned int>(sideIndex) + 23u)) != 0)
        return NULL;

    frontSide->drawChainA664 =
        g_Chain.CreateElem(reinterpret_cast<ChainCallback>(FrontSide::OnDraw));
    frontSide->drawChainA664->arg = frontSide;
    g_Chain.AddToDrawChain(frontSide->drawChainA664, static_cast<int>(static_cast<unsigned int>(sideIndex) + 26u));
    return frontSide;
}

void FrontSide::Destroy(FrontSide *frontSide)
{
    if (frontSide == NULL)
        return;

    ReleaseOptionalAnm(frontSide);
    g_Chain.Cut(frontSide->drawChainA664);
    g_Chain.Cut(frontSide->calcChainA660);
    operator delete(frontSide);
}

void FrontSide::LoadScript67()
{
    this->frontAnmA668->ExecuteAnmIdx(&this->vm59C8, 67);
}

void FrontSide::ResetTransitionState()
{
    this->updateTimerABDC = 0;
    if (this->auxA678.unknown554 != 0)
    {
        this->auxA678.unknown554 = 2;
        this->transitionTimerABD0 = 30;
    }
}

void FrontSide::ActivateMeterVms()
{
    this->frontAnmA668->ExecuteAnmIdx(&this->meterVms5C6C[0], 68);
    this->frontAnmA668->ExecuteAnmIdx(&this->meterVms5C6C[1], 69);
    this->frontAnmA668->ExecuteAnmIdx(&this->meterVms5C6C[2], 70);
    this->frontAnmA668->ExecuteAnmIdx(&this->meterVms5C6C[3], 71);
    this->transitionStateA65C = 1;
}

void FrontSide::DeactivateMeterVms()
{
    this->meterVms5C6C[0].SetInterrupt(1);
    this->meterVms5C6C[1].SetInterrupt(1);
    this->meterVms5C6C[2].SetInterrupt(1);
    this->meterVms5C6C[3].SetInterrupt(1);
    this->transitionStateA65C = 0;
}

int FrontSide::UpdateMeterHundreds(int level)
{
    this->frontAnmA668->ExecuteAnmIdx(&this->markerVms66FC[0], 72);
    this->frontAnmA668->ExecuteAnmIdx(&this->markerVms66FC[1], 73);
    this->frontAnmA668->ExecuteAnmIdx(&this->markerVms66FC[2], 74);
    return this->frontAnmA668->SetSprite(&this->markerVms66FC[2], static_cast<int>(static_cast<unsigned int>(level) + 53u));
}

void FrontSide::ClearState(int value)
{
    this->frontAnmA668->ExecuteAnmIdx(&this->stateVms6EE8[0], 75);
    this->frontAnmA668->ExecuteAnmIdx(&this->stateVms6EE8[1], static_cast<int>(static_cast<unsigned int>(value) + 76u));
}

void FrontSide::ResetPatternTiming()
{
    this->frontAnmA668->ExecuteAnmIdx(&this->vm7430, 78);
}

int FrontSide::SetPatternTiming(int frames)
{
    this->frontAnmA668->ExecuteAnmIdx(&this->timingVms76D4[0], 79);
    this->frontAnmA668->ExecuteAnmIdx(&this->timingVms76D4[1], 80);
    this->frontAnmA668->ExecuteAnmIdx(&this->timingVms76D4[2], 81);
    this->frontAnmA668->ExecuteAnmIdx(&this->timingVms76D4[3], 82);
    this->frontAnmA668->ExecuteAnmIdx(&this->timingVms76D4[4], 83);
    this->frontAnmA668->ExecuteAnmIdx(&this->timingVms76D4[5], 84);
    return this->SetTimingDigits(frames);
}

int FrontSide::SetTimingDigits(int frames)
{
    if (frames < 0)
    {
        memset(&this->timingVms76D4[0], 0, sizeof(AnmVm));
        memset(&this->timingVms76D4[1], 0, sizeof(AnmVm));
        memset(&this->timingVms76D4[2], 0, sizeof(AnmVm));
        memset(&this->timingVms76D4[3], 0, sizeof(AnmVm));
        memset(&this->timingVms76D4[4], 0, sizeof(AnmVm));
        memset(&this->timingVms76D4[5], 0, sizeof(AnmVm));
        return 0;
    }

    int minutes = frames / 60;
    int seconds = frames % 60;
    int savedMinutes = minutes;
    int hundreds = minutes / 100;
    if (hundreds == 0)
    {
        memset(&this->timingVms76D4[0], 0, sizeof(AnmVm));
        minutes = savedMinutes;
    }
    else
    {
        this->frontAnmA668->SetSprite(&this->timingVms76D4[0], hundreds + 53);
    }

    this->frontAnmA668->SetSprite(&this->timingVms76D4[1], minutes / 10 % 10 + 53);
    this->frontAnmA668->SetSprite(&this->timingVms76D4[2], minutes % 10 + 53);
    int scaledSeconds = 100 * seconds;
    this->frontAnmA668->SetSprite(&this->timingVms76D4[4], scaledSeconds / 60 / 10 + 53);
    return this->frontAnmA668->SetSprite(&this->timingVms76D4[5], scaledSeconds / 60 % 10 + 53);
}

int FrontSide::OnUpdate(FrontSide *frontSide)
{
    g_Supervisor.SelectSide(frontSide->sideIndexA66C);

    for (int i = 0; i < 11; ++i)
        g_AnmManager->ExecuteScript(&(frontSide->statusVms0000[i]));

    int value = frontSide->sideStateA670->player04->ownerState30410.state04;
    if (value > 999)
        value = 999;

    if (value >= 100)
    {
        frontSide->frontAnmA668->SetSprite(&frontSide->statusVms0000[2], value / 100 + 8);
        (&frontSide->statusVms0000[2])->drawEnabled = 1;
    }
    else
    {
        (&frontSide->statusVms0000[2])->drawEnabled = 0;
    }

    if (value >= 10)
    {
        frontSide->frontAnmA668->SetSprite(&frontSide->statusVms0000[3], value / 10 % 10 + 8);
        (&frontSide->statusVms0000[3])->drawEnabled = 1;
    }
    else
    {
        (&frontSide->statusVms0000[3])->drawEnabled = 0;
    }
    frontSide->frontAnmA668->SetSprite(&frontSide->statusVms0000[4], value % 10 + 8);

    PlayerLifecycleView *pulsePlayer = frontSide->sideStateA670->player04;
    if (pulsePlayer->ownerState30410.timer20 > 0 && pulsePlayer->ownerState30410.timer20 < 20)
    {
        if (static_cast<int>(pulsePlayer->ownerState30410.timer20) & 1)
        {
            for (int i = 4; i != 0; --i)
            {
                AnmVmScriptColor &color = (frontSide->statusVms0000[5 - i]).color1Script;
                color.r = 0xFF;
                color.g = 0x00;
                color.b = 0x00;
            }
        }
        else
        {
            for (int i = 4; i != 0; --i)
            {
                AnmVmScriptColor &color = (frontSide->statusVms0000[5 - i]).color1Script;
                color.r = 0xFF;
                color.g = 0xFF;
                color.b = 0xFF;
            }
        }
    }
    else
    {
        for (int i = 4; i != 0; --i)
        {
            AnmVmScriptColor &color = (frontSide->statusVms0000[5 - i]).color1Script;
            color.r = 0xD0;
            color.g = 0x80;
            color.b = 0x80;
        }
    }

    int score = frontSide->sideStateA670->player04->ownerState30410.maxValue10;
    int remaining = score % 100000;
    frontSide->frontAnmA668->SetSprite(&frontSide->statusVms0000[5], score / 100000 + 8);
    frontSide->frontAnmA668->SetSprite(&frontSide->statusVms0000[6], remaining / 10000 + 8);
    remaining %= 10000;
    frontSide->frontAnmA668->SetSprite(&frontSide->statusVms0000[7], remaining / 1000 + 8);
    remaining %= 1000;
    frontSide->frontAnmA668->SetSprite(&frontSide->statusVms0000[8], remaining / 100 + 8);
    remaining %= 100;
    frontSide->frontAnmA668->SetSprite(&frontSide->statusVms0000[9], remaining / 10 + 8);
    frontSide->frontAnmA668->SetSprite(&frontSide->statusVms0000[10], remaining % 10 + 8);

    int wideValue = frontSide->sideStateA670->runtime->value04;
    if (wideValue >= 100000000)
    {
        frontSide->frontAnmA668->SetSprite(&frontSide->scoreVms3F60[0], wideValue / 100000000 + 8);
        wideValue %= 100000000;
        (&frontSide->scoreVms3F60[0])->drawEnabled = 1;
    }
    else
    {
        (&frontSide->scoreVms3F60[0])->drawEnabled = 0;
    }
    frontSide->frontAnmA668->SetSprite(&frontSide->scoreVms3F60[1], wideValue / 10000000 + 8);
    wideValue %= 10000000;
    frontSide->frontAnmA668->SetSprite(&frontSide->scoreVms3F60[2], wideValue / 1000000 + 8);
    wideValue %= 1000000;
    frontSide->frontAnmA668->SetSprite(&frontSide->scoreVms3F60[3], wideValue / 100000 + 8);
    wideValue %= 100000;
    frontSide->frontAnmA668->SetSprite(&frontSide->scoreVms3F60[4], wideValue / 10000 + 8);
    wideValue %= 10000;
    frontSide->frontAnmA668->SetSprite(&frontSide->scoreVms3F60[5], wideValue / 1000 + 8);
    wideValue %= 1000;
    frontSide->frontAnmA668->SetSprite(&frontSide->scoreVms3F60[6], wideValue / 100 + 8);
    wideValue %= 100;
    frontSide->frontAnmA668->SetSprite(&frontSide->scoreVms3F60[7], wideValue / 10 + 8);
    frontSide->frontAnmA668->SetSprite(&frontSide->scoreVms3F60[8], wideValue % 10 + 8);
    frontSide->frontAnmA668->SetSprite(&frontSide->scoreVms3F60[9], static_cast<int>(static_cast<unsigned int>(frontSide->sideStateA670->runtime->counter14) + 8u));
    g_AnmManager->ExecuteScriptArray(frontSide->scoreVms3F60, 10);

    int stock = frontSide->sideStateA670->player04->header24.state84;
    for (int i = 0; i < 5; ++i)
    {
        g_AnmManager->ExecuteScript(&(frontSide->stockVms1D0C[i]));
        int shown = stock >= 2 ? 2 : stock;
        frontSide->frontAnmA668->SetSprite(&(frontSide->stockVms1D0C[i]), shown + 35);
        // The real signed HUD stock value is decremented as an i386
        // DWORD before the original negative/zero clamp. Preserve the
        // wrap result for even the native INT_MIN edge case.
        stock = static_cast<int>(static_cast<unsigned int>(stock) - 2u);
        if (stock < 0)
            stock = 0;
    }

    for (int i = 0; i < 7; ++i)
        g_AnmManager->ExecuteScript(&(frontSide->gaugeVms2A40[i]));
    FrontSideSideStateView *side = frontSide->sideStateA670;
    (&frontSide->gaugeVms2A40[0])->scale.x = side->player04->scalar30388 * 0.0025f;
    (&frontSide->gaugeVms2A40[1])->scale.x = side->player04->scalar30384 * 0.0025f;

    AnmVmScriptColor &meter0Color = (&frontSide->gaugeVms2A40[1])->color1Script;
    if (side->player04->scalar30384 >= 400.0f)
        meter0Color.d3dColor = (frontSide->updateTimerABDC % 2) ? 0xFFFFFFFFU : 0xFFFF0000U;
    else if (side->player04->scalar30384 >= 300.0f)
        meter0Color.d3dColor = (frontSide->updateTimerABDC % 2) ? 0xFFF0F000U : 0xFFF0F0C0U;
    else if (side->player04->scalar30384 >= 200.0f)
        meter0Color.d3dColor = (frontSide->updateTimerABDC % 2) ? 0xFFF0F000U : 0xFFC0C0C0U;
    else if (side->player04->scalar30384 >= 100.0f)
        meter0Color.d3dColor = (frontSide->updateTimerABDC % 2) ? 0xFFF0F080U : 0xFFC0C0C0U;
    else
        meter0Color.d3dColor = 0xFFC0C0C0U;

    AnmVmScriptColor &meter1Color = (&frontSide->gaugeVms2A40[0])->color1Script;
    if (side->player04->scalar30388 >= 400.0f)
        meter1Color.d3dColor = 0xFFC08080U;
    else if (side->player04->scalar30388 >= 300.0f)
        meter1Color.d3dColor = 0xFFB08080U;
    else if (side->player04->scalar30388 >= 200.0f)
        meter1Color.d3dColor = 0xFFA08080U;
    else
        meter1Color.d3dColor = 0xFF808080U;

    int gaugeValue = side->player04->header24.state7C;
    int gaugeRemainder = gaugeValue % 10;
    frontSide->frontAnmA668->SetSprite(&frontSide->gaugeVms2A40[3], gaugeValue / 10 + 8);
    frontSide->frontAnmA668->SetSprite(&frontSide->gaugeVms2A40[4], gaugeRemainder + 8);
    gaugeValue = frontSide->sideStateA670->player04->header24.state80;
    gaugeRemainder = gaugeValue % 10;
    frontSide->frontAnmA668->SetSprite(&frontSide->gaugeVms2A40[5], gaugeValue / 10 + 8);
    frontSide->frontAnmA668->SetSprite(&frontSide->gaugeVms2A40[6], gaugeRemainder + 8);

    g_AnmManager->ExecuteScript(&frontSide->auxA678.vms0C[0]);
    g_AnmManager->ExecuteScript(&frontSide->auxA678.vms0C[1]);
    (&frontSide->vm3CBC)->scale.x = static_cast<float>(frontSide->sideStateA670->player04->ownerState30410.timer14) * (1.0f / 105.0f);
    g_AnmManager->ExecuteScript(&frontSide->vm3CBC);
    g_AnmManager->ExecuteScript(&frontSide->vm59C8);
    g_AnmManager->ExecuteScript(&frontSide->meterVms5C6C[0]);
    frontSide->frontAnmA668->SetSprite(
        &frontSide->meterVms5C6C[1], static_cast<int>(frontSide->sideStateA670->player04->scalar30384) / 100 + 53);
    g_AnmManager->ExecuteScript(&frontSide->meterVms5C6C[1]);
    frontSide->frontAnmA668->SetSprite(
        &frontSide->meterVms5C6C[2], static_cast<int>(frontSide->sideStateA670->player04->scalar30388) / 100 + 53);
    g_AnmManager->ExecuteScript(&frontSide->meterVms5C6C[2]);
    g_AnmManager->ExecuteScript(&frontSide->meterVms5C6C[3]);
    g_AnmManager->ExecuteScriptArray(frontSide->markerVms66FC, 3);
    g_AnmManager->ExecuteScriptArray(frontSide->stateVms6EE8, 2);
    g_AnmManager->ExecuteScriptArray(frontSide->timingVms76D4, 6);
    g_AnmManager->ExecuteScript(&frontSide->vm7430);

    // Both marker counts are the consecutive native GameManager DWORDs at
    // +0x108/+0x10C. The live HUD side selects the count without copying
    // or adding a new bounds gate.
    if (!g_GameManager.IsGameMode2())
    {
        int rankIndex = 0;
        for (; rankIndex < static_cast<int>((g_GameManager.sides[frontSide->sideIndexA66C]).runtime->value00); ++rankIndex)
            (&(frontSide->rankVms86AC[rankIndex]))->drawEnabled = 1;
        for (; static_cast<unsigned int>(rankIndex) < 7; ++rankIndex)
            (&(frontSide->rankVms86AC[rankIndex]))->drawEnabled = 0;
        g_AnmManager->ExecuteScriptArray(frontSide->rankVms86AC, 7);
    }

    const int *markerCounts = &g_GameManager.field108;
    int markerCount = markerCounts[frontSide->sideIndexA66C];
    if (markerCount > 5)
        markerCount = 5;
    int markerIndex = 0;
    for (; markerIndex < markerCount; ++markerIndex)
        (&(frontSide->counterVms9928[markerIndex]))->drawEnabled = 1;
    for (; static_cast<unsigned int>(markerIndex) < 5; ++markerIndex)
        (&(frontSide->counterVms9928[markerIndex]))->drawEnabled = 0;
    g_AnmManager->ExecuteScriptArray(frontSide->counterVms9928, 5);

    if (frontSide->auxA678.unknown554 == 1)
    {
        frontSide->transitionTimerABD0++;
    }
    else if (frontSide->auxA678.unknown554 == 2)
    {
        frontSide->transitionTimerABD0--;
        if (frontSide->transitionTimerABD0 <= 0)
            frontSide->auxA678.unknown554 = 0;
    }
    frontSide->updateTimerABDC++;
    return 1;
}
