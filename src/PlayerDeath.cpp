#include "PlayerLifecycleView.hpp"

#include <stddef.h>

struct PlayerDeathSideStateView;

struct PlayerDeathSharedView
{
    unsigned char unknown0000[0xE934];
    unsigned int transitionColorE934[2];
    int transitionFramesE93C[2];

    int IsDeathTransitionBlocked();
    void SelectOpponentSide(int sideIndex);
};

typedef char PlayerDeathSharedColorAtE934[
    (offsetof(PlayerDeathSharedView, transitionColorE934) == 0xE934) ? 1 : -1];
typedef char PlayerDeathSharedFramesAtE93C[
    (offsetof(PlayerDeathSharedView, transitionFramesE93C) == 0xE93C) ? 1 : -1];

struct PlayerDeathAttackTargetView
{
    int CheckMode(int mode);
};

struct PlayerDeathAttackOwnerView
{
    void Configure(int mode0, int mode1, int zero, const char *path);
};

struct PlayerDeathFrontSideView
{
    void ClearState(int value);
};

struct PlayerDeathEffectManagerView
{
    void *SpawnEffectInFixedSlot(
        int effectId, PlayerPositionView *position, int count, unsigned int color);
};

struct PlayerDeathSideStateView
{
    unsigned char unknown00[0x0C];
    PlayerDeathEffectManagerView *effectManager0C;
    PlayerDeathAttackTargetView *attackTarget10;
    PlayerDeathAttackOwnerView *attackOwner14;
    PlayerDeathFrontSideView *frontSide18;
    unsigned char unknown1C[0x04];
    int shotType20;
};

typedef char PlayerDeathSideEffectAt0C[
    (offsetof(PlayerDeathSideStateView, effectManager0C) == 0x0C) ? 1 : -1];
typedef char PlayerDeathSideShotTypeAt20[
    (offsetof(PlayerDeathSideStateView, shotType20) == 0x20) ? 1 : -1];

struct PlayerDeathSoundView
{
    void PlaySoundPositionedByIdx(int soundIndex, float x);
    void PlaySoundByIdx(int soundIndex, int pan);
};

struct PlayerDeathTimerView
{
    int previous;
    float subFrame;
    int current;

    void operator=(int value);
};

typedef char PlayerDeathTimerSizeIsC[(sizeof(PlayerDeathTimerView) == 0x0C) ? 1 : -1];

struct PlayerDeathReplayRngView
{
    float GetRandomF32InRange(float max);
};

struct PlayerDeathVectorView
{
    float x;
    float y;
    float z;

    PlayerDeathVectorView *FromAngleMagnitude(float angle, float magnitude);
};

typedef char PlayerDeathVectorSizeIsC[(sizeof(PlayerDeathVectorView) == 0x0C) ? 1 : -1];

struct PlayerDeathVmHandleView
{
    unsigned char unknown00[0x04];
    void *vm04;
};

struct PlayerDeathActiveView
{
    unsigned char unknown00[0xC4];
    unsigned char activeC4;
};

typedef char PlayerDeathActiveAtC4[
    (offsetof(PlayerDeathActiveView, activeC4) == 0xC4) ? 1 : -1];

struct PlayerDeathCounterView
{
    void Add(int value);
};

struct PlayerDeathHeaderResetView
{
    void Reset();
};

struct PlayerDeathOwnerResetView
{
    void Reset();
};

struct PlayerDeathInterruptView
{
    void SetInterrupt(short interrupt);
};

extern PlayerDeathSharedView *g_PlayerDeathShared;
extern PlayerDeathSoundView g_PlayerDeathSound;
extern int g_PlayerDeathCounter;
extern int g_PlayerDeathCounterBonus;
extern PlayerDeathCounterView g_PlayerDeathCounterState;
extern PlayerDeathReplayRngView g_PlayerDeathReplayRng;

void PlayerLifecycleView::EnterDeathState()
{
    if (g_PlayerDeathShared->IsDeathTransitionBlocked())
        return;

    g_PlayerDeathSound.PlaySoundPositionedByIdx(4, position1B88.x);

    int counterStep = g_PlayerDeathCounter;
    if (selector != 0)
        counterStep += g_PlayerDeathCounterBonus;

    if (header24.state84 <= 1)
    {
        if (reinterpret_cast<PlayerDeathSideStateView *>(sideState)->shotType20 == 8 &&
            scalar30388 >= 200.0f)
        {
            if (scalar30388 >= 400.0f)
            {
                if (!reinterpret_cast<PlayerDeathSideStateView *>(opponentState)
                         ->attackTarget10->CheckMode(3))
                {
                    reinterpret_cast<PlayerDeathSideStateView *>(opponentState)
                        ->attackOwner14->Configure(2, 2, 0, primaryShtFile->modePath2);
                    scalar30384 = 0.0f;
                    scalar30388 = 0.0f;
                    SetBombMode2();
                    return;
                }

                reinterpret_cast<PlayerDeathSideStateView *>(opponentState)
                    ->attackOwner14->Configure(1, 1, 0, primaryShtFile->modePath1);
                scalar30384 = 100.0f;
                scalar30388 = 100.0f;
                SetBombMode1();
                return;
            }
            if (scalar30388 >= 300.0f)
            {
                reinterpret_cast<PlayerDeathSideStateView *>(opponentState)
                    ->attackOwner14->Configure(1, 1, 0, primaryShtFile->modePath1);
                scalar30384 = 0.0f;
                scalar30388 = 0.0f;
                SetBombMode1();
                return;
            }

            if (scalar30388 >= 200.0f)
            {
                reinterpret_cast<PlayerDeathSideStateView *>(opponentState)
                    ->attackOwner14->Configure(0, 0, 0, primaryShtFile->modePath0);
                scalar30384 = 0.0f;
                scalar30388 = 0.0f;
                SetBombMode0();
                return;
            }
            return;
        }

        header24.state84 = 0;
        g_PlayerDeathShared->SelectOpponentSide(1 - sideIndex);
        reinterpret_cast<PlayerDeathSideStateView *>(sideState)
            ->effectManager0C->SpawnEffectInFixedSlot(
            19, &position1B88, 4, 0xFF4040FFu);
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
            g_PlayerDeathSound.PlaySoundByIdx(48, sideIndex != 0 ? 500 : -500);
            reinterpret_cast<PlayerDeathSideStateView *>(sideState)
                ->frontSide18->ClearState(0);
            header24.state84 = 1;
        }
    }

    *reinterpret_cast<int *>(this) = 4;
    reinterpret_cast<PlayerDeathTimerView *>(&timer303C8)->operator=(0);

    float angle = g_PlayerDeathReplayRng.GetRandomF32InRange(3.1415927f);
    *reinterpret_cast<float *>(reinterpret_cast<unsigned char *>(this) + 0x1CD8) = angle;
    reinterpret_cast<PlayerDeathVectorView *>(
        reinterpret_cast<unsigned char *>(this) + 0x1CCC)->FromAngleMagnitude(angle, 1.0f);

    g_PlayerDeathShared->transitionFramesE93C[sideIndex] = 60;
    g_PlayerDeathShared->transitionColorE934[sideIndex] = 0x80FF0000u;

    PlayerDeathVmHandleView *vmHandle =
        *reinterpret_cast<PlayerDeathVmHandleView **>(
            reinterpret_cast<unsigned char *>(this) + 0x364);
    if (vmHandle != NULL)
    {
        reinterpret_cast<PlayerDeathInterruptView *>(vmHandle->vm04)->SetInterrupt(1);
        PlayerDeathActiveView *active =
            *reinterpret_cast<PlayerDeathActiveView **>(
                reinterpret_cast<unsigned char *>(this) + 0x368);
        *reinterpret_cast<PlayerDeathVmHandleView **>(
            reinterpret_cast<unsigned char *>(this) + 0x364) = NULL;
        active->activeC4 = 0;
        *reinterpret_cast<PlayerDeathActiveView **>(
            reinterpret_cast<unsigned char *>(this) + 0x368) = NULL;
    }

    g_PlayerDeathCounterState.Add(1500);
    if (g_PlayerDeathCounter > 4)
        g_PlayerDeathCounter -= 2;
    else if (g_PlayerDeathCounter > 2)
        --g_PlayerDeathCounter;

    reinterpret_cast<PlayerDeathOwnerResetView *>(&ownerState30410)->Reset();
    reinterpret_cast<PlayerDeathHeaderResetView *>(&header24)->Reset();
}
