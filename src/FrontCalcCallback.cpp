#include "AnmManager.hpp"
#include "AsciiManager.hpp"
#include "FrontSupport.hpp"
#include "ScreenEffect.hpp"
#include "ZunTimer.hpp"

#include <stddef.h>

struct FrontCalcVertex
{
    Float3 pos;
    float rhw;
    unsigned long diffuse;
};
typedef char FrontCalcVertexSizeIs14[
    (sizeof(FrontCalcVertex) == 0x14) ? 1 : -1];

struct FrontCalcMessageRuntimeView
{
    unsigned char unknown00[0x08];
    int currentIndex08;
    unsigned char unknown0C[0x1D70 - 0x0C];

    int Update();
    void Setup(int index, int unused);
    void SetupRandomForSide(int sideIndex);
};

typedef char FrontCalcMessageSizeIs1D70[
    (sizeof(FrontCalcMessageRuntimeView) == 0x1D70) ? 1 : -1];

struct FrontCalcRuntimeView
{
    float value00;
    unsigned char unknown04[0x30];
    float value34;
};

struct FrontCalcSideStateObject
{
    unsigned char unknown00[0x1C];
    int field1C;
    int field20;
};

struct FrontCalcSideTransitionOwner
{
    unsigned char unknown00[0x98];
    ZunTimer timer98;
};

struct FrontCalcAddedOwnerStateView
{
    void Reset();
};

struct FrontCalcPlayerView
{
    unsigned char unknown00000[0x30410];
    FrontCalcAddedOwnerStateView addedOwner30410;
};

struct FrontCalcSideView
{
    FrontCalcSideStateObject *stateObject00;
    FrontCalcPlayerView *player04;
    unsigned char unknown08[0x0C];
    FrontCalcSideTransitionOwner *transitionOwner14;
    void *frontSide18;
    FrontCalcRuntimeView *runtime1C;
    unsigned char unknown20[0x14];
    unsigned int flags34;
};
typedef char FrontCalcSideViewSizeIs38[
    (sizeof(FrontCalcSideView) == 0x38) ? 1 : -1];

struct FrontCalcGameManagerView
{
    FrontCalcSideView sides[2];
    unsigned char unknown70[0xA8 - 0x70];
    unsigned int flagsA8;
    unsigned char unknownAC[0x118 - 0xAC];
    int gameMode118;
    int setupMode11C;
    unsigned char unknown120[0x14];
    unsigned int flags134;

    int IsGameMode0();
    int IsGameMode1();
    int IsGameMode2();
    int IsReplayNeutral();
};

struct FrontCalcFrontSideView
{
    void BeginAuxTransition();
};

struct FrontCalcPlayerCollisionPoint
{
    float x;
    float y;
    float z;
};

struct FrontCalcCollisionView
{
    void *CreateCircleType1(
        const FrontCalcPlayerCollisionPoint *center,
        float radius,
        float radiusGrowth,
        int lifetime,
        int delay);
    void *CreateCircleType4(
        const FrontCalcPlayerCollisionPoint *center,
        float radius,
        float radiusGrowth,
        int value,
        int lifetime,
        int delay);
};

struct FrontCalcSoundPlayerView
{
    void PlaySoundByIdx(int soundIndex, int pan);
};

struct FrontCalcView
{
    unsigned char unknown00000[0x08];
    AnmLoaded *frontAnm08;
    AnmVm baseVms00C[7];
    unsigned char unknown01288[0xE86C - 0x1288];
    FrontCalcVertex panelVerticesE86C[2][5];
    int panelColorsE934[2];
    int panelTimersE93C[2];
    FrontCalcMessageRuntimeView messageRuntimeE944;
    AnmVm dynamicVm106B4;
    int dynamicSprite10958;
    int transitionVmCount1095C;
    int transitionFrame10960;
    int transitionSide10964;
    AnmVm transitionVms10968[7];
    unsigned char unknown11BE4[0x11EA4 - 0x11BE4];
    int transitionStarted11EA4;
    int transitionState11EA8;
    AnmVm resultVm11EAC;
    AnmVm resultVm12150;
    AnmVm markerVm123F4;
    AnmVm markerVm12698;
};

typedef char FrontCalcSizeIs1293C[
    (sizeof(FrontCalcView) == 0x1293C) ? 1 : -1];
typedef char FrontCalcPanelsAtE86C[
    (offsetof(FrontCalcView, panelVerticesE86C) == 0xE86C) ? 1 : -1];
typedef char FrontCalcMessageAtE944[
    (offsetof(FrontCalcView, messageRuntimeE944) == 0xE944) ? 1 : -1];
typedef char FrontCalcDynamicVmAt106B4[
    (offsetof(FrontCalcView, dynamicVm106B4) == 0x106B4) ? 1 : -1];
typedef char FrontCalcTransitionAt1095C[
    (offsetof(FrontCalcView, transitionVmCount1095C) == 0x1095C) ? 1 : -1];
typedef char FrontCalcResultAt11EAC[
    (offsetof(FrontCalcView, resultVm11EAC) == 0x11EAC) ? 1 : -1];

extern FrontCalcGameManagerView g_GameManager;
extern FrontCalcSoundPlayerView g_SoundPlayer;
extern int g_FrontDynamicSprite;
extern int g_EndingLifecycleState;
extern unsigned char g_FrontMode0Done;
extern unsigned char g_FrontOtherModeDone;
extern int g_FrontRuntimeValues90[4];
extern int g_GameStageValue;
extern int g_TitleNameTableIndex;
extern int g_SelectedOpponentParameter;
extern int g_TransitionResultTable[];
extern int g_TransitionResultTableAlt[];
extern void *g_NeutralMessageSource;
extern FrontCalcFrontSideView *g_FrontSides[2];

struct GameManagerSetupLayout
{
    static void CleanupGameplayState();
};
int __fastcall FrontCalcCallback(void *frontArg)
{
    FrontCalcView *front = static_cast<FrontCalcView *>(frontArg);

    g_AnmManager->ExecuteScript(&front->baseVms00C[0]);
    g_AnmManager->ExecuteScript(&front->baseVms00C[1]);
    g_AnmManager->ExecuteScript(&front->baseVms00C[2]);
    g_AnmManager->ExecuteScript(&front->baseVms00C[3]);
    g_AnmManager->ExecuteScript(&front->baseVms00C[4]);
    g_AnmManager->ExecuteScript(&front->baseVms00C[5]);
    g_AnmManager->ExecuteScript(&front->baseVms00C[6]);
    g_AnmManager->ExecuteScript(&front->resultVm11EAC);
    g_AnmManager->ExecuteScript(&front->resultVm12150);
    g_AnmManager->ExecuteScript(&front->dynamicVm106B4);

    if (front->dynamicSprite10958 != g_FrontDynamicSprite)
    {
        front->dynamicSprite10958 = g_FrontDynamicSprite;
        if (g_FrontDynamicSprite >= 0)
        {
            front->frontAnm08->ExecuteAnmIdx(&front->dynamicVm106B4, 87);
            front->frontAnm08->SetSprite(
                &front->dynamicVm106B4, g_FrontDynamicSprite + 82);
        }
    }

    unsigned long *diffuse = &front->panelVerticesE86C[0][0].diffuse;
    int *color = &front->panelColorsE934[0];
    unsigned long *middleDiffuse = &front->panelVerticesE86C[0][1].diffuse;

    int sideCount = 2;
    do
    {
        int timer = color[2];
        if (timer > 0)
        {
            if ((timer & 1) != 0)
            {
                middleDiffuse[-5] = *color;
                middleDiffuse[0] = *color;
                middleDiffuse[5] = *color;
                middleDiffuse[10] = *color;
                middleDiffuse[15] = *color;
            }
            else
            {
                unsigned long *cursor = diffuse;
                int count = 5;
                do
                {
                    *cursor = 0xFFFFFFFFUL;
                    cursor += sizeof(FrontCalcVertex) / sizeof(unsigned long);
                }
                while (--count != 0);
            }
            --color[2];
        }
        else
        {
            unsigned long *cursor = diffuse;
            int count = 5;
            do
            {
                *cursor = 0x80FFFFFFUL;
                cursor += sizeof(FrontCalcVertex) / sizeof(unsigned long);
            }
            while (--count != 0);
        }

        ++color;
        middleDiffuse += 25;
        diffuse += 25;
    }
    while (--sideCount != 0);

    front->messageRuntimeE944.Update();

    if (front->transitionVmCount1095C == 0)
        return 1;

    g_AnmManager->ExecuteScript(&front->transitionVms10968[0]);
    g_AnmManager->ExecuteScript(&front->transitionVms10968[1]);
    g_AnmManager->ExecuteScript(&front->transitionVms10968[2]);

    if (front->transitionVmCount1095C >= 2)
    {
        g_AnmManager->ExecuteScript(&front->transitionVms10968[3]);
        g_AnmManager->ExecuteScript(&front->transitionVms10968[4]);
        g_AnmManager->ExecuteScript(&front->transitionVms10968[5]);
        g_AnmManager->ExecuteScript(&front->transitionVms10968[6]);

        if (front->messageRuntimeE944.currentIndex08 < 0)
        {
            if (g_GameManager.IsGameMode0())
            {
                if (front->transitionSide10964 != 0)
                {
                    g_FrontMode0Done = 1;
                }
                else if (g_GameStageValue < 8)
                {
                    g_EndingLifecycleState = 3;
                }
                else
                {
                    g_EndingLifecycleState =
                        g_GameManager.IsReplayNeutral() ? 1 : 9;
                }
            }
            else if (g_GameManager.IsGameMode1())
            {
                if (front->transitionSide10964 != 0)
                {
                    g_FrontMode0Done = 1;
                }
                else if (g_GameStageValue < 8)
                {
                    g_EndingLifecycleState = 3;
                }
                else
                {
                    g_EndingLifecycleState =
                        g_GameManager.IsReplayNeutral() ? 1 : 9;
                }
            }
            else
            {
                g_FrontOtherModeDone = 1;
            }
        }

        ++front->transitionFrame10960;
        return 1;
    }

    int frame = front->transitionFrame10960;
    if (frame == 0)
    {
        unsigned int targetBit =
            (1u << (1 - front->transitionSide10964)) << 11;
        g_GameManager.flags134 ^=
            (g_GameManager.flags134 ^ targetBit) & 0x1800u;

        g_GameManager.sides[1 - front->transitionSide10964]
            .transitionOwner14->timer98 = 0;

        FrontCalcPlayerCollisionPoint point;
        point.x = 0.0f;
        point.y = 224.0f;
        point.z = 0.0f;

        reinterpret_cast<FrontCalcCollisionView *>(
            g_GameManager.sides[0].player04)->CreateCircleType1(
                &point, 500.0f, 0.0f, 30000, 0);
        reinterpret_cast<FrontCalcCollisionView *>(
            g_GameManager.sides[0].player04)->CreateCircleType4(
                &point, 500.0f, 0.0f, 200, 30000, 0);
        reinterpret_cast<FrontCalcCollisionView *>(
            g_GameManager.sides[1].player04)->CreateCircleType1(
                &point, 500.0f, 0.0f, 30000, 0);
        reinterpret_cast<FrontCalcCollisionView *>(
            g_GameManager.sides[1].player04)->CreateCircleType4(
                &point, 500.0f, 0.0f, 200, 30000, 0);

        g_GameManager.sides[0].player04->addedOwner30410.Reset();
        g_GameManager.sides[1].player04->addedOwner30410.Reset();
    }
    else if (frame == 30)
    {
        g_SoundPlayer.PlaySoundByIdx(49, 0);

        g_GameManager.sides[1 - front->transitionSide10964]
            .stateObject00->field1C = 1;
        g_GameManager.sides[1 - front->transitionSide10964]
            .stateObject00->field20 = 30;

        ScreenEffect::RegisterChain(
            SCREEN_EFFECT_SHAKE,
            60, 16, 0, 0, 35,
            1 - front->transitionSide10964);
        ScreenEffect::RegisterChain(
            SCREEN_EFFECT_SHAKE,
            60, 4, 0, 0, 35,
            front->transitionSide10964);
    }
    else if (frame == 60)
    {
        if (!g_GameManager.IsGameMode2())
        {
            int side = front->transitionSide10964;
            if (side != 0 &&
                g_GameManager.sides[0].runtime1C->value00 > 0.0f)
            {
                g_GameManager.sides[0].runtime1C->value00 -= 1.0f;
                g_GameManager.sides[0].runtime1C->value34 += 1.0f;
                front->transitionStarted11EA4 = 1;
            }
            else
            {
                short index;
                if (side == 0)
                {
                    index = *reinterpret_cast<short *>(
                        static_cast<unsigned char *>(g_NeutralMessageSource) + 8);
                }
                else
                {
                    index = *reinterpret_cast<short *>(
                        static_cast<unsigned char *>(g_NeutralMessageSource) + 10);
                }
                front->messageRuntimeE944.Setup(index, 0);
            }
        }
        else if (
            g_FrontRuntimeValues90[2] >= g_FrontRuntimeValues90[1] ||
            g_FrontRuntimeValues90[3] >= g_FrontRuntimeValues90[1])
        {
            front->messageRuntimeE944.SetupRandomForSide(
                front->transitionSide10964);
        }
        else
        {
            front->transitionStarted11EA4 = 1;
        }
    }
    else if (
        front->transitionStarted11EA4 != 0 && frame == 160)
    {
        reinterpret_cast<FrontCalcFrontSideView *>(
            g_GameManager.sides[0].frontSide18)->BeginAuxTransition();
        reinterpret_cast<FrontCalcFrontSideView *>(
            g_GameManager.sides[1].frontSide18)->BeginAuxTransition();
    }
    else if (
        front->transitionStarted11EA4 != 0 && frame == 190)
    {
        GameManagerSetupLayout::CleanupGameplayState();

        if (g_GameManager.IsGameMode0() ||
            g_GameManager.IsGameMode1())
        {
            int modeValue = g_FrontRuntimeValues90[0];
            if (modeValue >= 4 &&
                (g_GameManager.flags134 & 0x4000u) != 0)
            {
                int index =
                    45 * g_TitleNameTableIndex +
                    5 * g_GameStageValue;
                g_SelectedOpponentParameter =
                    g_TransitionResultTableAlt[index];
            }
            else
            {
                int index =
                    45 * g_TitleNameTableIndex +
                    5 * g_GameStageValue +
                    modeValue;
                g_SelectedOpponentParameter =
                    g_TransitionResultTable[index];
            }
        }
    }

    ++front->transitionFrame10960;
    return 1;
}
