// Target-bound TH09 FrontSide HUD owner; OnUpdate's complete 2381-byte
// replay includes every field/call relocation under pinned VC7.1 O2/Ob1.
#include "FrontSide.hpp"

#include "AnmManager.hpp"
#include "ZunMemory.hpp"

#include <stdlib.h>
#include <string.h>

union FrontSideColorView
{
    struct
    {
        unsigned char blue;
        unsigned char green;
        unsigned char red;
        unsigned char alpha;
    };
    unsigned int rgba;
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
    // Target folded body at 0x00435F00 reads +8; no unique ownership claim.
    int GetCurrent();
};

typedef char FrontSideTimerModuloSizeIs0C[
    (sizeof(FrontSideTimerModuloView) == 0x0C) ? 1 : -1];

extern Chain g_Chain;
extern FrontSideGameManagerView g_GameManager;
extern FrontSideSupervisorView g_Supervisor;
extern const char *g_FrontSideAuxPaths[];

#define FrontSideVm(vm) (reinterpret_cast<FrontSideVmView *>(vm))
#define FrontSideState(state) (reinterpret_cast<FrontSideSideStateView *>(state))

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

    int sideIndex = frontSide->sideIndexA66C;
    frontSide->auxA678.sideAnm00 =
        g_AnmManager->GetAnm(sideIndex + 5);

    FrontSideSideStateView *side = FrontSideState(frontSide->sideStateA670);
    if (sideIndex == 0 &&
        g_GameManager.field0FC == 8 && side->shotType20 == 6)
    {
        frontSide->auxA678.optionalAnm04 =
            g_AnmManager->PreloadAnm(
                7, g_FrontSideAuxPaths[g_GameManager.sides[0].auxPathIndex30]);
        if (frontSide->auxA678.optionalAnm04 == NULL)
            return -1;
    }

    for (unsigned int i = 0; i < 11; ++i)
    {
        frontSide->frontAnmA668->ExecuteAnmIdx(&frontSide->statusVms0000[i], i + 7);
        FrontSideVm(frontSide->statusVms0000 + i)->offset288 =
            FrontSideVm(frontSide->statusVms0000 + i)->position208;
    }
    for (unsigned int i = 0; i < 5; ++i)
        frontSide->frontAnmA668->ExecuteAnmIdx(&frontSide->stockVms1D0C[i], i + 35);
    for (unsigned int i = 0; i < 7; ++i)
        frontSide->frontAnmA668->ExecuteAnmIdx(&frontSide->gaugeVms2A40[i], i + 52);
    for (unsigned int i = 0; i < 10; ++i)
        frontSide->frontAnmA668->ExecuteAnmIdx(&frontSide->scoreVms3F60[i], i + 20);
    frontSide->frontAnmA668->ExecuteAnmIdx(&frontSide->vm3CBC, 59);

    if (!g_GameManager.IsMode2())
    {
        for (unsigned int i = 0; i < 7; ++i)
            frontSide->frontAnmA668->ExecuteAnmIdx(&frontSide->rankVms86AC[i], i + 40);
    }
    for (unsigned int i = 0; i < 5; ++i)
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

// These are expression-only layout views, not extra callable accessors.
#define FRONT_SIDE_STATE(owner) FrontSideState((owner)->sideStateA670)
#define FRONT_SIDE_PLAYER(owner) (FRONT_SIDE_STATE(owner)->player04)
#define FRONT_SIDE_RUNTIME(owner) (FRONT_SIDE_STATE(owner)->runtime1C)

int FrontSide::OnUpdate(FrontSide *frontSide)
{
    g_Supervisor.SelectSide(frontSide->sideIndexA66C);

    for (int i = 0; i < 11; ++i)
        g_AnmManager->ExecuteScript(&frontSide->statusVms0000[i]);

    int value = FRONT_SIDE_PLAYER(frontSide)->value30414;
    if (value > 999)
        value = 999;

    if (value >= 100)
    {
        frontSide->frontAnmA668->SetSprite(&frontSide->statusVms0000[2], value / 100 + 8);
        FrontSideVm(&frontSide->statusVms0000[2])->flags1F8 |= 2;
    }
    else
    {
        FrontSideVm(&frontSide->statusVms0000[2])->flags1F8 &= ~2U;
    }

    if (value >= 10)
    {
        frontSide->frontAnmA668->SetSprite(&frontSide->statusVms0000[3], value / 10 % 10 + 8);
        FrontSideVm(&frontSide->statusVms0000[3])->flags1F8 |= 2;
    }
    else
    {
        FrontSideVm(&frontSide->statusVms0000[3])->flags1F8 &= ~2U;
    }
    frontSide->frontAnmA668->SetSprite(&frontSide->statusVms0000[4], value % 10 + 8);

    FrontSidePlayerRuntimeView *pulsePlayer = FRONT_SIDE_PLAYER(frontSide);
    if (pulsePlayer->pulseTimer30430 > 0 && pulsePlayer->pulseTimer30430 < 20)
    {
        if (reinterpret_cast<FrontSideTimerModuloView *>(&pulsePlayer->pulseTimer30430)->GetCurrent() & 1)
        {
            for (int i = 4; i != 0; --i)
            {
                FrontSideColorView &color = FrontSideVm(&frontSide->statusVms0000[5 - i])->color1F0;
                color.red = 0xFF;
                color.green = 0x00;
                color.blue = 0x00;
            }
        }
        else
        {
            for (int i = 4; i != 0; --i)
            {
                FrontSideColorView &color = FrontSideVm(&frontSide->statusVms0000[5 - i])->color1F0;
                color.red = 0xFF;
                color.green = 0xFF;
                color.blue = 0xFF;
            }
        }
    }
    else
    {
        for (int i = 4; i != 0; --i)
        {
            FrontSideColorView &color = FrontSideVm(&frontSide->statusVms0000[5 - i])->color1F0;
            color.red = 0xD0;
            color.green = 0x80;
            color.blue = 0x80;
        }
    }

    int score = FRONT_SIDE_PLAYER(frontSide)->value30420;
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

    int wideValue = FRONT_SIDE_RUNTIME(frontSide)->value04;
    if (wideValue >= 100000000)
    {
        frontSide->frontAnmA668->SetSprite(&frontSide->scoreVms3F60[0], wideValue / 100000000 + 8);
        wideValue %= 100000000;
        FrontSideVm(&frontSide->scoreVms3F60[0])->flags1F8 |= 2;
    }
    else
    {
        FrontSideVm(&frontSide->scoreVms3F60[0])->flags1F8 &= ~2U;
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
    frontSide->frontAnmA668->SetSprite(&frontSide->scoreVms3F60[9], FRONT_SIDE_RUNTIME(frontSide)->counter14 + 8);
    g_AnmManager->ExecuteScriptArray(frontSide->scoreVms3F60, 10);

    int stock = FRONT_SIDE_PLAYER(frontSide)->valueA8;
    for (int i = 0; i < 5; ++i)
    {
        g_AnmManager->ExecuteScript(&frontSide->stockVms1D0C[i]);
        int shown = stock >= 2 ? 2 : stock;
        frontSide->frontAnmA668->SetSprite(&frontSide->stockVms1D0C[i], shown + 35);
        stock -= 2;
        if (stock < 0)
            stock = 0;
    }

    for (int i = 0; i < 7; ++i)
        g_AnmManager->ExecuteScript(&frontSide->gaugeVms2A40[i]);
    FrontSideSideStateView *side = FRONT_SIDE_STATE(frontSide);
    FrontSideVm(&frontSide->gaugeVms2A40[0])->scale18.x = side->player04->meter30388 * 0.0025f;
    FrontSideVm(&frontSide->gaugeVms2A40[1])->scale18.x = side->player04->meter30384 * 0.0025f;

    FrontSideColorView &meter0Color = FrontSideVm(&frontSide->gaugeVms2A40[1])->color1F0;
    if (side->player04->meter30384 >= 400.0f)
        meter0Color.rgba = reinterpret_cast<FrontSideTimerModuloView *>(&frontSide->updateTimerABDC)->Modulo(2) ? 0xFFFFFFFFU : 0xFFFF0000U;
    else if (side->player04->meter30384 >= 300.0f)
        meter0Color.rgba = reinterpret_cast<FrontSideTimerModuloView *>(&frontSide->updateTimerABDC)->Modulo(2) ? 0xFFF0F000U : 0xFFF0F0C0U;
    else if (side->player04->meter30384 >= 200.0f)
        meter0Color.rgba = reinterpret_cast<FrontSideTimerModuloView *>(&frontSide->updateTimerABDC)->Modulo(2) ? 0xFFF0F000U : 0xFFC0C0C0U;
    else if (side->player04->meter30384 >= 100.0f)
        meter0Color.rgba = reinterpret_cast<FrontSideTimerModuloView *>(&frontSide->updateTimerABDC)->Modulo(2) ? 0xFFF0F080U : 0xFFC0C0C0U;
    else
        meter0Color.rgba = 0xFFC0C0C0U;

    FrontSideColorView &meter1Color = FrontSideVm(&frontSide->gaugeVms2A40[0])->color1F0;
    if (side->player04->meter30388 >= 400.0f)
        meter1Color.rgba = 0xFFC08080U;
    else if (side->player04->meter30388 >= 300.0f)
        meter1Color.rgba = 0xFFB08080U;
    else if (side->player04->meter30388 >= 200.0f)
        meter1Color.rgba = 0xFFA08080U;
    else
        meter1Color.rgba = 0xFF808080U;

    int gaugeValue = side->player04->valueA0;
    int gaugeRemainder = gaugeValue % 10;
    frontSide->frontAnmA668->SetSprite(&frontSide->gaugeVms2A40[3], gaugeValue / 10 + 8);
    frontSide->frontAnmA668->SetSprite(&frontSide->gaugeVms2A40[4], gaugeRemainder + 8);
    gaugeValue = FRONT_SIDE_PLAYER(frontSide)->valueA4;
    gaugeRemainder = gaugeValue % 10;
    frontSide->frontAnmA668->SetSprite(&frontSide->gaugeVms2A40[5], gaugeValue / 10 + 8);
    frontSide->frontAnmA668->SetSprite(&frontSide->gaugeVms2A40[6], gaugeRemainder + 8);

    g_AnmManager->ExecuteScript(&frontSide->auxA678.vms0C[0]);
    g_AnmManager->ExecuteScript(&frontSide->auxA678.vms0C[1]);
    FrontSideVm(&frontSide->vm3CBC)->scale18.x = static_cast<float>(FRONT_SIDE_PLAYER(frontSide)->meterTimer30424) * (1.0f / 105.0f);
    g_AnmManager->ExecuteScript(&frontSide->vm3CBC);
    g_AnmManager->ExecuteScript(&frontSide->vm59C8);
    g_AnmManager->ExecuteScript(&frontSide->meterVms5C6C[0]);
    frontSide->frontAnmA668->SetSprite(
        &frontSide->meterVms5C6C[1], static_cast<int>(FRONT_SIDE_PLAYER(frontSide)->meter30384) / 100 + 53);
    g_AnmManager->ExecuteScript(&frontSide->meterVms5C6C[1]);
    frontSide->frontAnmA668->SetSprite(
        &frontSide->meterVms5C6C[2], static_cast<int>(FRONT_SIDE_PLAYER(frontSide)->meter30388) / 100 + 53);
    g_AnmManager->ExecuteScript(&frontSide->meterVms5C6C[2]);
    g_AnmManager->ExecuteScript(&frontSide->meterVms5C6C[3]);
    g_AnmManager->ExecuteScriptArray(frontSide->markerVms66FC, 3);
    g_AnmManager->ExecuteScriptArray(frontSide->stateVms6EE8, 2);
    g_AnmManager->ExecuteScriptArray(frontSide->timingVms76D4, 6);
    g_AnmManager->ExecuteScript(&frontSide->vm7430);

    if (!g_GameManager.IsMode2())
    {
        int rankIndex = 0;
        for (; rankIndex < static_cast<int>(g_GameManager.sides[frontSide->sideIndexA66C].runtime1C->value00); ++rankIndex)
            FrontSideVm(&frontSide->rankVms86AC[rankIndex])->flags1F8 |= 2;
        for (; static_cast<unsigned int>(rankIndex) < 7; ++rankIndex)
            FrontSideVm(&frontSide->rankVms86AC[rankIndex])->flags1F8 &= ~2U;
        g_AnmManager->ExecuteScriptArray(frontSide->rankVms86AC, 7);
    }

    int markerCount = g_GameManager.markerCounts108[frontSide->sideIndexA66C];
    if (markerCount > 5)
        markerCount = 5;
    int markerIndex = 0;
    for (; markerIndex < markerCount; ++markerIndex)
        FrontSideVm(&frontSide->counterVms9928[markerIndex])->flags1F8 |= 2;
    for (; static_cast<unsigned int>(markerIndex) < 5; ++markerIndex)
        FrontSideVm(&frontSide->counterVms9928[markerIndex])->flags1F8 &= ~2U;
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

#undef FRONT_SIDE_STATE
#undef FRONT_SIDE_PLAYER
#undef FRONT_SIDE_RUNTIME

#undef FrontSideVm
#undef FrontSideState
