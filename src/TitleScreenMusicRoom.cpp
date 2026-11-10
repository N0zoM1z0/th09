// Target-bound TH09 music-room view embedded in the TitleScreen state owner.
// TH08 supplies the source family only; TH09 target evidence fixes all layout,
// input, rendering, audio, and file-format details used below.

#include "FileSystem.hpp"
#include "AnmManager.hpp"
#include "AnmVmLifecycle.hpp"
#include "ReplayInputState.hpp"
#include "Supervisor.hpp"
#include "SoundPlayer.hpp"
#include "TitleScreenReplayView.hpp"

#include <stddef.h>

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef int i32;

extern "C" void *__cdecl memcpy(void *, const void *, size_t);
extern "C" void *__cdecl memset(void *, int, size_t);
extern "C" void __cdecl free(void *);

typedef char MusicRoomAnmVmSizeIs2A4[(sizeof(AnmVm) == 0x2A4) ? 1 : -1];
typedef char MusicRoomAnmVmFlagsAt1F8[(offsetof(AnmVm, flagsWord) == 0x1F8) ? 1 : -1];
typedef char MusicRoomAnmVmInterruptAt1FE[(offsetof(AnmVm, pendingInterrupt) == 0x1FE) ? 1 : -1];
typedef char MusicRoomAnmVmPosAt208[(offsetof(AnmVm, pos) == 0x208) ? 1 : -1];

// Music-room track records are fields of the full TitleScreenView.
// Their 0x292-byte POD shape is checked in TitleScreenReplayView.hpp.

extern AnmLoaded *g_TitleMusicDescriptionAnm;

typedef char MusicInputSlotSizeIs8E[(sizeof(ReplayInputState) == 0x8E) ? 1 : -1];
typedef char MusicInputFrontAt06[(offsetof(ReplayInputState, word06) == 6) ? 1 : -1];
extern u8 g_TitleBgmUnlocked[32];
extern const char *g_TitleBgmNotUnlockedWarning[8];


typedef char MusicRoomTracksAtC934[(offsetof(TitleScreenView, musicTracks) == 0xC934) ? 1 : -1];
typedef char MusicRoomSelectedAt11B74[(offsetof(TitleScreenView, musicSelectedSongIndex) == 0x11B74) ? 1 : -1];
typedef char MusicRoomCountAt11B78[(offsetof(TitleScreenView, musicTrackCount) == 0x11B78) ? 1 : -1];
typedef char MusicRoomListingAt11B7C[(offsetof(TitleScreenView, musicListingOffset) == 0x11B7C) ? 1 : -1];
typedef char MusicRoomPausedAt11B80[(offsetof(TitleScreenView, musicPaused) == 0x11B80) ? 1 : -1];
typedef char MusicRoomAnmAt11B90[(offsetof(TitleScreenView, titleAnm) == 0x11B90) ? 1 : -1];
typedef char MusicRoomVmsAt11B98[(offsetof(TitleScreenView, vms) == 0x11B98) ? 1 : -1];
typedef char MusicRoomEmbeddedAt11BA0[(offsetof(TitleScreenView, embeddedVms) == 0x11BA0) ? 1 : -1];
typedef char MusicRoomVmCountAt1B224[(offsetof(TitleScreenView, vmCount) == 0x1B224) ? 1 : -1];
typedef char MusicRoomTimerAt1B230[(offsetof(TitleScreenView, stateTimer2) == 0x1B230) ? 1 : -1];

int TitleScreenView::OnUpdateMusicRoom()
{
    i32 i;

    switch (currentScreenState)
    {
    case 1:
    {
        if (stateTimer2 == 2)
        {
            embeddedVms[49].pendingInterrupt = 1;
            if (g_TitleBgmUnlocked[musicSelectedSongIndex])
                g_AnmManager->DrawTextLeft(&embeddedVms[49], 0xE0E0FF, 0x302080,
                                   musicTracks[musicSelectedSongIndex].descriptions[0]);
            else
                g_AnmManager->DrawTextLeft(&embeddedVms[49], 0xE0E0FF, 0x302080,
                                   g_TitleBgmNotUnlockedWarning[0]);
        }
        else if (stateTimer2 == 4)
        {
            embeddedVms[50].pendingInterrupt = 1;
            if (g_TitleBgmUnlocked[musicSelectedSongIndex])
                g_AnmManager->DrawTextLeft(&embeddedVms[50], 0xE0E0FF, 0x302080,
                                   musicTracks[musicSelectedSongIndex].descriptions[1]);
            else
                g_AnmManager->DrawTextLeft(&embeddedVms[50], 0xE0E0FF, 0x302080,
                                   g_TitleBgmNotUnlockedWarning[1]);
        }
        else if (stateTimer2 == 6)
        {
            embeddedVms[51].pendingInterrupt = 1;
            if (g_TitleBgmUnlocked[musicSelectedSongIndex])
                g_AnmManager->DrawTextLeft(&embeddedVms[51], 0xE0E0FF, 0x302080,
                                   musicTracks[musicSelectedSongIndex].descriptions[2]);
            else
                g_AnmManager->DrawTextLeft(&embeddedVms[51], 0xE0E0FF, 0x302080,
                                   g_TitleBgmNotUnlockedWarning[2]);
        }
        else if (stateTimer2 == 8)
        {
            embeddedVms[52].pendingInterrupt = 1;
            if (g_TitleBgmUnlocked[musicSelectedSongIndex])
                g_AnmManager->DrawTextLeft(&embeddedVms[52], 0xE0E0FF, 0x302080,
                                   musicTracks[musicSelectedSongIndex].descriptions[3]);
            else
                g_AnmManager->DrawTextLeft(&embeddedVms[52], 0xE0E0FF, 0x302080,
                                   g_TitleBgmNotUnlockedWarning[3]);
        }
        else if (stateTimer2 == 10)
        {
            embeddedVms[53].pendingInterrupt = 1;
            if (g_TitleBgmUnlocked[musicSelectedSongIndex])
                g_AnmManager->DrawTextLeft(&embeddedVms[53], 0xE0E0FF, 0x302080,
                                   musicTracks[musicSelectedSongIndex].descriptions[4]);
            else
                g_AnmManager->DrawTextLeft(&embeddedVms[53], 0xE0E0FF, 0x302080,
                                   g_TitleBgmNotUnlockedWarning[4]);
        }
        else if (stateTimer2 == 12)
        {
            embeddedVms[54].pendingInterrupt = 1;
            if (g_TitleBgmUnlocked[musicSelectedSongIndex])
                g_AnmManager->DrawTextLeft(&embeddedVms[54], 0xE0E0FF, 0x302080,
                                   musicTracks[musicSelectedSongIndex].descriptions[5]);
            else
                g_AnmManager->DrawTextLeft(&embeddedVms[54], 0xE0E0FF, 0x302080,
                                   g_TitleBgmNotUnlockedWarning[5]);
        }
        else if (stateTimer2 == 14)
        {
            embeddedVms[55].pendingInterrupt = 1;
            if (g_TitleBgmUnlocked[musicSelectedSongIndex])
                g_AnmManager->DrawTextLeft(&embeddedVms[55], 0xE0E0FF, 0x302080,
                                   musicTracks[musicSelectedSongIndex].descriptions[6]);
            else
                g_AnmManager->DrawTextLeft(&embeddedVms[55], 0xE0E0FF, 0x302080,
                                   g_TitleBgmNotUnlockedWarning[6]);
        }
        else if (stateTimer2 == 16)
        {
            embeddedVms[56].pendingInterrupt = 1;
            if (g_TitleBgmUnlocked[musicSelectedSongIndex])
                g_AnmManager->DrawTextLeft(&embeddedVms[56], 0xE0E0FF, 0x302080,
                                   musicTracks[musicSelectedSongIndex].descriptions[7]);
            else
                g_AnmManager->DrawTextLeft(&embeddedVms[56], 0xE0E0FF, 0x302080,
                                   g_TitleBgmNotUnlockedWarning[7]);
        }

        if (MoveCursorVertical(musicTrackCount))
        {
            if (keyboardSelection < musicListingOffset)
            {
                musicListingOffset = keyboardSelection;
            }
            else if (keyboardSelection >= musicListingOffset + 10)
            {
                musicListingOffset = keyboardSelection - 9;
                if (musicListingOffset < 0)
                    musicListingOffset = 0;
            }

            SetRangeSelectionInterrupts(keyboardSelection, 159, musicTrackCount);

            for (i = 0; i < musicListingOffset; i++)
                vms[159 + i].drawEnabled = 0;

            i = musicListingOffset;
            // Keep the bound distinct from the visible and hidden traversal cursors.
            const i32 visibleEnd = i + 10 < musicTrackCount ? i + 10 : musicTrackCount;
            for (; i < visibleEnd; i++)
            {
                vms[159 + i].drawEnabled = 1;
                vms[159 + i].pos.y = (float)((i - musicListingOffset + 1) * 18) + 104.0f - 20.0f;
            }
            for (i32 hiddenRow = visibleEnd; hiddenRow <= 29; hiddenRow++)
                vms[159 + hiddenRow].drawEnabled = 0;
        }

        if (g_ReplayInputStates[2].word06 & 4)
            g_Supervisor.FadeOutMusic(20.0f);

        if (g_ReplayInputStates[2].word06 & 0x200)
        {
            if (!musicPaused)
                g_SoundPlayer.QueueCommand(6, 0, "Pause");
            else
                g_SoundPlayer.QueueCommand(7, 0, "UnPause");
            musicPaused = 1 - musicPaused;
        }

        if (g_ReplayInputStates[2].word06 & 0x4000)
        {
            if (musicPaused)
                g_SoundPlayer.QueueCommand(7, 0, "UnPause");
            musicPaused = 0;
            g_Supervisor.PlayAudio(musicTracks[musicSelectedSongIndex].path, 0);
        }

        if (g_ReplayInputStates[2].word06 & 0x1001)
        {
            if (musicPaused)
                g_SoundPlayer.QueueCommand(7, 0, "UnPause");
            musicSelectedSongIndex = keyboardSelection;
            musicPaused = 0;
            g_Supervisor.PlayAudio(musicTracks[keyboardSelection].path, 0);
            stateTimer2 = 0;
            for (i = 0; i < 8; i++)
                g_TitleMusicDescriptionAnm->SetAndExecuteScriptIdx(&embeddedVms[49 + i], 6 + i);
            goto common_tail;
        }

        if ((g_ReplayInputStates[2].word06 & 0xA) != 0)
        {
            PlayMenuSound(11, 0);
            stateTimer = 0;
            ChangeCurrentScreen(1);
            keyboardSelection = 5;
            return 1;
        }
        break;
    }

    case 0:
    {
        if (stateTimer2 == 0)
        {
            if (g_AnmManager->LoadSurface(0, "title/music00.png"))
                return 0;

            g_AnmManager->SetInterruptArray(reinterpret_cast<AnmVm *>(vms), vmCount, 16);
            g_AnmManager->ExecuteScriptArray(reinterpret_cast<AnmVm *>(vms), vmCount);

            keyboardSelection = 0;
            musicListingOffset = 0;
            currentScreenState = 0;
            stateTimer = 0;
            uiAux = 0;

            i32 fileSize;
            char *musicCmtFile = (char *)FileSystem::OpenFile("sprt/musiccmt.txt", &fileSize, 0);
            if (musicCmtFile == 0)
                return -1;

            char *cursor = musicCmtFile;
            i32 trackIndex = -1;
            while ((u32)(cursor - musicCmtFile) < (u32)fileSize)
            {
                if (*cursor == '@')
                {
                    cursor++;
                    trackIndex++;
                    i32 charIndex = 0;

                    while (*cursor != '\n' && *cursor != '\r')
                    {
                        musicTracks[trackIndex].path[charIndex] = *cursor;
                        cursor++;
                        charIndex++;
                        if ((u32)(cursor - musicCmtFile) >= (u32)fileSize)
                            goto parse_done;
                    }
                    while (*cursor == '\n' || *cursor == '\r')
                    {
                        cursor++;
                        if ((u32)(cursor - musicCmtFile) >= (u32)fileSize)
                            goto parse_done;
                    }

                    charIndex = 0;
                    while (*cursor != '\n' && *cursor != '\r')
                    {
                        musicTracks[trackIndex].title[charIndex] = *cursor;
                        cursor++;
                        charIndex++;
                        if ((u32)(cursor - musicCmtFile) >= (u32)fileSize)
                            goto parse_done;
                    }
                    while (*cursor == '\n' || *cursor == '\r')
                    {
                        cursor++;
                        if ((u32)(cursor - musicCmtFile) >= (u32)fileSize)
                            goto parse_done;
                    }

                    for (i32 descriptionLine = 0; descriptionLine < 7; descriptionLine++)
                    {
                        if (*cursor == '@')
                            break;

                        memset(musicTracks[trackIndex].descriptions[descriptionLine], 0,
                               sizeof(musicTracks[trackIndex].descriptions[descriptionLine]));
                        charIndex = 0;
                        while (*cursor != '\n' && *cursor != '\r')
                        {
                            musicTracks[trackIndex].descriptions[descriptionLine][charIndex] = *cursor;
                            cursor++;
                            charIndex++;
                            if ((u32)(cursor - musicCmtFile) >= (u32)fileSize)
                                goto parse_done;
                        }
                        while (*cursor == '\n' || *cursor == '\r')
                        {
                            cursor++;
                            if ((u32)(cursor - musicCmtFile) >= (u32)fileSize)
                                goto parse_done;
                        }
                    }
                }
                else
                {
                    cursor++;
                }
            }

        parse_done:
            free(musicCmtFile);
            musicTrackCount = trackIndex + 1;

            if (musicTrackCount > 0)
            {
                i = 159;
                MusicRoomTrackDescriptor *musicTrack = musicTracks;
                i32 vmIndex = 0;
                i32 rowY = 18;
                do
                {
                    titleAnm->SetAndExecuteScriptIdx(&vms[159 + vmIndex], i);
                    // vmIndex is the descriptor row; i is the 159-based script id.
                    if (g_TitleBgmUnlocked[vmIndex])
                    {
                        g_AnmManager->DrawTextLeft(&vms[159 + vmIndex], 0xD0E0FF, 0x302080,
                                           musicTrack->title);
                    }
                    else
                    {
                        char shortTitle[6];
                        memcpy(shortTitle, musicTrack->title, 5);
                        shortTitle[5] = '\0';
                        g_AnmManager->DrawTextLeft(&vms[159 + vmIndex], 0x80A0A0, 0x100040,
                                           "%5s \x81\x48\x81\x48\x81\x48\x81\x48"
                                           "\x81\x48\x81\x48\x81\x48\x81\x48", shortTitle);
                    }

                    vms[159 + vmIndex].pos.x = 93.0f;
                    vms[159 + vmIndex].pos.y = (float)rowY + 104.0f - 20.0f;
                    vms[159 + vmIndex].pos.z = 0.0f;
                    vms[159 + vmIndex].anchor = 3;
                    i++;
                    musicTrack++;
                    vmIndex++;
                    rowY += 18;
                } while (vmIndex < musicTrackCount);
            }

            SetRangeSelectionInterrupts(keyboardSelection, 159, musicTrackCount);

            for (i = 0; i < musicListingOffset; i++)
                vms[159 + i].drawEnabled = 0;

            i = musicListingOffset;
            // Keep the bound distinct from the visible and hidden traversal cursors.
            const i32 visibleEnd = i + 10 < musicTrackCount ? i + 10 : musicTrackCount;
            for (; i < visibleEnd; i++)
            {
                vms[159 + i].drawEnabled = 1;
                vms[159 + i].pos.y = (float)((i - musicListingOffset + 1) * 18) + 104.0f - 20.0f;
            }
            for (i32 hiddenRow = visibleEnd; hiddenRow <= 29; hiddenRow++)
                vms[159 + hiddenRow].drawEnabled = 0;

            musicSelectedSongIndex = keyboardSelection;
            for (i = 0; i < 8; i++)
                g_TitleMusicDescriptionAnm->SetAndExecuteScriptIdx(&embeddedVms[49 + i], 6 + i);
            musicPaused = 0;
        }

        if (stateTimer2 == 8)
        {
            currentScreenState = 1;
            stateTimer2 = 0;
        }
        goto common_tail;
    }

    default:
        break;
    }

common_tail:
    for (i = 0; i < 8; i++)
        g_AnmManager->ExecuteScript(&embeddedVms[49 + i]);
    stateTimer++;
    screenFrameCounter++;
    stateTimer2++;
    return 1;
}

int TitleScreenView::DrawMusicRoom()
{
    if (currentScreenState == 1)
    {
        i32 songVmIndex = musicSelectedSongIndex + 159;
        Float3 savedPosition = vms[songVmIndex].pos;
        u32 savedColor = vms[songVmIndex].color1;

        vms[songVmIndex].color1 = 0xFFFFFFFF;
        vms[songVmIndex].pos.x = 208.0f;
        vms[songVmIndex].pos.y = 293.0f;
        g_AnmManager->DrawNoRotation(&vms[songVmIndex]);

        vms[songVmIndex].pos = savedPosition;
        vms[songVmIndex].color1 = savedColor;

        for (i32 i = 0; i < 8; ++i)
            g_AnmManager->DrawNoRotation(&embeddedVms[49 + i]);
    }

    return 1;
}
