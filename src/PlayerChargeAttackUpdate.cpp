// SPDX-License-Identifier: GPL-3.0-only
// Control-flow structure adapted from Cirno-Labs/th09 src/player.cpp,
// Player::UpdateShotInput, d7653152becabb8c86f2f43d49015ecc11bd0652.
// GPL-3.0-only; see THIRD_PARTY_NOTICES.md. Native ABI/types remain TH09-bound.
#include "PlayerChargeAttackView.hpp"
#include "CardAttackStartView.hpp"
#include "PlayerResourceSubtractView.hpp"
#include "PlayerSpawnShotsView.hpp"
#include "EnemyManager.hpp"
#include "FrontSide.hpp"
#include "PlayerLifecycleView.hpp"
#include "PlayerStateEffectView.hpp"
#include "ReplayInputState.hpp"
#include "ZunTimer.hpp"
#include "SoundPlayer.hpp"
#include "GameConfiguration.hpp"
#include "GameManagerRuntimeView.hpp"
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

// The live 0x38-byte gameplay side is the very same GameManager side
// allocation used by Enemy/ECL and the original game setup factories.
// Preserve the existing Player-facing field names without a second struct.
typedef char PlayerChargeNativeSideStorage[
    (sizeof(PlayerSideStateView) == sizeof(AsciiGameManagerSideView) &&
     offsetof(PlayerSideStateView, attackTarget10) ==
         offsetof(AsciiGameManagerSideView, enemyManager10) &&
     offsetof(PlayerSideStateView, attackOwner14) ==
         offsetof(AsciiGameManagerSideView, cardAttack14) &&
     offsetof(PlayerSideStateView, frontSide18) ==
         offsetof(AsciiGameManagerSideView, frontSide18)) ? 1 : -1];

int PlayerChargeAttackView::UpdateChargeAttack()
{
    int side;
    u8 sideMode;
    int previousLevel;

    side = sideIndex;
    if ((g_ReplayInputStates[side].WasPressed(1) &&
         !(g_GameConfiguration->sideFocusMode[side] == 1 &&
           g_ReplayInputStates[side].IsHeld(4))) ||
        ((sideMode = g_GameConfiguration->sideFocusMode[side]) == 1 &&
         g_ReplayInputStates[side].IsHeld(1) &&
         !g_ReplayInputStates[side].IsHeld(4)))
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
        if (g_GameConfiguration->sideFocusMode[sideIndex] != 1)
        {
            goto afterCharge;
        }
    }
    else if ((sideMode == 1 && g_ReplayInputStates[side].IsHeld(4)) ||
             (sideMode == 0 && g_ReplayInputStates[side].IsHeld(1)))
    {
        timer303BC++;
        if (timer303BC >= 10 && (flags1B80 & 4u) == 0)
        {
            if (scalar30384 == 0.0f)
            {
                g_SoundPlayer.PlaySoundByIdx(43, sideIndex != 0 ? 500 : -500);
                reinterpret_cast<AsciiGameManagerSideView *>(sideState)
                    ->frontSide18->ActivateMeterVms();
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
            reinterpret_cast<AsciiGameManagerSideView *>(opponentState)
                ->enemyManager10->FindActiveEnemyBySideCategory(3) == NULL)
        {
            reinterpret_cast<CardAttackStartView *>(
                reinterpret_cast<AsciiGameManagerSideView *>(opponentState)
                    ->cardAttack14)->StartAttack(
                        2, 2, 0, primaryShtFile->modePath2);
            reinterpret_cast<PlayerResourceSubtractView *>(this)->SubtractResourceClamped(300.0f);
            timer1B74 = 8;
            reinterpret_cast<PlayerStateEffectView *>(this)->ConfigureState3Effect15();
        }
        else if (scalar30384 >= 300.0f)
        {
            reinterpret_cast<CardAttackStartView *>(
                reinterpret_cast<AsciiGameManagerSideView *>(opponentState)
                    ->cardAttack14)->StartAttack(
                        1, 1, 0, primaryShtFile->modePath1);
            reinterpret_cast<PlayerResourceSubtractView *>(this)->SubtractResourceClamped(200.0f);
            timer1B74 = 8;
            reinterpret_cast<PlayerStateEffectView *>(this)->ConfigureState3Effect14();
        }
        else if (scalar30384 >= 200.0f)
        {
            reinterpret_cast<CardAttackStartView *>(
                reinterpret_cast<AsciiGameManagerSideView *>(opponentState)
                    ->cardAttack14)->StartAttack(
                        0, 0, 0, primaryShtFile->modePath0);
            timer1B74 = 8;
            reinterpret_cast<PlayerResourceSubtractView *>(this)->SubtractResourceClamped(100.0f);
            reinterpret_cast<PlayerStateEffectView *>(this)->ConfigureState3Effect13();
        }
    }
    reinterpret_cast<AsciiGameManagerSideView *>(sideState)
        ->frontSide18->DeactivateMeterVms();
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
            reinterpret_cast<PlayerSpawnShotsView *>(this)
                ->SpawnShots(static_cast<int>(timer30398), 1);
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
            reinterpret_cast<PlayerSpawnShotsView *>(this)
                ->SpawnShots(static_cast<int>(timer303A4), 0);
        }
        timer303A4++;
        if (timer303A4 >= 15)
        {
            timer303A4 = -1;
        }
    }
    return 0;
}
