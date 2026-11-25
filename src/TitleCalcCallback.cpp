// Original TH09 Title calc callback: 264 code bytes and a 16-entry table.
// Layout views preserve observed fields without claiming the original TU.
#include "AnmManager.hpp"
#include "AnmVmLifecycle.hpp"
#include "TitleScreenReplayView.hpp"
#include "TitleScreenStates.hpp"
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
    case TITLE_SCREEN_START_MENU:
        result = title->OnUpdateStartMenu();
        break;
    case TITLE_SCREEN_MODE_SELECT:
        result = title->OnUpdateModeSelect();
        break;
    case TITLE_SCREEN_DIFFICULTY_STORY:
    case TITLE_SCREEN_DIFFICULTY_EXTRA:
    case TITLE_SCREEN_DIFFICULTY_MATCH:
        result = title->OnUpdateDifficultySelect();
        break;
    case TITLE_SCREEN_CHARACTER_STORY:
    case TITLE_SCREEN_CHARACTER_EXTRA:
        result = title->OnUpdateCharacterSelect();
        break;
    case TITLE_SCREEN_MATCH_CHARACTER_SELECT:
        switch (g_TitleModeSelection)
        {
        case TITLE_MATCH_MODE_STORY_VS_CPU:
            result = title->UpdateScreen8Mode0();
            break;
        case TITLE_MATCH_MODE_P1_VS_P2:
            result = title->UpdateScreen8Mode123();
            break;
        case TITLE_MATCH_MODE_P1_VS_CPU:
            result = title->UpdateScreen8Mode123();
            break;
        case TITLE_MATCH_MODE_CPU_VS_CPU:
            result = title->UpdateScreen8Mode123();
            break;
        case TITLE_MATCH_MODE_EXTRA:
            result = title->UpdateScreen8Mode0();
            break;
        }
        break;
    case TITLE_SCREEN_NETWORK_LOBBY:
        result = title->UpdateScreen16();
        break;
    case TITLE_SCREEN_OPTIONS:
        result = title->OnUpdateOptions();
        break;
    case TITLE_SCREEN_KEY_CONFIG:
        result = title->OnUpdateKeyConfig();
        break;
    case TITLE_SCREEN_REPLAY_MENU:
        result = title->OnUpdateReplayMenu();
        break;
    case TITLE_SCREEN_REPLAY_SAVE:
        result = title->UpdateReplaySave();
        break;
    case TITLE_SCREEN_MUSIC_ROOM:
        result = title->OnUpdateMusicRoom();
        break;
    case TITLE_SCREEN_RESULT_NAME_ENTRY:
        result = title->OnUpdateResultNameEntry();
        break;
    case TITLE_SCREEN_RESULT_VIEW:
        result = title->OnUpdateResult();
        break;
    }

    g_AnmManager->ExecuteScriptArray(title->vms, title->vmCount);
    AnmVm *help = title->currentHelpTextVm;
    if (help != 0)
        g_AnmManager->ExecuteScript(help);
    return result;
}
