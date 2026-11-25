// SPDX-License-Identifier: GPL-3.0-only
// Control-flow structure adapted from Cirno-Labs/th09 src/player.cpp,
// Player::UpdateShotInput, d7653152becabb8c86f2f43d49015ecc11bd0652.
// GPL-3.0-only; see THIRD_PARTY_NOTICES.md. Native ABI/types remain TH09-bound.
#include "CardAttack.hpp"
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

// The charge mode is the native two-side row at GameConfiguration +0xB4.
// The caller only supplies registered playable sides 0 and 1.

// TH09's charge input uses two documented states of the per-side focus row.
// Mode zero charges from held shot input; mode one charges from held focus
// input while accounting for the other key's state. The retail identifier
// spelling is not known, but both values and input choices are target-bound.
enum PlayerChargeFocusMode
{
    PLAYER_CHARGE_FROM_SHOT_INPUT = 0,
    PLAYER_CHARGE_FROM_FOCUS_INPUT = 1
};

// Exact TH09 input samplers bind Z to bit 0 and Focus/Shift to bit 2.
// These masks are independent from the configuration's 0/1 charge modes.
enum PlayerChargeInputButton
{
    PLAYER_CHARGE_BUTTON_SHOOT = 0x0001,
    PLAYER_CHARGE_BUTTON_FOCUS = 0x0004
};


// TH09's charge meter is measured in 100-point tiers. The retail owner
// starts a release at 100, exposes progressively stronger attacks at
// 200/300/400 and consumes 100/200/300 points for the respective attacks.
// These are the decoded meter values, not new gameplay thresholds.
enum PlayerChargeMeterLevel
{
    PLAYER_CHARGE_RELEASE_LEVEL = 100,
    PLAYER_CHARGE_FIRST_ATTACK_LEVEL = 200,
    PLAYER_CHARGE_SECOND_ATTACK_LEVEL = 300,
    PLAYER_CHARGE_THIRD_ATTACK_LEVEL = 400
};

enum PlayerChargeResourceCost
{
    PLAYER_CHARGE_FIRST_ATTACK_COST = 100,
    PLAYER_CHARGE_SECOND_ATTACK_COST = 200,
    PLAYER_CHARGE_THIRD_ATTACK_COST = 300
};

// All charge timing thresholds are native frame counts. These are the
// original cooldown, warm-up and automatic-release limits, not a new timer.
enum PlayerChargeFrameTiming
{
    PLAYER_CHARGE_RETRY_FRAMES = 3,
    PLAYER_CHARGE_FIRST_FIRING_FRAME = 1,
    PLAYER_CHARGE_COOLDOWN_SHORT_STEP = 5,
    PLAYER_CHARGE_COOLDOWN_LONG_STEP = 10,
    PLAYER_CHARGE_MIN_HOLD_FRAMES = 10,
    PLAYER_CHARGE_EFFECT_FRAMES = 8,
    PLAYER_CHARGE_AUTORELEASE_FRAMES = 300,
    PLAYER_CHARGE_NORMAL_FIRE_LIMIT = 15,
    PLAYER_CHARGE_METER_INTERVAL = 100
};

struct FocusModesForSide
{
    const u8 *rowEnd;
    int offsetFromEnd;

    u8 Read() const
    {
        return rowEnd[offsetFromEnd];
    }
};

static inline u8 ReadFocusMode(int side, const GameConfiguration *configuration)
{
    const FocusModesForSide options = {
        configuration->sideFocusMode + GameplaySetupNativeSides::PlayableCount,
        side - GameplaySetupNativeSides::PlayableCount
    };
    return options.Read();
}

int PlayerLifecycleView::UpdateChargeAttack()
{
    // PlayerRegisterChain is only called from GameManagerSetup's two-side
    // setup loop; the playable side index is therefore always 0 or 1.
    const int side = sideIndex;
    volatile u8 sideMode;
    u8 modeSnapshot;
    int previousLevel;
    // Both focus-mode tests and the post-charge test reread the real
    // two-byte configuration row, without caching the mode across calls.
    if ((g_ReplayInputStates[side].WasPressed(PLAYER_CHARGE_BUTTON_SHOOT) &&
         !(ReadFocusMode(side, g_GameConfiguration) == PLAYER_CHARGE_FROM_FOCUS_INPUT &&
           g_ReplayInputStates[side].IsHeld(PLAYER_CHARGE_BUTTON_FOCUS))) ||
        ((modeSnapshot = ReadFocusMode(side, g_GameConfiguration),
          sideMode = modeSnapshot, modeSnapshot == PLAYER_CHARGE_FROM_FOCUS_INPUT) &&
         g_ReplayInputStates[side].IsHeld(PLAYER_CHARGE_BUTTON_SHOOT) &&
         !g_ReplayInputStates[side].IsHeld(PLAYER_CHARGE_BUTTON_FOCUS)))
    {
        if (timer303B0 == 0)
        {
            if (timer303A4 < 0)
            {
                timer303A4 = 0;
            }
            else if (timer303A4 >= PLAYER_CHARGE_COOLDOWN_LONG_STEP)
            {
                timer303A4 -= PLAYER_CHARGE_COOLDOWN_LONG_STEP;
            }
            else if (timer303A4 >= PLAYER_CHARGE_COOLDOWN_SHORT_STEP)
            {
                timer303A4 -= PLAYER_CHARGE_COOLDOWN_SHORT_STEP;
            }
            timer303B0 = PLAYER_CHARGE_RETRY_FRAMES;
            timer303BC = PLAYER_CHARGE_FIRST_FIRING_FRAME;
        }
        else if (timer303B0 > 0)
        {
            timer303B0--;
        }
        if (g_GameConfiguration->sideFocusMode[sideIndex] != PLAYER_CHARGE_FROM_FOCUS_INPUT)
        {
            goto afterCharge;
        }
    }
    else if ((sideMode == PLAYER_CHARGE_FROM_FOCUS_INPUT && g_ReplayInputStates[side].IsHeld(PLAYER_CHARGE_BUTTON_FOCUS)) ||
             (sideMode == PLAYER_CHARGE_FROM_SHOT_INPUT && g_ReplayInputStates[side].IsHeld(PLAYER_CHARGE_BUTTON_SHOOT)))
    {
        timer303BC++;
        if (timer303BC >= PLAYER_CHARGE_MIN_HOLD_FRAMES && flagBits1B80.chargeAttackActive02 == 0)
        {
            if (scalar30384 == 0.0f)
            {
                g_SoundPlayer.PlaySoundByIdx(SOUND_CHARGE_0, sideIndex != 0 ? 500 : -500);
                sideState->frontSide18->ActivateMeterVms();
            }
            previousLevel = static_cast<int>(scalar30384) / PLAYER_CHARGE_METER_INTERVAL;
            scalar30384 +=
                g_Supervisor.framerateMultiplier5B8 * primaryShtFile->chargeRate24;
            if (scalar30384 >= scalar30388)
            {
                ZunTimer &maxHoldTimer = timer3038C;
                scalar30384 = scalar30388;
                maxHoldTimer++;
                if (maxHoldTimer >= PLAYER_CHARGE_AUTORELEASE_FRAMES)
                {
                    goto resolveAttack;
                }
            }
            else
            {
                timer3038C = 0;
            }
            if (previousLevel < static_cast<int>(scalar30384) / PLAYER_CHARGE_METER_INTERVAL)
            {
                g_SoundPlayer.PlaySoundByIdx(SOUND_CHARGE_UP, sideIndex != 0 ? 500 : -500);
            }
        }
        goto afterCharge;
    }
resolveAttack:
    if (scalar30384 >= static_cast<float>(PLAYER_CHARGE_RELEASE_LEVEL))
    {
        flagBits1B80.chargeAttackActive02 = 1;
        timer30398 = 0;
        timer1B74 = PLAYER_CHARGE_EFFECT_FRAMES;
        if (scalar30384 >= static_cast<float>(PLAYER_CHARGE_THIRD_ATTACK_LEVEL) &&
            opponentState->enemyManager10->FindActiveEnemyBySideCategory(3) == NULL)
        {
            opponentState->cardAttack14->StartAttack(
                        2, 2, 0, primaryShtFile->modePath2);
            SubtractResourceClamped(static_cast<float>(PLAYER_CHARGE_THIRD_ATTACK_COST));
            timer1B74 = PLAYER_CHARGE_EFFECT_FRAMES;
            ConfigureState3Effect15();
        }
        else if (scalar30384 >= static_cast<float>(PLAYER_CHARGE_SECOND_ATTACK_LEVEL))
        {
            opponentState->cardAttack14->StartAttack(
                        1, 1, 0, primaryShtFile->modePath1);
            SubtractResourceClamped(static_cast<float>(PLAYER_CHARGE_SECOND_ATTACK_COST));
            timer1B74 = PLAYER_CHARGE_EFFECT_FRAMES;
            ConfigureState3Effect14();
        }
        else if (scalar30384 >= static_cast<float>(PLAYER_CHARGE_FIRST_ATTACK_LEVEL))
        {
            opponentState->cardAttack14->StartAttack(
                        0, 0, 0, primaryShtFile->modePath0);
            timer1B74 = PLAYER_CHARGE_EFFECT_FRAMES;
            SubtractResourceClamped(static_cast<float>(PLAYER_CHARGE_FIRST_ATTACK_COST));
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
    if (flagBits1B80.chargeAttackActive02 != 0)
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
            flagBits1B80.chargeAttackActive02 = 0;
        }
    }
    if (timer303A4 >= 0)
    {
        if (timer303A4.HasTicked())
        {
            SpawnShots(static_cast<int>(timer303A4), 0);
        }
        timer303A4++;
        if (timer303A4 >= PLAYER_CHARGE_NORMAL_FIRE_LIMIT)
        {
            timer303A4 = -1;
        }
    }
    return 0;
}
