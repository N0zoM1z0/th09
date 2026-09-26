// Target-bound TH09 music-room view embedded in the TitleScreen state owner.
// TH08 supplies the source family only; TH09 target evidence fixes all layout,
// input, rendering, audio, and file-format details used below.

#include "FileSystem.hpp"

#include <stddef.h>

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef int i32;

extern "C" void *__cdecl memcpy(void *, const void *, size_t);
extern "C" void *__cdecl memset(void *, int, size_t);
extern "C" void __cdecl free(void *);

struct AnmVmView {
    u8 unknown000[0x1F8];
    u32 flags;
    u8 unknown1FC[2];
    u16 pendingInterrupt;
    u8 unknown200[8];
    float posX;
    float posY;
    float posZ;
    u8 unknown214[4];
    short baseSpriteIndex;
    u8 unknown21A[0x8A];
};

typedef char MusicRoomAnmVmSizeIs2A4[(sizeof(AnmVmView) == 0x2A4) ? 1 : -1];
typedef char MusicRoomAnmVmFlagsAt1F8[(offsetof(AnmVmView, flags) == 0x1F8) ? 1 : -1];
typedef char MusicRoomAnmVmInterruptAt1FE[(offsetof(AnmVmView, pendingInterrupt) == 0x1FE) ? 1 : -1];
typedef char MusicRoomAnmVmPosAt208[(offsetof(AnmVmView, posX) == 0x208) ? 1 : -1];

struct MusicRoomTrackDescriptor {
    char path[64];
    char title[66];
    char descriptions[8][66];
};

typedef char MusicRoomTrackDescriptorSizeIs292[(sizeof(MusicRoomTrackDescriptor) == 0x292) ? 1 : -1];

struct TitleAnmLoadedView {
    void SetAndExecuteScriptIdx(AnmVmView *vm, i32 scriptIndex);
};

struct TitleAnmManagerView {
    i32 LoadSurface(i32 slot, const char *path);
    void SetInterruptArray(AnmVmView *vms, i32 count, i32 interrupt);
    void ExecuteScriptArray(AnmVmView *vms, i32 count);
    i32 ExecuteScript(AnmVmView *vm);
    i32 DrawNoRotation(AnmVmView *vm);
};

struct TitleSupervisorView {
    i32 PlayAudio(char *path, i32 bgmUnlockIndex);
    i32 FadeOutMusic(float durationSeconds);
};

struct SoundPlayerView {
    void QueueCommand(i32 opcode, i32 argument, char *path);
};

extern TitleAnmLoadedView *g_TitleMusicDescriptionAnm;
extern TitleAnmManagerView *g_TitleAnmManager;
extern TitleSupervisorView g_TitleSupervisor;
extern SoundPlayerView g_SoundPlayer;
extern u32 g_TitleInputFlags;
extern u8 g_TitleBgmUnlocked[32];
extern const char *g_TitleBgmNotUnlockedWarning[8];

extern void __cdecl DrawTitleMusicText(TitleAnmManagerView *manager, AnmVmView *vm,
                                       u32 color1, u32 color2, const char *format, ...);

struct TitleScreenView {
    i32 keyboardSelection;                    // +0x00000
    u8 unknown00004[0x24];
    i32 currentScreenState;                   // +0x00028
    i32 stateTimer;                           // +0x0002C
    u8 unknown00030[0xC8E0];
    i32 screenFrameCounter;                   // +0x0C910
    u8 unknown0C914[0x20];
    MusicRoomTrackDescriptor musicTracks[32]; // +0x0C934
    i32 musicSelectedSongIndex;               // +0x11B74
    i32 musicTrackCount;                      // +0x11B78
    i32 musicListingOffset;                   // +0x11B7C
    i32 musicPaused;                          // +0x11B80
    u8 unknown11B84[0x0C];
    TitleAnmLoadedView *musicAnm;             // +0x11B90
    u8 unknown11B94[0x04];
    AnmVmView *vms;                           // +0x11B98
    i32 uiAux;                                // +0x11B9C
    AnmVmView embeddedVms[57];                // +0x11BA0
    i32 vmCount;                              // +0x1B224
    i32 currentScreen;                        // +0x1B228
    u8 unknown1B22C[4];
    i32 stateTimer2;                          // +0x1B230

    i32 PlayMenuSound(i32 soundId, i32 unused);
    i32 ChangeCurrentScreen(i32 screen);
    i32 MoveCursorVertical(i32 count);
    i32 SetMenuSelectionSprites(i32 selected, i32 start, i32 count);
    i32 OnUpdateMusicRoom();
    i32 DrawMusicRoom();
};

typedef char MusicRoomTracksAtC934[(offsetof(TitleScreenView, musicTracks) == 0xC934) ? 1 : -1];
typedef char MusicRoomSelectedAt11B74[(offsetof(TitleScreenView, musicSelectedSongIndex) == 0x11B74) ? 1 : -1];
typedef char MusicRoomCountAt11B78[(offsetof(TitleScreenView, musicTrackCount) == 0x11B78) ? 1 : -1];
typedef char MusicRoomListingAt11B7C[(offsetof(TitleScreenView, musicListingOffset) == 0x11B7C) ? 1 : -1];
typedef char MusicRoomPausedAt11B80[(offsetof(TitleScreenView, musicPaused) == 0x11B80) ? 1 : -1];
typedef char MusicRoomAnmAt11B90[(offsetof(TitleScreenView, musicAnm) == 0x11B90) ? 1 : -1];
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
                DrawTitleMusicText(g_TitleAnmManager, &embeddedVms[49], 0xE0E0FF, 0x302080,
                                   musicTracks[musicSelectedSongIndex].descriptions[0]);
            else
                DrawTitleMusicText(g_TitleAnmManager, &embeddedVms[49], 0xE0E0FF, 0x302080,
                                   g_TitleBgmNotUnlockedWarning[0]);
        }
        else if (stateTimer2 == 4)
        {
            embeddedVms[50].pendingInterrupt = 1;
            if (g_TitleBgmUnlocked[musicSelectedSongIndex])
                DrawTitleMusicText(g_TitleAnmManager, &embeddedVms[50], 0xE0E0FF, 0x302080,
                                   musicTracks[musicSelectedSongIndex].descriptions[1]);
            else
                DrawTitleMusicText(g_TitleAnmManager, &embeddedVms[50], 0xE0E0FF, 0x302080,
                                   g_TitleBgmNotUnlockedWarning[1]);
        }
        else if (stateTimer2 == 6)
        {
            embeddedVms[51].pendingInterrupt = 1;
            if (g_TitleBgmUnlocked[musicSelectedSongIndex])
                DrawTitleMusicText(g_TitleAnmManager, &embeddedVms[51], 0xE0E0FF, 0x302080,
                                   musicTracks[musicSelectedSongIndex].descriptions[2]);
            else
                DrawTitleMusicText(g_TitleAnmManager, &embeddedVms[51], 0xE0E0FF, 0x302080,
                                   g_TitleBgmNotUnlockedWarning[2]);
        }
        else if (stateTimer2 == 8)
        {
            embeddedVms[52].pendingInterrupt = 1;
            if (g_TitleBgmUnlocked[musicSelectedSongIndex])
                DrawTitleMusicText(g_TitleAnmManager, &embeddedVms[52], 0xE0E0FF, 0x302080,
                                   musicTracks[musicSelectedSongIndex].descriptions[3]);
            else
                DrawTitleMusicText(g_TitleAnmManager, &embeddedVms[52], 0xE0E0FF, 0x302080,
                                   g_TitleBgmNotUnlockedWarning[3]);
        }
        else if (stateTimer2 == 10)
        {
            embeddedVms[53].pendingInterrupt = 1;
            if (g_TitleBgmUnlocked[musicSelectedSongIndex])
                DrawTitleMusicText(g_TitleAnmManager, &embeddedVms[53], 0xE0E0FF, 0x302080,
                                   musicTracks[musicSelectedSongIndex].descriptions[4]);
            else
                DrawTitleMusicText(g_TitleAnmManager, &embeddedVms[53], 0xE0E0FF, 0x302080,
                                   g_TitleBgmNotUnlockedWarning[4]);
        }
        else if (stateTimer2 == 12)
        {
            embeddedVms[54].pendingInterrupt = 1;
            if (g_TitleBgmUnlocked[musicSelectedSongIndex])
                DrawTitleMusicText(g_TitleAnmManager, &embeddedVms[54], 0xE0E0FF, 0x302080,
                                   musicTracks[musicSelectedSongIndex].descriptions[5]);
            else
                DrawTitleMusicText(g_TitleAnmManager, &embeddedVms[54], 0xE0E0FF, 0x302080,
                                   g_TitleBgmNotUnlockedWarning[5]);
        }
        else if (stateTimer2 == 14)
        {
            embeddedVms[55].pendingInterrupt = 1;
            if (g_TitleBgmUnlocked[musicSelectedSongIndex])
                DrawTitleMusicText(g_TitleAnmManager, &embeddedVms[55], 0xE0E0FF, 0x302080,
                                   musicTracks[musicSelectedSongIndex].descriptions[6]);
            else
                DrawTitleMusicText(g_TitleAnmManager, &embeddedVms[55], 0xE0E0FF, 0x302080,
                                   g_TitleBgmNotUnlockedWarning[6]);
        }
        else if (stateTimer2 == 16)
        {
            embeddedVms[56].pendingInterrupt = 1;
            if (g_TitleBgmUnlocked[musicSelectedSongIndex])
                DrawTitleMusicText(g_TitleAnmManager, &embeddedVms[56], 0xE0E0FF, 0x302080,
                                   musicTracks[musicSelectedSongIndex].descriptions[7]);
            else
                DrawTitleMusicText(g_TitleAnmManager, &embeddedVms[56], 0xE0E0FF, 0x302080,
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

            SetMenuSelectionSprites(keyboardSelection, 159, musicTrackCount);

            for (i = 0; i < musicListingOffset; i++)
                vms[159 + i].flags &= ~2u;

            i32 visibleEnd = musicTrackCount;
            if (musicListingOffset + 10 < visibleEnd)
                visibleEnd = musicListingOffset + 10;
            for (i = musicListingOffset; i < visibleEnd; i++)
            {
                vms[159 + i].flags |= 2;
                vms[159 + i].posY = (float)((i - musicListingOffset + 1) * 18) + 104.0f - 20.0f;
            }
            for (i = visibleEnd; i <= 29; i++)
                vms[159 + i].flags &= ~2u;
        }

        if (g_TitleInputFlags & 4)
            g_TitleSupervisor.FadeOutMusic(20.0f);

        if (g_TitleInputFlags & 0x200)
        {
            if (musicPaused)
                g_SoundPlayer.QueueCommand(7, 0, "UnPause");
            else
                g_SoundPlayer.QueueCommand(6, 0, "Pause");
            musicPaused = 1 - musicPaused;
        }

        if (g_TitleInputFlags & 0x4000)
        {
            if (musicPaused)
                g_SoundPlayer.QueueCommand(7, 0, "UnPause");
            musicPaused = 0;
            g_TitleSupervisor.PlayAudio(musicTracks[musicSelectedSongIndex].path, 0);
        }

        if (g_TitleInputFlags & 0x1001)
        {
            if (musicPaused)
                g_SoundPlayer.QueueCommand(7, 0, "UnPause");
            musicSelectedSongIndex = keyboardSelection;
            musicPaused = 0;
            g_TitleSupervisor.PlayAudio(musicTracks[keyboardSelection].path, 0);
            stateTimer2 = 0;
            for (i = 0; i < 8; i++)
                g_TitleMusicDescriptionAnm->SetAndExecuteScriptIdx(&embeddedVms[49 + i], 6 + i);
            goto common_tail;
        }

        if ((g_TitleInputFlags & 0xA) != 0)
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
            if (g_TitleAnmManager->LoadSurface(0, "title/music00.png"))
                return 0;

            g_TitleAnmManager->SetInterruptArray(vms, vmCount, 16);
            g_TitleAnmManager->ExecuteScriptArray(vms, vmCount);

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
                        musicTracks[trackIndex].path[charIndex++] = *cursor++;
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
                        musicTracks[trackIndex].title[charIndex++] = *cursor++;
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
                            musicTracks[trackIndex].descriptions[descriptionLine][charIndex++] = *cursor++;
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

            MusicRoomTrackDescriptor *musicTrack = &musicTracks[0];
            for (i = 159; i - 159 < musicTrackCount; i++, musicTrack++)
            {
                musicAnm->SetAndExecuteScriptIdx(&vms[i], i);
                if (g_TitleBgmUnlocked[i - 159])
                {
                    DrawTitleMusicText(g_TitleAnmManager, &vms[i], 0xD0E0FF, 0x302080,
                                       musicTrack->title);
                }
                else
                {
                    char shortTitle[6];
                    memcpy(shortTitle, musicTrack->title, 5);
                    shortTitle[5] = '\0';
                    DrawTitleMusicText(g_TitleAnmManager, &vms[i], 0x80A0A0, 0x100040,
                                       "%5s ", shortTitle);
                }

                vms[i].posX = 93.0f;
                vms[i].posY = (float)((i - 159 + 1) * 18) + 104.0f - 20.0f;
                vms[i].posZ = 0.0f;
                ((u8 *)&vms[i].flags)[1] |= 0x18;
            }

            SetMenuSelectionSprites(keyboardSelection, 159, musicTrackCount);

            for (i = 0; i < musicListingOffset; i++)
                vms[159 + i].flags &= ~2u;

            i32 visibleEnd = musicTrackCount;
            if (musicListingOffset + 10 < visibleEnd)
                visibleEnd = musicListingOffset + 10;
            for (i = musicListingOffset; i < visibleEnd; i++)
            {
                vms[159 + i].flags |= 2;
                vms[159 + i].posY = (float)((i - musicListingOffset + 1) * 18) + 104.0f - 20.0f;
            }
            for (i = visibleEnd; i <= 29; i++)
                vms[159 + i].flags &= ~2u;

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
        g_TitleAnmManager->ExecuteScript(&embeddedVms[49 + i]);
    stateTimer++;
    screenFrameCounter++;
    stateTimer2++;
    return 1;
}

struct MusicRoomFloat3View
{
    float x;
    float y;
    float z;
};

inline u32 &MusicRoomColor(AnmVmView *vm)
{
    return *reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(vm) + 0x1F0);
}

int TitleScreenView::DrawMusicRoom()
{
    if (currentScreenState == 1)
    {
        i32 songVmIndex = musicSelectedSongIndex + 159;
        MusicRoomFloat3View savedPosition =
            *reinterpret_cast<MusicRoomFloat3View *>(&vms[songVmIndex].posX);
        u32 savedColor = MusicRoomColor(&vms[songVmIndex]);

        MusicRoomColor(&vms[songVmIndex]) = 0xFFFFFFFF;
        vms[songVmIndex].posX = 208.0f;
        vms[songVmIndex].posY = 293.0f;
        g_TitleAnmManager->DrawNoRotation(&vms[songVmIndex]);

        *reinterpret_cast<MusicRoomFloat3View *>(&vms[songVmIndex].posX) =
            savedPosition;
        MusicRoomColor(&vms[songVmIndex]) = savedColor;

        for (i32 i = 0; i < 8; ++i)
            g_TitleAnmManager->DrawNoRotation(&embeddedVms[49 + i]);
    }

    return 1;
}
