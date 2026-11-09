#include "RuntimePolarMath.hpp"
#include "AsciiManager.hpp"
#include "PlayerLifecycleView.hpp"
#include "RngRuntimeLeaves.hpp"
#include "FrontFactoryConstructView.hpp"
#include "GameManagerRuntimeView.hpp"
#include "FrontSide.hpp"
#include "EffectManager.hpp"
#include "SoundPlayer.hpp"

#include <stddef.h>

// The original side allocation at +0x0C/+0x10/+0x14/+0x18 is the
// constructed 0x38-byte AsciiGameManagerSideView, not a second side.
struct PlayerDeathAttackTargetView
{
    int CheckMode(int mode);
};

struct PlayerDeathAttackOwnerView
{
    void Configure(int mode0, int mode1, int zero, const char *path);
};

struct PlayerDeathCounterView
{
    void Add(int value);
};

extern int g_PlayerDeathCounter;
extern int g_PlayerDeathCounterBonus;
extern PlayerDeathCounterView g_PlayerDeathCounterState;
extern RngRuntimeView g_ReplayRng;

void PlayerLifecycleView::EnterDeathState()
{
    if (g_GameManager.frontFactoryA8->GetTransitionBlockFlag())
        return;

    g_SoundPlayer.PlaySoundPositionedByIdx(4, position1B88.x);

    int counterStep = g_PlayerDeathCounter;
    if (selector != 0)
        counterStep += g_PlayerDeathCounterBonus;

    if (header24.state84 <= 1)
    {
        if (sideState->shotType20 == 8 &&
            scalar30388 >= 200.0f)
        {
            if (scalar30388 >= 400.0f)
            {
                if (!reinterpret_cast<PlayerDeathAttackTargetView *>(opponentState->attackTarget10)->CheckMode(3))
                {
                    reinterpret_cast<PlayerDeathAttackOwnerView *>(opponentState->attackOwner14)->Configure(2, 2, 0, primaryShtFile->modePath2);
                    scalar30384 = 0.0f;
                    scalar30388 = 0.0f;
                    SetBombMode2();
                    return;
                }

                reinterpret_cast<PlayerDeathAttackOwnerView *>(opponentState->attackOwner14)->Configure(1, 1, 0, primaryShtFile->modePath1);
                scalar30384 = 100.0f;
                scalar30388 = 100.0f;
                SetBombMode1();
                return;
            }
            if (scalar30388 >= 300.0f)
            {
                reinterpret_cast<PlayerDeathAttackOwnerView *>(opponentState->attackOwner14)->Configure(1, 1, 0, primaryShtFile->modePath1);
                scalar30384 = 0.0f;
                scalar30388 = 0.0f;
                SetBombMode1();
                return;
            }

            if (scalar30388 >= 200.0f)
            {
                reinterpret_cast<PlayerDeathAttackOwnerView *>(opponentState->attackOwner14)->Configure(0, 0, 0, primaryShtFile->modePath0);
                scalar30384 = 0.0f;
                scalar30388 = 0.0f;
                SetBombMode0();
                return;
            }
            return;
        }

        header24.state84 = 0;
        g_GameManager.frontFactoryA8->SelectOpponentSide(1 - sideIndex);
        sideState->effectManager0C->SpawnEffectInFixedSlot(
            19, reinterpret_cast<const EffectFloat3 *>(&position1B88),
            4, 0xFF4040FFu);
    }
    else
    {
        int remaining = header24.state84 - counterStep;
        if (remaining > 1)
        {
            header24.state84 = remaining;
        }
        else
        {
            g_SoundPlayer.PlaySoundByIdx(48, sideIndex != 0 ? 500 : -500);
            sideState->frontSide18->ClearState(0);
            header24.state84 = 1;
        }
    }

    updateState00 = 4;
    timer303C8 = 0;

    float angle = g_ReplayRng.GetRandomF32SignedInRange(3.1415927f);
    movementFields1CCC.movementAngle1CD8 = angle;
    SetPolar2D(reinterpret_cast<Float2 *>(reinterpret_cast<Float3 *>(&movementFields1CCC.velocity1CCC)), angle, 1.0f);

    g_GameManager.frontFactoryA8->panelTimersE93C[sideIndex] = 60;
    g_GameManager.frontFactoryA8->panelColorsE934[sideIndex] = 0x80FF0000u;

    Effect *vmHandle = focusEffect364;
    if (vmHandle != NULL)
    {
        vmHandle->vms->SetInterrupt(1);
        Effect *active = secondaryEffect368;
        focusEffect364 = NULL;
        active->active = 0;
        secondaryEffect368 = NULL;
    }

    g_PlayerDeathCounterState.Add(1500);
    if (g_PlayerDeathCounter > 4)
        g_PlayerDeathCounter -= 2;
    else if (g_PlayerDeathCounter > 2)
        --g_PlayerDeathCounter;

    ownerState30410.Reset();
    header24.Reset();
}
