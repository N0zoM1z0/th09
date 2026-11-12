#include "AnmManager.hpp"
#include "SoundPlayer.hpp"
#include "AsciiManager.hpp"
#include "ScreenEffect.hpp"
#include "ZunTimer.hpp"
#include "FrontFactoryConstructView.hpp"
#include "FrontSide.hpp"
#include "Background.hpp"
#include "CardAttack.hpp"
#include "GameManagerRuntimeView.hpp"

#include <stddef.h>

extern AsciiGameManagerView g_GameManager;
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

struct GameManagerSetupLayout
{
    static void CleanupGameplayState();
};
// The native Front allocation owns two 0x64-byte panel rows containing
// five 0x14-byte vertices each; colors/timers and result-table entries
// are independent DWORDs. Preserve live signed selectors and original
// Win32 address words without adding a bounds guard or shadow storage.
#define FRONT_NATIVE_VERTEX(front, side, vertex) \
    (*reinterpret_cast<FrontPanelVertexView *>(reinterpret_cast<unsigned int>(&(front)->panelVerticesE86C[0][0]) + \
        static_cast<unsigned int>(side) * sizeof((front)->panelVerticesE86C[0]) + \
        static_cast<unsigned int>(vertex) * sizeof((front)->panelVerticesE86C[0][0])))
#define FRONT_NATIVE_TIMER(front, side) \
    (*reinterpret_cast<int *>(reinterpret_cast<unsigned int>(&(front)->panelTimersE93C[0]) + static_cast<unsigned int>(side) * sizeof((front)->panelTimersE93C[0])))
#define FRONT_NATIVE_COLOR(front, side) \
    (*reinterpret_cast<int *>(reinterpret_cast<unsigned int>(&(front)->panelColorsE934[0]) + static_cast<unsigned int>(side) * sizeof((front)->panelColorsE934[0])))
#define FRONT_NATIVE_SIDE(index) \
    (*reinterpret_cast<AsciiGameManagerSideView *>(reinterpret_cast<unsigned int>(&g_GameManager.sides[0]) + static_cast<unsigned int>(index) * sizeof(g_GameManager.sides[0])))
#define FRONT_NATIVE_RESULT(table, index) \
    (*reinterpret_cast<int *>(reinterpret_cast<unsigned int>(&(table)[0]) + static_cast<unsigned int>(index) * sizeof((table)[0])))

int __fastcall FrontCalcCallback(void *frontArg)
{
    FrontFactoryConstructView *front = static_cast<FrontFactoryConstructView *>(frontArg);

    g_AnmManager->ExecuteScript(&front->frontVms00C[0]);
    g_AnmManager->ExecuteScript(&front->frontVms00C[1]);
    g_AnmManager->ExecuteScript(&front->frontVms00C[2]);
    g_AnmManager->ExecuteScript(&front->frontVms00C[3]);
    g_AnmManager->ExecuteScript(&front->frontVms00C[4]);
    g_AnmManager->ExecuteScript(&front->frontVms00C[5]);
    g_AnmManager->ExecuteScript(&front->frontVms00C[6]);
    g_AnmManager->ExecuteScript(&front->resultVms11EAC[0]);
    g_AnmManager->ExecuteScript(&front->resultVms11EAC[1]);
    g_AnmManager->ExecuteScript(&front->vm106B4);

    if (g_FrontDynamicSprite != front->dynamicSprite10958)
    {
        front->dynamicSprite10958 = g_FrontDynamicSprite;
        if (g_FrontDynamicSprite >= 0)
        {
            front->frontAnm08->ExecuteAnmIdx(&front->vm106B4, 87);
            front->frontAnm08->SetSprite(
                &front->vm106B4,
                static_cast<int>(static_cast<unsigned int>(g_FrontDynamicSprite) + 82u));
        }
    }

    for (int side = 0; side < 2; ++side)
    {
        if (FRONT_NATIVE_TIMER(front, side) > 0)
        {
            if ((FRONT_NATIVE_TIMER(front, side) & 1) != 0)
            {
                for (int vertex = 0; vertex < 5; ++vertex)
                    FRONT_NATIVE_VERTEX(front, side, vertex).diffuse =
                        FRONT_NATIVE_COLOR(front, side);
            }
            else
            {
                for (int vertex = 0; vertex < 5; ++vertex)
                    FRONT_NATIVE_VERTEX(front, side, vertex).diffuse = 0xFFFFFFFFUL;
            }
            --*reinterpret_cast<unsigned int *>(&FRONT_NATIVE_TIMER(front, side));
        }
        else
        {
            for (int vertex = 0; vertex < 5; ++vertex)
                FRONT_NATIVE_VERTEX(front, side, vertex).diffuse = 0x80FFFFFFUL;
        }
    }

    front->messageRuntimeE944.Update();

    if (front->transitionVmCount1095C == 0)
        return 1;

    g_AnmManager->ExecuteScript(&front->resultVms10968[0]);
    g_AnmManager->ExecuteScript(&front->resultVms10968[1]);
    g_AnmManager->ExecuteScript(&front->resultVms10968[2]);

    if (front->transitionVmCount1095C >= 2)
    {
        g_AnmManager->ExecuteScript(&front->resultVms10968[3]);
        g_AnmManager->ExecuteScript(&front->resultVms10968[4]);
        g_AnmManager->ExecuteScript(&front->resultVms10968[5]);
        g_AnmManager->ExecuteScript(&front->resultVms10968[6]);

        if (front->messageRuntimeE944.currentIndex08 < 0)
        {
            if (g_GameManager.IsGameMode0())
            {
                if (front->transitionSide10964 == 0)
                {
                    if (g_GameStageValue >= 8)
                    {
                        g_EndingLifecycleState =
                            g_GameManager.IsReplayNeutral() ? 1 : 9;
                    }
                    else
                    {
                        g_EndingLifecycleState = 3;
                    }
                }
                else
                {
                    g_FrontMode0Done = 1;
                }
            }
            else if (g_GameManager.IsGameMode1())
            {
                if (front->transitionSide10964 == 0)
                {
                    if (g_GameStageValue >= 8)
                    {
                        g_EndingLifecycleState =
                            g_GameManager.IsReplayNeutral() ? 1 : 9;
                    }
                    else
                    {
                        g_EndingLifecycleState = 3;
                    }
                }
                else
                {
                    g_FrontMode0Done = 1;
                }
            }
            else
            {
                g_FrontOtherModeDone = 1;
            }
        }

    }
    else
    {
        int frame = front->transitionFrame10960;
        if (frame == 0)
        {
            unsigned int targetBit =
                (1u << (1 - front->transitionSide10964)) << 11;
            g_GameManager.flags ^=
                (g_GameManager.flags ^ targetBit) & 0x1800u;

            FRONT_NATIVE_SIDE(static_cast<int>(1u - static_cast<unsigned int>(front->transitionSide10964)))
                .cardAttack14->timer98 = 0;

            PlayerPositionView point;
            point.x = 0.0f;
            point.y = 224.0f;
            point.z = 0.0f;

            g_GameManager.sides[0].player04->CreateCircleType1(
                    reinterpret_cast<const PlayerRegionPointView *>(&point), 500.0f, 0.0f, 30000, 0);
            g_GameManager.sides[0].player04->CreateCircleType4(
                    reinterpret_cast<const PlayerRegionPointView *>(&point), 500.0f, 0.0f, 200, 30000, 0);
            g_GameManager.sides[1].player04->CreateCircleType1(
                    reinterpret_cast<const PlayerRegionPointView *>(&point), 500.0f, 0.0f, 30000, 0);
            g_GameManager.sides[1].player04->CreateCircleType4(
                    reinterpret_cast<const PlayerRegionPointView *>(&point), 500.0f, 0.0f, 200, 30000, 0);

            g_GameManager.sides[0].player04->ownerState30410.Reset();
            g_GameManager.sides[1].player04->ownerState30410.Reset();
        }
        else if (frame == 30)
        {
            g_SoundPlayer.PlaySoundByIdx(49, 0);

            FRONT_NATIVE_SIDE(static_cast<int>(1u - static_cast<unsigned int>(front->transitionSide10964)))
                .background00->transitionMode = 1;
            FRONT_NATIVE_SIDE(static_cast<int>(1u - static_cast<unsigned int>(front->transitionSide10964)))
                .background00->transitionFrames = 30;

            ScreenEffect::RegisterChain(
                SCREEN_EFFECT_SHAKE,
                60, 16, 0, 0, 35,
                static_cast<int>(
                    1u - static_cast<unsigned int>(front->transitionSide10964)));
            ScreenEffect::RegisterChain(
                SCREEN_EFFECT_SHAKE,
                60, 4, 0, 0, 35,
                front->transitionSide10964);
        }
        else if (frame == 60)
        {
            if (!g_GameManager.IsGameMode2())
            {
                if (front->transitionSide10964 != 0 &&
                    g_GameManager.sides[0].runtime->value00 > 0.0f)
                {
                    g_GameManager.sides[0].runtime->value00 -= 1.0f;
                    g_GameManager.sides[0].runtime->resultTransition34 += 1.0f;
                    front->transitionStarted11EA4 = 1;
                }
                else if (front->transitionSide10964 == 0)
                {
                    front->messageRuntimeE944.Setup(
                        *reinterpret_cast<short *>(
                            static_cast<unsigned char *>(g_NeutralMessageSource) + 8),
                        0);
                }
                else
                {
                    front->messageRuntimeE944.Setup(
                        *reinterpret_cast<short *>(
                            static_cast<unsigned char *>(g_NeutralMessageSource) + 10),
                        0);
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
            g_GameManager.sides[0].frontSide18->BeginAuxTransition();
            g_GameManager.sides[1].frontSide18->BeginAuxTransition();
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
                    (g_GameManager.flags & 0x4000u) != 0)
                {
                    g_SelectedOpponentParameter =
                        FRONT_NATIVE_RESULT(g_TransitionResultTableAlt,
                            static_cast<int>(45u * static_cast<unsigned int>(g_TitleNameTableIndex) +
                                             5u * static_cast<unsigned int>(g_GameStageValue)));
                }
                else
                {
                    g_SelectedOpponentParameter =
                        FRONT_NATIVE_RESULT(g_TransitionResultTable,
                            static_cast<int>(45u * static_cast<unsigned int>(g_TitleNameTableIndex) +
                                             5u * static_cast<unsigned int>(g_GameStageValue) +
                                             static_cast<unsigned int>(modeValue)));
                }
            }
        }

    }

    // The original transition frame advances as an i386 DWORD, with a
    // signed state comparison performed before the increment.
    ++*reinterpret_cast<unsigned int *>(&front->transitionFrame10960);
    return 1;
}

#undef FRONT_NATIVE_VERTEX
#undef FRONT_NATIVE_TIMER
#undef FRONT_NATIVE_COLOR
#undef FRONT_NATIVE_SIDE
#undef FRONT_NATIVE_RESULT
