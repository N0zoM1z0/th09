#include "Ending.hpp"

#include "AnmManager.hpp"
#include "GameManagerMode.hpp"
#include "PlayStats.hpp"
#include "ScreenEffect.hpp"
#include "Supervisor.hpp"

#include <stddef.h>

struct Float3
{
    Float3(float x, float y, float z);
    float x;
    float y;
    float z;
};

struct EndingAnmVmPositionView
{
    unsigned char unknown000[0x208];
    Float3 pos;
};
typedef char EndingAnmVmPositionAt208[
    (offsetof(EndingAnmVmPositionView, pos) == 0x208) ? 1 : -1];

struct EndingSupervisorTextAnmView
{
    unsigned char unknown000[0x5CC];
    AnmLoaded *textAnm;
};
typedef char EndingSupervisorTextAnmAt5CC[
    (offsetof(EndingSupervisorTextAnmView, textAnm) == 0x5CC) ? 1 : -1];

extern unsigned int g_EndingLifecycleFlags;
extern int g_EndingActive;
extern ScreenEffect *g_TitleFadeReleaseEffect;
extern const char *g_EndingFilePaths[16];

int Ending::AddedCallback(Ending *ending)
{
    g_EndingLifecycleFlags |= 0x400u;
    g_AnmManager->ClearTexture();
    g_AnmManager->ClearSprite();
    g_AnmManager->ClearBlendMode();
    g_AnmManager->ClearVertexShader();
    ScreenEffect::Clear(0xFFFFFFFFu);
    g_EndingActive = 1;

    EndingSupervisorTextAnmView *supervisor =
        reinterpret_cast<EndingSupervisorTextAnmView *>(&g_Supervisor);
    for (int i = 0; i < 15; i++)
    {
        supervisor->textAnm->ExecuteAnmIdx(&ending->endingVms[i], i + 14);
        reinterpret_cast<EndingAnmVmPositionView *>(&ending->endingVms[i])->pos =
            Float3(64.0f, i * 16.0f + 400.0f, 0.0f);
    }

    if (g_GameManager.IsGameMode0())
    {
        g_PlayStatsRecord.IncrementCharacterStat(
            (char)g_GameManager.side0Character, g_GameManager.difficulty);
    }
    else if (g_GameManager.IsGameMode1())
    {
        g_PlayStatsRecord.IncrementCharacterStat(
            (char)g_GameManager.side0Character, g_GameManager.difficulty);
    }

    int completed = 0;
    int i;
    for (i = 0; i <= 4; i++)
    {
        if (g_PlayStatsRecord.HasAnyPrimaryCharacterStat((char)i))
            completed++;
    }
    if (completed >= 2)
    {
        if (g_PlayStatsRecord.GetCharacterStat5(5))
            g_PlayStatsRecord.SetCharacterUnlocked1(5);
        if (g_PlayStatsRecord.GetCharacterStat5(7))
            g_PlayStatsRecord.SetCharacterUnlocked1(7);
    }

    completed = 0;
    for (i = 0; i <= 8; i++)
    {
        if (g_PlayStatsRecord.HasAnyPrimaryCharacterStat((char)i))
            completed++;
    }
    if (completed >= 4)
    {
        if (g_PlayStatsRecord.GetCharacterStat5(6))
            g_PlayStatsRecord.SetCharacterUnlocked1(6);
        if (g_PlayStatsRecord.GetCharacterStat5(8))
            g_PlayStatsRecord.SetCharacterUnlocked1(8);
    }

    completed = 0;
    for (i = 0; i <= 8; i++)
    {
        if (g_PlayStatsRecord.HasAnyPrimaryCharacterStat((char)i))
            completed++;
    }
    if (completed >= 9)
    {
        if (g_PlayStatsRecord.GetCharacterStat5(10))
            g_PlayStatsRecord.SetCharacterUnlocked1(10);
        if (g_PlayStatsRecord.GetCharacterStat5(9))
            g_PlayStatsRecord.SetCharacterUnlocked1(9);
        if (g_PlayStatsRecord.GetCharacterStat5(11))
            g_PlayStatsRecord.SetCharacterUnlocked1(11);
    }

    completed = 0;
    for (i = 0; i <= 11; i++)
    {
        if (g_PlayStatsRecord.HasAnyPrimaryCharacterStat((char)i))
            completed++;
    }
    if (completed > 11)
        g_PlayStatsRecord.SetCharacterUnlocked2(12);

    if (g_PlayStatsRecord.HasAnyPrimaryCharacterStat(12))
        g_PlayStatsRecord.SetCharacterUnlocked2(13);

    if (g_PlayStatsRecord.HasAnyPrimaryCharacterStat(13))
    {
        for (i = 0; i <= 13; i++)
        {
            g_PlayStatsRecord.SetCharacterUnlocked1((char)i);
            g_PlayStatsRecord.SetCharacterUnlocked2((char)i);
        }
        g_PlayStatsRecord.SetCharacterUnlocked0(14);
        g_PlayStatsRecord.SetCharacterUnlocked0(15);
    }

    ending->hasSeenEnding = 1;
    if (ending->LoadEndingScript(g_EndingFilePaths[g_GameManager.side0Character]) != 0)
        return -1;

    if (g_TitleFadeReleaseEffect != 0)
        g_TitleFadeReleaseEffect->BeginFadeRelease();
    g_TitleFadeReleaseEffect = 0;
    return 0;
}
