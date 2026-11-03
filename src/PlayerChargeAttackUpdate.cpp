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
    int side = sideIndex;
    u8 sideMode;

    if ((g_ReplayInputStates[side].WasPressed(1) &&
         (g_GameConfiguration->sideFocusMode[side] != 1 ||
          !g_ReplayInputStates[side].IsHeld(4))) ||
        ((sideMode = g_GameConfiguration->sideFocusMode[side]) == 1 &&
         g_ReplayInputStates[side].IsHeld(1) &&
         !g_ReplayInputStates[side].IsHeld(4)))
    {
        ZunTimer *chargeDelay =
            &timer303B0;
        if (*chargeDelay == 0)
        {
            ZunTimer *normalShotTimer =
                &timer303A4;
            if (*normalShotTimer < 0)
                *normalShotTimer = 0;
            else if (*normalShotTimer >= 10)
                *normalShotTimer -= 10;
            else if (*normalShotTimer >= 5)
                *normalShotTimer -= 5;

            *chargeDelay = 3;
            timer303BC = 1;
        }
        else if (*chargeDelay > 0)
        {
            (*chargeDelay)--;
        }

        // The post-timer gate reloads the player side; the initial input
        // disjunction intentionally keeps its entry-side snapshot.
        if (g_GameConfiguration->sideFocusMode[sideIndex] == 1)
            goto resolveAttack;
        goto afterCharge;
    }

    if ((sideMode != 1 || !g_ReplayInputStates[side].IsHeld(4)) &&
        (sideMode != 0 || !g_ReplayInputStates[side].IsHeld(1)))
    {
        goto resolveAttack;
    }

    {
        ZunTimer *holdTimer =
            &timer303BC;
        (*holdTimer)++;
        if (!(*holdTimer >= 10))
            goto afterCharge;
    }

    if ((flags1B80 & 4u) != 0)
        goto shooting;

    if (scalar30384 == 0.0f)
    {
        g_SoundPlayer.PlaySoundByIdx(
            43, sideIndex != 0 ? 500 : -500);
        reinterpret_cast<AsciiGameManagerSideView *>(sideState)
            ->frontSide18->ActivateMeterVms();
    }

    {
        int previousLevel = static_cast<int>(scalar30384) / 100;
        PlayerShtFileView *sht = primaryShtFile;

        scalar30384 +=
            g_Supervisor.framerateMultiplier5B8 *
            sht->chargeRate24;

        if (scalar30384 >= scalar30388)
        {
            scalar30384 = scalar30388;
            ZunTimer *fullTimer =
                &timer3038C;
            (*fullTimer)++;
            if (*fullTimer >= 300)
                goto resolveAttack;
        }
        else
        {
            timer3038C = 0;
        }

        int currentLevel = static_cast<int>(scalar30384) / 100;
        if (previousLevel < currentLevel)
        {
            g_SoundPlayer.PlaySoundByIdx(
                50, sideIndex != 0 ? 500 : -500);
        }
    }
    goto afterCharge;

resolveAttack:
    if (scalar30384 >= 100.0f)
    {
        flags1B80 |= 4u;
        timer30398 = 0;
        timer1B74 = 8;

        if (scalar30384 >= 400.0f &&
            reinterpret_cast<AsciiGameManagerSideView *>(opponentState)
                ->enemyManager10->FindActiveEnemyBySideCategory(3) == 0)
        {
            reinterpret_cast<CardAttackStartView *>(
                reinterpret_cast<AsciiGameManagerSideView *>(opponentState)
                    ->cardAttack14)->StartAttack(
                        2, 2, 0,
                        primaryShtFile->modePath2);
            reinterpret_cast<PlayerResourceSubtractView *>(this)
                ->SubtractResourceClamped(300.0f);
            timer1B74 = 8;
            reinterpret_cast<PlayerStateEffectView *>(this)
                ->ConfigureState3Effect15();
        }
        else if (scalar30384 >= 300.0f)
        {
            reinterpret_cast<CardAttackStartView *>(
                reinterpret_cast<AsciiGameManagerSideView *>(opponentState)
                    ->cardAttack14)->StartAttack(
                        1, 1, 0,
                        primaryShtFile->modePath1);
            reinterpret_cast<PlayerResourceSubtractView *>(this)
                ->SubtractResourceClamped(200.0f);
            timer1B74 = 8;
            reinterpret_cast<PlayerStateEffectView *>(this)
                ->ConfigureState3Effect14();
        }
        else if (scalar30384 >= 200.0f)
        {
            reinterpret_cast<CardAttackStartView *>(
                reinterpret_cast<AsciiGameManagerSideView *>(opponentState)
                    ->cardAttack14)->StartAttack(
                        0, 0, 0,
                        primaryShtFile->modePath0);
            timer1B74 = 8;
            reinterpret_cast<PlayerResourceSubtractView *>(this)
                ->SubtractResourceClamped(100.0f);
            reinterpret_cast<PlayerStateEffectView *>(this)
                ->ConfigureState3Effect13();
        }
    }

    reinterpret_cast<AsciiGameManagerSideView *>(sideState)
        ->frontSide18->DeactivateMeterVms();
    timer303BC = -1;
    scalar30384 = 0.0f;

    if (timer303B0 > 0)
        (timer303B0)--;

afterCharge:
    if ((flags1B80 & 4u) != 0)
    {
shooting:
        ZunTimer *shotTimer =
            &timer30398;
        if (shotTimer->HasTicked())
        {
            reinterpret_cast<PlayerSpawnShotsView *>(this)
                ->SpawnShots(
                    static_cast<int>(*shotTimer), 1);
        }

        (*shotTimer)++;
        // Finish the timer conversion before reacquiring the SHT record.
        // This preserves the target's load order across the out-of-line call.
        float shotTime = static_cast<float>(*shotTimer);
        PlayerShtFileView *sht = primaryShtFile;
        if (shotTime >= sht->shotDuration28)
        {
            *shotTimer = 0;
            flags1B80 &= ~4u;
        }
    }

    {
        ZunTimer *normalShotTimer =
            &timer303A4;
        if (*normalShotTimer >= 0)
        {
            if (normalShotTimer->HasTicked())
            {
                reinterpret_cast<PlayerSpawnShotsView *>(this)
                    ->SpawnShots(
                        static_cast<int>(*normalShotTimer), 0);
            }

            (*normalShotTimer)++;
            if (*normalShotTimer >= 15)
                *normalShotTimer = -1;
        }
    }

    return 0;
}
