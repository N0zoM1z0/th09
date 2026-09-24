#include "FrontSide.hpp"

#include "AnmManager.hpp"
#include "ZunMemory.hpp"

#include <stdlib.h>
#include <string.h>

struct FrontSideColorView
{
    unsigned char blue;
    unsigned char green;
    unsigned char red;
    unsigned char alpha;
};

struct FrontSideFloat2View
{
    float x;
    float y;
};

struct FrontSideFloat3View
{
    float x;
    float y;
    float z;
};

struct FrontSideVmView
{
    unsigned char unknown000[0x18];
    FrontSideFloat2View scale18;
    unsigned char unknown020[0x1F0 - 0x20];
    FrontSideColorView color1F0;
    unsigned char unknown1F4[0x04];
    unsigned int flags1F8;
    unsigned char unknown1FC[0x208 - 0x1FC];
    FrontSideFloat3View position208;
    unsigned char unknown214[0x288 - 0x214];
    FrontSideFloat3View offset288;
    unsigned char unknown294[0x2A4 - 0x294];
};

typedef char FrontSideVmViewSizeIs2A4[
    (sizeof(FrontSideVmView) == 0x2A4) ? 1 : -1];
typedef char FrontSideVmScaleAt18[
    (offsetof(FrontSideVmView, scale18) == 0x18) ? 1 : -1];
typedef char FrontSideVmColorAt1F0[
    (offsetof(FrontSideVmView, color1F0) == 0x1F0) ? 1 : -1];
typedef char FrontSideVmFlagsAt1F8[
    (offsetof(FrontSideVmView, flags1F8) == 0x1F8) ? 1 : -1];
typedef char FrontSideVmPositionAt208[
    (offsetof(FrontSideVmView, position208) == 0x208) ? 1 : -1];
typedef char FrontSideVmOffsetAt288[
    (offsetof(FrontSideVmView, offset288) == 0x288) ? 1 : -1];

struct FrontSideSideRuntimeView
{
    float value00;
    int value04;
    int value08;
    int value0C;
    unsigned char unknown10[4];
    unsigned char counter14;
};

struct FrontSidePlayerRuntimeView
{
    unsigned char unknown00000[0xA0];
    int valueA0;
    int valueA4;
    int valueA8;
    unsigned char unknown000AC[0x30384 - 0xAC];
    float meter30384;
    float meter30388;
    unsigned char unknown3038C[0x30414 - 0x3038C];
    int value30414;
    unsigned char unknown30418[0x30420 - 0x30418];
    int value30420;
    ZunTimer meterTimer30424;
    ZunTimer pulseTimer30430;
};

typedef char FrontSidePlayerValueA0AtA0[
    (offsetof(FrontSidePlayerRuntimeView, valueA0) == 0xA0) ? 1 : -1];
typedef char FrontSidePlayerMeter0At30384[
    (offsetof(FrontSidePlayerRuntimeView, meter30384) == 0x30384) ? 1 : -1];
typedef char FrontSidePlayerValue30414At30414[
    (offsetof(FrontSidePlayerRuntimeView, value30414) == 0x30414) ? 1 : -1];
typedef char FrontSidePlayerTimer30424At30424[
    (offsetof(FrontSidePlayerRuntimeView, meterTimer30424) == 0x30424) ? 1 : -1];
typedef char FrontSidePlayerTimer30430At30430[
    (offsetof(FrontSidePlayerRuntimeView, pulseTimer30430) == 0x30430) ? 1 : -1];

struct FrontSideSideStateView
{
    unsigned char unknown00[4];
    FrontSidePlayerRuntimeView *player04;
    unsigned char unknown08[0x14];
    FrontSideSideRuntimeView *runtime1C;
    int shotType20;
    int previousShotType24;
    unsigned char unknown28[4];
    int characterIndex2C;
    int auxPathIndex30;
    unsigned int flags34;
};

typedef char FrontSideSideStateSizeIs38[
    (sizeof(FrontSideSideStateView) == 0x38) ? 1 : -1];

struct FrontSideGameManagerView
{
    FrontSideSideStateView sides[2];
    unsigned char unknown070[0xFC - 0x70];
    int field0FC;
    unsigned char unknown100[8];
    int markerCounts108[2];
    unsigned char unknown110[8];
    int gameMode118;
    int difficulty11C;

    int IsMode2();
};

struct FrontSideSupervisorView
{
    void SelectSide(int sideIndex);
};

struct FrontSideTimerModuloView
{
    int previous;
    float subFrame;
    int current;

    int Modulo(int divisor);
};

typedef char FrontSideTimerModuloSizeIs0C[
    (sizeof(FrontSideTimerModuloView) == 0x0C) ? 1 : -1];

extern Chain g_Chain;
extern FrontSideGameManagerView g_GameManager;
extern FrontSideSupervisorView g_Supervisor;
extern const char *g_FrontSideAuxPaths[];

static __forceinline FrontSideVmView *FrontSideVm(AnmVm *vm)
{
    return reinterpret_cast<FrontSideVmView *>(vm);
}

static __forceinline FrontSideSideStateView *FrontSideState(PlayerSideStateView *state)
{
    return reinterpret_cast<FrontSideSideStateView *>(state);
}

FrontSideAuxView::FrontSideAuxView()
{
}

FrontSide::FrontSide()
{
}

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

    frontSide->auxA678.sideAnm00 =
        g_AnmManager->GetAnm(frontSide->sideIndexA66C + 5);

    FrontSideSideStateView *side = FrontSideState(frontSide->sideStateA670);
    if (frontSide->sideIndexA66C == 0 &&
        g_GameManager.field0FC == 8 && side->shotType20 == 6)
    {
        frontSide->auxA678.optionalAnm04 =
            g_AnmManager->PreloadAnm(
                7, g_FrontSideAuxPaths[g_GameManager.sides[0].auxPathIndex30]);
        if (frontSide->auxA678.optionalAnm04 == NULL)
            return -1;
    }

    for (int i = 0; i < 11; ++i)
    {
        frontSide->frontAnmA668->ExecuteAnmIdx(&frontSide->statusVms0000[i], i + 7);
        FrontSideVm(frontSide->statusVms0000 + i)->offset288 =
            FrontSideVm(frontSide->statusVms0000 + i)->position208;
    }
    for (int i = 0; i < 5; ++i)
        frontSide->frontAnmA668->ExecuteAnmIdx(&frontSide->stockVms1D0C[i], i + 35);
    for (int i = 0; i < 7; ++i)
        frontSide->frontAnmA668->ExecuteAnmIdx(&frontSide->gaugeVms2A40[i], i + 52);
    for (int i = 0; i < 10; ++i)
        frontSide->frontAnmA668->ExecuteAnmIdx(&frontSide->scoreVms3F60[i], i + 20);
    frontSide->frontAnmA668->ExecuteAnmIdx(&frontSide->vm3CBC, 59);

    if (!g_GameManager.IsMode2())
    {
        for (int i = 0; i < 7; ++i)
            frontSide->frontAnmA668->ExecuteAnmIdx(&frontSide->rankVms86AC[i], i + 40);
    }
    for (int i = 0; i < 5; ++i)
        frontSide->frontAnmA668->ExecuteAnmIdx(&frontSide->counterVms9928[i], i + 47);

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

    frontSide->sideStateA670 = reinterpret_cast<PlayerSideStateView *>(
        &g_GameManager.sides[sideIndex]);
    frontSide->sideIndexA66C = sideIndex;
    frontSide->otherSideStateA674 = reinterpret_cast<PlayerSideStateView *>(
        &g_GameManager.sides[1 - sideIndex]);

    frontSide->calcChainA660 =
        g_Chain.CreateElem(reinterpret_cast<ChainCallback>(FrontSide::OnUpdate));
    frontSide->calcChainA660->arg = frontSide;
    frontSide->calcChainA660->addedCallback =
        reinterpret_cast<ChainLifetimeCallback>(FrontSide::AddedCallback);
    if (g_Chain.AddToCalcChain(frontSide->calcChainA660, sideIndex + 23) != 0)
        return NULL;

    frontSide->drawChainA664 =
        g_Chain.CreateElem(reinterpret_cast<ChainCallback>(FrontSide::OnDraw));
    frontSide->drawChainA664->arg = frontSide;
    g_Chain.AddToDrawChain(frontSide->drawChainA664, sideIndex + 26);
    return frontSide;
}

void FrontSide::Destroy(FrontSide *frontSide)
{
    if (frontSide == NULL)
        return;

    ReleaseOptionalAnm(frontSide);
    g_Chain.Cut(frontSide->drawChainA664);
    g_Chain.Cut(frontSide->calcChainA660);
    free(frontSide);
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
    return this->frontAnmA668->SetSprite(&this->markerVms66FC[2], level + 53);
}

void FrontSide::ClearState(int value)
{
    this->frontAnmA668->ExecuteAnmIdx(&this->stateVms6EE8[0], 75);
    this->frontAnmA668->ExecuteAnmIdx(&this->stateVms6EE8[1], value + 76);
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

static const FrontSideColorView kMeter0HighOdd = {0xFF, 0xFF, 0xFF, 0xFF};
static const FrontSideColorView kMeter0HighEven = {0x00, 0x00, 0xFF, 0xFF};
static const FrontSideColorView kMeter0MidHighOdd = {0x00, 0xF0, 0xF0, 0xFF};
static const FrontSideColorView kMeter0MidHighEven = {0xC0, 0xF0, 0xF0, 0xFF};
static const FrontSideColorView kMeter0MidOdd = {0x00, 0xF0, 0xF0, 0xFF};
static const FrontSideColorView kMeter0MidEven = {0xC0, 0xC0, 0xC0, 0xFF};
static const FrontSideColorView kMeter0LowOdd = {0x80, 0xF0, 0xF0, 0xFF};
static const FrontSideColorView kMeter0LowEven = {0xC0, 0xC0, 0xC0, 0xFF};
static const FrontSideColorView kMeter0Base = {0xC0, 0xC0, 0xC0, 0xFF};
static const FrontSideColorView kMeter1High = {0x80, 0x80, 0xC0, 0xFF};
static const FrontSideColorView kMeter1MidHigh = {0x80, 0x80, 0xB0, 0xFF};
static const FrontSideColorView kMeter1Mid = {0x80, 0x80, 0xA0, 0xFF};
static const FrontSideColorView kMeter1Low = {0x80, 0x80, 0x80, 0xFF};

int FrontSide::OnUpdate(FrontSide *frontSide)
{
    g_Supervisor.SelectSide(frontSide->sideIndexA66C);

    for (int i = 0; i < 11; ++i)
        g_AnmManager->ExecuteScript(&frontSide->statusVms0000[i]);

    FrontSideSideStateView *side = FrontSideState(frontSide->sideStateA670);
    FrontSidePlayerRuntimeView *player = side->player04;
    FrontSideSideRuntimeView *runtime = side->runtime1C;

    int value = player->value30414;
    if (value > 999)
        value = 999;

    FrontSideVmView *vm = FrontSideVm(&frontSide->statusVms0000[2]);
    if (value >= 100)
    {
        frontSide->frontAnmA668->SetSprite(&frontSide->statusVms0000[2], value / 100 + 8);
        vm->flags1F8 |= 2;
    }
    else
    {
        vm->flags1F8 &= ~2U;
    }

    vm = FrontSideVm(&frontSide->statusVms0000[3]);
    if (value >= 10)
    {
        frontSide->frontAnmA668->SetSprite(&frontSide->statusVms0000[3], value / 10 % 10 + 8);
        vm->flags1F8 |= 2;
    }
    else
    {
        vm->flags1F8 &= ~2U;
    }
    frontSide->frontAnmA668->SetSprite(&frontSide->statusVms0000[4], value % 10 + 8);

    if (player->pulseTimer30430 > 0 && player->pulseTimer30430 < 20)
    {
        for (int i = 1; i < 5; ++i)
        {
            FrontSideColorView &vmColor = FrontSideVm(&frontSide->statusVms0000[i])->color1F0;
            vmColor.red = 0xFF;
            if ((player->pulseTimer30430.current & 1) == 0)
            {
                vmColor.blue = 0xFF;
                vmColor.green = 0xFF;
            }
            else
            {
                vmColor.blue = 0x00;
                vmColor.green = 0x00;
            }
        }
    }
    else
    {
        for (int i = 1; i < 5; ++i)
        {
            FrontSideColorView &vmColor = FrontSideVm(&frontSide->statusVms0000[i])->color1F0;
            vmColor.blue = 0x80;
            vmColor.green = 0x80;
            vmColor.red = 0xD0;
        }
    }

    int score = player->value30420;
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

    int wideValue = runtime->value04;
    vm = FrontSideVm(&frontSide->scoreVms3F60[0]);
    if (wideValue >= 100000000)
    {
        frontSide->frontAnmA668->SetSprite(&frontSide->scoreVms3F60[0], wideValue / 100000000 + 8);
        wideValue %= 100000000;
        vm->flags1F8 |= 2;
    }
    else
    {
        vm->flags1F8 &= ~2U;
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
    frontSide->frontAnmA668->SetSprite(&frontSide->scoreVms3F60[9], runtime->counter14 + 8);
    g_AnmManager->ExecuteScriptArray(frontSide->scoreVms3F60, 10);

    int stock = player->valueA8;
    for (int i = 0; i < 5; ++i)
    {
        g_AnmManager->ExecuteScript(&frontSide->stockVms1D0C[i]);
        int shown = stock < 2 ? stock : 2;
        frontSide->frontAnmA668->SetSprite(&frontSide->stockVms1D0C[i], shown + 35);
        stock -= 2;
        if (stock < 0)
            stock = 0;
    }

    for (int i = 0; i < 7; ++i)
        g_AnmManager->ExecuteScript(&frontSide->gaugeVms2A40[i]);
    FrontSideVm(&frontSide->gaugeVms2A40[0])->scale18.x = player->meter30388 * 0.0025f;
    FrontSideVm(&frontSide->gaugeVms2A40[1])->scale18.x = player->meter30384 * 0.0025f;

    FrontSideColorView &meter0Color = FrontSideVm(&frontSide->gaugeVms2A40[1])->color1F0;
    FrontSideTimerModuloView *updateTimer =
        reinterpret_cast<FrontSideTimerModuloView *>(&frontSide->updateTimerABDC);
    if (player->meter30384 >= 400.0f)
        meter0Color = updateTimer->Modulo(2) ? kMeter0HighOdd : kMeter0HighEven;
    else if (player->meter30384 >= 300.0f)
        meter0Color = updateTimer->Modulo(2) ? kMeter0MidHighOdd : kMeter0MidHighEven;
    else if (player->meter30384 >= 200.0f)
        meter0Color = updateTimer->Modulo(2) ? kMeter0MidOdd : kMeter0MidEven;
    else if (player->meter30384 >= 100.0f)
        meter0Color = updateTimer->Modulo(2) ? kMeter0LowOdd : kMeter0LowEven;
    else
        meter0Color = kMeter0Base;

    FrontSideColorView &meter1Color = FrontSideVm(&frontSide->gaugeVms2A40[0])->color1F0;
    if (player->meter30388 >= 400.0f)
        meter1Color = kMeter1High;
    else if (player->meter30388 >= 300.0f)
        meter1Color = kMeter1MidHigh;
    else if (player->meter30388 >= 200.0f)
        meter1Color = kMeter1Mid;
    else
        meter1Color = kMeter1Low;

    frontSide->frontAnmA668->SetSprite(&frontSide->gaugeVms2A40[3], player->valueA0 / 10 + 8);
    frontSide->frontAnmA668->SetSprite(&frontSide->gaugeVms2A40[4], player->valueA0 % 10 + 8);
    frontSide->frontAnmA668->SetSprite(&frontSide->gaugeVms2A40[5], player->valueA4 / 10 + 8);
    frontSide->frontAnmA668->SetSprite(&frontSide->gaugeVms2A40[6], player->valueA4 % 10 + 8);

    g_AnmManager->ExecuteScript(&frontSide->auxA678.vms0C[0]);
    g_AnmManager->ExecuteScript(&frontSide->auxA678.vms0C[1]);
    FrontSideVm(&frontSide->vm3CBC)->scale18.x = static_cast<float>(player->meterTimer30424) * (1.0f / 105.0f);
    g_AnmManager->ExecuteScript(&frontSide->vm3CBC);
    g_AnmManager->ExecuteScript(&frontSide->vm59C8);
    g_AnmManager->ExecuteScript(&frontSide->meterVms5C6C[0]);
    frontSide->frontAnmA668->SetSprite(
        &frontSide->meterVms5C6C[1], static_cast<int>(player->meter30384) / 100 + 53);
    g_AnmManager->ExecuteScript(&frontSide->meterVms5C6C[1]);
    frontSide->frontAnmA668->SetSprite(
        &frontSide->meterVms5C6C[2], static_cast<int>(player->meter30388) / 100 + 53);
    g_AnmManager->ExecuteScript(&frontSide->meterVms5C6C[2]);
    g_AnmManager->ExecuteScript(&frontSide->meterVms5C6C[3]);
    g_AnmManager->ExecuteScriptArray(frontSide->markerVms66FC, 3);
    g_AnmManager->ExecuteScriptArray(frontSide->stateVms6EE8, 2);
    g_AnmManager->ExecuteScriptArray(frontSide->timingVms76D4, 6);
    g_AnmManager->ExecuteScript(&frontSide->vm7430);

    if (!g_GameManager.IsMode2())
    {
        int count = static_cast<int>(g_GameManager.sides[frontSide->sideIndexA66C].runtime1C->value00);
        int rankIndex = 0;
        for (; rankIndex < count && rankIndex < 7; ++rankIndex)
            FrontSideVm(&frontSide->rankVms86AC[rankIndex])->flags1F8 |= 2;
        for (; rankIndex < 7; ++rankIndex)
            FrontSideVm(&frontSide->rankVms86AC[rankIndex])->flags1F8 &= ~2U;
        g_AnmManager->ExecuteScriptArray(frontSide->rankVms86AC, 7);
    }

    int markerCount = g_GameManager.markerCounts108[frontSide->sideIndexA66C];
    if (markerCount > 5)
        markerCount = 5;
    int markerIndex = 0;
    for (; markerIndex < markerCount; ++markerIndex)
        FrontSideVm(&frontSide->counterVms9928[markerIndex])->flags1F8 |= 2;
    for (; markerIndex < 5; ++markerIndex)
        FrontSideVm(&frontSide->counterVms9928[markerIndex])->flags1F8 &= ~2U;
    g_AnmManager->ExecuteScriptArray(frontSide->counterVms9928, 5);

    if (frontSide->transitionStateA65C == 1)
    {
        frontSide->transitionTimerABD0++;
    }
    else if (frontSide->transitionStateA65C == 2)
    {
        frontSide->transitionTimerABD0--;
        if (frontSide->transitionTimerABD0 <= 0)
            frontSide->transitionStateA65C = 0;
    }
    frontSide->updateTimerABDC++;
    return 1;
}
