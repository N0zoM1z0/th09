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
// Only the native global identity is needed here.
struct AsciiGameManagerView;
extern AsciiGameManagerView g_GameManager;
#include "Supervisor.hpp"

#include <stddef.h>

typedef unsigned char u8;
typedef unsigned short u16;

// Observed GameManager +E8 points to the current GameConfiguration.
struct GameManagerChargeConfigurationView
{
    u8 unknown000[0xE8];
    GameConfiguration *configurationE8;
};
typedef char PlayerChargeConfigurationAtE8[
    (offsetof(GameManagerChargeConfigurationView, configurationE8) == 0xE8) ? 1 : -1];
typedef char PlayerChargeConfigurationModeAtB4[
    (offsetof(GameConfiguration, sideFocusMode) == 0xB4) ? 1 : -1];

struct PlayerChargeSideView
{
    void *state00;
    void *player04;
    u8 unknown08[8];
    EnemyManagerView *enemyManager10;
    void *attackOwner14;
    FrontSide *frontSide18;
    u8 unknown1C[0x1C];
};
typedef char PlayerChargeSideSizeIs38[
    (sizeof(PlayerChargeSideView) == 0x38) ? 1 : -1];

int PlayerChargeAttackView::UpdateChargeAttack()
{
    int side = sideIndex;
    u8 sideMode;

    if ((g_ReplayInputStates[side].WasPressed(1) &&
         (reinterpret_cast<const GameManagerChargeConfigurationView *>(&g_GameManager)->configurationE8->sideFocusMode[side] != 1 ||
          !g_ReplayInputStates[side].IsHeld(4))) ||
        ((sideMode = reinterpret_cast<const GameManagerChargeConfigurationView *>(&g_GameManager)->configurationE8->sideFocusMode[side]) == 1 &&
         g_ReplayInputStates[side].IsHeld(1) &&
         !g_ReplayInputStates[side].IsHeld(4)))
    {
        ZunTimer *chargeDelay =
            reinterpret_cast<ZunTimer *>(&timer303B0);
        if (*chargeDelay == 0)
        {
            ZunTimer *normalShotTimer =
                reinterpret_cast<ZunTimer *>(&timer303A4);
            if (*normalShotTimer < 0)
                *normalShotTimer = 0;
            else if (*normalShotTimer >= 10)
                *normalShotTimer -= 10;
            else if (*normalShotTimer >= 5)
                *normalShotTimer -= 5;

            *chargeDelay = 3;
            *reinterpret_cast<ZunTimer *>(&timer303BC) = 1;
        }
        else if (*chargeDelay > 0)
        {
            (*chargeDelay)--;
        }

        // The post-timer gate reloads the player side; the initial input
        // disjunction intentionally keeps its entry-side snapshot.
        if (reinterpret_cast<const GameManagerChargeConfigurationView *>(&g_GameManager)->configurationE8->sideFocusMode[sideIndex] == 1)
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
            reinterpret_cast<ZunTimer *>(&timer303BC);
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
        reinterpret_cast<PlayerChargeSideView *>(sideState)
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
                reinterpret_cast<ZunTimer *>(&timer3038C);
            (*fullTimer)++;
            if (*fullTimer >= 300)
                goto resolveAttack;
        }
        else
        {
            *reinterpret_cast<ZunTimer *>(&timer3038C) = 0;
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
        *reinterpret_cast<ZunTimer *>(&timer30398) = 0;
        *reinterpret_cast<ZunTimer *>(&timer1B74) = 8;

        if (scalar30384 >= 400.0f &&
            reinterpret_cast<PlayerChargeSideView *>(opponentState)
                ->enemyManager10->FindActiveEnemyBySideCategory(3) == 0)
        {
            reinterpret_cast<CardAttackStartView *>(
                reinterpret_cast<PlayerChargeSideView *>(opponentState)
                    ->attackOwner14)->StartAttack(
                        2, 2, 0,
                        primaryShtFile->modePath2);
            reinterpret_cast<PlayerResourceSubtractView *>(this)
                ->SubtractResourceClamped(300.0f);
            *reinterpret_cast<ZunTimer *>(&timer1B74) = 8;
            reinterpret_cast<PlayerStateEffectView *>(this)
                ->ConfigureState3Effect15();
        }
        else if (scalar30384 >= 300.0f)
        {
            reinterpret_cast<CardAttackStartView *>(
                reinterpret_cast<PlayerChargeSideView *>(opponentState)
                    ->attackOwner14)->StartAttack(
                        1, 1, 0,
                        primaryShtFile->modePath1);
            reinterpret_cast<PlayerResourceSubtractView *>(this)
                ->SubtractResourceClamped(200.0f);
            *reinterpret_cast<ZunTimer *>(&timer1B74) = 8;
            reinterpret_cast<PlayerStateEffectView *>(this)
                ->ConfigureState3Effect14();
        }
        else if (scalar30384 >= 200.0f)
        {
            reinterpret_cast<CardAttackStartView *>(
                reinterpret_cast<PlayerChargeSideView *>(opponentState)
                    ->attackOwner14)->StartAttack(
                        0, 0, 0,
                        primaryShtFile->modePath0);
            *reinterpret_cast<ZunTimer *>(&timer1B74) = 8;
            reinterpret_cast<PlayerResourceSubtractView *>(this)
                ->SubtractResourceClamped(100.0f);
            reinterpret_cast<PlayerStateEffectView *>(this)
                ->ConfigureState3Effect13();
        }
    }

    reinterpret_cast<PlayerChargeSideView *>(sideState)
        ->frontSide18->DeactivateMeterVms();
    *reinterpret_cast<ZunTimer *>(&timer303BC) = -1;
    scalar30384 = 0.0f;

    if (*reinterpret_cast<ZunTimer *>(&timer303B0) > 0)
        (*reinterpret_cast<ZunTimer *>(&timer303B0))--;

afterCharge:
    if ((flags1B80 & 4u) != 0)
    {
shooting:
        ZunTimer *shotTimer =
            reinterpret_cast<ZunTimer *>(&timer30398);
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
            reinterpret_cast<ZunTimer *>(&timer303A4);
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
