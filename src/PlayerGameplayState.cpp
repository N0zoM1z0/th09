#include "GameManagerMode.hpp"
#include "PlayerLifecycleView.hpp"
#include "ZunTimer.hpp"

#include <stddef.h>
#include <string.h>

// Maintained natural reconstruction of the Player gameplay-pattern owner at
// 0x004049A0.  Exact identifier spelling remains unknown; the exact OnUpdate
// caller already names this receiver seam PlayerUpdateSelectorState and passes
// Player +0x24 in ECX.  Private views below preserve only target-observed
// geometry and leave the small leaf owners independent.

namespace
{

struct PlayerGameplayHeaderView
{
    ZunTimer frameTimer00;
    int crossedThreshold0C;
    int recentPatterns10[8];
    int recentPatternFlags30[8];
    int retryCooldown50;
    int currentPattern54;
    float distanceThreshold58;
    ZunTimer openingTimer5C;
    ZunTimer secondaryTimer68;
    unsigned int flags74;
    PlayerLifecycleView *player78;
};

typedef char PlayerGameplayHeaderCrossedAt0C[
    (offsetof(PlayerGameplayHeaderView, crossedThreshold0C) == 0x0C) ? 1 : -1];
typedef char PlayerGameplayHeaderRecentAt10[
    (offsetof(PlayerGameplayHeaderView, recentPatterns10) == 0x10) ? 1 : -1];
typedef char PlayerGameplayHeaderPatternAt54[
    (offsetof(PlayerGameplayHeaderView, currentPattern54) == 0x54) ? 1 : -1];
typedef char PlayerGameplayHeaderPlayerAt78[
    (offsetof(PlayerGameplayHeaderView, player78) == 0x78) ? 1 : -1];

struct PlayerPatternConfig
{
    int openingLong00;
    int openingMedium04;
    int openingShort08;
    int secondaryLong0C;
    int secondaryMedium10;
    int secondaryShort14;
    int protocolValue18;
};
typedef char PlayerPatternConfigSizeIs1C[
    (sizeof(PlayerPatternConfig) == 0x1C) ? 1 : -1];

struct PlayerSideProtocolView
{
    unsigned char unknown00[0x2C];
    unsigned short patternFlags2C;
    unsigned char unknown2E[4];
    unsigned char eventFlags32;
    unsigned char unknown33;
    unsigned char thresholdFlags34;
    unsigned char unknown35[0x8E - 0x35];
};
typedef char PlayerSideProtocolSizeIs8E[
    (sizeof(PlayerSideProtocolView) == 0x8E) ? 1 : -1];
typedef char PlayerSideProtocolPatternAt2C[
    (offsetof(PlayerSideProtocolView, patternFlags2C) == 0x2C) ? 1 : -1];
typedef char PlayerSideProtocolEventAt32[
    (offsetof(PlayerSideProtocolView, eventFlags32) == 0x32) ? 1 : -1];

struct PlayerGameplayMethods
{
    float *ResolvePatternOffset(
        int pattern, int alternate, float *offsetX, float *offsetY);
    void EnterGameplayMode(int mode);
    int GetUpdateState();
};

struct PlayerPatternOffsetShtView
{
    unsigned char unknown00[0x14];
    float normalAxisSpeed14;
    float focusedAxisSpeed18;
    float normalDiagonalSpeed1C;
    float focusedDiagonalSpeed20;
};

struct PlayerPatternOffsetView
{
    unsigned char unknown0000[0x1CDC];
    float speedMultiplier1CDC;
    float speedMultiplier1CE0;
    float speedMultiplier1CE4;
    float speedMultiplier1CE8;
    unsigned char unknown1CEC[0x30338 - 0x1CEC];
    PlayerPatternOffsetShtView *primaryShtFile30338;
};

float *PlayerGameplayMethods::ResolvePatternOffset(
    int pattern,
    int alternate,
    float *offsetX,
    float *offsetY)
{
    PlayerPatternOffsetView *player =
        reinterpret_cast<PlayerPatternOffsetView *>(this);

    *offsetY = 0.0f;
    *offsetX = 0.0f;

    if (alternate)
    {
        switch (pattern)
        {
        case 4:
            *offsetX = player->primaryShtFile30338->focusedAxisSpeed18;
            break;
        case 3:
            *offsetX = -player->primaryShtFile30338->focusedAxisSpeed18;
            break;
        case 1:
            *offsetY = -player->primaryShtFile30338->focusedAxisSpeed18;
            break;
        case 2:
            *offsetY = player->primaryShtFile30338->focusedAxisSpeed18;
            break;
        case 5:
            *offsetX = -player->primaryShtFile30338->focusedDiagonalSpeed20;
            *offsetY = *offsetX;
            break;
        case 7:
            *offsetY = player->primaryShtFile30338->focusedDiagonalSpeed20;
            *offsetX = -*offsetY;
            break;
        case 6:
            *offsetX = player->primaryShtFile30338->focusedDiagonalSpeed20;
            *offsetY = -*offsetX;
            break;
        case 8:
            *offsetX = player->primaryShtFile30338->focusedDiagonalSpeed20;
            *offsetY = *offsetX;
            break;
        default:
            break;
        }
    }
    else
    {
        switch (pattern)
        {
        case 4:
            *offsetX = player->primaryShtFile30338->normalAxisSpeed14;
            break;
        case 3:
            *offsetX = -player->primaryShtFile30338->normalAxisSpeed14;
            break;
        case 1:
            *offsetY = -player->primaryShtFile30338->normalAxisSpeed14;
            break;
        case 2:
            *offsetY = player->primaryShtFile30338->normalAxisSpeed14;
            break;
        case 5:
            *offsetX = -player->primaryShtFile30338->normalDiagonalSpeed1C;
            *offsetY = *offsetX;
            break;
        case 7:
            *offsetY = player->primaryShtFile30338->normalDiagonalSpeed1C;
            *offsetX = -*offsetY;
            break;
        case 6:
            *offsetX = player->primaryShtFile30338->normalDiagonalSpeed1C;
            *offsetY = -*offsetX;
            break;
        case 8:
            *offsetX = player->primaryShtFile30338->normalDiagonalSpeed1C;
            *offsetY = *offsetX;
            break;
        default:
            break;
        }
    }

    *offsetX =
        player->speedMultiplier1CE4 *
        player->speedMultiplier1CDC *
        *offsetX;
    *offsetY =
        player->speedMultiplier1CE8 *
        player->speedMultiplier1CE0 *
        *offsetY;
    return offsetY;
}

struct PlayerGameplayFrontSideView
{
    void ResetPatternTiming();
    void SetPatternTiming(int value);
};

struct PlayerGameplayTimerCurrentView
{
    int previous;
    float subFrame;
    int current;

    int GetCurrent();
};

struct PlayerGameplayRngView
{
    unsigned short GetRandomU16InRange(unsigned short maximum);
    unsigned int GetRandomU32InRange(unsigned int maximum);
    float GetRandomF32InRange(float maximum);
};

extern PlayerSideProtocolView g_PlayerSideProtocols[2];
extern PlayerPatternConfig g_PlayerPatternConfigs[];
extern int g_PlayerPatternGrid[12][10];
extern int g_PlayerPatternFlags[];
extern int g_PlayerPatternAlternates[];
extern int g_PlayerPatternProtocolValue;
extern PlayerGameplayRngView g_PlayerGameplayRng;
extern float g_PlayerPlayfieldMinX;
extern float g_PlayerPlayfieldMinY;
extern float g_PlayerPlayfieldWidth;
extern float g_PlayerPlayfieldHeight;

struct PlayerSharedRuntimeView
{
    int IsBlocked();
    unsigned char unknown0000[0x1095C];
    int updateBlock1095C;
    int drawCounter10960;
};
extern PlayerSharedRuntimeView *g_PlayerSharedRuntime;

template <typename T>
__forceinline T &GameplayField(void *owner, size_t offset)
{
    return *reinterpret_cast<T *>(
        reinterpret_cast<unsigned char *>(owner) + offset);
}

__forceinline PlayerGameplayMethods *GameplayMethods(PlayerLifecycleView *player)
{
    return reinterpret_cast<PlayerGameplayMethods *>(player);
}

__forceinline PlayerGameplayFrontSideView *FrontSide(PlayerSideStateView *side)
{
    return reinterpret_cast<PlayerGameplayFrontSideView *>(side->frontSide18);
}

__forceinline int HeaderTimerIsModulo(PlayerGameplayHeaderView *header, int divisor)
{
    return header->frameTimer00.HasTickedEvery(divisor);
}

__forceinline void ClampPatternPosition(PlayerPositionView *position)
{
    if (position->x < g_PlayerPlayfieldMinX)
        position->x = g_PlayerPlayfieldMinX;
    else if (position->x > g_PlayerPlayfieldMinX + g_PlayerPlayfieldWidth)
        position->x = g_PlayerPlayfieldMinX + g_PlayerPlayfieldWidth;

    if (position->y < g_PlayerPlayfieldMinY)
        position->y = g_PlayerPlayfieldMinY;
    else if (position->y > g_PlayerPlayfieldMinY + g_PlayerPlayfieldHeight)
        position->y = g_PlayerPlayfieldMinY + g_PlayerPlayfieldHeight;
}

__forceinline void ResolvePatternPosition(
    PlayerLifecycleView *player,
    int pattern,
    int alternate,
    PlayerPositionView *position)
{
    float offsetX;
    float offsetY;
    GameplayMethods(player)->ResolvePatternOffset(
        pattern, alternate, &offsetX, &offsetY);
    *position = player->position1B88;
    position->x += offsetX;
    position->y += offsetY;
    ClampPatternPosition(position);
}

__forceinline int IsPatternPositionBlocked(
    PlayerLifecycleView *player,
    const PlayerPositionView &position,
    const PlayerPositionView &halfSize,
    float radius)
{
    return player->collisionQuery36C.FindCollision(
               position, halfSize, radius) != NULL;
}

__forceinline void ShiftPatternHistory(int *history, int value)
{
    for (int index = 7; index > 0; --index)
        history[index] = history[index - 1];
    history[0] = value;
}

__forceinline void *PrimaryTargetManager(PlayerLifecycleView *player)
{
    return player->sideState->attackTarget10;
}

__forceinline void *PrimaryTargetEnemy(PlayerLifecycleView *player)
{
    void *manager = PrimaryTargetManager(player);
    return manager ? GameplayField<void *>(manager, 0x2AC444) : NULL;
}

__forceinline void ResolveOpponentTarget(
    PlayerGameplayHeaderView *header,
    PlayerPositionView *target)
{
    PlayerLifecycleView *player = header->player78;
    float *overridePosition = reinterpret_cast<float *>(
        reinterpret_cast<unsigned char *>(player) + 0x30F64);
    if (overridePosition[0] > -999.0f)
    {
        target->x = overridePosition[0];
        target->y = overridePosition[1];
        target->z = overridePosition[2];
    }
    else
    {
        void *manager = PrimaryTargetManager(player);
        void *enemy = manager
                          ? GameplayField<void *>(manager, 0x2AC444)
                          : NULL;
        if (enemy != NULL)
        {
            *target = GameplayField<PlayerPositionView>(enemy, 0x2D74);
            target->y += 128.0f;
            unsigned int flags = GameplayField<unsigned int>(enemy, 0x3380);
            if ((flags & 0xC00U) == 0xC00U || (flags & 0x2000U) != 0)
                target->y = 400.0f;
            goto target_resolved;
        }

        enemy = manager
                    ? GameplayField<void *>(manager, 0x2AC448)
                    : NULL;
        if (enemy == NULL)
        {
            target->x = -1000.0f;
            target->y = 0.0f;
            target->z = 0.0f;
            return;
        }

        *target = GameplayField<PlayerPositionView>(enemy, 0x2D74);
        target->y += 128.0f;
    }

target_resolved:
    if (header->recentPatterns10[0] == header->recentPatterns10[2] &&
        header->recentPatterns10[1] == header->recentPatterns10[3] &&
        header->recentPatterns10[0] != header->recentPatterns10[1])
    {
        target->x = -target->x;
        target->y = g_PlayerGameplayRng.GetRandomF32InRange(448.0f);
    }
}

__forceinline int SelectTargetPattern(
    PlayerGameplayHeaderView *header,
    int currentPattern,
    int alternate,
    const PlayerPositionView &boxHalfSize)
{
    PlayerLifecycleView *player = header->player78;
    PlayerPositionView target;
    ResolveOpponentTarget(header, &target);
    if (target.x <= -999.0f || target.y >= 448.0f)
        return currentPattern;

    int pattern;
    PlayerPositionView candidate;
    if (target.x + 6.0f < player->position1B88.x)
    {
        pattern =
            (target.y + 6.0f < player->position1B88.y &&
             player->position1B88.y > 48.0f)
                ? 5
                : 7;
        ResolvePatternPosition(player, pattern, alternate, &candidate);
    }
    else if (target.x - 6.0f > player->position1B88.x)
    {
        pattern =
            (target.y + 6.0f < player->position1B88.y &&
             player->position1B88.y > 48.0f)
                ? 6
                : 8;
        ResolvePatternPosition(player, pattern, alternate, &candidate);
    }
    else
    {
        return currentPattern;
    }

    if (IsPatternPositionBlocked(player, candidate, boxHalfSize, 0.0f))
        return currentPattern;
    return pattern;
}

} // namespace

void __fastcall PlayerUpdateSelectorState(void *state)
{
    PlayerGameplayHeaderView *header =
        reinterpret_cast<PlayerGameplayHeaderView *>(state);
    memset(
        &g_PlayerSideProtocols[header->player78->sideIndex],
        0,
        0x58);

    PlayerConstructedVector3View constructedHalfSizes[3];
    PlayerPositionView *halfSizes =
        reinterpret_cast<PlayerPositionView *>(constructedHalfSizes);
    float radii[3];
    radii[0] = 48.0f;
    radii[1] = 16.0f;
    radii[2] = 0.0f;

    if (g_PlayerSharedRuntime->updateBlock1095C != 0 ||
        g_PlayerSharedRuntime->IsBlocked())
        return;

    PlayerPatternConfig *config =
        &g_PlayerPatternConfigs[header->player78->sideState->characterIndex2C];
    if (g_GameManager.IsGameMode1())
    {
        FrontSide(header->player78->opponentState)->SetPatternTiming(
            config->openingShort08 * 60 -
            reinterpret_cast<PlayerGameplayTimerCurrentView *>(
                &header->openingTimer5C)->GetCurrent());
    }

    if ((header->flags74 & 1U) == 0)
    {
        if ((header->player78->sideState->flags34 & 1U) == 0 &&
            GameplayField<int>(header->player78->opponentState->manager04, 0x3044C) <= 360)
            header->openingTimer5C++;

        int phase = header->player78->header24.state84;
        int threshold = phase >= 10
                            ? config->openingLong00
                            : (phase >= 2 ? config->openingMedium04
                                          : config->openingShort08);
        if (g_GameManager.IsGameMode1() &&
            GameplayMethods(header->player78)->GetUpdateState() != 1)
        {
            GameplayMethods(header->player78)->EnterGameplayMode(3);
            reinterpret_cast<ZunTimer *>(&header->player78->timer303C8)->SetCurrent(2);
        }
        if (reinterpret_cast<PlayerGameplayTimerCurrentView *>(
                &header->openingTimer5C)->GetCurrent() / 60 >= threshold)
        {
            header->flags74 |= 1U;
            if (g_GameManager.IsGameMode1())
            {
                FrontSide(header->player78->opponentState)->ResetPatternTiming();
                FrontSide(header->player78->opponentState)->SetPatternTiming(-1);
            }
        }
    }
    else if ((header->flags74 & 2U) == 0)
    {
        header->secondaryTimer68++;
        int phase = header->player78->header24.state84;
        int threshold = phase >= 10
                            ? config->secondaryLong0C
                            : (phase >= 2 ? config->secondaryMedium10
                                          : config->secondaryShort14);
        if (reinterpret_cast<PlayerGameplayTimerCurrentView *>(
                &header->secondaryTimer68)->GetCurrent() / 60 >= threshold)
            header->flags74 |= 2U;
    }

    g_PlayerPatternProtocolValue = config->protocolValue18;

    if (header->player78->scalar30388 < header->distanceThreshold58)
    {
        if (header->crossedThreshold0C != 0)
        {
            g_PlayerSideProtocols[header->player78->sideIndex].thresholdFlags34 |= 1U;
            header->crossedThreshold0C = 0;
        }
    }
    else
    {
        header->crossedThreshold0C = 1;
    }

    int cell = header->player78->position1B88.x < -96.0f
                   ? 0
                   : (header->player78->position1B88.x < 0.0f
                          ? 1
                          : (header->player78->position1B88.x < 96.0f ? 2 : 3));
    if (header->player78->position1B88.y < 288.0f)
        cell += 4;
    if (header->player78->position1B88.y < 192.0f)
        cell += 4;

    halfSizes[0].x = 28.0f;
    halfSizes[0].y = 28.0f;
    halfSizes[1].x = 10.0f;
    halfSizes[1].y = 10.0f;
    halfSizes[2] = header->player78->hurtboxHalfSize;

    int alternate = 0;
    {
        void *manager = PrimaryTargetManager(header->player78);
        if (manager != NULL && GameplayField<int>(manager, 0x2AC3B8) >= 4)
        {
            g_PlayerSideProtocols[header->player78->sideIndex].patternFlags2C |= 4U;
            alternate = 1;
        }
    }

    g_PlayerPatternGrid[cell][1] = header->currentPattern54;
    int selectedPattern = header->currentPattern54;
    int halfSizeIndex = 0;

    if ((header->flags74 & 2U) == 0 ||
        GameplayField<int>(header->player78->opponentState->manager04, 0x3044C) > 300)
    {
        if (header->retryCooldown50 > 0)
        {
            --header->retryCooldown50;
            PlayerPositionView candidate;
            ResolvePatternPosition(
                header->player78, selectedPattern, alternate, &candidate);
            for (halfSizeIndex = 0; halfSizeIndex < 2; ++halfSizeIndex)
            {
                if (!IsPatternPositionBlocked(
                        header->player78,
                        candidate,
                        halfSizes[halfSizeIndex],
                        radii[halfSizeIndex]))
                    goto pattern_selected;
            }
        }

retry_pattern_grid:
        for (halfSizeIndex = 0; halfSizeIndex < 3; ++halfSizeIndex)
        {
            for (int patternIndex = 0; patternIndex < 10; ++patternIndex)
            {
                selectedPattern = g_PlayerPatternGrid[cell][patternIndex];
                PlayerPositionView candidate;
                ResolvePatternPosition(
                    header->player78, selectedPattern, alternate, &candidate);
                if (!IsPatternPositionBlocked(
                        header->player78,
                        candidate,
                        halfSizes[halfSizeIndex],
                        radii[halfSizeIndex]))
                    goto pattern_selected;
            }
        }

        if ((g_PlayerSideProtocols[header->player78->sideIndex].patternFlags2C & 4U) != 0)
        {
            g_PlayerSideProtocols[header->player78->sideIndex].patternFlags2C &= ~4U;
            alternate = 0;
            goto retry_pattern_grid;
        }

        if ((header->flags74 & 1U) == 0 &&
            GameplayMethods(header->player78)->GetUpdateState() == 0 &&
            reinterpret_cast<ZunTimer *>(&header->player78->timer1B74)->operator==(0))
        {
            if (header->crossedThreshold0C != 0 &&
                header->player78->scalar30384 >= 100.0f)
            {
                g_PlayerSideProtocols[header->player78->sideIndex].thresholdFlags34 |= 1U;
                header->crossedThreshold0C = 0;
                return;
            }
            g_PlayerSideProtocols[header->player78->sideIndex].eventFlags32 |= 2U;
            g_PlayerSideProtocols[header->player78->sideIndex].patternFlags2C |= 2U;
        }
        header->currentPattern54 = selectedPattern;
        return;
    }

pattern_selected:
    {
        PlayerSideProtocolView *protocol =
            &g_PlayerSideProtocols[header->player78->sideIndex];

        if (header->player78->scalar30384 < header->distanceThreshold58)
    {
        if (header->crossedThreshold0C != 0)
            protocol->patternFlags2C |= 1U;
    }
    else
    {
        protocol->thresholdFlags34 |= 1U;
        header->crossedThreshold0C = 0;
        header->distanceThreshold58 =
            (g_PlayerGameplayRng.GetRandomU32InRange(4) + 1.0f) * 100.0f;
        if (header->distanceThreshold58 >= 400.0f)
            header->distanceThreshold58 = 400.0f;
    }

    int patternFlag = g_PlayerPatternFlags[selectedPattern];
    if (patternFlag != 0 && (header->flags74 & 2U) == 0)
    {
        if (header->retryCooldown50 == 0 &&
            header->recentPatternFlags30[0] == header->recentPatternFlags30[1] &&
            header->recentPatternFlags30[1] == header->recentPatternFlags30[2] &&
            header->recentPatternFlags30[2] == header->recentPatternFlags30[3] &&
            header->recentPatternFlags30[3] == header->recentPatternFlags30[4] &&
            header->recentPatternFlags30[4] == header->recentPatternFlags30[5])
        {
            int candidatePattern = g_PlayerPatternAlternates[
                g_PlayerGameplayRng.GetRandomU16InRange(2) * 9 + selectedPattern];
            PlayerPositionView candidate;
            ResolvePatternPosition(
                header->player78, candidatePattern, alternate, &candidate);
            int testHalfSize = halfSizeIndex < 2
                                   ? halfSizeIndex + 1
                                   : halfSizeIndex;
            if (!IsPatternPositionBlocked(
                    header->player78,
                    candidate,
                    halfSizes[testHalfSize],
                    radii[testHalfSize]))
            {
                selectedPattern = candidatePattern;
                header->retryCooldown50 = 8;
                patternFlag = g_PlayerPatternFlags[selectedPattern];
            }
        }

        protocol->patternFlags2C |=
            static_cast<unsigned short>(g_PlayerPatternFlags[selectedPattern]);
        if (header->retryCooldown50 == 0)
            ShiftPatternHistory(
                header->recentPatternFlags30,
                g_PlayerPatternFlags[selectedPattern]);
    }
    else
    {
        selectedPattern = SelectTargetPattern(
            header, selectedPattern, alternate, halfSizes[0]);
        protocol->patternFlags2C |=
            static_cast<unsigned short>(g_PlayerPatternFlags[selectedPattern]);
    }

    ShiftPatternHistory(
        header->recentPatterns10,
        g_PlayerPatternFlags[selectedPattern]);

    void *targetEnemy = PrimaryTargetEnemy(header->player78);
    unsigned int targetFlags = targetEnemy != NULL
                                   ? GameplayField<unsigned int>(targetEnemy, 0x3380)
                                   : 0;
    if ((targetFlags & 0xC00U) == 0xC00U ||
        (targetFlags & 0x2000U) != 0 ||
        GameplayField<int>(header->player78, 0xC110) == 0)
    {
        void *manager = PrimaryTargetManager(header->player78);
        int divisor = manager != NULL &&
                              GameplayField<int>(manager, 0x2AC3AC) != 0
                          ? 10
                          : 30;
        if (HeaderTimerIsModulo(header, divisor))
            protocol->eventFlags32 |= 1U;
    }

    if (g_GameManager.IsGameMode1() && (header->flags74 & 2U) != 0 &&
        GameplayField<int>(header->player78->opponentState->manager04, 0x3044C) <= 300)
        protocol->eventFlags32 &= ~1U;

    header->currentPattern54 = selectedPattern;
    header->frameTimer00++;
    }
}
