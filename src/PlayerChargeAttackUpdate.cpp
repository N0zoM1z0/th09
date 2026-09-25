#include "EnemyManager.hpp"
#include "FrontSide.hpp"
#include "PlayerLifecycleView.hpp"
#include "ReplayInputState.hpp"
#include "ZunTimer.hpp"

#include <stddef.h>

typedef unsigned char u8;
typedef unsigned short u16;

struct ReplayInputPressedView
{
    u8 unknown00[0x32];
    u16 historyPressed32;

    u16 WasPressed(u16 mask);
};

struct ReplayInputGateView
{
    u8 unknown000[0xB4];
    u8 sideMode[2];
};

struct GameManagerReplayView
{
    u8 unknown000[0xE8];
    ReplayInputGateView *inputGateE8;
};
typedef char PlayerChargeInputGateAtE8[
    (offsetof(GameManagerReplayView, inputGateE8) == 0xE8) ? 1 : -1];

struct PlayerChargeShtView
{
    u8 unknown00[0x24];
    float chargeRate24;
    float shotDuration28;
    const char *modePath2C;
    u8 unknown30[0x3C];
    const char *modePath6C;
    u8 unknown70[0x3C];
    const char *modePathAC;
};
typedef char PlayerChargeShtPath0At2C[
    (offsetof(PlayerChargeShtView, modePath2C) == 0x2C) ? 1 : -1];
typedef char PlayerChargeShtPath1At6C[
    (offsetof(PlayerChargeShtView, modePath6C) == 0x6C) ? 1 : -1];
typedef char PlayerChargeShtPath2AtAC[
    (offsetof(PlayerChargeShtView, modePathAC) == 0xAC) ? 1 : -1];

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

struct CardAttackStartView
{
    void StartAttack(
        int callbackRow, int attackClass, int value, const char *text);
};

struct PlayerResourceSubtractView
{
    void SubtractResourceClamped(float amount);
};

struct PlayerStateEffectView
{
    void *ConfigureState3Effect13();
    void *ConfigureState3Effect14();
    void *ConfigureState3Effect15();
};

struct PlayerSpawnShotsView
{
    void __fastcall SpawnShots(int value, int powerLevel);
};

struct PlayerChargeTimerCurrentView
{
    int previous00;
    float subFrame04;
    int current08;

    int GetCurrent();
};

class SoundPlayer
{
  public:
    void PlaySoundByIdx(int soundIndex, int pan);
};

struct PlayerSupervisorRuntimeView
{
    u8 unknown000[0x5B8];
    float framerateMultiplier5B8;
};

extern GameManagerReplayView g_GameManager;
extern SoundPlayer g_SoundPlayer;
extern PlayerSupervisorRuntimeView g_PlayerSupervisorRuntime;

struct PlayerChargeAttackView : PlayerLifecycleView
{
    int UpdateChargeAttack();
};

int PlayerChargeAttackView::UpdateChargeAttack()
{
    int side = sideIndex;
    u8 sideMode;

    if ((reinterpret_cast<ReplayInputPressedView *>(&g_ReplayInputStates[side])->WasPressed(1) &&
         (g_GameManager.inputGateE8->sideMode[side] != 1 ||
          !g_ReplayInputStates[side].IsHeld(4))) ||
        ((sideMode = g_GameManager.inputGateE8->sideMode[side]) == 1 &&
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

        if (g_GameManager.inputGateE8->sideMode[side] == 1)
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
        PlayerChargeShtView *sht =
            reinterpret_cast<PlayerChargeShtView *>(primaryShtFile);

        scalar30384 +=
            g_PlayerSupervisorRuntime.framerateMultiplier5B8 *
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
                        reinterpret_cast<PlayerChargeShtView *>(primaryShtFile)
                            ->modePathAC);
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
                        reinterpret_cast<PlayerChargeShtView *>(primaryShtFile)
                            ->modePath6C);
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
                        reinterpret_cast<PlayerChargeShtView *>(primaryShtFile)
                            ->modePath2C);
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
            int current =
                reinterpret_cast<PlayerChargeTimerCurrentView *>(
                    shotTimer)->GetCurrent();
            reinterpret_cast<PlayerSpawnShotsView *>(this)
                ->SpawnShots(current, 1);
        }

        (*shotTimer)++;
        PlayerChargeShtView *sht =
            reinterpret_cast<PlayerChargeShtView *>(primaryShtFile);
        if (static_cast<float>(*shotTimer) >= sht->shotDuration28)
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
                int current =
                    reinterpret_cast<PlayerChargeTimerCurrentView *>(
                        normalShotTimer)->GetCurrent();
                reinterpret_cast<PlayerSpawnShotsView *>(this)
                    ->SpawnShots(current, 0);
            }

            (*normalShotTimer)++;
            if (*normalShotTimer >= 15)
                *normalShotTimer = -1;
        }
    }

    return 0;
}
