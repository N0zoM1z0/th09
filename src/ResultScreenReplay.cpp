// Target-bound ResultScreen replay-flow reconstruction views. Original C++ type
// names, source ownership details, and translation-unit partition remain unresolved.

#include <stddef.h>

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef int i32;

extern "C" int __cdecl sprintf(char *, const char *, ...);
extern "C" char *__cdecl strcpy(char *, const char *);
extern "C" void *__cdecl memset(void *, int, size_t);
extern "C" void __cdecl free(void *);
extern "C" int __cdecl _mkdir(const char *);

struct ReplayDataView {
    u32 magic;
    u8 bytes[0x1E8];
};

typedef char ReplayDataSizeIs1EC[(sizeof(ReplayDataView) == 0x1EC) ? 1 : -1];

struct ResultNameRecordView {
    char name[9];
    u8 unknown09[35];
};

typedef char ResultNameRecordSizeIs2C[(sizeof(ResultNameRecordView) == 0x2C) ? 1 : -1];

struct InputView {
    u16 IsPressedScrolling(i32 mask);
};

struct SoundPlayerView {
    void PlaySoundByIdx(i32 soundId, i32 unused);
};

struct UiManagerView {
    int LoadFile(i32 slot, const char *path);
    void SetUi(i32 vm, i32 script, i32 state);
    void UpdateUi(i32 vm, i32 script);
};

struct ReplayManagerView {
    static ReplayDataView *LoadReplayData(ReplayDataView *data, i32 fileSize);
    static int SaveReplay(ReplayManagerView *replayManager, const char *replayPath, const char *replayName);
};

struct FileSystemView {
    static int CheckIfFileAlreadyExists(const char *path);
    static void *OpenFile(const char *path, i32 *fileSize, i32 loadFromDisk);
};

struct ResultScreenView {
    i32 keyboardSelection;                    // +0x00000
    i32 replayNameCursor;                     // +0x00004
    i32 nameBankIndex;                        // +0x00008
    i32 nameSlotIndex;                        // +0x0000C
    u8 unknown00010[0x18];
    i32 replaySaveState;                      // +0x00028
    i32 stateTimer;                           // +0x0002C
    u8 unknown00030[0x5C];
    char replayName[9];                       // +0x0008C
    u8 unknown00095[0x1F7];
    char replayPaths[25][0x200];              // +0x0028C
    u8 unknown0348C[0x3458];
    ReplayDataView replays[50];               // +0x068E4
    u8 unknown0C8FC[0x0C];
    i32 selectedReplay;                       // +0x0C908
    i32 unknown0C90C;
    i32 replayUiFrameCounter;                 // +0x0C910
    u8 unknown0C914[0x5284];
    i32 uiVm;                                 // +0x11B98
    i32 uiAux;                                // +0x11B9C
    u8 unknown11BA0[0x9684];
    i32 uiScript;                             // +0x1B224
    i32 screenMode;                           // +0x1B228
    i32 unknown1B22C;
    i32 phaseTimer;                           // +0x1B230
    i32 unknown1B234;
    i32 dispatcherState;                      // +0x1B238
    i32 resultAction;                         // +0x1B23C
    i32 transitionId;                         // +0x1B240
    i32 transitionMode;                       // +0x1B244

    int UpdateReplaySave();
    void SetUiAction(i32 action, i32 value);
    void CloseReplayUi(i32 value);
    int MoveTwoChoiceCursor(i32 count);
    int MoveReplayCursor(i32 count);
    void RefreshUi();
};

typedef char ResultStateAt28[(offsetof(ResultScreenView, replaySaveState) == 0x28) ? 1 : -1];
typedef char ResultReplayNameAt8C[(offsetof(ResultScreenView, replayName) == 0x8C) ? 1 : -1];
typedef char ResultPathsAt28C[(offsetof(ResultScreenView, replayPaths) == 0x28C) ? 1 : -1];
typedef char ResultReplaysAt68E4[(offsetof(ResultScreenView, replays) == 0x68E4) ? 1 : -1];
typedef char ResultSelectedReplayAtC908[(offsetof(ResultScreenView, selectedReplay) == 0xC908) ? 1 : -1];
typedef char ResultUiVmAt11B98[(offsetof(ResultScreenView, uiVm) == 0x11B98) ? 1 : -1];
typedef char ResultUiScriptAt1B224[(offsetof(ResultScreenView, uiScript) == 0x1B224) ? 1 : -1];
typedef char ResultPhaseAt1B230[(offsetof(ResultScreenView, phaseTimer) == 0x1B230) ? 1 : -1];

extern InputView g_ResultInput;
extern u32 g_ResultInputFlags;
extern SoundPlayerView g_SoundPlayer;
extern UiManagerView *g_ResultUiManager;
extern ReplayManagerView *g_ReplayManager;
extern char *g_ResultAlphabet;
extern char g_ReplayName[];
extern ResultNameRecordView g_ResultNameTable[][5][5];
extern i32 g_ResultNameTableIndex;
extern i32 g_ResultMode;
extern u32 g_ResultGlobalFlags;
extern void **g_ResultOptionPointers;

int ResultScreenView::UpdateReplaySave()
{
    char path[256];
    char source[64];
    i32 fileSize;
    ReplayDataView *loadedReplay;
    char *replayPath;
    i32 nextIndex;
    ReplayDataView *replayCursor;
    u32 inputFlags;

    switch (replaySaveState)
    {
    case 0:
        if (phaseTimer == 0)
        {
            if (g_ResultGlobalFlags & 0x2000)
            {
                stateTimer = 0;
                CloseReplayUi(1);
                keyboardSelection = 0;
                return 1;
            }

            if (g_ResultOptionPointers[42] != 0)
            {
                i32 replayIndex = 0;
                do
                {
                    nextIndex = replayIndex + 1;
                    sprintf(path, "./replay/th9_udn%.3d.rpy", replayIndex + 1);
                    if (!FileSystemView::CheckIfFileAlreadyExists(path))
                        break;
                    replayIndex = nextIndex;
                }
                while (nextIndex < 1000);

                if (replayIndex >= 1000)
                    replayIndex = 999;

                sprintf(path, "./replay/th9_udn%.3d.rpy", replayIndex + 1);
                ReplayManagerView::SaveReplay(g_ReplayManager, path, g_ReplayName);
                stateTimer = 0;
                CloseReplayUi(1);
                keyboardSelection = 0;
                resultAction = 2;
                return 1;
            }

            if (g_ResultUiManager->LoadFile(0, "title/replay00.png"))
                return 0;

            g_ResultUiManager->SetUi(uiVm, uiScript, 19);
            g_ResultUiManager->UpdateUi(uiVm, uiScript);
            keyboardSelection = 0;
            strcpy(replayName, g_ReplayName);
            replaySaveState = 0;
            stateTimer = 0;
            uiAux = 0;

            if (g_ResultMode == 2)
                goto load_replays;

            transitionId = 125;
            transitionMode = 2;
            RefreshUi();
            _mkdir("replay");
        }

        if (phaseTimer == 8)
            replaySaveState = 1;
        break;

    case 1:
        if (MoveTwoChoiceCursor(2))
            RefreshUi();

        if (g_ResultInputFlags & 0xA)
        {
            if (keyboardSelection == 0)
            {
                SetUiAction(11, 0);
                keyboardSelection = 1;
                RefreshUi();
                break;
            }

        cancel_replay_save:
            SetUiAction(11, 0);
            stateTimer = 0;
            CloseReplayUi(1);
            keyboardSelection = 0;
            strcpy(
                g_ReplayName,
                g_ResultNameTable[nameBankIndex][g_ResultNameTableIndex][nameSlotIndex].name);
            return 1;
        }

        if (g_ResultInputFlags & 0x1001)
        {
            if (keyboardSelection == 1)
                goto cancel_replay_save;

        load_replays:
            memset(replays, 0, 0x6018);
            replayCursor = replays;
            replayPath = replayPaths[0];
            for (i32 replayIndex = 0; replayIndex < 25; replayIndex++)
            {
                sprintf(source, "./replay/th9_%.2d.rpy", replayIndex + 1);
                void *fileData = FileSystemView::OpenFile(source, &fileSize, 1);
                if (fileData != 0)
                {
                    loadedReplay = ReplayManagerView::LoadReplayData((ReplayDataView *)fileData, fileSize);
                    if (loadedReplay != 0)
                    {
                        *replayCursor = *loadedReplay;
                        strcpy(replayPath, source);
                        free(loadedReplay);
                    }
                }
                replayCursor++;
                replayPath += 0x200;
            }
            replaySaveState = 2;
            phaseTimer = 0;
            keyboardSelection = 0;
            g_ResultUiManager->SetUi(uiVm, uiScript, 22);
            g_ResultUiManager->UpdateUi(uiVm, uiScript);
        }
        break;

    case 2:
        if (phaseTimer < 6)
            break;

        if (MoveReplayCursor(25))
            RefreshUi();

        if (g_ResultInputFlags & 0xA)
        {
            SetUiAction(11, 0);
            keyboardSelection = 0;
            phaseTimer = 0;
            replaySaveState = 1;
            g_ResultUiManager->SetUi(uiVm, uiScript, 19);
            g_ResultUiManager->UpdateUi(uiVm, uiScript);
            transitionId = 125;
            transitionMode = 2;
            RefreshUi();
            break;
        }

        if (g_ResultInputFlags & 0x1001)
        {
            SetUiAction(10, 0);
            selectedReplay = keyboardSelection;
            keyboardSelection = 0;
            if (replays[selectedReplay].magic != 0)
            {
                replaySaveState = 3;
                g_ResultUiManager->SetUi(uiVm, uiScript, 21);
                g_ResultUiManager->UpdateUi(uiVm, uiScript);
                transitionId = 129;
                transitionMode = 2;
                RefreshUi();
            }
            else
            {
                keyboardSelection = 95;
                replaySaveState = 4;
                g_ResultUiManager->SetUi(uiVm, uiScript, 23);
                g_ResultUiManager->UpdateUi(uiVm, uiScript);
            }
            phaseTimer = 0;
            strcpy(replayName, g_ReplayName);
        }
        break;

    case 3:
        if (phaseTimer < 10)
            break;

        if (MoveTwoChoiceCursor(2))
            RefreshUi();

        inputFlags = g_ResultInputFlags;
        if (!(inputFlags & 0xA))
        {
            if (!(inputFlags & 0x1001))
                break;
            if (keyboardSelection != 1)
            {
                SetUiAction(10, 0);
                keyboardSelection = 95;
                replaySaveState = 4;
                g_ResultUiManager->SetUi(uiVm, uiScript, 23);
                g_ResultUiManager->UpdateUi(uiVm, uiScript);
                break;
            }
        }

        SetUiAction(11, 0);
        replaySaveState = 2;
        keyboardSelection = 0;
        g_ResultUiManager->SetUi(uiVm, uiScript, 22);
        g_ResultUiManager->UpdateUi(uiVm, uiScript);
        break;

    case 4:
        if (phaseTimer < 10)
            break;

        if (g_ResultInput.IsPressedScrolling(0x10))
        {
            g_SoundPlayer.PlaySoundByIdx(12, 0);
            keyboardSelection -= 16;
            if (keyboardSelection < 0)
                keyboardSelection += 96;
            if (keyboardSelection == 93)
                keyboardSelection = 77;
            stateTimer = 0;
        }

        if (g_ResultInput.IsPressedScrolling(0x20))
        {
            g_SoundPlayer.PlaySoundByIdx(12, 0);
            keyboardSelection += 16;
            if (keyboardSelection >= 96)
                keyboardSelection -= 96;
            if (keyboardSelection == 93)
                keyboardSelection = 13;
            stateTimer = 0;
        }

        if (g_ResultInput.IsPressedScrolling(0x40))
        {
            g_SoundPlayer.PlaySoundByIdx(12, 0);
            if (keyboardSelection % 16 == 0)
                keyboardSelection += 15;
            else
            {
                keyboardSelection--;
                if (keyboardSelection == 93)
                    keyboardSelection = 92;
            }
            stateTimer = 0;
        }

        if (g_ResultInput.IsPressedScrolling(0x80))
        {
            g_SoundPlayer.PlaySoundByIdx(12, 0);
            if (keyboardSelection % 16 == 15)
                keyboardSelection -= 15;
            else
            {
                keyboardSelection++;
                if (keyboardSelection == 93)
                    keyboardSelection = 94;
            }
            stateTimer = 0;
        }

        if (g_ResultInputFlags & 0x1001)
        {
            if (keyboardSelection == 95)
            {
                SetUiAction(10, 0);
                sprintf(path, "replay/th9_%.2d.rpy", selectedReplay + 1);
                ReplayManagerView::SaveReplay(g_ReplayManager, path, replayName);
                stateTimer = 0;
                CloseReplayUi(1);
                keyboardSelection = 0;
                strcpy(g_ReplayName, replayName);
                return 1;
            }

            char value;
            if (keyboardSelection == 94)
                value = ' ';
            else
                value = g_ResultAlphabet[keyboardSelection];
            replayName[replayNameCursor] = value;
            SetUiAction(10, 0);
            if (replayNameCursor < 7)
                replayNameCursor++;
            else
                keyboardSelection = 95;
            stateTimer = 0;
        }

        if (g_ResultInputFlags & 0xA)
        {
            SetUiAction(11, 0);
            if (replayNameCursor == 7 && replayName[7] != ' ')
            {
                replayName[7] = ' ';
            }
            else if (replayNameCursor > 0)
            {
                replayNameCursor--;
                replayName[replayNameCursor] = ' ';
            }
            else
            {
                keyboardSelection = 0;
                replaySaveState = 2;
                g_ResultUiManager->SetUi(uiVm, uiScript, 22);
                g_ResultUiManager->UpdateUi(uiVm, uiScript);
                phaseTimer = 0;
            }
            stateTimer = 0;
        }
        break;
    }

    stateTimer++;
    replayUiFrameCounter++;
    phaseTimer++;
    return 1;
}
