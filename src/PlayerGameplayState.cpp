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
    unsigned short eventFlags32;
    unsigned short thresholdFlags34;
    unsigned char unknown36[0x8E - 0x36];
};
typedef char PlayerSideProtocolSizeIs8E[
    (sizeof(PlayerSideProtocolView) == 0x8E) ? 1 : -1];
typedef char PlayerSideProtocolPatternAt2C[
    (offsetof(PlayerSideProtocolView, patternFlags2C) == 0x2C) ? 1 : -1];
typedef char PlayerSideProtocolEventAt32[
    (offsetof(PlayerSideProtocolView, eventFlags32) == 0x32) ? 1 : -1];
typedef char PlayerSideProtocolThresholdAt34[
    (offsetof(PlayerSideProtocolView, thresholdFlags34) == 0x34) ? 1 : -1];

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

} // namespace

void __fastcall PlayerUpdateSelectorState(void *state)
{
    PlayerGameplayHeaderView *header =
        reinterpret_cast<PlayerGameplayHeaderView *>(state);
    int selectedPattern = 0;
    int alternate = 0;
    PlayerConstructedVector3View constructedHalfSizes[3];
    memset(
        &g_PlayerSideProtocols[header->player78->sideIndex],
        0,
        0x58);
    PlayerPositionView *halfSizes =
        reinterpret_cast<PlayerPositionView *>(constructedHalfSizes);
    // The third collision radius is implicitly zero-initialized.
    float radii[3] = {48.0f, 16.0f};
    int halfSizeIndex = 0;
    // All collision probes reuse these genuine output workspaces.
    float offsetX;
    float offsetY;
    PlayerPositionView candidate;

    if (g_PlayerSharedRuntime->updateBlock1095C != 0 ||
        g_PlayerSharedRuntime->IsBlocked())
        return;

    // Preserve the initial receiver across the mode and timer calls.
    PlayerLifecycleView *initialPlayer = header->player78;
    PlayerPatternConfig *config =
        &g_PlayerPatternConfigs[initialPlayer->sideState->characterIndex2C];
    if (g_GameManager.IsGameMode1())
    {
        int openingTime = reinterpret_cast<PlayerGameplayTimerCurrentView *>(
            &header->openingTimer5C)->GetCurrent();
        reinterpret_cast<PlayerGameplayFrontSideView *>(initialPlayer->opponentState->frontSide18)->SetPatternTiming(
            config->openingShort08 * 60 - openingTime);
    }

    if ((header->flags74 & 1U) == 0)
    {
        if ((header->player78->sideState->flags34 & 1U) == 0 &&
            (*reinterpret_cast<int *>(reinterpret_cast<unsigned char *>(header->player78->opponentState->manager04) + 0x3044C)) <= 360)
            header->openingTimer5C++;

        PlayerLifecycleView *phasePlayer = header->player78;
        int phase = phasePlayer->header24.state84;
        int threshold = phase >= 10
            ? config->openingLong00
            : (phase >= 2 ? config->openingMedium04
                          : config->openingShort08);
        if (g_GameManager.IsGameMode1() &&
            reinterpret_cast<PlayerGameplayMethods *>(phasePlayer)->GetUpdateState() != 1)
        {
            reinterpret_cast<PlayerGameplayMethods *>(phasePlayer)->EnterGameplayMode(3);
            reinterpret_cast<ZunTimer *>(&header->player78->timer303C8)->SetCurrent(2);
        }
        if (reinterpret_cast<PlayerGameplayTimerCurrentView *>(
            &header->openingTimer5C)->GetCurrent() / 60 >= threshold)
        {
            header->flags74 |= 1U;
            if (g_GameManager.IsGameMode1())
            {
                reinterpret_cast<PlayerGameplayFrontSideView *>((header->player78->opponentState)->frontSide18)->ResetPatternTiming();
                reinterpret_cast<PlayerGameplayFrontSideView *>((header->player78->opponentState)->frontSide18)->SetPatternTiming(-1);
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

    if (header->player78->scalar30388 >= header->distanceThreshold58)
    {
        header->crossedThreshold0C = 1;
    }
    else
    {
        if (header->crossedThreshold0C != 0)
        {
            g_PlayerSideProtocols[header->player78->sideIndex].thresholdFlags34 |= 1U;
            header->crossedThreshold0C = 0;
        }
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

    {
        void *manager = (header->player78)->sideState->attackTarget10;
        if ((*reinterpret_cast<int *>(reinterpret_cast<unsigned char *>(manager) + 0x2AC3B8)) >= 4)
        {
            g_PlayerSideProtocols[header->player78->sideIndex].patternFlags2C |= 4U;
            alternate = 1;
        }
    }

    g_PlayerPatternGrid[cell][1] = header->currentPattern54;

    if ((header->flags74 & 2U) == 0 ||
        (*reinterpret_cast<int *>(reinterpret_cast<unsigned char *>(header->player78->opponentState->manager04) + 0x3044C)) > 300)
    {
        if (header->retryCooldown50 != 0)
        {
            --header->retryCooldown50;
            for (halfSizeIndex = 0; halfSizeIndex < 2; ++halfSizeIndex)
            {
                selectedPattern = header->currentPattern54;
                PlayerLifecycleView *candidatePlayer = header->player78;

                {
                    reinterpret_cast<PlayerGameplayMethods *>(candidatePlayer)->ResolvePatternOffset(
                        selectedPattern, alternate, &offsetX, &offsetY);
                    candidate = candidatePlayer->position1B88;
                    candidate.x += offsetX;
                    candidate.y += offsetY;
                    if (candidate.x < g_PlayerPlayfieldMinX)
                        candidate.x = g_PlayerPlayfieldMinX;
                    else if (candidate.x > g_PlayerPlayfieldMinX + g_PlayerPlayfieldWidth)
                        candidate.x = g_PlayerPlayfieldMinX + g_PlayerPlayfieldWidth;
                    if (candidate.y < g_PlayerPlayfieldMinY)
                        candidate.y = g_PlayerPlayfieldMinY;
                    else if (candidate.y > g_PlayerPlayfieldMinY + g_PlayerPlayfieldHeight)
                        candidate.y = g_PlayerPlayfieldMinY + g_PlayerPlayfieldHeight;
                }
                if (!((candidatePlayer)->collisionQuery36C.FindCollision(candidate, halfSizes[halfSizeIndex], radii[halfSizeIndex]) != NULL))
                    goto pattern_selected;
            }
        }

    retry_pattern_grid:
        {
            halfSizeIndex = 0;
            PlayerPositionView *gridHalfSize = halfSizes;
            float *gridRadius = radii;
            for (; halfSizeIndex < 3;
                ++halfSizeIndex, ++gridRadius, ++gridHalfSize)
            {
                for (int patternIndex = halfSizeIndex; patternIndex < 10; ++patternIndex)
                {
                    selectedPattern = g_PlayerPatternGrid[cell][patternIndex];
                    PlayerLifecycleView *candidatePlayer = header->player78;

                    {
                        reinterpret_cast<PlayerGameplayMethods *>(candidatePlayer)->ResolvePatternOffset(
                            selectedPattern, alternate, &offsetX, &offsetY);
                        candidate = candidatePlayer->position1B88;
                        candidate.x += offsetX;
                        candidate.y += offsetY;
                        if (candidate.x < g_PlayerPlayfieldMinX)
                            candidate.x = g_PlayerPlayfieldMinX;
                        else if (candidate.x > g_PlayerPlayfieldMinX + g_PlayerPlayfieldWidth)
                            candidate.x = g_PlayerPlayfieldMinX + g_PlayerPlayfieldWidth;
                        if (candidate.y < g_PlayerPlayfieldMinY)
                            candidate.y = g_PlayerPlayfieldMinY;
                        else if (candidate.y > g_PlayerPlayfieldMinY + g_PlayerPlayfieldHeight)
                            candidate.y = g_PlayerPlayfieldMinY + g_PlayerPlayfieldHeight;
                    }
                    if (!((candidatePlayer)->collisionQuery36C.FindCollision(candidate, *gridHalfSize, *gridRadius) != NULL))
                        goto pattern_selected;
                }
            }
        }

        if ((g_PlayerSideProtocols[header->player78->sideIndex].patternFlags2C & 4U) != 0)
        {
            g_PlayerSideProtocols[header->player78->sideIndex].patternFlags2C &= ~4U;
            alternate = 0;
            goto retry_pattern_grid;
        }

        if ((header->flags74 & 1U) == 0)
        {
            PlayerLifecycleView *player = header->player78;
            if (reinterpret_cast<PlayerGameplayMethods *>(player)->GetUpdateState() == 0 &&
                reinterpret_cast<ZunTimer *>(&player->timer1B74)->operator==(0))
            {
                if (header->crossedThreshold0C != 0 &&
                    player->scalar30384 >= 100.0f)
                {
                    g_PlayerSideProtocols[player->sideIndex].thresholdFlags34 |= 1U;
                    header->crossedThreshold0C = 0;
                    return;
                }
                g_PlayerSideProtocols[player->sideIndex].eventFlags32 |= 2U;
                g_PlayerSideProtocols[header->player78->sideIndex].patternFlags2C |= 2U;
            }
        }
        header->currentPattern54 = selectedPattern;
        return;
    }

pattern_selected:
    {
        if (header->player78->scalar30384 >= header->distanceThreshold58)
        {
            g_PlayerSideProtocols[header->player78->sideIndex].thresholdFlags34 |= 1U;
            header->crossedThreshold0C = 0;
            header->distanceThreshold58 =
                (static_cast<int>(g_PlayerGameplayRng.GetRandomU32InRange(4)) + 1.0f) * 100.0f;
            if (header->distanceThreshold58 >= 400.0f)
                header->distanceThreshold58 = 400.0f;
        }
        else
        {
            if (header->crossedThreshold0C != 0)
                g_PlayerSideProtocols[header->player78->sideIndex].patternFlags2C |= 1U;
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
                PlayerLifecycleView *candidatePlayer = header->player78;

                {
                    reinterpret_cast<PlayerGameplayMethods *>(candidatePlayer)->ResolvePatternOffset(
                        candidatePattern, alternate, &offsetX, &offsetY);
                    candidate = candidatePlayer->position1B88;
                    candidate.x += offsetX;
                    candidate.y += offsetY;
                    if (candidate.x < g_PlayerPlayfieldMinX)
                        candidate.x = g_PlayerPlayfieldMinX;
                    else if (candidate.x > g_PlayerPlayfieldMinX + g_PlayerPlayfieldWidth)
                        candidate.x = g_PlayerPlayfieldMinX + g_PlayerPlayfieldWidth;
                    if (candidate.y < g_PlayerPlayfieldMinY)
                        candidate.y = g_PlayerPlayfieldMinY;
                    else if (candidate.y > g_PlayerPlayfieldMinY + g_PlayerPlayfieldHeight)
                        candidate.y = g_PlayerPlayfieldMinY + g_PlayerPlayfieldHeight;
                }
                int testHalfSize = halfSizeIndex < 2
                ? halfSizeIndex + 1
                    : halfSizeIndex;
                if (!((candidatePlayer)->collisionQuery36C.FindCollision(candidate, halfSizes[testHalfSize], radii[testHalfSize]) != NULL))
                {
                    selectedPattern = candidatePattern;
                    header->retryCooldown50 = 8;
                    patternFlag = g_PlayerPatternFlags[selectedPattern];
                }
            }

            g_PlayerSideProtocols[header->player78->sideIndex].patternFlags2C |=
                static_cast<unsigned short>(g_PlayerPatternFlags[selectedPattern]);
            if (header->retryCooldown50 == 0)
            {
                int *history = header->recentPatternFlags30;
                for (int historyIndex = 7; historyIndex > 0; --historyIndex)
                    history[historyIndex] = history[historyIndex - 1];
            }
            // Cooldown suppresses shifting, not the current flag refresh.
            header->recentPatternFlags30[0] = g_PlayerPatternFlags[selectedPattern];
        }
        else
        {
            {
                PlayerPositionView target;
                {
                    PlayerLifecycleView *player = header->player78;
                    target.x = -1000.0f;
                    const PlayerPositionView *overridePosition = reinterpret_cast<const PlayerPositionView *>(
                        reinterpret_cast<unsigned char *>(player) + 0x30F64);
                    if (overridePosition->x > -999.0f)
                    {
                        target = *overridePosition;
                    }
                    else
                    {
                        void *manager = (player)->sideState->attackTarget10;
                        void *enemy = (*reinterpret_cast<void * *>(reinterpret_cast<unsigned char *>(manager) + 0x2AC444));
                        if (enemy != NULL)
                        {
                            target = (*reinterpret_cast<PlayerPositionView *>(reinterpret_cast<unsigned char *>(enemy) + 0x2D74));
                            target.y += 128.0f;
                            unsigned int flags = (*reinterpret_cast<unsigned int *>(reinterpret_cast<unsigned char *>(enemy) + 0x3380));
                            if ((flags & 0xC00U) == 0xC00U || (flags & 0x2000U) != 0)
                                target.y = 400.0f;
                            goto opponent_target_resolved;
                        }

                        enemy = (*reinterpret_cast<void * *>(reinterpret_cast<unsigned char *>(manager) + 0x2AC448));
                        if (enemy != NULL)
                        {
                            target = (*reinterpret_cast<PlayerPositionView *>(reinterpret_cast<unsigned char *>(enemy) + 0x2D74));
                            target.y += 128.0f;
                        }
                    }

                opponent_target_resolved:
                    if (header->recentPatterns10[0] == header->recentPatterns10[2] &&
                        header->recentPatterns10[1] == header->recentPatterns10[3] &&
                        header->recentPatterns10[0] != header->recentPatterns10[1])
                    {
                        target.x = -target.x;
                        target.y = g_PlayerGameplayRng.GetRandomF32InRange(448.0f);
                    }

                }
                if (target.x > -999.0f)
                {
                    if (target.y < 448.0f)
                    {
                        int pattern;
                        PlayerLifecycleView *trackingPlayer = header->player78;
                        candidate = trackingPlayer->position1B88;
                        if (target.x + 6.0f < trackingPlayer->position1B88.x)
                        {
                            pattern =
                                (target.y + 6.0f < candidate.y && candidate.y > 48.0f) ? 5 : 7;
                            {
                                PlayerLifecycleView *candidatePlayer = trackingPlayer;
                                reinterpret_cast<PlayerGameplayMethods *>(candidatePlayer)->ResolvePatternOffset(
                                    pattern, alternate, &offsetX, &offsetY);
                                candidate.x += offsetX;
                                candidate.y += offsetY;
                                if (candidate.x < g_PlayerPlayfieldMinX)
                                    candidate.x = g_PlayerPlayfieldMinX;
                                else if (candidate.x > g_PlayerPlayfieldMinX + g_PlayerPlayfieldWidth)
                                    candidate.x = g_PlayerPlayfieldMinX + g_PlayerPlayfieldWidth;
                                if (candidate.y < g_PlayerPlayfieldMinY)
                                    candidate.y = g_PlayerPlayfieldMinY;
                                else if (candidate.y > g_PlayerPlayfieldMinY + g_PlayerPlayfieldHeight)
                                    candidate.y = g_PlayerPlayfieldMinY + g_PlayerPlayfieldHeight;
                            }
                            if (trackingPlayer->collisionQuery36C.FindCollision(candidate, halfSizes[1], 0.0f) == NULL)
                                g_PlayerSideProtocols[header->player78->sideIndex].patternFlags2C |=
                                    static_cast<unsigned short>(g_PlayerPatternFlags[pattern]);
                            else
                                g_PlayerSideProtocols[header->player78->sideIndex].patternFlags2C |=
                                    static_cast<unsigned short>(g_PlayerPatternFlags[selectedPattern]);
                        }
                        else if (target.x - 6.0f > trackingPlayer->position1B88.x)
                        {
                            // Probe the retained pattern; emit the new direction only
                            // after a clear collision query, matching the original asymmetry.
                            pattern =
                                (target.y + 6.0f < candidate.y && candidate.y > 48.0f) ? 6 : 8;
                            {
                                PlayerLifecycleView *candidatePlayer = trackingPlayer;
                                reinterpret_cast<PlayerGameplayMethods *>(candidatePlayer)->ResolvePatternOffset(
                                    selectedPattern, alternate, &offsetX, &offsetY);
                                candidate.x += offsetX;
                                candidate.y += offsetY;
                                if (candidate.x < g_PlayerPlayfieldMinX)
                                    candidate.x = g_PlayerPlayfieldMinX;
                                else if (candidate.x > g_PlayerPlayfieldMinX + g_PlayerPlayfieldWidth)
                                    candidate.x = g_PlayerPlayfieldMinX + g_PlayerPlayfieldWidth;
                                if (candidate.y < g_PlayerPlayfieldMinY)
                                    candidate.y = g_PlayerPlayfieldMinY;
                                else if (candidate.y > g_PlayerPlayfieldMinY + g_PlayerPlayfieldHeight)
                                    candidate.y = g_PlayerPlayfieldMinY + g_PlayerPlayfieldHeight;
                            }
                            if (trackingPlayer->collisionQuery36C.FindCollision(candidate, halfSizes[1], 0.0f) == NULL)
                                g_PlayerSideProtocols[header->player78->sideIndex].patternFlags2C |=
                                    static_cast<unsigned short>(g_PlayerPatternFlags[pattern]);
                            else
                                g_PlayerSideProtocols[header->player78->sideIndex].patternFlags2C |=
                                    static_cast<unsigned short>(g_PlayerPatternFlags[selectedPattern]);
                        }
                        else
                        {
                            g_PlayerSideProtocols[trackingPlayer->sideIndex].patternFlags2C |=
                                static_cast<unsigned short>(g_PlayerPatternFlags[selectedPattern]);
                            goto target_pattern_done;
                        }
                    }
                    else
                        g_PlayerSideProtocols[header->player78->sideIndex].patternFlags2C |=
                            static_cast<unsigned short>(g_PlayerPatternFlags[selectedPattern]);
                }
                else
                    g_PlayerSideProtocols[header->player78->sideIndex].patternFlags2C |=
                        static_cast<unsigned short>(g_PlayerPatternFlags[selectedPattern]);
            target_pattern_done:
                ;
            }
        }

        PlayerLifecycleView *player;
        {
            int *history = header->recentPatterns10;
            for (int historyIndex = 7; historyIndex > 0; --historyIndex)
                history[historyIndex] = history[historyIndex - 1];
            player = header->player78;
            history[0] = g_PlayerPatternFlags[selectedPattern];
        }

        void *manager = (player)->sideState->attackTarget10;
        void *targetEnemy = (*reinterpret_cast<void * *>(reinterpret_cast<unsigned char *>(manager) + 0x2AC444));
        if ((targetEnemy != NULL &&
            (((*reinterpret_cast<unsigned int *>(reinterpret_cast<unsigned char *>(targetEnemy) + 0x3380)) & 0xC00U) == 0xC00U ||
            ((*reinterpret_cast<unsigned int *>(reinterpret_cast<unsigned char *>(targetEnemy) + 0x3380)) & 0x2000U) != 0)) ||
            (*reinterpret_cast<int *>(reinterpret_cast<unsigned char *>(player) + 0xC110)) == 0)
        {
            int emitEvent = (*reinterpret_cast<int *>(reinterpret_cast<unsigned char *>(manager) + 0x2AC3AC)) != 0
            ? header->frameTimer00.HasTickedEvery(10)
                : header->frameTimer00.HasTickedEvery(30);
            if (emitEvent)
                g_PlayerSideProtocols[player->sideIndex].eventFlags32 |= 1U;
        }

        if (g_GameManager.IsGameMode1() && (header->flags74 & 2U) != 0 &&
            (*reinterpret_cast<int *>(reinterpret_cast<unsigned char *>(header->player78->opponentState->manager04) + 0x3044C)) <= 300)
            g_PlayerSideProtocols[header->player78->sideIndex].eventFlags32 &= ~1U;

        header->currentPattern54 = selectedPattern;
        header->frameTimer00++;
    }
}
