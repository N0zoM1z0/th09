// SPDX-License-Identifier: GPL-3.0-only
// Control-flow structure adapted from Cirno-Labs/th09 src/player.cpp,
// Player::UpdateShotInput, d7653152becabb8c86f2f43d49015ecc11bd0652.
// GPL-3.0-only; see THIRD_PARTY_NOTICES.md. Native ABI/types remain TH09-bound.
#include "CardAttackStartView.hpp"
#include "EnemyManager.hpp"
#include "FrontSide.hpp"
#include "PlayerLifecycleView.hpp"
#include "ReplayInputState.hpp"
#include "ZunTimer.hpp"
#include "SoundPlayer.hpp"
#include "GameConfiguration.hpp"
#include "GameManagerRuntimeView.hpp"
#include "GameplaySetupNativeSides.hpp"
// The original g_GameConfiguration pointer has its own canonical symbol at
// 0x004A7E78, the same address as the GameManager's +0xE8 pointer field.
// Its independent FrontResultBonus caller binds this actual global directly.
extern GameConfiguration *g_GameConfiguration;
#include "Supervisor.hpp"

#include <stddef.h>

typedef unsigned char u8;
typedef unsigned short u16;

typedef char PlayerChargeConfigurationModeAtB4[
    (offsetof(GameConfiguration, sideFocusMode) == 0xB4) ? 1 : -1];
typedef char PlayerChargePlayableSideCountIsTwo[
    (GameplaySetupNativeSides::PlayableCount == 2) ? 1 : -1];

// Player and FrontSide use the shared 0x38-byte side allocation.
int PlayerLifecycleView::UpdateChargeAttack()
{
    // PlayerRegisterChain is only called from GameManagerSetup's two-side
    // setup loop; the playable side index is therefore always 0 or 1.
    short side = static_cast<short>(sideIndex);
    volatile u8 sideMode;
    u8 modeSnapshot;
    int previousLevel;
    // The original input receiver uses the real 0x8E-byte ReplayInputState
    // stride and this owner's signed-short side snapshot as a native DWORD.
    // Keep that snapshot and all original input calls in the same order.
    ReplayInputState *input = reinterpret_cast<ReplayInputState *>(
        reinterpret_cast<unsigned int>(&g_ReplayInputStates[0]) +
        static_cast<unsigned int>(static_cast<int>(side)) *
        sizeof(g_ReplayInputStates[0]));
    // The two focus-mode tests and the post-charge test read the genuine
    // configuration byte row through native 32-bit byte addresses. No new
    // side clamping or cached mode snapshot has been added.
    if ((input->WasPressed(1) &&
         !(*reinterpret_cast<const u8 *>(
            reinterpret_cast<unsigned int>(&g_GameConfiguration->sideFocusMode[0]) +
            static_cast<unsigned int>(static_cast<int>(side))) == 1 &&
           input->IsHeld(4))) ||
        ((modeSnapshot = *reinterpret_cast<const u8 *>(
            reinterpret_cast<unsigned int>(&g_GameConfiguration->sideFocusMode[0]) +
            static_cast<unsigned int>(static_cast<int>(side))), sideMode = modeSnapshot, modeSnapshot == 1) &&
         input->IsHeld(1) &&
         !input->IsHeld(4)))
    {
        if (timer303B0 == 0)
        {
            if (timer303A4 < 0)
            {
                timer303A4 = 0;
            }
            else if (timer303A4 >= 10)
            {
                timer303A4 -= 10;
            }
            else if (timer303A4 >= 5)
            {
                timer303A4 -= 5;
            }
            timer303B0 = 3;
            timer303BC = 1;
        }
        else if (timer303B0 > 0)
        {
            timer303B0--;
        }
        if (*reinterpret_cast<const u8 *>(
            reinterpret_cast<unsigned int>(&g_GameConfiguration->sideFocusMode[0]) +
            static_cast<unsigned int>(sideIndex)) != 1)
        {
            goto afterCharge;
        }
    }
    else if ((sideMode == 1 && input->IsHeld(4)) ||
             (sideMode == 0 && input->IsHeld(1)))
    {
        timer303BC++;
        if (timer303BC >= 10 && (flags1B80 & 4u) == 0)
        {
            if (scalar30384 == 0.0f)
            {
                g_SoundPlayer.PlaySoundByIdx(43, sideIndex != 0 ? 500 : -500);
                sideState->frontSide18->ActivateMeterVms();
            }
            previousLevel = static_cast<int>(scalar30384) / 100;
            scalar30384 +=
                g_Supervisor.framerateMultiplier5B8 * primaryShtFile->chargeRate24;
            if (scalar30384 >= scalar30388)
            {
                scalar30384 = scalar30388;
                timer3038C++;
                if (timer3038C >= 300)
                {
                    goto resolveAttack;
                }
            }
            else
            {
                timer3038C = 0;
            }
            if (previousLevel < static_cast<int>(scalar30384) / 100)
            {
                g_SoundPlayer.PlaySoundByIdx(50, sideIndex != 0 ? 500 : -500);
            }
        }
        goto afterCharge;
    }
resolveAttack:
    if (scalar30384 >= 100.0f)
    {
        flags1B80 |= 4u;
        timer30398 = 0;
        timer1B74 = 8;
        if (scalar30384 >= 400.0f &&
            opponentState->enemyManager10->FindActiveEnemyBySideCategory(3) == NULL)
        {
            reinterpret_cast<CardAttackStartView *>(
                opponentState->cardAttack14)->StartAttack(
                        2, 2, 0, primaryShtFile->modePath2);
            SubtractResourceClamped(300.0f);
            timer1B74 = 8;
            ConfigureState3Effect15();
        }
        else if (scalar30384 >= 300.0f)
        {
            reinterpret_cast<CardAttackStartView *>(
                opponentState->cardAttack14)->StartAttack(
                        1, 1, 0, primaryShtFile->modePath1);
            SubtractResourceClamped(200.0f);
            timer1B74 = 8;
            ConfigureState3Effect14();
        }
        else if (scalar30384 >= 200.0f)
        {
            reinterpret_cast<CardAttackStartView *>(
                opponentState->cardAttack14)->StartAttack(
                        0, 0, 0, primaryShtFile->modePath0);
            timer1B74 = 8;
            SubtractResourceClamped(100.0f);
            ConfigureState3Effect13();
        }
    }
    sideState->frontSide18->DeactivateMeterVms();
    timer303BC = -1;
    scalar30384 = 0.0f;
    if (timer303B0 > 0)
    {
        timer303B0--;
    }
afterCharge:
    if ((flags1B80 & 4u) != 0)
    {
        if (timer30398.HasTicked())
        {
            SpawnShots(static_cast<int>(timer30398), 1);
        }
        timer30398++;
        // Complete the timer call before reloading the SHT pointer.
        float shotTime = static_cast<float>(timer30398);
        PlayerShtFileView *sht = primaryShtFile;
        if (shotTime >= sht->shotDuration28)
        {
            timer30398 = 0;
            flags1B80 &= ~4u;
        }
    }
    if (timer303A4 >= 0)
    {
        if (timer303A4.HasTicked())
        {
            SpawnShots(static_cast<int>(timer303A4), 0);
        }
        timer303A4++;
        if (timer303A4 >= 15)
        {
            timer303A4 = -1;
        }
    }
    return 0;
}
