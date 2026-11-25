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
#include "GameManagerSetupView.hpp"
#include "Supervisor.hpp"

#include <stddef.h>

extern AsciiGameManagerView g_GameManager;
extern int g_SoundPlayerBgmValue;
extern int g_OpponentParameterTable[];

// Two consecutive 16-bit script selections in the real file-backed
// neutral-message record. Their byte offsets are observed at the call sites.
struct FrontNeutralMessageVariantView
{
    unsigned char header00[8];
    short selection08;
    short selection0A;
};
typedef char FrontNeutralMessageVariantNativeOffsets[
    (sizeof(FrontNeutralMessageVariantView) == 0x0C &&
     offsetof(FrontNeutralMessageVariantView, selection08) == 0x08 &&
     offsetof(FrontNeutralMessageVariantView, selection0A) == 0x0A) ? 1 : -1];

// The native Front allocation owns two 0x64-byte panel rows containing
// five 0x14-byte vertices each; colors/timers and result-table entries
// are independent DWORDs. Retain the original signed selector arithmetic
// and call ordering without adding a bounds guard or shadow storage.
#include "FrontNativePanelVertices.hpp"
#include "FrontNativePanelState.hpp"
// The GameManager's native 0x38-byte side records and result-table
// entries are already declared as C++ arrays with the original strides.
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

    if (g_SoundPlayerBgmValue != front->dynamicSprite10958)
    {
        front->dynamicSprite10958 = g_SoundPlayerBgmValue;
        if (g_SoundPlayerBgmValue >= 0)
        {
            front->frontAnm08->ExecuteAnmIdx(&front->vm106B4, 87);
            front->frontAnm08->SetSprite(
                &front->vm106B4,
                static_cast<int>(static_cast<unsigned int>(g_SoundPlayerBgmValue) + 82u));
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
            --FRONT_NATIVE_TIMER(front, side);
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
                    if (g_GameManager.field0FC >= 8)
                    {
                        g_Supervisor.curState590 =
                            g_GameManager.IsReplayNeutral() ? 1 : 9;
                    }
                    else
                    {
                        g_Supervisor.curState590 = 3;
                    }
                }
                else
                {
                    g_GameManager.menuBlock13D = 1;
                }
            }
            else if (g_GameManager.IsGameMode1())
            {
                if (front->transitionSide10964 == 0)
                {
                    if (g_GameManager.field0FC >= 8)
                    {
                        g_Supervisor.curState590 =
                            g_GameManager.IsReplayNeutral() ? 1 : 9;
                    }
                    else
                    {
                        g_Supervisor.curState590 = 3;
                    }
                }
                else
                {
                    g_GameManager.menuBlock13D = 1;
                }
            }
            else
            {
                g_GameManager.menuBlock13E = 1;
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

            g_GameManager.sides[static_cast<int>(1u - static_cast<unsigned int>(front->transitionSide10964))]
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
            g_SoundPlayer.PlaySoundByIdx(SOUND_PLAYER_DEFEATED, 0);

            g_GameManager.sides[static_cast<int>(1u - static_cast<unsigned int>(front->transitionSide10964))]
                .background00->transitionMode = 1;
            g_GameManager.sides[static_cast<int>(1u - static_cast<unsigned int>(front->transitionSide10964))]
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
                    // The two script selections are real 16-bit words
                    // of the file-backed neutral-message record.
                    front->messageRuntimeE944.Setup(
                        reinterpret_cast<const FrontNeutralMessageVariantView *>(
                            g_GameManager.neutralMessageSourceF0)->selection08,
                        0);
                }
                else
                {
                    front->messageRuntimeE944.Setup(
                        reinterpret_cast<const FrontNeutralMessageVariantView *>(
                            g_GameManager.neutralMessageSourceF0)->selection0A,
                        0);
                }
            }
            else if (
                g_GameManager.runtimeValues100[2] >= g_GameManager.runtimeValues100[1] ||
                g_GameManager.runtimeValues100[3] >= g_GameManager.runtimeValues100[1])
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
                int modeValue = g_GameManager.runtimeValues100[0];
                if (modeValue >= 4 &&
                    (g_GameManager.flags & 0x4000u) != 0)
                {
                    g_GameManager.sides[1].selectedParameter2C =
                        g_OpponentParameterTable[static_cast<int>(45u * static_cast<unsigned int>(g_GameManager.difficulty) +
                                             5u * static_cast<unsigned int>(g_GameManager.field0FC)) + 4];
                }
                else
                {
                    g_GameManager.sides[1].selectedParameter2C =
                        g_OpponentParameterTable[static_cast<int>(45u * static_cast<unsigned int>(g_GameManager.difficulty) +
                                             5u * static_cast<unsigned int>(g_GameManager.field0FC) +
                                             static_cast<unsigned int>(modeValue)) + 1];
                }
            }
        }

    }

    // Advance the native transition frame after its signed state checks.
    ++front->transitionFrame10960;
    return 1;
}

#undef FRONT_NATIVE_VERTEX
#undef FRONT_NATIVE_TIMER
#undef FRONT_NATIVE_COLOR
