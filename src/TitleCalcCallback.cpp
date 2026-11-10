// Original TH09 Title calc callback: 264 code bytes and a 16-entry table.
// Layout views preserve observed fields without claiming the original TU.
#include "AnmManager.hpp"
#include "AnmVmLifecycle.hpp"
#include "TitleScreenReplayView.hpp"
#include <stddef.h>

// The original 0x1B388-byte Title owner is now shared by its calc/draw
// callbacks and all TitleScreen state-machine methods.
typedef char TitleCalcFullOwnerFields[
    (sizeof(TitleScreenView) == 0x1B388 &&
     offsetof(TitleScreenView, vms) == 0x11B98 &&
     offsetof(TitleScreenView, currentHelpTextVm) == 0x11B9C &&
     offsetof(TitleScreenView, vmCount) == 0x1B224 &&
     offsetof(TitleScreenView, currentScreen) == 0x1B228 &&
     offsetof(TitleScreenView, chainState) == 0x1B238) ? 1 : -1];

extern int g_TitleModeSelection;

int __fastcall TitleCalcCallback(void *object)
{
    TitleScreenView *title = static_cast<TitleScreenView *>(object);
    int result = 1;
    if (title->chainState != 0)
        return title->chainState == 2 ? 4 : 1;

    switch (title->currentScreen)
    {
    case 1:
        result = title->OnUpdateStartMenu();
        break;
    case 6:
        result = title->OnUpdateModeSelect();
        break;
    case 2:
    case 4:
    case 7:
        result = title->OnUpdateDifficultySelect();
        break;
    case 3:
    case 5:
        result = title->OnUpdateCharacterSelect();
        break;
    case 8:
        switch (g_TitleModeSelection)
        {
        case 0:
            result = title->UpdateScreen8Mode0();
            break;
        case 1:
            result = title->UpdateScreen8Mode123();
            break;
        case 2:
            result = title->UpdateScreen8Mode123();
            break;
        case 3:
            result = title->UpdateScreen8Mode123();
            break;
        case 4:
            result = title->UpdateScreen8Mode0();
            break;
        }
        break;
    case 16:
        result = title->UpdateScreen16();
        break;
    case 9:
        result = title->OnUpdateOptions();
        break;
    case 10:
        result = title->OnUpdateKeyConfig();
        break;
    case 11:
        result = title->OnUpdateReplayMenu();
        break;
    case 15:
        result = title->UpdateReplaySave();
        break;
    case 12:
        result = title->OnUpdateMusicRoom();
        break;
    case 14:
        result = title->OnUpdateResultNameEntry();
        break;
    case 13:
        result = title->OnUpdateResult();
        break;
    }

    g_AnmManager->ExecuteScriptArray(title->vms, title->vmCount);
    AnmVm *help = title->currentHelpTextVm;
    if (help != 0)
        g_AnmManager->ExecuteScript(help);
    return result;
}
