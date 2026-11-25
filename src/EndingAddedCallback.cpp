#include "Ending.hpp"

#include "AnmManager.hpp"
#include "GameManagerRuntimeView.hpp"
#include "PlayStats.hpp"
#include "ScreenEffect.hpp"
#include "Supervisor.hpp"

#include <stddef.h>

#include "EndingNativeVm.hpp"

// The text ANM pointer belongs to the actual Supervisor at +0x5CC.
typedef char EndingSupervisorNativeTextOwner[
    (offsetof(Supervisor, textAnm5CC) == 0x5CC) ? 1 : -1];

ScreenEffect *g_TitleFadeReleaseEffect = NULL;
const char *g_EndingFilePaths[16] = {
    "end00.end",
    "end01.end",
    "end02.end",
    "end03.end",
    "end04.end",
    "end05.end",
    "end06.end",
    "end07.end",
    "end08.end",
    "end09.end",
    "end10.end",
    "end11.end",
    "end12.end",
    "end13.end",
    NULL,
    NULL,
};

// Ending scripts select the actual sixteen-element global path table.
// Keep the original live character selector without caching the table row.

int Ending::AddedCallback(Ending *ending)
{
    g_Supervisor.flags |= 0x400u;
    g_AnmManager->ClearTexture();
    g_AnmManager->ClearSprite();
    g_AnmManager->ClearBlendMode();
    g_AnmManager->ClearVertexShader();
    ScreenEffect::Clear(0xFFFFFFFFu);
    g_Supervisor.startupField5A8 = 1;

    Supervisor *supervisor = &g_Supervisor;
    for (int i = 0; i < 15; i++)
    {
        reinterpret_cast<AnmLoaded *>(supervisor->textAnm5CC)->ExecuteAnmIdx(
            &TH09_ENDING_NATIVE_VM(ending, i), i + 14);
        TH09_ENDING_NATIVE_VM(ending, i).pos =
            Float3(64.0f, i * 16.0f + 400.0f, 0.0f);
    }

    if (g_GameManager.IsGameMode0())
    {
        g_PlayStatsRecord.IncrementCharacterStat(
            (char)g_GameManager.sides[0].character20, g_GameManager.difficulty);
    }
    else if (g_GameManager.IsGameMode1())
    {
        g_PlayStatsRecord.IncrementCharacterStat(
            (char)g_GameManager.sides[0].character20, g_GameManager.difficulty);
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
    if (ending->LoadEndingScript(g_EndingFilePaths[g_GameManager.sides[0].character20]) != 0)
        return -1;

    if (g_TitleFadeReleaseEffect != 0)
        g_TitleFadeReleaseEffect->BeginFadeRelease();
    g_TitleFadeReleaseEffect = 0;
    return 0;
}


#undef TH09_ENDING_NATIVE_VM
