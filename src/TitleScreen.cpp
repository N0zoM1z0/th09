// SPDX-License-Identifier: GPL-3.0-only
// Cursor functions adapted from Cirno-Labs/th09 src/mainmenu.cpp,
// commit d7653152becabb8c86f2f43d49015ecc11bd0652.
// Original MIT portions retain their grant in LICENSE; see THIRD_PARTY_NOTICES.md.
// Target-bound TH09 TitleScreen reconstruction views. The physical TitleScreen
// owner is target-proved; original identifier spelling, TU partition, and data-definition ownership remain unresolved.

#include "ReplayInputState.hpp"
#include "TitleInputView.hpp"
#include "TitleNativeNameKeyboard.hpp"
#include "PlayStats.hpp"

// These native fields are the original result browser's input/unlock storage.
typedef char TitleResultNativeFieldLayout[
    (sizeof(ReplayInputState) == 0x8E &&
     offsetof(ReplayInputState, word06) == 0x06 &&
     offsetof(PlayStatsRecordView, characterUnlocked) == 0x4C &&
     sizeof(((PlayStatsRecordView *)0)->characterUnlocked[0]) == 16)
        ? 1 : -1];

#include "AsciiManager.hpp"
#include "AnmManager.hpp"
#include "SoundPlayer.hpp"
#include "Supervisor.hpp"
#include "SupervisorThreadStartView.hpp"
#include "SupervisorSetupLayoutView.hpp"
#include "Chain.hpp"
#include "FileSystem.hpp"
#include "ScreenEffect.hpp"
#include "SupervisorNetworkState.hpp"

#include <stddef.h>
#include <windows.h>
#include <d3d8.h>

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef int i32;

extern "C" int __cdecl sprintf(char *, const char *, ...);
extern "C" char *__cdecl strcpy(char *, const char *);
extern "C" char *__cdecl strcat(char *, const char *);
extern "C" void *__cdecl memset(void *, int, size_t);
extern "C" void __cdecl free(void *);
extern "C" int __cdecl _mkdir(const char *);
extern "C" int __cdecl _chdir(const char *);

#include "ReplayDataOriginalView.hpp"

struct TitleNameRecordView {
    char name[9];
    u8 unknown09[35];
};

typedef char TitleNameRecordSizeIs2C[(sizeof(TitleNameRecordView) == 0x2C) ? 1 : -1];

// The cursor movement functions operate on the same native 0x8E-byte
// input records as the independently exact InputView scrolling leaf.
// Keep the original table symbol distinct from the last-slot global alias.
typedef InputView TitleInputStatePhysicalView;
typedef char TitleInputStatePhysicalViewSize[
    (sizeof(TitleInputStatePhysicalView) == sizeof(ReplayInputState) &&
     sizeof(TitleInputStatePhysicalView) == 0x8E) ? 1 : -1];

struct TitleInputStateTableView {
    TitleInputStatePhysicalView states[3];
};
typedef char TitleInputStateNativeTableSize[
    (sizeof(TitleInputStateTableView) == sizeof(ReplayInputState) * 3 &&
     offsetof(TitleInputStateTableView, states) == 0) ? 1 : -1];

struct TitleCharacterConfigView {
    u8 unknown000[0x7C];
    i32 optionValues[16][6];

    int GetOptionState(char character, i32 option);
};







#include "ReplayManagerRuntimeView.hpp"

struct TitleFloat3View {
    TitleFloat3View() {}
    TitleFloat3View(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
    float x;
    float y;
    float z;
};

typedef char TitleFloat3ViewSizeIs0C[(sizeof(TitleFloat3View) == 0x0C) ? 1 : -1];
// The exact Supervisor loading-VM method accepts the native Float3 window.
typedef char TitleNativeLoadingPositionStorage[
    (sizeof(TitleFloat3View) == sizeof(Float3) &&
     sizeof(TitleFloat3View) == sizeof(SetupLoadingPosition) &&
     offsetof(TitleFloat3View, x) == offsetof(SetupLoadingPosition, x) &&
     offsetof(TitleFloat3View, y) == offsetof(SetupLoadingPosition, y) &&
     offsetof(TitleFloat3View, z) == offsetof(SetupLoadingPosition, z) &&
     offsetof(TitleFloat3View, x) == offsetof(Float3, x) &&
     offsetof(TitleFloat3View, y) == offsetof(Float3, y) &&
     offsetof(TitleFloat3View, z) == offsetof(Float3, z)) ? 1 : -1];

// Preserve the separately original-bound Title interpolation methods, but
// share the native 0x2A4-byte AnmVm allocation with ANM and other game owners.
#include "TitleScreenReplayView.hpp"


typedef char AnmVmSizeIs2A4[(sizeof(AnmVmView) == 0x2A4) ? 1 : -1];
typedef char AnmVmColorAt1F0[(offsetof(AnmVmView, color1) == 0x1F0) ? 1 : -1];
typedef char AnmVmFlagsAt1F8[(offsetof(AnmVmView, flags) == 0x1F8) ? 1 : -1];
typedef char AnmVmInterruptAt1FE[(offsetof(AnmVmView, pendingInterrupt) == 0x1FE) ? 1 : -1];
// Result-page arrays share the implemented native ANM storage contract.
typedef char TitleResultNativeVmArrayStorage[
    (sizeof(AnmVmView) == sizeof(AnmVm) &&
     offsetof(AnmVmView, pendingInterrupt) == offsetof(AnmVm, pendingInterrupt))
        ? 1 : -1];
typedef char AnmVmPositionAt208[(offsetof(AnmVmView, pos) == 0x208) ? 1 : -1];
typedef char AnmVmActiveSpriteAt214[(offsetof(AnmVmView, activeSpriteIndex) == 0x214) ? 1 : -1];
typedef char AnmVmSpriteAt218[(offsetof(AnmVmView, baseSpriteIndex) == 0x218) ? 1 : -1];
typedef char AnmVmFontWidthAt298[(offsetof(AnmVmView, fontWidth) == 0x298) ? 1 : -1];
typedef char AnmVmInterpCurrentAt050[(offsetof(AnmVmView, interpCurrentTimers) == 0x050) ? 1 : -1];
typedef char AnmVmInterpEndAt0A4[(offsetof(AnmVmView, interpEndTimers) == 0x0A4) ? 1 : -1];
typedef char AnmVmInterpModeAt0F8[(offsetof(AnmVmView, interpModes) == 0x0F8) ? 1 : -1];
typedef char AnmVmPosInitialAt238[(offsetof(AnmVmView, posInitial) == 0x238) ? 1 : -1];
typedef char AnmVmPosFinalAt244[(offsetof(AnmVmView, posFinal) == 0x244) ? 1 : -1];
typedef char AnmVmColorInitialAt278[(offsetof(AnmVmView, color1Initial) == 0x278) ? 1 : -1];
typedef char AnmVmColorFinalAt27C[(offsetof(AnmVmView, color1Final) == 0x27C) ? 1 : -1];
typedef char AnmVmFontHeightAt299[(offsetof(AnmVmView, fontHeight) == 0x299) ? 1 : -1];

// Title sprite/script resources point at the same native 0x1C-byte AnmLoaded
// record used by the independently exact ANM sprite and script providers.
// The derived view adds no storage and retains the preload result pointer role.

typedef char TitleNativeAnmLoadedView[
    (sizeof(TitleAnmView) == sizeof(AnmLoaded) &&
     sizeof(TitleAnmView) == 0x1C) ? 1 : -1];

// Every actual animation manager method used by the Title callback is now
// independently exact. Reuse the original manager class and its +0x08
// capture state instead of a 12-byte private prefix and eight fake methods.
struct TitleAnmManagerView : AnmManager {};
typedef char TitleNativeAnmManagerStorage[
    (sizeof(TitleAnmManagerView) == sizeof(AnmManager) &&
     offsetof(TitleAnmManagerView, captureSurfaceIndex08) == 0x08) ? 1 : -1];

struct TitleSideInputView {
    u32 currentInput;
    u16 pressedInput;
    u16 repeatInput;

    u16 IsPressedScrolling(u16 mask);
};

struct TitleSelectionRandomView {
    i32 NextU32();
};

// Title and independent Supervisor loading callbacks share this same
// original-bound source-facing method class and native Supervisor storage.
#include "TitleSupervisorRuntimeView.hpp"

// Source-facing alias only for the independently unclassified one-byte
// folded no-op at 0x0043D2B0. Its original owner is unknown; do not add
// a fabricated InitializeVms method to the genuine AsciiManager type.
struct TitleAsciiManagerView {
    void InitializeVms();
};

struct TitleMidiOutputView {
    void PlayFile(i32 track);
};

#include "TitleScoreRecordView.hpp"



typedef char TitleConfigSnapshotSizeIsCC[(sizeof(TitleConfigSnapshotView) == 0xCC) ? 1 : -1];
typedef char TitleConfigSnapshotMatchesSupervisorConfig[
    (sizeof(TitleConfigSnapshotView) == sizeof(GameConfiguration) &&
     offsetof(Supervisor, config388) == 0x388) ? 1 : -1];

extern void __cdecl DrawTitleHelpText(TitleAnmManagerView *manager, AnmVmView *vm,
                                      u32 color, i32 width, const char *text);



typedef char TitleSide0CursorAt10[(offsetof(TitleScreenView, side0CharacterCursor) == 0x10) ? 1 : -1];
typedef char TitleSide1CursorAt14[(offsetof(TitleScreenView, side1CharacterCursor) == 0x14) ? 1 : -1];
typedef char TitleSide0ConfirmedAt18[(offsetof(TitleScreenView, side0CharacterConfirmed) == 0x18) ? 1 : -1];
typedef char TitleSide1ConfirmedAt1C[(offsetof(TitleScreenView, side1CharacterConfirmed) == 0x1C) ? 1 : -1];
typedef char TitleStateAt28[(offsetof(TitleScreenView, replaySaveState) == 0x28) ? 1 : -1];
typedef char TitleReplayNameAt8C[(offsetof(TitleScreenView, replayName) == 0x8C) ? 1 : -1];
typedef char TitleReplayPathsAt28C[(offsetof(TitleScreenView, replayPaths) == 0x28C) ? 1 : -1];
typedef char TitleReplayAllocationsAt681C[(offsetof(TitleScreenView, replayAllocations) == 0x681C) ? 1 : -1];
typedef char TitleCurrentReplayAtC8FC[(offsetof(TitleScreenView, currentReplay) == 0xC8FC) ? 1 : -1];
typedef char TitleReplayEnumResetAtC900[(offsetof(TitleScreenView, replayEnumerationResetState) == 0xC900) ? 1 : -1];
typedef char TitleReplaySlotCountAtC904[(offsetof(TitleScreenView, replaySlotCount) == 0xC904) ? 1 : -1];
typedef char TitleReplaysAt68E4[(offsetof(TitleScreenView, replays) == 0x68E4) ? 1 : -1];
typedef char TitleSelectedReplayAtC908[(offsetof(TitleScreenView, selectedReplay) == 0xC908) ? 1 : -1];
typedef char TitleUnknownC918AtC918[(offsetof(TitleScreenView, unknown0C918) == 0xC918) ? 1 : -1];
typedef char TitleUiVmAt11B98[(offsetof(TitleScreenView, uiVm) == 0x11B98) ? 1 : -1];
typedef char TitleUiScriptAt1B224[(offsetof(TitleScreenView, uiScript) == 0x1B224) ? 1 : -1];
typedef char TitlePhaseAt1B230[(offsetof(TitleScreenView, phaseTimer) == 0x1B230) ? 1 : -1];
typedef char TitlePrevCursorAt24[(offsetof(TitleScreenView, previousCursor) == 0x24) ? 1 : -1];
typedef char TitlePrevScreenAt84[(offsetof(TitleScreenView, previousScreen) == 0x84) ? 1 : -1];
typedef char TitlePracticeStateAt88[(offsetof(TitleScreenView, practiceState) == 0x88) ? 1 : -1];
typedef char TitleSide0SettingAt11B84[(offsetof(TitleScreenView, side0CharacterSetting) == 0x11B84) ? 1 : -1];
typedef char TitleSide1SettingAt11B88[(offsetof(TitleScreenView, side1CharacterSetting) == 0x11B88) ? 1 : -1];
typedef char TitleAnmAt11B90[(offsetof(TitleScreenView, titleAnm) == 0x11B90) ? 1 : -1];
typedef char TitleResultAnmAt11B94[(offsetof(TitleScreenView, resultTextAnm) == 0x11B94) ? 1 : -1];
typedef char TitleVmsAt11B98[(offsetof(TitleScreenView, vms) == 0x11B98) ? 1 : -1];
typedef char TitleHelpAt11BA0[(offsetof(TitleScreenView, embeddedVms) == 0x11BA0) ? 1 : -1];
typedef char TitleVmCountAt1B224[(offsetof(TitleScreenView, vmCount) == 0x1B224) ? 1 : -1];
typedef char TitleCurrentScreenAt1B228[(offsetof(TitleScreenView, currentScreen) == 0x1B228) ? 1 : -1];
typedef char TitleIdleAt1B234[(offsetof(TitleScreenView, idleFrames) == 0x1B234) ? 1 : -1];
typedef char TitleMenuStartAt1B240[(offsetof(TitleScreenView, menuVmStart) == 0x1B240) ? 1 : -1];
typedef char TitleMenuCountAt1B244[(offsetof(TitleScreenView, menuItemCount) == 0x1B244) ? 1 : -1];
typedef char TitleCalcChainAt1B248[(offsetof(TitleScreenView, calcChain) == 0x1B248) ? 1 : -1];
typedef char TitleDrawChainAt1B24C[(offsetof(TitleScreenView, drawChain) == 0x1B24C) ? 1 : -1];
typedef char TitleConfigAt1B2BC[(offsetof(TitleScreenView, configSnapshot) == 0x1B2BC) ? 1 : -1];
typedef char TitleScreenSizeIs1B388[(sizeof(TitleScreenView) == 0x1B388) ? 1 : -1];

extern TitleInputStateTableView g_TitleInputStateTable;
extern u32 g_TitleInputFlags;
// Read-width view at replay side0 +6, overlapping its known +6/+8 words.
// This is not an independent data-owner or original-layout claim.
extern u32 g_TitleSide0InputFlags;

extern ReplayManagerView *g_ReplayManager;
extern char *g_TitleAlphabet;
extern char g_ReplayName[];
extern TitleNameRecordView g_TitleNameTable[][5][5];
extern i32 g_TitleNameTableIndex;
extern i32 g_GameMode;
extern u32 g_TitleGameFlags;
extern TitleSupervisorView g_TitleSupervisor;
extern AsciiManager g_AsciiManager;
extern Chain g_Chain;
extern TitleMidiOutputView *g_TitleMidiOutput;
extern u8 g_TitleLockedMenuItem;
extern i32 g_TitleModeSelection;
extern u8 g_TitleDifficulty;
extern const char *g_DemoReplayPaths[3];
extern u8 g_DemoReplayIndex;
extern char g_SelectedReplayPath[];
extern i32 g_GameCurrentStage;
extern i32 g_GameConfigSelector;
extern i32 g_GameValueDB8;
extern i32 g_GameValueDF0;
extern i32 g_TitleGlobalMode;
extern i32 g_TitleLaunchState;
extern i32 g_TitleTransitionCounter0;
extern i32 g_TitleTransitionCounter1;
extern u8 g_OptionA;
extern u8 g_OptionB;
extern u8 g_OptionC;
extern u8 g_OptionD;
extern const char *g_TitleHelpText[8];
extern i32 g_GameSide0Value20;
extern i32 g_GameSide1Value20;
extern i32 g_GameSide0Value30;
extern i32 g_GameSide1Value30;
extern i32 g_GameSide0CharacterSetting;
extern i32 g_GameSide1CharacterSetting;
extern i32 g_TitleCharacterReturnSelection;
extern char g_TitleCharacterOrder[16];
extern char g_TitleCharacterOrder14[16];
extern char g_TitleResultCharacterOrder[16];
// Title result cursor ordering and PLST unlock flags share precisely one
// sixteen-character native extent. A separate guessed cursor capacity may
// silently let the unlock scan wrap outside the original stored row.
typedef char TitleResultNativeFullCharacterCapacity[
    (sizeof(g_TitleResultCharacterOrder) ==
     sizeof(((PlayStatsRecordView *)0)->characterUnlocked[0])) ? 1 : -1];
extern char g_TitleCharacterOrderMode123[16];
extern char g_TitleScreen16Order[16];
extern char g_TitleScreen16Entries[16];
extern i32 g_TitleScreen16SelectionResult;
extern TitleCharacterConfigView g_TitleCharacterConfig;
extern u8 g_TitleCharacterUnlocked[16];
extern u8 g_TitleCharacterUnlockedNormal[16];
extern u8 g_TitleCharacterUnlockedMode4[16];
extern i32 g_TitleResultUnlockStep;
extern ScreenEffect *g_TitleFadeReleaseEffect;
extern TitleSideInputView g_TitleSide0Input;
extern TitleSideInputView g_TitleSide1Input;
extern TitleSelectionRandomView g_TitleSelectionRandom;
extern void __fastcall PrepareTitleMode4Network(void *optionState);
extern void __fastcall ResetTitleMode4Supervisor(TitleSupervisorView *supervisor);
extern int SaveTitleScoreData();


extern TitleScreenView *g_TitleScreen;

void __cdecl TitleScreenView::TitleSetupThread(void *unused)
{
    (void)unused;

    while (g_AnmManager->captureSurfaceIndex08 >= 0)
        Sleep(1);

    TitleAnmView *loadedTitleAnm = static_cast<TitleAnmView *>(g_AnmManager->PreloadAnm(15, "title01.anm"));
    TitleScreenView *title = g_TitleScreen;
    title->titleAnm = loadedTitleAnm;
    if (loadedTitleAnm == 0)
    {
        title->chainState = 2;
        return;
    }

    TitleAnmView *loadedResultAnm = static_cast<TitleAnmView *>(g_AnmManager->PreloadAnm(17, "resulttext.anm"));
    title = g_TitleScreen;
    title->resultTextAnm = loadedResultAnm;
    if (loadedResultAnm == 0)
    {
        title->chainState = 2;
        return;
    }

    if (g_TitleSupervisor.subthreadCloseRequestActive)
        return;

    for (i32 i = 0; i < 14; ++i)
    {
        reinterpret_cast<AnmLoaded *>(g_TitleSupervisor.textAnm5CC)->SetAndExecuteScriptIdx(
            reinterpret_cast<AnmVm *>(&title->embeddedVms[i]), 5);
        reinterpret_cast<AnmLoaded *>(g_TitleSupervisor.textAnm5CC)->SetSprite(
            reinterpret_cast<AnmVm *>(&g_TitleScreen->embeddedVms[i]),
            g_TitleScreen->embeddedVms[i].activeSpriteIndex + i);
        reinterpret_cast<AnmLoaded *>(g_TitleSupervisor.textAnm5CC)->SetSprite(
            reinterpret_cast<AnmVm *>(&g_TitleScreen->embeddedVms[i + 35]), i + 21);

        title = g_TitleScreen;
        float y = (float)i * 16.0f;
        if (i <= 4)
            *reinterpret_cast<TitleFloat3View *>(&title->embeddedVms[i + 35].pos) =
                TitleFloat3View(64.0f, y + 352.0f, 0.0f);
        else
            *reinterpret_cast<TitleFloat3View *>(&title->embeddedVms[i + 35].pos) =
                TitleFloat3View(64.0f, y + 362.0f, 0.0f);
        title->embeddedVms[i + 35].flagsLow |= 0x1802;
        title->embeddedVms[i + 35].fontWidth = 15;
        title->embeddedVms[i + 35].fontHeight = 15;
    }

    if (g_TitleSupervisor.subthreadCloseRequestActive)
        return;

    if (title->currentScreen == 1)
    {
        if (g_AnmManager->PreloadSurface(0, "title/title00.png"))
        {
            g_TitleScreen->chainState = 2;
            return;
        }
    }
    else
    {
        if (g_AnmManager->PreloadSurface(0, "title/result00.png"))
        {
            g_TitleScreen->chainState = 2;
            return;
        }
    }

    if ((g_TitleGameFlags & 2) == 0)
    {
        if (g_TitleSupervisor.value594 != 5)
            g_TitleSupervisor.LoadMusic(0);
        if (g_TitleSupervisor.totalPlayTime == 0)
            ScreenEffect::RegisterChain(SCREEN_EFFECT_FULL_FADE_IN, 70, 0xFFFFFF, 0, 0, 35, 2);
        else
            ScreenEffect::RegisterChain(SCREEN_EFFECT_FULL_FADE_IN, 70, 0xFFFFFF, 0, 0, 35, 2);
    }

    title = g_TitleScreen;
    title->currentHelpTextVm = &title->embeddedVms[0];
    title->chainState = 0;
    if (g_TitleFadeReleaseEffect != 0)
        g_TitleFadeReleaseEffect->BeginFadeRelease();
    g_TitleFadeReleaseEffect = 0;
    g_TitleSupervisor.HideLoadingVms();
    g_TitleSupervisor.runningSubthreadHandle = 0;
    g_TitleSupervisor.subthreadCloseRequestActive = 0;
    g_TitleSupervisor.subthreadActive = 0;
}

int TitleScreenView::ActualAddedCallback()
{
    g_ScreenEffectCounter = 0;
    g_AsciiManager.Reset();
    reinterpret_cast<TitleAsciiManagerView *>(&g_AsciiManager)->InitializeVms();
    g_TitleSupervisor.framerateMultiplier5B8 = 1.0f;

    if (g_TitleGameFlags & 8)
    {
        g_GameSide0Value20 = 0;
        g_GameSide1Value20 = 1;
    }
    if (g_TitleGameFlags & 2)
        g_TitleGameFlags &= ~8u;

    ChangeCurrentScreen(1);
    g_TitleSupervisor.ClearRecordingFpsWarningState();

    if (g_SupervisorNetworkState->active != 0 && g_GameMode != 2)
    {
        resumeState = 2;
        g_TitleModeSelection = 0;
        g_TitleDifficulty = 1;
    }

    switch (registrationContext)
    {
    case 0:
        if ((g_TitleGameFlags & 0xA) == 0)
        {
            if (g_GameMode == 0)
                keyboardSelection = 0;
            else if (g_GameMode == 1)
                keyboardSelection = 1;
            else if (g_GameMode == 2)
                resumeState = 1;
        }
        break;
    case 1:
        currentScreen = 14;
        break;
    default:
        keyboardSelection = 0;
        break;
    }

    practiceState = 0;
    g_TitleGameFlags &= ~1u;

    TitleFloat3View loadingPosition(500.0f, 440.0f, 0.0f);
    if (g_TitleSupervisor.value594 == 2)
    {
        g_TitleSupervisor.SetupLoadingVmsAndInitCapture(reinterpret_cast<SetupLoadingPosition *>(&loadingPosition));
        g_TitleSupervisor.StartEffect(0);
    }
    else if (g_TitleSupervisor.value594 != 0)
    {
        g_TitleSupervisor.SetupLoadingVms(reinterpret_cast<Float3 *>(&loadingPosition));
    }

    g_TitleGameFlags &= ~2u;
    chainState = 1;
    g_TitleSupervisor.ThreadStart(
        reinterpret_cast<LPTHREAD_START_ROUTINE>(TitleScreenView::TitleSetupThread), 0);
    return 0;
}

int TitleScreenView::AddedCallback(TitleScreenView *titleScreen)
{
    return titleScreen->ActualAddedCallback();
}

int TitleScreenView::Release()
{
    if (currentReplay != 0)
    {
        free(currentReplay);
        currentReplay = 0;
    }

    if (vms != 0)
    {
        delete[] vms;
        vms = 0;
    }

    for (i32 i = 0; i < 50; i++)
    {
        if (replayAllocations[i] != 0)
            free(replayAllocations[i]);
        replayAllocations[i] = 0;
    }
    return 0;
}

int TitleScreenView::DeletedCallback(TitleScreenView *titleScreen)
{
    g_TitleSupervisor.d3dDevice08->ResourceManagerDiscardBytes(0);
    g_AnmManager->ReleaseAnm(15);
    g_AnmManager->ReleaseAnm(17);
    g_AnmManager->ReleaseSurface(0);
    g_Chain.Cut(titleScreen->drawChain);
    titleScreen->drawChain = 0;
    titleScreen->Release();
    delete titleScreen;
    return 0;
}



// Retail menu cursor movement uses i386 DWORD increments, decrements and
// wrap adjustments; retain signed comparisons on each 32-bit result.
int TitleScreenView::MoveCursorVertical(i32 count)
{
    if (count == 0)
        return 0;

    if (g_TitleInput.IsPressedScrolling(0x10))
    {
        keyboardSelection = static_cast<i32>(static_cast<u32>(keyboardSelection) - 1u);
        g_SoundPlayer.PlaySoundByIdx(12, 0);
        if (keyboardSelection < 0)
            keyboardSelection = static_cast<i32>(static_cast<u32>(count) - 1u);
        if (keyboardSelection >= count)
            keyboardSelection = 0;
        return -1;
    }
    else if (g_TitleInput.IsPressedScrolling(0x20))
    {
        keyboardSelection = static_cast<i32>(static_cast<u32>(keyboardSelection) + 1u);
        g_SoundPlayer.PlaySoundByIdx(12, 0);
        if (keyboardSelection < 0)
            keyboardSelection = static_cast<i32>(static_cast<u32>(count) - 1u);
        if (keyboardSelection >= count)
            keyboardSelection = 0;
        return 1;
    }

    return 0;
}


int TitleScreenView::MoveCursorHorizontal(i32 count)
{
    if (count == 0)
        return 0;

    i32 direction;
    if (g_TitleInput.IsPressedScrolling(0x40))
    {
        if ((keyboardSelection = static_cast<i32>(static_cast<u32>(keyboardSelection) - 1u)) < 0)
            keyboardSelection = static_cast<i32>(static_cast<u32>(keyboardSelection) + static_cast<u32>(count));
        direction = -1;
    }
    else if (g_TitleInput.IsPressedScrolling(0x80))
    {
        if ((keyboardSelection = static_cast<i32>(static_cast<u32>(keyboardSelection) + 1u)) >= count)
            keyboardSelection = static_cast<i32>(static_cast<u32>(keyboardSelection) - static_cast<u32>(count));
        direction = 1;
    }
    else
    {
        return 0;
    }

    g_SoundPlayer.PlaySoundByIdx(12, 0);
    return direction;
}


void TitleScreenView::SetMenuSelectionSprites(i32 selected, i32 start, i32 count)
{
    for (i32 index = start; index < start + count; index++)
    {
        titleAnm->SetSprite(&vms[index], vms[index].baseSpriteIndex + 1);
        vms[index].pendingInterrupt = 8;
    }

    i32 selectedIndex = selected + start;
    titleAnm->SetSprite(&vms[selectedIndex], vms[selectedIndex].baseSpriteIndex);
    vms[selectedIndex].pendingInterrupt = 7;
}


// The two original Title interpolation methods assign the native timer's
// three physical words.  Keep their original local inlining, without a
// second timer layout or an out-of-line ZunTimer callback.
static __forceinline void AssignInterpolatedZunTimer(ZunTimer &timer, int current)
{
    timer.current = current;
    timer.subFrame = (float)current;
    timer.previous = -999999;
}

char AnmVmView::ConfigurePositionInterpolation(
    i32 duration, char mode, TitleFloat3View *start, TitleFloat3View *end)
{
    AssignInterpolatedZunTimer(interpCurrentTimers[0], 0);
    AssignInterpolatedZunTimer(interpEndTimers[0], duration);

    interpModes[0] = mode;
    *reinterpret_cast<TitleFloat3View *>(&posInitial) = *start;
    *reinterpret_cast<TitleFloat3View *>(&posFinal) = *end;
    return mode;
}


int AnmVmView::ConfigureColorInterpolation(
    i32 duration, char mode, u32 startColor, u32 endColor)
{
    AssignInterpolatedZunTimer(interpCurrentTimers[1], 0);
    AssignInterpolatedZunTimer(interpEndTimers[1], duration);

    interpModes[1] = mode;
    color1Initial.r = (startColor >> 16) & 0xFF;
    color1Initial.g = (startColor >> 8) & 0xFF;
    color1Initial.b = startColor & 0xFF;
    color1Final.r = (endColor >> 16) & 0xFF;
    color1Final.g = (endColor >> 8) & 0xFF;
    color1Final.b = endColor & 0xFF;
    return endColor;
}



int TitleScreenView::SetTwoDigitVmValue(AnmVmView *digitVms, short value)
{
    if (value < 0)
    {
        digitVms[0].flags &= ~2u;
        digitVms[1].flags &= ~2u;
    }
    else
    {
        titleAnm->SetSprite(
            &digitVms[0],
            digitVms[0].baseSpriteIndex + 2 * (value / 10));
        titleAnm->SetSprite(
            &digitVms[1],
            digitVms[1].baseSpriteIndex + 2 * (value % 10));
        digitVms[0].flags |= 2;
        digitVms[1].flags |= 2;
    }
    return 0;
}


void TitleScreenView::SetIndexedSpriteChoice(i32 choice, i32 vmIndex)
{
    titleAnm->SetSprite(
        &vms[vmIndex],
        vms[vmIndex].baseSpriteIndex + 2 * choice);
    vms[vmIndex].pendingInterrupt = 7;
}


void TitleScreenView::SetRangeSelectionInterrupts(i32 selected, i32 start, i32 count)
{
    for (i32 index = start; index < start + count; index++)
        vms[index].pendingInterrupt = 8;

    vms[start + selected].pendingInterrupt = 7;
}


void TitleScreenView::UpdateMenuSelection()
{
    SetMenuSelectionSprites(keyboardSelection, menuVmStart, menuItemCount);
}


int TitleScreenView::MoveCursorFourWay(i32 count)
{
    if (count == 0)
        return 0;

    if (g_TitleInput.IsPressedScrolling(0x40))
    {
        if ((keyboardSelection = static_cast<i32>(static_cast<u32>(keyboardSelection) - 1u)) < 0)
            keyboardSelection = static_cast<i32>(static_cast<u32>(keyboardSelection) + static_cast<u32>(count));
        g_SoundPlayer.PlaySoundByIdx(12, 0);
        return -1;
    }

    if (g_TitleInput.IsPressedScrolling(0x80))
    {
        if ((keyboardSelection = static_cast<i32>(static_cast<u32>(keyboardSelection) + 1u)) >= count)
            keyboardSelection = static_cast<i32>(static_cast<u32>(keyboardSelection) - static_cast<u32>(count));
        g_SoundPlayer.PlaySoundByIdx(12, 0);
        return 1;
    }

    if (g_TitleInput.IsPressedScrolling(0x10))
    {
        keyboardSelection = static_cast<i32>(static_cast<u32>(keyboardSelection) - 1u);
        g_SoundPlayer.PlaySoundByIdx(12, 0);
        if (keyboardSelection < 0)
            keyboardSelection = static_cast<i32>(static_cast<u32>(count) - 1u);
        if (keyboardSelection >= count)
            keyboardSelection = 0;
        return -1;
    }

    if (g_TitleInput.IsPressedScrolling(0x20))
    {
        keyboardSelection = static_cast<i32>(static_cast<u32>(keyboardSelection) + 1u);
        g_SoundPlayer.PlaySoundByIdx(12, 0);
        if (keyboardSelection < 0)
            keyboardSelection = static_cast<i32>(static_cast<u32>(count) - 1u);
        if (keyboardSelection >= count)
            keyboardSelection = 0;
        return 1;
    }

    return 0;
}


// The three character-unlock cursor loops use original i386 DWORD
// arithmetic for direction advances and bound wrapping. Preserve signed
// comparisons on the resulting bit pattern without signed C++ overflow.
void *TitleScreenView::MoveCharacterCursor(i32 side, i32 direction, char *order, i32 count)
{
    void *result = g_SupervisorNetworkState;
    if (g_SupervisorNetworkState->active == 0)
    {
        i32 *cursor = &side0CharacterCursor + side;
        result = cursor;
        while (!g_TitleCharacterUnlocked[order[*cursor]])
        {
            *cursor = static_cast<i32>(static_cast<u32>(*cursor) + static_cast<u32>(direction));
            if (*cursor < 0)
                *cursor = static_cast<i32>(static_cast<u32>(*cursor) + static_cast<u32>(count));
            if (*cursor >= count)
                *cursor = static_cast<i32>(static_cast<u32>(*cursor) - static_cast<u32>(count));
        }
    }
    return result;
}


void *TitleScreenView::MoveCharacterCursorNormal(i32 side, i32 direction, char *order, i32 count)
{
    void *result = g_SupervisorNetworkState;
    if (g_SupervisorNetworkState->active == 0)
    {
        i32 *cursor = &side0CharacterCursor + side;
        result = cursor;
        while (!g_TitleCharacterUnlockedNormal[order[*cursor]])
        {
            *cursor = static_cast<i32>(static_cast<u32>(*cursor) + static_cast<u32>(direction));
            if (*cursor < 0)
                *cursor = static_cast<i32>(static_cast<u32>(*cursor) + static_cast<u32>(count));
            if (*cursor >= count)
                *cursor = static_cast<i32>(static_cast<u32>(*cursor) - static_cast<u32>(count));
        }
    }
    return result;
}


void *TitleScreenView::MoveCharacterCursorMode4(i32 side, i32 direction, char *order, i32 count)
{
    void *result = g_SupervisorNetworkState;
    if (g_SupervisorNetworkState->active == 0)
    {
        i32 *cursor = &side0CharacterCursor + side;
        result = cursor;
        while (!g_TitleCharacterUnlockedMode4[order[*cursor]])
        {
            *cursor = static_cast<i32>(static_cast<u32>(*cursor) + static_cast<u32>(direction));
            if (*cursor < 0)
                *cursor = static_cast<i32>(static_cast<u32>(*cursor) + static_cast<u32>(count));
            if (*cursor >= count)
                *cursor = static_cast<i32>(static_cast<u32>(*cursor) - static_cast<u32>(count));
        }
    }
    return result;
}


int TitleScreenView::MoveCharacterCursorHorizontal(i32 cursorIndex, i32 count)
{
    if (count == 0)
        return 0;

    i32 direction;
    if (g_TitleInputStateTable.states[cursorIndex].IsPressedScrolling(0x40))
    {
        i32 *cursor = &side0CharacterCursor + cursorIndex;
        *cursor = static_cast<i32>(static_cast<u32>(*cursor) - 1u);
        if (*cursor < 0)
            *cursor = static_cast<i32>(static_cast<u32>(*cursor) + static_cast<u32>(count));
        direction = -1;
    }
    else if (g_TitleInputStateTable.states[cursorIndex].IsPressedScrolling(0x80))
    {
        i32 *cursor = &side0CharacterCursor + cursorIndex;
        *cursor = static_cast<i32>(static_cast<u32>(*cursor) + 1u);
        if (*cursor >= count)
            *cursor = static_cast<i32>(static_cast<u32>(*cursor) - static_cast<u32>(count));
        direction = 1;
    }
    else if (g_TitleInputStateTable.states[cursorIndex].IsPressedScrolling(0x10))
    {
        i32 *cursor = &side0CharacterCursor + cursorIndex;
        *cursor = static_cast<i32>(static_cast<u32>(*cursor) - 1u);
        if (*cursor < 0)
            *cursor = static_cast<i32>(static_cast<u32>(*cursor) + static_cast<u32>(count));
        direction = -1;
    }
    else if (g_TitleInputStateTable.states[cursorIndex].IsPressedScrolling(0x20))
    {
        i32 *cursor = &side0CharacterCursor + cursorIndex;
        *cursor = static_cast<i32>(static_cast<u32>(*cursor) + 1u);
        if (*cursor >= count)
            *cursor = static_cast<i32>(static_cast<u32>(*cursor) - static_cast<u32>(count));
        direction = 1;
    }
    else
    {
        return 0;
    }

    g_SoundPlayer.PlaySoundByIdx(12, 0);
    return direction;
}


int TitleScreenView::MoveCharacterCursorHorizontalForInput(i32 inputIndex, i32 cursorIndex, i32 count)
{
    if (count == 0)
        return 0;

    i32 direction;
    if (g_TitleInputStateTable.states[inputIndex].IsPressedScrolling(0x40))
    {
        i32 *cursor = &side0CharacterCursor + cursorIndex;
        *cursor = static_cast<i32>(static_cast<u32>(*cursor) - 1u);
        if (*cursor < 0)
            *cursor = static_cast<i32>(static_cast<u32>(*cursor) + static_cast<u32>(count));
        direction = -1;
    }
    else if (g_TitleInputStateTable.states[inputIndex].IsPressedScrolling(0x80))
    {
        i32 *cursor = &side0CharacterCursor + cursorIndex;
        *cursor = static_cast<i32>(static_cast<u32>(*cursor) + 1u);
        if (*cursor >= count)
            *cursor = static_cast<i32>(static_cast<u32>(*cursor) - static_cast<u32>(count));
        direction = 1;
    }
    else if (g_TitleInputStateTable.states[inputIndex].IsPressedScrolling(0x10))
    {
        i32 *cursor = &side0CharacterCursor + cursorIndex;
        *cursor = static_cast<i32>(static_cast<u32>(*cursor) - 1u);
        if (*cursor < 0)
            *cursor = static_cast<i32>(static_cast<u32>(*cursor) + static_cast<u32>(count));
        direction = -1;
    }
    else if (g_TitleInputStateTable.states[inputIndex].IsPressedScrolling(0x20))
    {
        i32 *cursor = &side0CharacterCursor + cursorIndex;
        *cursor = static_cast<i32>(static_cast<u32>(*cursor) + 1u);
        if (*cursor >= count)
            *cursor = static_cast<i32>(static_cast<u32>(*cursor) - static_cast<u32>(count));
        direction = 1;
    }
    else
    {
        return 0;
    }

    g_SoundPlayer.PlaySoundByIdx(12, 0);
    return direction;
}


void TitleScreenView::SetCharacterCursorActive(i32 selectedCharacter, i32 start, i32 count, i32 stride)
{
    for (int i = start; i < start + count * stride; i += stride)
    {
        titleAnm->SetSprite(&vms[i], vms[i].baseSpriteIndex + 1);
        vms[i].pendingInterrupt = 8;
        g_AnmManager->ExecuteScript(&vms[i]);
    }
    titleAnm->SetSprite(&vms[selectedCharacter * stride + start], vms[selectedCharacter * stride + start].baseSpriteIndex);
    vms[selectedCharacter * stride + start].pendingInterrupt = 7;
    g_AnmManager->ExecuteScript(&vms[selectedCharacter * stride + start]);
}


void TitleScreenView::SetCharacterCursorReverse(i32 selectedCharacter, i32 start, i32 count, i32 stride)
{
    for (int i = start; i < start + count * stride; i += stride)
    {
        titleAnm->SetSprite(&vms[i], vms[i].baseSpriteIndex + 1);
        vms[i].pendingInterrupt = 15;
    }
    titleAnm->SetSprite(&vms[selectedCharacter * stride + start], vms[selectedCharacter * stride + start].baseSpriteIndex);
    vms[selectedCharacter * stride + start].pendingInterrupt = 14;
}


void TitleScreenView::SetCharacterCursorInactive(i32 selectedCharacter, i32 start, i32 count, i32 stride)
{
    for (int i = start; i < start + count * stride; i += stride)
    {
        titleAnm->SetSprite(&vms[i], vms[i].baseSpriteIndex + 1);
        vms[i].pendingInterrupt = 18;
    }
    titleAnm->SetSprite(&vms[selectedCharacter * stride + start], vms[selectedCharacter * stride + start].baseSpriteIndex);
    vms[selectedCharacter * stride + start].pendingInterrupt = 17;
}


int TitleScreenView::SetCharacterSettingSprites(i32 side0Setting, i32 side1Setting)
{
    titleAnm->SetSprite(&vms[142], side0Setting == 0 ? 197 : (side0Setting == 1 ? 196 : 195));
    side0Setting -= 2;
    titleAnm->SetSprite(&vms[143], side0Setting <= 0 ? 197 : (side0Setting == 1 ? 196 : 195));
    side0Setting -= 2;
    titleAnm->SetSprite(&vms[144], side0Setting <= 0 ? 197 : (side0Setting == 1 ? 196 : 195));
    side0Setting -= 2;
    titleAnm->SetSprite(&vms[145], side0Setting <= 0 ? 197 : (side0Setting == 1 ? 196 : 195));
    side0Setting -= 2;
    titleAnm->SetSprite(&vms[146], side0Setting <= 0 ? 197 : (side0Setting == 1 ? 196 : 195));

    int setting = side1Setting;
    titleAnm->SetSprite(&vms[148], setting == 0 ? 197 : (setting == 1 ? 196 : 195));
    setting -= 2;
    titleAnm->SetSprite(&vms[149], setting <= 0 ? 197 : (setting == 1 ? 196 : 195));
    setting -= 2;
    titleAnm->SetSprite(&vms[150], setting <= 0 ? 197 : (setting == 1 ? 196 : 195));
    setting -= 2;
    titleAnm->SetSprite(&vms[151], setting <= 0 ? 197 : (setting == 1 ? 196 : 195));
    setting -= 2;
    return titleAnm->SetSprite(&vms[152], setting <= 0 ? 197 : (setting == 1 ? 196 : 195));
}


int TitleScreenView::UpdateCharacterSettings(i32 side0Setting, i32 side1Setting)
{
    vms[141].pendingInterrupt = 9;
    vms[147].pendingInterrupt = 9;
    vms[142].pendingInterrupt = 9;
    vms[143].pendingInterrupt = 9;
    vms[144].pendingInterrupt = 9;
    vms[145].pendingInterrupt = 9;
    vms[146].pendingInterrupt = 9;
    vms[148].pendingInterrupt = 9;
    vms[149].pendingInterrupt = 9;
    vms[150].pendingInterrupt = 9;
    vms[151].pendingInterrupt = 9;
    vms[152].pendingInterrupt = 9;
    return SetCharacterSettingSprites(side0Setting, side1Setting);
}


int TitleScreenView::SetCharacterSettingIndicator(i32 side, i32 value)
{
    value = value ? 9 : 10;
    vms[153 + side].pendingInterrupt = (u16)value;
    return value;
}


int TitleCharacterConfigView::GetOptionState(char character, i32 option)
{
    if (option >= 0)
        return optionValues[character][option];
    else
    {
        return optionValues[character][0] |
               optionValues[character][1] |
               optionValues[character][2] |
               optionValues[character][3];
    }
}


void TitleScreenView::UpdateCharacterSelectionVisuals(
    i32 side, i32 selectedCharacter, i32 otherCharacter, char *order)
{
    AnmVmView *marker = &vms[189 + side];

    i32 selectedVisibleIndex = 0;
    for (i32 orderIndex = 0; orderIndex < 16; orderIndex++)
    {
        i32 character = order[orderIndex];
        if (selectedCharacter != character)
        {
            if (currentScreen == 8)
            {
                if (g_TitleCharacterUnlocked[character] ||
                    g_SupervisorNetworkState->active)
                    selectedVisibleIndex++;
            }
            else if (currentScreen == 3)
            {
                if (g_TitleCharacterUnlockedNormal[character] != 0)
                    selectedVisibleIndex++;
            }
            else
            {
                if (g_TitleCharacterUnlockedMode4[character] != 0)
                    selectedVisibleIndex++;
            }
        }
        else
        {
            break;
        }
    }

    TitleFloat3View targetPosition;
    targetPosition.x = marker->pos.x;
    targetPosition.y = selectedVisibleIndex * 16.0f + 128.0f;
    targetPosition.z = 0.0f;
    marker->ConfigurePositionInterpolation(
        8, 4, reinterpret_cast<TitleFloat3View *>(&marker->pos), &targetPosition);

    i32 otherVisibleIndex = 0;
    for (i32 orderIndex = 0; orderIndex < 16; orderIndex++)
    {
        i32 character = order[orderIndex];
        if (otherCharacter != character)
        {
            if (currentScreen == 8)
            {
                if (g_TitleCharacterUnlocked[character] ||
                    g_SupervisorNetworkState->active)
                    otherVisibleIndex++;
            }
            else if (currentScreen == 3)
            {
                if (g_TitleCharacterUnlockedNormal[character] != 0)
                    otherVisibleIndex++;
            }
            else
            {
                if (g_TitleCharacterUnlockedMode4[character] != 0)
                    otherVisibleIndex++;
            }
        }
        else
        {
            break;
        }
    }

    i32 visibleIndex = 0;
    for (i32 orderIndex = 0; orderIndex < 16; orderIndex++)
    {
        i32 character = order[orderIndex];
        if (g_TitleCharacterUnlocked[character] ||
            g_SupervisorNetworkState->active)
        {
            AnmVmView *vm = &vms[191 + visibleIndex];
            if (selectedVisibleIndex == visibleIndex ||
                otherVisibleIndex == visibleIndex)
                vm->ConfigureColorInterpolation(
                    8, 4, vm->color1, 0xFFFFFFFFu);
            else
                vm->ConfigureColorInterpolation(
                    8, 4, vm->color1, 0xFFA0A0A0u);
            visibleIndex++;
        }
    }
}


void TitleScreenView::UpdateScreen16SelectionVisuals(i32 unusedSide, i32 selectedCharacter, char *order)
{
    (void)unusedSide;

    AnmVmView *marker = &vms[189];
    i32 selectedVisibleIndex = 0;
    for (i32 orderIndex = 0; orderIndex < 16; orderIndex++)
    {
        i32 character = order[orderIndex];
        if (selectedCharacter != character)
        {
            if (orderIndex > 13 || g_TitleCharacterUnlocked[character] || g_SupervisorNetworkState->active)
                selectedVisibleIndex++;
        }
        else
        {
            break;
        }
    }

    TitleFloat3View targetPosition;
    targetPosition.x = marker->pos.x;
    targetPosition.y = selectedVisibleIndex * 16.0f + 128.0f;
    targetPosition.z = 0.0f;
    marker->ConfigurePositionInterpolation(
        8, 4, reinterpret_cast<TitleFloat3View *>(&marker->pos), &targetPosition);

    marker = &vms[190];
    targetPosition.x = marker->pos.x;
    marker->ConfigurePositionInterpolation(
        8, 4, reinterpret_cast<TitleFloat3View *>(&marker->pos), &targetPosition);

    i32 visibleIndex = 0;
    for (i32 orderIndex = 0; orderIndex < 16; orderIndex++)
    {
        i32 character = order[orderIndex];
        if (g_TitleCharacterUnlocked[character] || orderIndex > 13 || g_SupervisorNetworkState->active)
        {
            AnmVmView *vm = &vms[207 + visibleIndex];
            if (selectedVisibleIndex == visibleIndex)
                vm->ConfigureColorInterpolation(8, 4, vm->color1, 0xFFFFFFFFu);
            else
                vm->ConfigureColorInterpolation(8, 4, vm->color1, 0xFFA0A0A0u);
            visibleIndex++;
        }
    }
}

int TitleScoreRecordView::InsertIntoTable()
{
    i32 rankIndex;
    for (rankIndex = 0; rankIndex < 5; rankIndex++)
    {
        if (score >= g_TitleScoreTable[category0][category1][rankIndex].score)
            break;
    }

    if (rankIndex < 5)
    {
        for (i32 index = 4; index > rankIndex; index--)
        {
            g_TitleScoreTable[category0][category1][index] =
                g_TitleScoreTable[category0][category1][index - 1];
            g_TitleScoreTable[category0][category1][index].rank = (u8)index;
        }

        g_TitleScoreTable[category0][category1][rankIndex] = *this;
        g_TitleScoreTable[category0][category1][rankIndex].rank = (u8)rankIndex;
        return rankIndex;
    }

    return 99;
}


int TitleScreenView::OnUpdateCharacterSelect()
{
    char characters[2];
    i32 value;
    switch (currentScreenState)
    {
    case 0:
        if (stateTimer2 == 0)
        {
            g_AnmManager->SetInterruptArray(vms, vmCount, 13);
            g_AnmManager->ExecuteScriptArray(vms, vmCount);

            i32 visibleCount = 0;
            i32 visibleByteOffset = 0;
            for (value = 0; value < 14; value++)
            {
                i32 character = g_TitleCharacterOrder14[value];
                if ((g_TitleDifficulty == 4 && g_TitleCharacterUnlockedMode4[character]) ||
                    (g_TitleDifficulty != 4 && g_TitleCharacterUnlockedNormal[character]))
                {
                    titleAnm->SetSprite(reinterpret_cast<AnmVmView *>(
                        reinterpret_cast<unsigned char *>(vms) +
                        191 * sizeof(AnmVmView) + visibleByteOffset),
                        character + 235);
                    reinterpret_cast<AnmVmView *>(
                        reinterpret_cast<unsigned char *>(vms) +
                        191 * sizeof(AnmVmView) + visibleByteOffset)->flags |= 2;
                    visibleCount++;
                    visibleByteOffset += sizeof(AnmVmView);
                }
            }
            if (visibleCount < 16)
            {
                i32 hiddenByteOffset = visibleCount * sizeof(AnmVmView);
                do
                {
                    reinterpret_cast<AnmVmView *>(
                        reinterpret_cast<unsigned char *>(vms) +
                        191 * sizeof(AnmVmView) + hiddenByteOffset)->flags &= ~2u;
                    hiddenByteOffset += sizeof(AnmVmView);
                } while (hiddenByteOffset < 16 * (i32)sizeof(AnmVmView));
            }

            vms[189].flags |= 2;
            vms[190].flags &= ~2u;

            side0CharacterCursor = 0;
            while (g_TitleCharacterOrder14[side0CharacterCursor] != g_GameSide0Value20)
                side0CharacterCursor++;

            if (g_TitleDifficulty == 4)
                MoveCharacterCursorMode4(0, 1, g_TitleCharacterOrder14, 14);
            else
                MoveCharacterCursorNormal(0, 1, g_TitleCharacterOrder14, 14);

            characters[0] = g_TitleCharacterOrder14[side0CharacterCursor];
            characters[1] = g_TitleCharacterOrder14[side1CharacterCursor];

            i32 selectedCharacter = characters[0];
            vms[93 + selectedCharacter * 2].color1Bytes[2] = 0xFF;
            vms[93 + selectedCharacter * 2].color1Bytes[1] = 0xFF;
            vms[93 + selectedCharacter * 2].color1Bytes[0] = 0xFF;
            vms[61 + selectedCharacter * 2].color1Bytes[2] = 0xFF;
            vms[61 + selectedCharacter * 2].color1Bytes[1] = 0xFF;
            vms[61 + selectedCharacter * 2].color1Bytes[0] = 0xFF;
            vms[93 + characters[1] * 2].color1Bytes[2] = 0xFF;
            vms[93 + characters[1] * 2].color1Bytes[1] = 0xFF;
            vms[93 + characters[1] * 2].color1Bytes[0] = 0xFF;
            vms[61 + characters[1] * 2].color1Bytes[2] = 0xFF;
            vms[61 + characters[1] * 2].color1Bytes[1] = 0xFF;
            vms[61 + characters[1] * 2].color1Bytes[0] = 0xFF;

            SetCharacterCursorActive(characters[0], 92, 14, 2);
            SetCharacterCursorActive(characters[0], 60, 14, 2);
            side0CharacterConfirmed = 0;
            currentScreenState = 0;
            stateTimer = 0;
            uiAux = 0;
            unknown0C918 = 1;
            side1CharacterConfirmed = 1;
            UpdateCharacterSelectionVisuals(0, characters[0], characters[0], g_TitleCharacterOrder14);

            characterSettingInputActive = 0;
            side0CharacterSetting = 10;
            side1CharacterSetting = 10;
            if (g_TitleInput.currentInput & 4)
            {
                characterSettingInputActive = 1;
                UpdateCharacterSettings(10, 10);
            }

            if (g_TitleDifficulty == 4)
                SetCharacterSettingIndicator(0, g_TitleCharacterConfig.optionValues[characters[0]][4]);
            else
                SetCharacterSettingIndicator(0, g_TitleCharacterConfig.GetOptionState(characters[0], g_TitleDifficulty));
        }

        if (stateTimer2 == 8)
            currentScreenState = 1;
        break;

    case 1:
    {
        if (side0CharacterConfirmed == 0)
        {
            if ((g_TitleInput.currentInput & 4) == 0)
            {
                value = MoveCharacterCursorHorizontalForInput(2, 0, 14);
                if (value != 0)
                {
                    if (g_TitleDifficulty == 4)
                        MoveCharacterCursorMode4(0, value, g_TitleCharacterOrder14, 14);
                    else
                        MoveCharacterCursorNormal(0, value, g_TitleCharacterOrder14, 14);

                    characters[0] = g_TitleCharacterOrder14[side0CharacterCursor];
                    if (g_TitleDifficulty == 4)
                        SetCharacterSettingIndicator(0, g_TitleCharacterConfig.optionValues[characters[0]][4]);
                    else
                        SetCharacterSettingIndicator(0, g_TitleCharacterConfig.GetOptionState(characters[0], g_TitleDifficulty));

                    if (value > 0)
                    {
                        SetCharacterCursorActive(characters[0], 92, 14, 2);
                        SetCharacterCursorActive(characters[0], 60, 14, 2);
                    }
                    else
                    {
                        SetCharacterCursorReverse(characters[0], 92, 14, 2);
                        SetCharacterCursorReverse(characters[0], 60, 14, 2);
                    }
                    UpdateCharacterSelectionVisuals(0, characters[0], characters[0], g_TitleCharacterOrder14);
                }
            }
            else
            {
                if (characterSettingInputActive)
                {
                    if (g_TitleInput.IsPressedScrolling(0x40))
                    {
                        if (side0CharacterSetting > 1)
                            side0CharacterSetting--;
                        UpdateCharacterSettings(side0CharacterSetting, side1CharacterSetting);
                    }
                    if (g_TitleInput.IsPressedScrolling(0x80))
                    {
                        if (side0CharacterSetting < 10)
                            side0CharacterSetting++;
                        UpdateCharacterSettings(side0CharacterSetting, side1CharacterSetting);
                    }
                }
            }
        }

        characters[0] = g_TitleCharacterOrder14[side0CharacterCursor];
        if ((static_cast<u16>(g_TitleInputFlags) & 0x1001) && side0CharacterConfirmed == 0)
        {
            PlayMenuSound(10, 0);
            side0CharacterConfirmed = 1;
            value = characters[0];
            vms[92 + value * 2].color1Bytes[2] = 0x80;
            vms[92 + value * 2].color1Bytes[1] = 0x80;
            vms[92 + value * 2].color1Bytes[0] = 0x80;
            vms[60 + value * 2].color1Bytes[2] = 0x80;
            vms[60 + value * 2].color1Bytes[1] = 0x80;
            vms[60 + value * 2].color1Bytes[0] = 0x80;
            g_GameSide0Value30 = (g_TitleInput.currentInput >> 2) & 1;
        }

        if (side0CharacterConfirmed && side1CharacterConfirmed)
        {
            g_TitleSupervisor.StopAudio();
            g_GameSide0Value20 = characters[0];
            g_GameSide1Value20 = 0;
            g_GameSide1Value30 = (g_TitleInput.currentInput >> 14) & 1;
            g_GameCurrentStage = 0;

            if (currentScreen == 3)
            {
                g_GameValueDB8 = 0;
                g_GameMode = 0;
                g_GameValueDF0 = 1;
                g_TitleGlobalMode = 2;
            }
            else if (currentScreen == 5)
            {
                g_GameValueDB8 = 0;
                g_GameMode = 1;
                g_GameValueDF0 = 1;
                g_TitleGlobalMode = 2;
            }
            else
            {
                if (g_TitleModeSelection == 0)
                {
                    g_GameValueDB8 = 0;
                    g_GameValueDF0 = 0;
                    g_GameMode = 2;
                    g_TitleGlobalMode = 2;
                }
                else if (g_TitleModeSelection == 1)
                {
                    g_GameValueDB8 = 0;
                    g_GameMode = 2;
                    g_GameValueDF0 = 1;
                    g_TitleGlobalMode = 2;
                }
                else if (g_TitleModeSelection == 2)
                {
                    g_GameValueDB8 = 1;
                    g_GameValueDF0 = 0;
                    g_GameMode = 2;
                    g_TitleGlobalMode = 2;
                }
                else if (g_TitleModeSelection == 3)
                {
                    g_GameValueDB8 = 1;
                    g_GameMode = 2;
                    g_GameValueDF0 = 1;
                    g_TitleGlobalMode = 2;
                }
            }

            g_TitleCharacterReturnSelection = g_TitleDifficulty;
            g_GameSide0CharacterSetting = side0CharacterSetting;
            g_GameSide1CharacterSetting = side1CharacterSetting;
            return 0;
        }

        if ((static_cast<u8>(g_TitleInputFlags) & 0xA) && side0CharacterConfirmed == 0)
        {
            g_TitleCharacterReturnSelection = keyboardSelection;
            PlayMenuSound(11, 0);
            stateTimer = 0;
            g_GameSide0Value20 = characters[0];
            if (currentScreen == 3)
            {
                ChangeCurrentScreen(2);
                return 1;
            }
            else if (currentScreen == 5)
            {
                ChangeCurrentScreen(4);
                return 1;
            }
            else
                ChangeCurrentScreen(7);
            return 1;
        }
        break;
    }
    }

    stateTimer++;
    screenFrameCounter++;
    stateTimer2++;
    return 1;
}


int TitleScreenView::UpdateScreen8Mode0()
{
    char characters[2];
    switch (currentScreenState)
    {
    case 0:
    {
        if (stateTimer2 == 0)
        {
            i32 value;

            g_AnmManager->SetInterruptArray(vms, vmCount, 13);
            g_AnmManager->ExecuteScriptArray(vms, vmCount);

            {
                i32 visibleCount = 0;
                for (value = 0; value < 16; value++)
                {
                    char character = g_TitleCharacterOrder[value];
                    if (g_TitleCharacterUnlocked[character] || g_SupervisorNetworkState->active)
                    {
                        titleAnm->SetSprite(
                            &vms[191 + visibleCount],
                            character + 235);
                        vms[191 + visibleCount].flags |= 2;
                        visibleCount++;
                    }
                }
                for (value = visibleCount; value < 16; value++)
                    vms[191 + value].flags &= ~2u;
            }

            vms[189].flags |= 2;
            vms[190].flags |= 2;

            side0CharacterCursor = 0;
            while (g_TitleCharacterOrder[side0CharacterCursor] != g_GameSide0Value20)
                side0CharacterCursor++;
            side1CharacterCursor = 0;
            while (g_TitleCharacterOrder[side1CharacterCursor] != g_GameSide1Value20)
                side1CharacterCursor++;

            MoveCharacterCursor(0, 1, g_TitleCharacterOrder, 16);
            MoveCharacterCursor(1, 1, g_TitleCharacterOrder, 16);

            characters[1] = g_TitleCharacterOrder[side1CharacterCursor];
            value = g_TitleCharacterOrder[side0CharacterCursor];
            SetCharacterCursorActive(value, 92, 16, 2);
            SetCharacterCursorActive(g_TitleCharacterOrder[side1CharacterCursor], 93, 16, 2);
            SetCharacterCursorActive(g_TitleCharacterOrder[side0CharacterCursor], 60, 16, 2);
            SetCharacterCursorActive(g_TitleCharacterOrder[side1CharacterCursor], 61, 16, 2);

            unknown0C918 = 1;
            side1CharacterConfirmed = 0;
            side0CharacterConfirmed = 0;
            currentScreenState = 0;
            stateTimer = 0;
            uiAux = 0;
            side0CharacterSetting = g_GameSide0CharacterSetting;
            side1CharacterSetting = g_GameSide1CharacterSetting;
            UpdateCharacterSettings(side0CharacterSetting, side1CharacterSetting);
            UpdateCharacterSelectionVisuals(0, value, characters[1], g_TitleCharacterOrder);
            UpdateCharacterSelectionVisuals(1, characters[1], value, g_TitleCharacterOrder);
        }

        if (stateTimer2 == 8)
            currentScreenState = 1;
        break;
    }

    case 1:
    {
        i32 value;

        if (g_TitleModeSelection == 4 && g_SupervisorNetworkState->active == 0)
        {
            PlayMenuSound(11, 0);
            stateTimer = 0;
            if (currentScreen == 3)
            {
                ChangeCurrentScreen(2);
                return 1;
            }
            else if (currentScreen == 5)
            {
                ChangeCurrentScreen(4);
                return 1;
            }
            else
                ChangeCurrentScreen(7);
            return 1;
        }

        if (side0CharacterConfirmed == 0)
        {
            if ((g_TitleSide0Input.currentInput & 4) == 0)
            {
                value = MoveCharacterCursorHorizontal(0, 16);
                if (value != 0)
                {
                    MoveCharacterCursor(0, value, g_TitleCharacterOrder, 16);
                    characters[0] = g_TitleCharacterOrder[side0CharacterCursor];
                    characters[1] = g_TitleCharacterOrder[side1CharacterCursor];
                    if (value > 0)
                    {
                        SetCharacterCursorActive(characters[0], 92, 16, 2);
                        SetCharacterCursorActive(characters[0], 60, 16, 2);
                    }
                    else
                    {
                        SetCharacterCursorReverse(characters[0], 92, 16, 2);
                        SetCharacterCursorReverse(characters[0], 60, 16, 2);
                    }
                    UpdateCharacterSelectionVisuals(0, characters[0], characters[1], g_TitleCharacterOrder);
                }
            }
            else
            {
                if (g_TitleSide0Input.IsPressedScrolling(64))
                {
                    if (side0CharacterSetting > 1)
                        side0CharacterSetting--;
                    UpdateCharacterSettings(side0CharacterSetting, side1CharacterSetting);
                }
                if (g_TitleSide0Input.IsPressedScrolling(128))
                {
                    if (side0CharacterSetting < 10)
                        side0CharacterSetting++;
                    UpdateCharacterSettings(side0CharacterSetting, side1CharacterSetting);
                }
            }

            if (g_TitleSide0Input.currentInput & 0x100)
            {
                side0CharacterCursor += g_TitleSelectionRandom.NextU32() & 0xF;
                if (side0CharacterCursor >= 16)
                    side0CharacterCursor -= 16;
                MoveCharacterCursor(0, 1, g_TitleCharacterOrder, 16);
                characters[0] = g_TitleCharacterOrder[side0CharacterCursor];
                characters[1] = g_TitleCharacterOrder[side1CharacterCursor];
                SetCharacterCursorActive(characters[0], 92, 16, 2);
                SetCharacterCursorActive(characters[0], 60, 16, 2);
                UpdateCharacterSelectionVisuals(0, characters[0], characters[1], g_TitleCharacterOrder);
            }
        }

        if (side1CharacterConfirmed == 0)
        {
            if ((g_TitleSide1Input.currentInput & 4) == 0)
            {
                value = MoveCharacterCursorHorizontal(1, 16);
                if (value != 0)
                {
                    MoveCharacterCursor(1, value, g_TitleCharacterOrder, 16);
                    characters[0] = g_TitleCharacterOrder[side0CharacterCursor];
                    characters[1] = g_TitleCharacterOrder[side1CharacterCursor];
                    if (value < 0)
                    {
                        SetCharacterCursorActive(characters[1], 93, 16, 2);
                        SetCharacterCursorActive(characters[1], 61, 16, 2);
                    }
                    else
                    {
                        SetCharacterCursorReverse(characters[1], 93, 16, 2);
                        SetCharacterCursorReverse(characters[1], 61, 16, 2);
                    }
                    UpdateCharacterSelectionVisuals(1, characters[1], characters[0], g_TitleCharacterOrder);
                }
            }
            else
            {
                if (g_TitleSide1Input.IsPressedScrolling(64))
                {
                    if (side1CharacterSetting > 1)
                        side1CharacterSetting--;
                    UpdateCharacterSettings(side0CharacterSetting, side1CharacterSetting);
                }
                if (g_TitleSide1Input.IsPressedScrolling(128))
                {
                    if (side1CharacterSetting < 10)
                        side1CharacterSetting++;
                    UpdateCharacterSettings(side0CharacterSetting, side1CharacterSetting);
                }
            }

            if (g_TitleSide1Input.currentInput & 0x100)
            {
                side1CharacterCursor += g_TitleSelectionRandom.NextU32() & 0xF;
                if (side1CharacterCursor >= 16)
                    side1CharacterCursor -= 16;
                MoveCharacterCursor(1, 1, g_TitleCharacterOrder, 16);
                characters[0] = g_TitleCharacterOrder[side0CharacterCursor];
                value = g_TitleCharacterOrder[side1CharacterCursor];
                SetCharacterCursorActive(value, 93, 16, 2);
                SetCharacterCursorActive(value, 61, 16, 2);
                UpdateCharacterSelectionVisuals(1, value, characters[0], g_TitleCharacterOrder);
            }
        }

        characters[0] = g_TitleCharacterOrder[side0CharacterCursor];
        characters[1] = g_TitleCharacterOrder[side1CharacterCursor];

        if ((g_TitleSide0Input.repeatInput & 0x1001) && side0CharacterConfirmed == 0)
        {
            PlayMenuSound(10, 0);
            side0CharacterConfirmed = 1;
            value = characters[0];
            vms[92 + value * 2].color1Bytes[2] = 0x80;
            vms[92 + value * 2].color1Bytes[1] = 0x80;
            vms[92 + value * 2].color1Bytes[0] = 0x80;
            vms[60 + value * 2].color1Bytes[2] = 0x80;
            vms[60 + value * 2].color1Bytes[1] = 0x80;
            vms[60 + value * 2].color1Bytes[0] = 0x80;
            g_GameSide0Value30 = (g_TitleSide0Input.currentInput >> 2) & 1;
            if (side1CharacterConfirmed && characters[0] == characters[1])
                g_GameSide0Value30 = 1 - g_GameSide1Value30;
        }

        if ((g_TitleSide1Input.repeatInput & 0x1001) && side1CharacterConfirmed == 0)
        {
            PlayMenuSound(10, 0);
            side1CharacterConfirmed = 1;
            value = characters[1];
            vms[93 + value * 2].color1Bytes[2] = 0x80;
            vms[93 + value * 2].color1Bytes[1] = 0x80;
            vms[93 + value * 2].color1Bytes[0] = 0x80;
            vms[61 + value * 2].color1Bytes[2] = 0x80;
            vms[61 + value * 2].color1Bytes[1] = 0x80;
            vms[61 + value * 2].color1Bytes[0] = 0x80;
            g_GameSide1Value30 = (g_TitleSide1Input.currentInput >> 2) & 1;
            if (side0CharacterConfirmed && characters[0] == characters[1])
                g_GameSide1Value30 = 1 - g_GameSide0Value30;
        }

        if (side0CharacterConfirmed && side1CharacterConfirmed)
        {
            g_GameSide0Value20 = characters[0];
            g_GameSide1Value20 = characters[1];
            g_GameSide0CharacterSetting = side0CharacterSetting;
            g_GameSide1CharacterSetting = side1CharacterSetting;
            PlayMenuSound(10, 0);
            ChangeCurrentScreen(16);
            UpdateScreen16();
            return 1;
        }

        u32 side0CancelInput = g_TitleSide0InputFlags & 0xA;
        if ((side0CancelInput && side0CharacterConfirmed == 0) ||
            ((g_TitleSide1Input.repeatInput & 0xA) && side1CharacterConfirmed == 0))
        {
            g_TitleCharacterReturnSelection = keyboardSelection;
            PlayMenuSound(11, 0);
            stateTimer = 0;
            g_GameSide0Value20 = characters[0];
            g_GameSide1Value20 = characters[1];
            if (currentScreen == 3)
            {
                ChangeCurrentScreen(2);
                return 1;
            }
            else if (currentScreen == 5)
            {
                ChangeCurrentScreen(4);
                return 1;
            }
            else
                ChangeCurrentScreen(7);
            return 1;
        }

        if (static_cast<u16>(side0CancelInput) && side0CharacterConfirmed == 1)
        {
            side0CharacterConfirmed = 0;
            PlayMenuSound(11, 0);
            value = characters[0];
            vms[92 + value * 2].color1Bytes[2] = 0xFF;
            vms[92 + value * 2].color1Bytes[1] = 0xFF;
            vms[92 + value * 2].color1Bytes[0] = 0xFF;
            vms[60 + value * 2].color1Bytes[2] = 0xFF;
            vms[60 + value * 2].color1Bytes[1] = 0xFF;
            vms[60 + value * 2].color1Bytes[0] = 0xFF;
        }

        if ((g_TitleSide1Input.repeatInput & 0xA) && side1CharacterConfirmed == 1)
        {
            side1CharacterConfirmed = 0;
            PlayMenuSound(11, 0);
            value = characters[1];
            vms[93 + value * 2].color1Bytes[2] = 0xFF;
            vms[93 + value * 2].color1Bytes[1] = 0xFF;
            vms[93 + value * 2].color1Bytes[0] = 0xFF;
            vms[61 + value * 2].color1Bytes[2] = 0xFF;
            vms[61 + value * 2].color1Bytes[1] = 0xFF;
            vms[61 + value * 2].color1Bytes[0] = 0xFF;
        }
        break;
    }
    }

    stateTimer++;
    screenFrameCounter++;
    stateTimer2++;
    return 1;
}

int TitleScreenView::UpdateScreen8Mode123()
{
    i32 value;
    char characters[2];
    switch (currentScreenState)
    {
    case 0:
        if (stateTimer2 == 0)
        {
            g_AnmManager->SetInterruptArray(vms, vmCount, 13);
            g_AnmManager->ExecuteScriptArray(vms, vmCount);

            i32 visibleCount = 0;
            for (value = 0; value < 16; value++)
            {
                char character = g_TitleCharacterOrderMode123[value];
                if (g_TitleCharacterUnlocked[character])
                {
                    titleAnm->SetSprite(&vms[191 + visibleCount], character + 235);
                    vms[191 + visibleCount].flags |= 2;
                    visibleCount++;
                }
            }
            for (i32 index = visibleCount; index < 16; index++)
                vms[191 + index].flags &= ~2u;

            vms[189].flags |= 2;
            vms[190].flags &= ~2u;

            side0CharacterCursor = 0;
            while (g_TitleCharacterOrderMode123[side0CharacterCursor] != g_GameSide0Value20)
                side0CharacterCursor++;
            side1CharacterCursor = 0;
            while (g_TitleCharacterOrderMode123[side1CharacterCursor] != g_GameSide1Value20)
                side1CharacterCursor++;

            MoveCharacterCursor(0, 1, g_TitleCharacterOrderMode123, 16);
            MoveCharacterCursor(1, 1, g_TitleCharacterOrderMode123, 16);

            characters[0] = g_TitleCharacterOrderMode123[side0CharacterCursor];
            characters[1] = g_TitleCharacterOrderMode123[side1CharacterCursor];

            vms[93 + characters[0] * 2].color1Bytes[2] = 0xFF;
            vms[93 + characters[0] * 2].color1Bytes[1] = 0xFF;
            vms[93 + characters[0] * 2].color1Bytes[0] = 0xFF;
            vms[61 + characters[0] * 2].color1Bytes[2] = 0xFF;
            vms[61 + characters[0] * 2].color1Bytes[1] = 0xFF;
            vms[61 + characters[0] * 2].color1Bytes[0] = 0xFF;
            vms[93 + characters[1] * 2].color1Bytes[2] = 0xFF;
            vms[93 + characters[1] * 2].color1Bytes[1] = 0xFF;
            vms[93 + characters[1] * 2].color1Bytes[0] = 0xFF;
            vms[61 + characters[1] * 2].color1Bytes[2] = 0xFF;
            vms[61 + characters[1] * 2].color1Bytes[1] = 0xFF;
            vms[61 + characters[1] * 2].color1Bytes[0] = 0xFF;

            SetCharacterCursorActive(characters[0], 92, 16, 2);
            SetCharacterCursorActive(characters[1], 93, 16, 2);
            SetCharacterCursorActive(characters[0], 60, 16, 2);
            SetCharacterCursorActive(characters[1], 61, 16, 2);

            unknown0C918 = 1;
            side1CharacterConfirmed = 0;
            side0CharacterConfirmed = 0;
            currentScreenState = 0;
            stateTimer = 0;
            uiAux = 0;
            side0CharacterSetting = g_GameSide0CharacterSetting;
            side1CharacterSetting = g_GameSide1CharacterSetting;
            UpdateCharacterSettings(side0CharacterSetting, side1CharacterSetting);
            UpdateCharacterSelectionVisuals(
                0, characters[0], characters[0], g_TitleCharacterOrderMode123);
        }

        if (stateTimer2 == 8)
            currentScreenState = 1;
        break;

    case 1:
    {
        characters[0] = g_TitleCharacterOrderMode123[side0CharacterCursor];
        characters[1] = g_TitleCharacterOrderMode123[side1CharacterCursor];

        if (side0CharacterConfirmed == 0)
        {
            if ((g_TitleInput.currentInput & 4) == 0)
            {
                value = MoveCharacterCursorHorizontalForInput(2, 0, 16);
                if (value != 0)
                {
                    MoveCharacterCursor(0, value, g_TitleCharacterOrderMode123, 16);
                    characters[0] = g_TitleCharacterOrderMode123[side0CharacterCursor];
                    characters[1] = g_TitleCharacterOrderMode123[side1CharacterCursor];
                    if (value > 0)
                    {
                        SetCharacterCursorActive(characters[0], 92, 16, 2);
                        SetCharacterCursorActive(characters[0], 60, 16, 2);
                    }
                    else
                    {
                        SetCharacterCursorReverse(characters[0], 92, 16, 2);
                        SetCharacterCursorReverse(characters[0], 60, 16, 2);
                    }
                    UpdateCharacterSelectionVisuals(
                        0, characters[0], characters[0], g_TitleCharacterOrderMode123);
                }
            }
            else
            {
                if (g_TitleInput.IsPressedScrolling(0x40))
                {
                    if (side0CharacterSetting > 1)
                        side0CharacterSetting--;
                    UpdateCharacterSettings(side0CharacterSetting, side1CharacterSetting);
                }
                if (g_TitleInput.IsPressedScrolling(0x80))
                {
                    if (side0CharacterSetting < 10)
                        side0CharacterSetting++;
                    UpdateCharacterSettings(side0CharacterSetting, side1CharacterSetting);
                }
            }
        }
        if (side0CharacterConfirmed != 0 && side1CharacterConfirmed == 0)
        {
            if ((g_TitleInput.currentInput & 4) == 0)
            {
                value = MoveCharacterCursorHorizontalForInput(2, 1, 16);
                if (value != 0)
                {
                    MoveCharacterCursor(1, value, g_TitleCharacterOrderMode123, 16);
                    characters[0] = g_TitleCharacterOrderMode123[side0CharacterCursor];
                    characters[1] = g_TitleCharacterOrderMode123[side1CharacterCursor];
                    if (value < 0)
                    {
                        SetCharacterCursorActive(characters[1], 93, 16, 2);
                        SetCharacterCursorActive(characters[1], 61, 16, 2);
                    }
                    else
                    {
                        SetCharacterCursorReverse(characters[1], 93, 16, 2);
                        SetCharacterCursorReverse(characters[1], 61, 16, 2);
                    }
                    UpdateCharacterSelectionVisuals(
                        1, characters[1], characters[0], g_TitleCharacterOrderMode123);
                }
            }
            else
            {
                if (g_TitleInput.IsPressedScrolling(0x40))
                {
                    if (side1CharacterSetting > 1)
                        side1CharacterSetting--;
                    UpdateCharacterSettings(side0CharacterSetting, side1CharacterSetting);
                }
                if (g_TitleInput.IsPressedScrolling(0x80))
                {
                    if (side1CharacterSetting < 10)
                        side1CharacterSetting++;
                    UpdateCharacterSettings(side0CharacterSetting, side1CharacterSetting);
                }
            }
        }

        u32 confirmInput = g_TitleInputFlags & 0x1001;
        if (confirmInput && side0CharacterConfirmed == 0)
        {
            PlayMenuSound(10, 0);
            side0CharacterConfirmed = 1;
            vms[92 + characters[0] * 2].color1Bytes[2] = 0x80;
            vms[92 + characters[0] * 2].color1Bytes[1] = 0x80;
            vms[92 + characters[0] * 2].color1Bytes[0] = 0x80;
            vms[60 + characters[0] * 2].color1Bytes[2] = 0x80;
            vms[60 + characters[0] * 2].color1Bytes[1] = 0x80;
            vms[60 + characters[0] * 2].color1Bytes[0] = 0x80;
            g_GameSide0Value30 = (g_TitleInput.currentInput >> 2) & 1;
            if (side1CharacterConfirmed && characters[0] == characters[1])
                g_GameSide0Value30 = 1 - g_GameSide1Value30;
            vms[190].flags |= 2;
            UpdateCharacterSelectionVisuals(
                1, characters[1], characters[0], g_TitleCharacterOrderMode123);
        }
        else if (side0CharacterConfirmed != 0 &&
                 static_cast<u16>(confirmInput) &&
                 side1CharacterConfirmed == 0)
        {
            PlayMenuSound(10, 0);
            side1CharacterConfirmed = 1;
            vms[93 + characters[1] * 2].color1Bytes[2] = 0x80;
            vms[93 + characters[1] * 2].color1Bytes[1] = 0x80;
            vms[93 + characters[1] * 2].color1Bytes[0] = 0x80;
            vms[61 + characters[1] * 2].color1Bytes[2] = 0x80;
            vms[61 + characters[1] * 2].color1Bytes[1] = 0x80;
            vms[61 + characters[1] * 2].color1Bytes[0] = 0x80;
            g_GameSide1Value30 = (g_TitleInput.currentInput >> 2) & 1;
            if (side0CharacterConfirmed && characters[0] == characters[1])
                g_GameSide1Value30 = 1 - g_GameSide0Value30;
        }

        if (side0CharacterConfirmed && side1CharacterConfirmed)
        {
            g_GameSide0Value20 = characters[0];
            g_GameSide1Value20 = characters[1];
            g_GameSide0CharacterSetting = side0CharacterSetting;
            g_GameSide1CharacterSetting = side1CharacterSetting;
            PlayMenuSound(10, 0);
            ChangeCurrentScreen(16);
            UpdateScreen16();
            return 1;
        }

        u32 cancelInput = g_TitleInputFlags & 0xA;
        if (cancelInput && side0CharacterConfirmed == 0)
        {
            g_TitleCharacterReturnSelection = keyboardSelection;
            PlayMenuSound(11, 0);
            stateTimer = 0;
            g_GameSide0Value20 = characters[0];
            g_GameSide1Value20 = characters[1];
            if (currentScreen == 3)
            {
                ChangeCurrentScreen(2);
                return 1;
            }
            else if (currentScreen == 5)
            {
                ChangeCurrentScreen(4);
                return 1;
            }
            else
                ChangeCurrentScreen(7);
            return 1;
        }

        if (side0CharacterConfirmed != 0 && static_cast<u16>(cancelInput) && side1CharacterConfirmed)
        {
            side1CharacterConfirmed = 0;
            PlayMenuSound(11, 0);
            vms[93 + characters[1] * 2].color1Bytes[2] = 0xFF;
            vms[93 + characters[1] * 2].color1Bytes[1] = 0xFF;
            vms[93 + characters[1] * 2].color1Bytes[0] = 0xFF;
            vms[61 + characters[1] * 2].color1Bytes[2] = 0xFF;
            vms[61 + characters[1] * 2].color1Bytes[1] = 0xFF;
            vms[61 + characters[1] * 2].color1Bytes[0] = 0xFF;
        }
        else if (static_cast<u16>(cancelInput) && side0CharacterConfirmed == 1)
        {
            side0CharacterConfirmed = 0;
            PlayMenuSound(11, 0);
            vms[92 + characters[0] * 2].color1Bytes[2] = 0xFF;
            vms[92 + characters[0] * 2].color1Bytes[1] = 0xFF;
            vms[92 + characters[0] * 2].color1Bytes[0] = 0xFF;
            vms[60 + characters[0] * 2].color1Bytes[2] = 0xFF;
            vms[60 + characters[0] * 2].color1Bytes[1] = 0xFF;
            vms[60 + characters[0] * 2].color1Bytes[0] = 0xFF;
            vms[190].flags &= ~2u;
            UpdateCharacterSelectionVisuals(
                0, characters[0], characters[0], g_TitleCharacterOrderMode123);
        }
        break;
    }
    }

    stateTimer++;
    screenFrameCounter++;
    stateTimer2++;
    return 1;
}


int TitleScreenView::UpdateScreen16()
{
    switch (currentScreenState)
    {
    case 0:
        if (stateTimer2 == 0)
        {
            g_AnmManager->SetInterruptArray(vms, vmCount, 26);
            g_AnmManager->ExecuteScriptArray(vms, vmCount);

            i32 visibleCount = 0;
            i32 orderIndex = 0;
            for (; orderIndex < 16; orderIndex++)
            {
                char character = g_TitleScreen16Order[orderIndex];
                if (character > 13 || g_TitleCharacterUnlocked[character] || g_SupervisorNetworkState->active)
                {
                    titleAnm->SetSprite(&vms[207 + visibleCount], character + 251);
                    vms[207 + visibleCount].flags |= 2;
                    g_TitleScreen16Entries[visibleCount] = g_TitleScreen16Order[orderIndex];
                    visibleCount++;
                }
            }

            keyboardSelection = visibleCount - 2;
            if (visibleCount < 16)
            {
                i32 index = visibleCount;
                char *entry = g_TitleScreen16Entries + index;
                do
                {
                    vms[207 + index].flags &= ~2u;
                    index++;
                    *entry++ = -1;
                } while (index < 16);
            }

            vms[189].flags |= 2;
            vms[190].flags |= 2;
            char selectedCharacter = g_TitleScreen16Entries[keyboardSelection];
            currentScreenState = 0;
            stateTimer = 0;
            uiAux = 0;
            unknown0C918 = 1;
            UpdateScreen16SelectionVisuals(0, selectedCharacter,
                                           g_TitleScreen16Order);
        }

        if (stateTimer2 == 8)
            currentScreenState = 1;
        break;

    case 1:
    {
        i32 direction = MoveCursorFourWay(16);
        if (direction != 0)
        {
            while (g_TitleScreen16Entries[keyboardSelection] < 0)
                keyboardSelection = (direction + keyboardSelection) % 16;
            UpdateScreen16SelectionVisuals(0, g_TitleScreen16Entries[keyboardSelection],
                                           g_TitleScreen16Order);
        }

        if (g_TitleInput.currentInput & 0x100)
        {
            keyboardSelection += g_TitleSelectionRandom.NextU32() & 0xF;
            if (keyboardSelection >= 16)
                keyboardSelection -= 16;
            UpdateScreen16SelectionVisuals(0, g_TitleScreen16Entries[keyboardSelection],
                                           g_TitleScreen16Order);
        }

        if (static_cast<u16>(g_TitleInputFlags) & 0x1001)
        {
            PlayMenuSound(10, 0);
            g_TitleSupervisor.StopAudio();
            g_GameCurrentStage = 0;

            if (currentScreen == 3)
            {
                g_GameValueDF0 = 1;
                g_TitleGlobalMode = 2;
                g_GameMode = 0;
                g_GameValueDB8 = 0;
            }
            else if (currentScreen == 5)
            {
                g_GameValueDF0 = 1;
                g_TitleGlobalMode = 2;
                g_GameMode = 1;
                g_GameValueDB8 = 0;
            }
            else
            {
                if (g_TitleModeSelection == 0 || g_TitleModeSelection == 4)
                {
                    g_GameValueDF0 = 0;
                    g_GameValueDB8 = 0;
                    g_TitleGlobalMode = 2;
                    g_GameMode = 2;
                }
                else if (g_TitleModeSelection == 1)
                {
                    g_GameValueDF0 = 1;
                    g_GameValueDB8 = 0;
                    g_TitleGlobalMode = 2;
                    g_GameMode = 2;
                }
                else if (g_TitleModeSelection == 2)
                {
                    g_GameValueDF0 = 0;
                    g_GameValueDB8 = 1;
                    g_TitleGlobalMode = 2;
                    g_GameMode = 2;
                }
                else if (g_TitleModeSelection == 3)
                {
                    g_GameValueDF0 = 1;
                    g_GameValueDB8 = 1;
                    g_TitleGlobalMode = 2;
                    g_GameMode = 2;
                }
            }

            g_TitleCharacterReturnSelection = g_TitleDifficulty;
            g_GameSide0CharacterSetting = side0CharacterSetting;
            g_GameSide1CharacterSetting = side1CharacterSetting;

            g_TitleScreen16SelectionResult = g_TitleScreen16Entries[keyboardSelection];
            if (g_TitleScreen16SelectionResult == 14)
                g_TitleScreen16SelectionResult = -1;
            else if (g_TitleScreen16SelectionResult == 15)
                g_TitleScreen16SelectionResult = -2;
            else
                g_TitleScreen16SelectionResult = g_TitleScreen16Entries[keyboardSelection];
            return 0;
        }

        if (static_cast<u8>(g_TitleInputFlags) & 0xA)
        {
            PlayMenuSound(11, 0);
            stateTimer = 0;
            ChangeCurrentScreen(8);
            switch (g_TitleModeSelection)
            {
            case 0:
                UpdateScreen8Mode0();
                break;
            case 1:
                UpdateScreen8Mode123();
                break;
            case 2:
                UpdateScreen8Mode123();
                break;
            case 3:
                UpdateScreen8Mode123();
                break;
            }
            return 1;
        }
        break;
    }
    }

    stateTimer++;
    screenFrameCounter++;
    stateTimer2++;
    return 1;
}


int TitleScreenView::OnUpdateResultNameEntry()
{
    switch (currentScreenState)
    {
    case 0:
        if (stateTimer2 == 0)
        {
            g_TitleSupervisor.StopAudio();
            if (g_AnmManager->LoadSurface(0, "title/result00.png"))
                return 0;

            if (vmCount == 0)
            {
                vmCount = 223;
                vms = new AnmVmView[vmCount];
                titleAnm->ExecuteAnmIdxArray(vms, 0, vmCount);
            }

            g_AnmManager->SetInterruptArray(vms, vmCount, 16);
            g_AnmManager->ExecuteScriptArray(vms, vmCount);

            if (g_GameMode == 2)
            {
                stateTimer = 0;
                ChangeCurrentScreen(15);
                return 1;
            }

            nameBankIndex = g_GameSide0Value20;
            replayNameCursor = 0;
            SetCharacterCursorInactive(nameBankIndex, 60, 16, 2);
            keyboardSelection = TitleNativeNameKeyboard::ConfirmKey;
            currentScreenState = 0;
            stateTimer = 0;
            uiAux = 0;
            nameSlotIndex = g_TitleScoreRecord.InsertIntoTable();
            if (nameSlotIndex >= 5)
            {
                keyboardSelection = TitleNativeNameKeyboard::ConfirmKey;
            }
            else
            {
                strcpy(
                    g_TitleNameTable[nameBankIndex][g_TitleNameTableIndex][nameSlotIndex].name,
                    g_ReplayName);
            }
        }

        if (stateTimer2 == 8)
            currentScreenState = 1;
        break;

    case 1:
        if (nameSlotIndex < 5)
        {
            if (g_TitleInput.IsPressedScrolling(0x10))
            {
                g_SoundPlayer.PlaySoundByIdx(12, 0);
                keyboardSelection -= TitleNativeNameKeyboard::Columns;
                if (keyboardSelection < 0)
                    keyboardSelection += TitleNativeNameKeyboard::KeyCount;
                if (keyboardSelection == TitleNativeNameKeyboard::GapKey)
                    keyboardSelection = 77;
                stateTimer = 0;
            }

            if (g_TitleInput.IsPressedScrolling(0x20))
            {
                g_SoundPlayer.PlaySoundByIdx(12, 0);
                keyboardSelection += TitleNativeNameKeyboard::Columns;
                if (keyboardSelection >= TitleNativeNameKeyboard::KeyCount)
                    keyboardSelection -= TitleNativeNameKeyboard::KeyCount;
                if (keyboardSelection == TitleNativeNameKeyboard::GapKey)
                    keyboardSelection = 13;
                stateTimer = 0;
            }

            if (g_TitleInput.IsPressedScrolling(0x40))
            {
                g_SoundPlayer.PlaySoundByIdx(12, 0);
                if (keyboardSelection % TitleNativeNameKeyboard::Columns == 0)
                    keyboardSelection += TitleNativeNameKeyboard::ConfirmColumn;
                else
                {
                    keyboardSelection--;
                    if (keyboardSelection == TitleNativeNameKeyboard::GapKey)
                        keyboardSelection = 92;
                }
                stateTimer = 0;
            }

            if (g_TitleInput.IsPressedScrolling(0x80))
            {
                g_SoundPlayer.PlaySoundByIdx(12, 0);
                if (keyboardSelection % TitleNativeNameKeyboard::Columns == TitleNativeNameKeyboard::ConfirmColumn)
                    keyboardSelection -= TitleNativeNameKeyboard::ConfirmColumn;
                else
                {
                    keyboardSelection++;
                    if (keyboardSelection == TitleNativeNameKeyboard::GapKey)
                        keyboardSelection = 94;
                }
                stateTimer = 0;
            }
        }

        if (g_TitleInputFlags & 0x1001)
        {
            if (keyboardSelection == TitleNativeNameKeyboard::ConfirmKey)
            {
                SaveTitleScoreData();
                PlayMenuSound(11, 0);
                stateTimer = 0;
                ChangeCurrentScreen(15);
                if (nameSlotIndex < 5)
                {
                    strcpy(
                        g_ReplayName,
                        g_TitleNameTable[nameBankIndex][g_TitleNameTableIndex][nameSlotIndex].name);
                }
                return 1;
            }

            char value;
            if (keyboardSelection == TitleNativeNameKeyboard::SpaceKey)
                value = ' ';
            else
                value = g_TitleAlphabet[keyboardSelection];
            g_TitleNameTable[nameBankIndex][g_TitleNameTableIndex][nameSlotIndex].name[replayNameCursor] = value;
            PlayMenuSound(10, 0);
            if (replayNameCursor < 7)
                replayNameCursor++;
            else
                keyboardSelection = TitleNativeNameKeyboard::ConfirmKey;
            stateTimer = 0;
        }

        if (g_TitleInputFlags & 0xA)
        {
            PlayMenuSound(11, 0);
            if (replayNameCursor == 7 &&
                g_TitleNameTable[nameBankIndex][g_TitleNameTableIndex][nameSlotIndex].name[7] != ' ')
            {
                g_TitleNameTable[nameBankIndex][g_TitleNameTableIndex][nameSlotIndex].name[7] = ' ';
            }
            else if (replayNameCursor > 0)
            {
                replayNameCursor--;
                g_TitleNameTable[nameBankIndex][g_TitleNameTableIndex][nameSlotIndex].name[replayNameCursor] = ' ';
            }
            stateTimer = 0;
        }
        break;
    }

    stateTimer++;
    screenFrameCounter++;
    stateTimer2++;
    return 1;
}


int TitleScreenView::OnUpdateResult()
{
    enum { FullCharacterSlots = sizeof(g_PlayStatsRecord.characterUnlocked[0]) };
    switch (currentScreenState)
    {
    case 0:
        if (phaseTimer == 0)
        {
            if (g_AnmManager->LoadSurface(0, "title/result00.png"))
                return 0;

            g_AnmManager->SetInterruptArray(reinterpret_cast<AnmVm *>(vms), vmCount, 16);
            g_AnmManager->ExecuteScriptArray(reinterpret_cast<AnmVm *>(vms), vmCount);
            keyboardSelection = 0;
            nameBankIndex = 0;
            SetCharacterCursorInactive(g_TitleResultCharacterOrder[0], 60, FullCharacterSlots, 2);
            currentScreenState = 0;
            stateTimer = 0;
            currentHelpTextVm = 0;
            nameSlotIndex = 0;
        }

        if (phaseTimer == 8)
            currentScreenState = 1;
        break;

    case 1:
    {
        if (g_TitleResultUnlockStep < 3)
        {
            if (g_ReplayInputStates[2].word06 & 0x2000)
                g_TitleResultUnlockStep = static_cast<int>(
                    static_cast<unsigned int>(g_TitleResultUnlockStep) + 1u);
            else if (g_ReplayInputStates[2].word06)
                g_TitleResultUnlockStep = 0;
        }
        else if (g_TitleResultUnlockStep < 7)
        {
            if (g_ReplayInputStates[2].word06 & 0x400)
                g_TitleResultUnlockStep = static_cast<int>(
                    static_cast<unsigned int>(g_TitleResultUnlockStep) + 1u);
            else if (g_ReplayInputStates[2].word06)
                g_TitleResultUnlockStep = 0;
        }
        else if (g_TitleResultUnlockStep < 8)
        {
            if (g_ReplayInputStates[2].word06 & 0x200)
                g_TitleResultUnlockStep = static_cast<int>(
                    static_cast<unsigned int>(g_TitleResultUnlockStep) + 1u);
            else if (g_ReplayInputStates[2].word06)
                g_TitleResultUnlockStep = 0;
        }
        else if (g_TitleResultUnlockStep < 10)
        {
            if (g_ReplayInputStates[2].word06 & 0x4000)
                g_TitleResultUnlockStep = static_cast<int>(
                    static_cast<unsigned int>(g_TitleResultUnlockStep) + 1u);
            else if (g_ReplayInputStates[2].word06)
                g_TitleResultUnlockStep = 0;
        }

        if (g_TitleResultUnlockStep >= 10)
        {
            for (i32 i = 0; i < FullCharacterSlots; i++)
                g_PlayStatsRecord.characterUnlocked[0][(char)i] = 1;
            for (i32 i = 0; i <= 13; i++)
            {
                g_PlayStatsRecord.characterUnlocked[1][(char)i] = 1;
                g_PlayStatsRecord.characterUnlocked[2][(char)i] = 1;
            }
            g_SoundPlayer.PlaySoundByIdx(28, 0);
            g_TitleResultUnlockStep = 0;
        }

        i32 direction = MoveCursorHorizontal(14);
        if (direction != 0)
        {
            while (!g_PlayStatsRecord.characterUnlocked[1][keyboardSelection] &&
                   !g_PlayStatsRecord.characterUnlocked[2][keyboardSelection])
            {
                keyboardSelection = static_cast<int>(
                    static_cast<unsigned int>(keyboardSelection) +
                    static_cast<unsigned int>(direction));
                if (keyboardSelection < 0)
                    keyboardSelection = static_cast<int>(
                        static_cast<unsigned int>(keyboardSelection) +
                        static_cast<unsigned int>(FullCharacterSlots));
                if (keyboardSelection >= FullCharacterSlots)
                    keyboardSelection = static_cast<int>(
                        static_cast<unsigned int>(keyboardSelection) -
                        static_cast<unsigned int>(FullCharacterSlots));
            }

            // Use the stable character-table slot for the cursor argument
            // and retain its pre-call value for the selected result bank.
            char *characterSlot = &g_TitleResultCharacterOrder[keyboardSelection];
            char character = *characterSlot;
            SetCharacterCursorInactive(*characterSlot, 60, FullCharacterSlots, 2);
            nameBankIndex = character;
        }

        if (g_ReplayInputStates[2].word06 & 0xA)
        {
            PlayMenuSound(11, 0);
            stateTimer = 0;
            ChangeCurrentScreen(1);
            keyboardSelection = 4;
            return 1;
        }
        break;
    }
    }

    // Retail increments these three signed state counters as native
    // i386 DWORDs, wrapping at 32 bits before storing their signed values.
    stateTimer = static_cast<int>(static_cast<unsigned int>(stateTimer) + 1u);
    screenFrameCounter = static_cast<int>(
        static_cast<unsigned int>(screenFrameCounter) + 1u);
    phaseTimer = static_cast<int>(static_cast<unsigned int>(phaseTimer) + 1u);
    return 1;
}


int TitleScreenView::OnUpdateReplayMenu()
{
    char fullPath[1024];
    WIN32_FIND_DATAA findData;
    char path[64];
    i32 fileSize;
    HANDLE findHandle;

    switch (currentScreenState)
    {
    case 0:
        if (phaseTimer == 0)
        {
            g_TitleGameFlags &= ~8u;
            if (previousScreen != 11 && g_AnmManager->LoadSurface(0, "title/replay00.png"))
                return 0;

            g_AnmManager->SetInterruptArray(vms, vmCount, 22);
            keyboardSelection = 0;
            currentScreenState = 0;
            stateTimer = 0;
            uiAux = 0;
            memset(replays, 0, sizeof(replays));

            for (i32 replayIndex = 0; replayIndex < 25; replayIndex++)
            {
                sprintf(path, "./replay/th9_%.2d.rpy", replayIndex + 1);
                ReplayDataView *fileData =
                    (ReplayDataView *)FileSystem::OpenFile(path, &fileSize, 1);
                if (fileData != 0)
                {
                    ReplayDataView *loadedReplay =
                        ReplayManagerView::LoadReplayData(fileData, fileSize);
                    replayAllocations[replayIndex] = loadedReplay;
                    if (loadedReplay != 0)
                    {
                        replays[replayIndex] = *loadedReplay;
                        strcpy(replayPaths[replayIndex], path);
                    }
                }
            }

            _mkdir("replay");
            _chdir("replay");
            findHandle = FindFirstFileA("th9_ud????.rpy", &findData);
            if (findHandle != INVALID_HANDLE_VALUE)
            {
                for (i32 replayIndex = 0; replayIndex < 25; replayIndex++)
                {
                    strcpy(fullPath, "replay/");
                    strcat(fullPath, findData.cFileName);
                    ReplayDataView *fileData =
                        (ReplayDataView *)FileSystem::OpenFile(fullPath, &fileSize, 1);
                    if (fileData == 0)
                        continue;

                    ReplayDataView *loadedReplay =
                        ReplayManagerView::LoadReplayData(fileData, fileSize);
                    replayAllocations[25 + replayIndex] = loadedReplay;
                    if (loadedReplay != 0)
                    {
                        replays[25 + replayIndex] = *loadedReplay;
                        sprintf(replayPaths[25 + replayIndex], "./replay/%s", findData.cFileName);
                    }

                    if (!FindNextFileA(findHandle, &findData))
                        break;
                }
            }
            FindClose(findHandle);
            _chdir("../");
            replaySlotCount = 50;
            replayEnumerationResetState = 0;
        }

        if (phaseTimer >= 8)
        {
            currentScreenState = 1;
            stateTimer = 0;
        }
        break;

    case 1:
        if (g_TitleInput.IsPressedScrolling(0x10))
        {
            g_SoundPlayer.PlaySoundByIdx(12, 0);
            if (--keyboardSelection < 0)
                keyboardSelection += 50;
            stateTimer = 0;
        }
        if (g_TitleInput.IsPressedScrolling(0x20))
        {
            g_SoundPlayer.PlaySoundByIdx(12, 0);
            if (++keyboardSelection >= 50)
                keyboardSelection -= 50;
            stateTimer = 0;
        }
        if (g_TitleInput.IsPressedScrolling(0x40))
        {
            g_SoundPlayer.PlaySoundByIdx(12, 0);
            keyboardSelection -= 25;
            if (keyboardSelection < 0)
                keyboardSelection += 50;
            stateTimer = 0;
        }
        if (g_TitleInput.IsPressedScrolling(0x80))
        {
            g_SoundPlayer.PlaySoundByIdx(12, 0);
            keyboardSelection += 25;
            if (keyboardSelection >= 50)
                keyboardSelection -= 50;
            stateTimer = 0;
        }

        if (static_cast<u8>(g_TitleInputFlags) & 0xA)
        {
            PlayMenuSound(11, 0);
            stateTimer = 0;
            ChangeCurrentScreen(1);
            keyboardSelection = 3;
            for (i32 replayIndex = 0; replayIndex < 50; replayIndex++)
            {
                if (replayAllocations[replayIndex] != 0)
                    free(replayAllocations[replayIndex]);
                replayAllocations[replayIndex] = 0;
            }
            return 1;
        }

        if (static_cast<u16>(g_TitleInputFlags) & 0x1001)
        {
            if (replays[keyboardSelection].magic != 0)
            {
                PlayMenuSound(10, 0);
                selectedReplay = keyboardSelection;
                currentScreenState = 2;
                stateTimer = 0;
                keyboardSelection = 0;
                phaseTimer = 0;
                while (replays[selectedReplay].frameStart[0][keyboardSelection] == 0)
                    keyboardSelection++;
            }
            else
            {
                PlayMenuSound(39, 0);
            }
        }
        break;

    case 2:
    {
        i32 direction = MoveCursorVertical(10);
        if (direction != 0)
        {
            while (replays[selectedReplay].frameStart[0][keyboardSelection] == 0)
            {
                keyboardSelection += direction;
                if (keyboardSelection >= 10)
                    keyboardSelection -= 10;
                if (keyboardSelection < 0)
                    keyboardSelection += 10;
            }
        }

        if (static_cast<u8>(g_TitleInputFlags) & 0xA)
        {
            currentScreenState = 1;
            stateTimer = 0;
            keyboardSelection = selectedReplay;
        }

        if (static_cast<u16>(g_TitleInputFlags) & 0x1001)
        {
            g_GameSide0Value20 = side0CharacterCursor;
            g_GameSide1Value20 = 0;
            g_GameCurrentStage = keyboardSelection;
            g_TitleNameTableIndex = replays[selectedReplay].value0D7;
            strcpy(g_SelectedReplayPath, replayPaths[selectedReplay]);
            g_TitleGameFlags |= 8u;

            if (replays[selectedReplay].value1E4 == 0)
            {
                g_GameValueDF0 = 1;
                g_TitleGlobalMode = 2;
                g_GameMode = 0;
                g_GameValueDB8 = 0;
            }
            else if (replays[selectedReplay].value1E4 == 1)
            {
                g_GameValueDF0 = 1;
                g_TitleGlobalMode = 2;
                g_GameMode = 1;
                g_GameValueDB8 = 0;
            }
            else
            {
                if (replays[selectedReplay].value1E5 == 0)
                {
                    g_GameValueDF0 = 0;
                    g_GameValueDB8 = 0;
                    g_TitleGlobalMode = 2;
                    g_GameMode = 2;
                }
                else if (replays[selectedReplay].value1E5 == 1)
                {
                    g_GameValueDF0 = 1;
                    g_GameValueDB8 = 0;
                    g_TitleGlobalMode = 2;
                    g_GameMode = 2;
                }
                else if (replays[selectedReplay].value1E5 == 2)
                {
                    g_GameValueDF0 = 0;
                    g_GameValueDB8 = 1;
                    g_TitleGlobalMode = 2;
                    g_GameMode = 2;
                }
                else if (replays[selectedReplay].value1E5 == 3)
                {
                    g_GameValueDF0 = 1;
                    g_GameValueDB8 = 1;
                    g_TitleGlobalMode = 2;
                    g_GameMode = 2;
                }
                else if (replays[selectedReplay].value1E5 == 4)
                {
                    g_GameValueDF0 = 0;
                    g_GameValueDB8 = 0;
                    g_TitleGlobalMode = 2;
                    g_GameMode = 2;
                }
            }

            for (i32 replayIndex = 0; replayIndex < 50; replayIndex++)
            {
                if (replayAllocations[replayIndex] != 0)
                    free(replayAllocations[replayIndex]);
                replayAllocations[replayIndex] = 0;
            }
            g_TitleNameTableIndex = g_TitleDifficulty;
            return 0;
        }
        break;
    }
    }

    replayUiFrameCounter++;
    stateTimer++;
    phaseTimer++;
    return 1;
}


int TitleScreenView::UpdateReplaySave()
{
    char path[256];
    char source[64];
    i32 fileSize;
    ReplayDataView *loadedReplay;
    char *replayPath;
    i32 replayIndex;
    ReplayDataView *replayCursor;
    u32 inputFlags;

    switch (replaySaveState)
    {
    case 0:
        if (phaseTimer == 0)
        {
            if (g_TitleGameFlags & 0x2000)
            {
                stateTimer = 0;
                ChangeCurrentScreen(1);
                keyboardSelection = 0;
                return 1;
            }

            if (g_SupervisorNetworkState->active != 0)
            {
                for (replayIndex = 0; replayIndex < 1000; replayIndex++)
                {
                    sprintf(path, "./replay/th9_udn%.3d.rpy", replayIndex + 1);
                    if (!FileSystem::CheckIfFileAlreadyExists(path))
                        break;
                }

                if (replayIndex >= 1000)
                    replayIndex = 999;

                sprintf(path, "./replay/th9_udn%.3d.rpy", replayIndex + 1);
                ReplayManagerView::SaveReplay(g_ReplayManager, path, g_ReplayName);
                stateTimer = 0;
                ChangeCurrentScreen(1);
                keyboardSelection = 0;
                resultAction = 2;
                return 1;
            }

            if (g_AnmManager->LoadSurface(0, "title/replay00.png"))
                return 0;

            g_AnmManager->SetInterruptArray(vms, vmCount, 19);
            g_AnmManager->ExecuteScriptArray(vms, vmCount);
            keyboardSelection = 0;
            strcpy(replayName, g_ReplayName);
            replaySaveState = 0;
            stateTimer = 0;
            uiAux = 0;

            if (g_GameMode == 2)
                goto load_replays;

            transitionId = 125;
            transitionMode = 2;
            UpdateMenuSelection();
            _mkdir("replay");
        }

        if (phaseTimer == 8)
            replaySaveState = 1;
        break;

    case 1:
        if (MoveCursorHorizontal(2))
            UpdateMenuSelection();

        if (g_TitleInputFlags & 0xA)
        {
            if (keyboardSelection == 0)
            {
                PlayMenuSound(11, 0);
                keyboardSelection = 1;
                UpdateMenuSelection();
                break;
            }

        cancel_replay_save:
            PlayMenuSound(11, 0);
            stateTimer = 0;
            ChangeCurrentScreen(1);
            keyboardSelection = 0;
            strcpy(
                g_ReplayName,
                g_TitleNameTable[nameBankIndex][g_TitleNameTableIndex][nameSlotIndex].name);
            return 1;
        }

        if (g_TitleInputFlags & 0x1001)
        {
            if (keyboardSelection == 1)
                goto cancel_replay_save;

        load_replays:
            memset(replays, 0, 0x6018);
            for (replayIndex = 0, replayPath = replayPaths[0], replayCursor = replays;
                 replayIndex < 25; replayIndex++, replayCursor++, replayPath += 0x200)
            {
                sprintf(source, "./replay/th9_%.2d.rpy", replayIndex + 1);
                void *fileData = FileSystem::OpenFile(source, &fileSize, 1);
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
            }
            replaySaveState = 2;
            phaseTimer = 0;
            keyboardSelection = 0;
            g_AnmManager->SetInterruptArray(vms, vmCount, 22);
            g_AnmManager->ExecuteScriptArray(vms, vmCount);
        }
        break;

    case 2:
        if (phaseTimer < 6)
            break;

        if (MoveCursorVertical(25))
            UpdateMenuSelection();

        if (g_TitleInputFlags & 0xA)
        {
            PlayMenuSound(11, 0);
            keyboardSelection = 0;
            phaseTimer = 0;
            replaySaveState = 1;
            g_AnmManager->SetInterruptArray(vms, vmCount, 19);
            g_AnmManager->ExecuteScriptArray(vms, vmCount);
            transitionId = 125;
            transitionMode = 2;
            UpdateMenuSelection();
            break;
        }

        if (g_TitleInputFlags & 0x1001)
        {
            PlayMenuSound(10, 0);
            selectedReplay = keyboardSelection;
            keyboardSelection = 0;
            if (replays[selectedReplay].magic != 0)
            {
                replaySaveState = 3;
                g_AnmManager->SetInterruptArray(vms, vmCount, 21);
                g_AnmManager->ExecuteScriptArray(vms, vmCount);
                transitionId = 129;
                transitionMode = 2;
                UpdateMenuSelection();
            }
            else
            {
                keyboardSelection = TitleNativeNameKeyboard::ConfirmKey;
                replaySaveState = 4;
                g_AnmManager->SetInterruptArray(vms, vmCount, 23);
                g_AnmManager->ExecuteScriptArray(vms, vmCount);
            }
            phaseTimer = 0;
            strcpy(replayName, g_ReplayName);
        }
        break;

    case 3:
        if (phaseTimer < 10)
            break;

        if (MoveCursorHorizontal(2))
            UpdateMenuSelection();

        inputFlags = g_TitleInputFlags;
        if (inputFlags & 0xA)
            goto cancel_replay_overwrite;

        if (inputFlags & 0x1001)
        {
            if (keyboardSelection == 1)
            {
            cancel_replay_overwrite:
                PlayMenuSound(11, 0);
                replaySaveState = 2;
                keyboardSelection = 0;
                g_AnmManager->SetInterruptArray(vms, vmCount, 22);
                g_AnmManager->ExecuteScriptArray(vms, vmCount);
            }
            else
            {
                PlayMenuSound(10, 0);
                keyboardSelection = TitleNativeNameKeyboard::ConfirmKey;
                replaySaveState = 4;
                g_AnmManager->SetInterruptArray(vms, vmCount, 23);
                g_AnmManager->ExecuteScriptArray(vms, vmCount);
            }
        }
        break;

    case 4:
        if (phaseTimer < 10)
            break;

        if (g_TitleInput.IsPressedScrolling(0x10))
        {
            g_SoundPlayer.PlaySoundByIdx(12, 0);
            keyboardSelection -= TitleNativeNameKeyboard::Columns;
            if (keyboardSelection < 0)
                keyboardSelection += TitleNativeNameKeyboard::KeyCount;
            if (keyboardSelection == TitleNativeNameKeyboard::GapKey)
                keyboardSelection = 77;
            stateTimer = 0;
        }

        if (g_TitleInput.IsPressedScrolling(0x20))
        {
            g_SoundPlayer.PlaySoundByIdx(12, 0);
            keyboardSelection += TitleNativeNameKeyboard::Columns;
            if (keyboardSelection >= TitleNativeNameKeyboard::KeyCount)
                keyboardSelection -= TitleNativeNameKeyboard::KeyCount;
            if (keyboardSelection == TitleNativeNameKeyboard::GapKey)
                keyboardSelection = 13;
            stateTimer = 0;
        }

        if (g_TitleInput.IsPressedScrolling(0x40))
        {
            g_SoundPlayer.PlaySoundByIdx(12, 0);
            if (keyboardSelection % TitleNativeNameKeyboard::Columns == 0)
                keyboardSelection += TitleNativeNameKeyboard::ConfirmColumn;
            else
            {
                keyboardSelection--;
                if (keyboardSelection == TitleNativeNameKeyboard::GapKey)
                    keyboardSelection = 92;
            }
            stateTimer = 0;
        }

        if (g_TitleInput.IsPressedScrolling(0x80))
        {
            g_SoundPlayer.PlaySoundByIdx(12, 0);
            if (keyboardSelection % TitleNativeNameKeyboard::Columns == TitleNativeNameKeyboard::ConfirmColumn)
                keyboardSelection -= TitleNativeNameKeyboard::ConfirmColumn;
            else
            {
                keyboardSelection++;
                if (keyboardSelection == TitleNativeNameKeyboard::GapKey)
                    keyboardSelection = 94;
            }
            stateTimer = 0;
        }

        if (g_TitleInputFlags & 0x1001)
        {
            if (keyboardSelection == TitleNativeNameKeyboard::ConfirmKey)
            {
                PlayMenuSound(10, 0);
                sprintf(path, "replay/th9_%.2d.rpy", selectedReplay + 1);
                ReplayManagerView::SaveReplay(g_ReplayManager, path, replayName);
                stateTimer = 0;
                ChangeCurrentScreen(1);
                keyboardSelection = 0;
                strcpy(g_ReplayName, replayName);
                return 1;
            }

            char value;
            if (keyboardSelection == TitleNativeNameKeyboard::SpaceKey)
                value = ' ';
            else
                value = g_TitleAlphabet[keyboardSelection];
            replayName[replayNameCursor] = value;
            PlayMenuSound(10, 0);
            if (replayNameCursor < 7)
                replayNameCursor++;
            else
                keyboardSelection = TitleNativeNameKeyboard::ConfirmKey;
            stateTimer = 0;
        }

        if (g_TitleInputFlags & 0xA)
        {
            PlayMenuSound(11, 0);
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
                g_AnmManager->SetInterruptArray(vms, vmCount, 22);
                g_AnmManager->ExecuteScriptArray(vms, vmCount);
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


int TitleScreenView::OnUpdateStartMenu()
{
    switch (currentScreenState)
    {
    case 0:
        if (stateTimer2 == 0)
        {
            if (previousScreen == 0 || previousScreen == 14 || previousScreen == 12 || previousScreen == 15)
                g_TitleSupervisor.PlayMusic(0, 0);

            if (previousScreen == 0 || previousScreen == 13 || previousScreen == 14 ||
                previousScreen == 11 || previousScreen == 15 || previousScreen == 12 ||
                previousScreen == 2 || previousScreen == 4 || previousScreen == 7 || previousScreen == 6)
            {
                if (g_AnmManager->LoadSurface(0, "title/title00.png"))
                    return 0;
            }

            g_ReplayManager->Release();
            g_ReplayManager = 0;

            if (vmCount == 0)
            {
                vmCount = 223;
                vms = new AnmVmView[vmCount];
                titleAnm->ExecuteAnmIdxArray(vms, 0, vmCount);
            }

            g_AnmManager->SetInterruptArray(vms, vmCount, 2);
            g_AnmManager->ExecuteScriptArray(vms, vmCount);
            if (!g_TitleLockedMenuItem)
                vms[2].color1 = 0x30908040;

            if (resumeState != 0)
            {
                g_TitleGameFlags &= ~8u;
                keyboardSelection = g_TitleModeSelection;
                ChangeCurrentScreen(6);
                g_AnmManager->SetInterruptArray(vms, vmCount, 5);
                return 1;
            }

            menuVmStart = 1;
            menuItemCount = 8;
            UpdateMenuSelection();
            if (g_TitleGameFlags & 8)
            {
                ChangeCurrentScreen(11);
                g_AnmManager->SetInterruptArray(vms, vmCount, 5);
                return 1;
            }
            g_TitleGameFlags &= ~2u;
        }

        if ((u32)stateTimer2 < 8)
        {
            DrawTitleHelpText(static_cast<TitleAnmManagerView *>(g_AnmManager), &embeddedVms[stateTimer2], 0xFFF0E0, 0x300000,
                              g_TitleHelpText[stateTimer2]);
            stateTimer2++;
            return 1;
        }

        previousCursor = -1;
        stateTimer2 = 0;
        stateTimer = 0;
        currentScreenState = 1;
        idleFrames = 0;

    case 1:
        {
            i32 moved = MoveCursorVertical(8);
            if (moved != 0)
            {
                if (!g_TitleLockedMenuItem && keyboardSelection == 1)
                    keyboardSelection = moved + 1;
                UpdateMenuSelection();
            }
        }

        if (previousCursor != keyboardSelection)
        {
            currentHelpTextVm = &embeddedVms[keyboardSelection];
            currentHelpTextVm->pendingInterrupt = 1;
        }
        previousCursor = keyboardSelection;

        if (!g_TitleLockedMenuItem)
            vms[2].color1 = 0xB0B0B040;

        if (stateTimer2 >= 10)
        {
            if (g_TitleInput.currentInput != 0)
                idleFrames = 0;

            idleFrames++;
            if (idleFrames > 1500)
            {
                g_TitleGameFlags |= 2u;
                strcpy(g_SelectedReplayPath, g_DemoReplayPaths[g_DemoReplayIndex]);
                g_DemoReplayIndex++;
                g_DemoReplayIndex %= 3;
                g_TitleGameFlags |= 8u;
                g_GameCurrentStage = 9;
                g_GameConfigSelector = 3;
                g_GameValueDB8 = 0;
                g_GameValueDF0 = 1;
                g_TitleGlobalMode = 2;
                g_GameMode = 2;
                return 0;
            }

            if (g_TitleInputFlags & 0x1001)
            {
                switch (keyboardSelection)
                {
                case 0:
                    g_TitleGameFlags &= ~8u;
                    PlayMenuSound(10, 0);
                    if (keyboardSelection >= 4)
                        keyboardSelection = 2;
                    ChangeCurrentScreen(2);
                    g_AnmManager->SetInterruptArray(vms, vmCount, 5);
                    g_TitleTransitionCounter0 = 10;
                    g_TitleTransitionCounter1 = 10;
                    return 1;
                case 1:
                    if (g_TitleLockedMenuItem)
                    {
                        g_TitleGameFlags &= ~8u;
                        PlayMenuSound(10, 0);
                        if (keyboardSelection >= 4)
                            keyboardSelection = 2;
                        ChangeCurrentScreen(4);
                        g_AnmManager->SetInterruptArray(vms, vmCount, 5);
                        g_TitleTransitionCounter0 = 10;
                        g_TitleTransitionCounter1 = 10;
                        return 1;
                    }
                    PlayMenuSound(39, 0);
                    return 1;
                case 2:
                    g_TitleGameFlags &= ~8u;
                    PlayMenuSound(10, 0);
                    keyboardSelection = g_TitleModeSelection;
                    ChangeCurrentScreen(6);
                    g_AnmManager->SetInterruptArray(vms, vmCount, 5);
                    g_TitleTransitionCounter0 = 10;
                    g_TitleTransitionCounter1 = 10;
                    return 1;
                case 3:
                    PlayMenuSound(10, 0);
                    ChangeCurrentScreen(11);
                    g_AnmManager->SetInterruptArray(vms, vmCount, 5);
                    currentHelpTextVm->pendingInterrupt = 2;
                    return 1;
                case 5:
                    PlayMenuSound(10, 0);
                    ChangeCurrentScreen(12);
                    g_AnmManager->SetInterruptArray(vms, vmCount, 5);
                    currentHelpTextVm->pendingInterrupt = 2;
                    return 1;
                case 4:
                    PlayMenuSound(10, 0);
                    ChangeCurrentScreen(13);
                    g_AnmManager->SetInterruptArray(vms, vmCount, 5);
                    return 1;
                case 6:
                    PlayMenuSound(10, 0);
                    currentScreenState = 0;
                    keyboardSelection = 0;
                    stateTimer2 = 0;
                    stateTimer = 0;
                    OnUpdateOptions();
                    SetMenuSelectionSprites(g_OptionA, 18, 5);
                    SetMenuSelectionSprites(g_OptionB, 21, 2);
                    SetMenuSelectionSprites(g_OptionC, 23, 3);
                    SetMenuSelectionSprites(g_OptionD, 34, 2);
                    currentScreenState = 3;
                    stateTimer = 0;
                    optionResetCounter = 0;
                    keyboardSelection = 0;
                    break;
                case 7:
                    currentScreenState = 2;
                    stateTimer = 0;
                    g_AnmManager->SetInterruptArray(vms, vmCount, 1);
                    if (g_OptionC == 2)
                        g_TitleMidiOutput->PlayFile(30);
                    break;
                }
            }

            if (g_TitleInputFlags & 0xA)
            {
                titleAnm->SetSprite(
                    &vms[keyboardSelection + 1],
                    vms[keyboardSelection + 1].baseSpriteIndex + 1);
                keyboardSelection = 7;
                titleAnm->SetSprite(
                    &vms[keyboardSelection + 1],
                    vms[keyboardSelection + 1].baseSpriteIndex);
                PlayMenuSound(11, 0);
            }
        }
        break;

    case 2:
        if (stateTimer >= 60)
        {
            delete[] vms;
            vms = 0;
            vmCount = 0;
            stateTimer2 = 0;
            g_TitleGlobalMode = -1;
            return 0;
        }
        break;

    case 3:
        if (stateTimer >= 30)
        {
            ChangeCurrentScreen(9);
            keyboardSelection = 0;
            // Copy the live configuration through the existing 0xCC snapshot view.
            configSnapshot = *reinterpret_cast<const TitleConfigSnapshotView *>(
                &g_Supervisor.config388);
            return 1;
        }
        break;
    }

    screenFrameCounter++;
    stateTimer++;
    stateTimer2++;
    return 1;
}


int TitleScreenView::OnUpdateDifficultySelect()
{
    i32 oldScreen;

    switch (currentScreenState)
    {
    case 0:
        if (stateTimer2 == 0)
        {
            if (g_AnmManager->LoadSurface(0, "title/select00.png"))
                return 0;

            if (currentScreen == 4)
                g_AnmManager->SetInterruptArray(vms, vmCount, 25);
            else
                g_AnmManager->SetInterruptArray(vms, vmCount, 12);
            g_AnmManager->ExecuteScriptArray(vms, vmCount);

            if (currentScreen == 7)
            {
                vms[131 + g_TitleModeSelection].pendingInterrupt = 9;
                if (resumeState == 1)
                {
                    ChangeCurrentScreen(8);
                    switch (g_TitleModeSelection)
                    {
                    case 0:
                        UpdateScreen8Mode0();
                        break;
                    case 1:
                        UpdateScreen8Mode123();
                        break;
                    case 2:
                        UpdateScreen8Mode123();
                        break;
                    case 3:
                        UpdateScreen8Mode123();
                        break;
                    case 4:
                    default:
                        break;
                    }
                    vms[136 + g_TitleDifficulty].pendingInterrupt = 10;
                    resumeState = 0;
                    return 1;
                }
                if (resumeState == 2)
                {
                    UpdateScreen8Mode0();
                    vms[136 + g_TitleDifficulty].pendingInterrupt = 10;
                    resumeState = 0;
                    ChangeCurrentScreen(8);
                    return 1;
                }
            }

            keyboardSelection = g_TitleDifficulty;
            if (currentScreen == 4)
                keyboardSelection = 4;
            else if (keyboardSelection >= 4)
                keyboardSelection = 1;

            menuVmStart = 136;
            menuItemCount = 4;
            UpdateMenuSelection();
            unknown0C918 = 1;
            currentScreenState = 0;
            stateTimer = 0;
            currentHelpTextVm = 0;
        }
        if (stateTimer2 == 8)
            currentScreenState = 1;
        break;

    case 1:
        if (currentScreen != 4)
        {
            if (MoveCursorVertical(4))
                UpdateMenuSelection();
        }
        else if (g_TitleInputFlags & 0x30)
        {
            PlayMenuSound(12, 0);
        }

        if (g_TitleInputFlags & 0x1001)
        {
            g_TitleDifficulty = (u8)keyboardSelection;
            PlayMenuSound(10, 0);
            if (currentScreen == 7)
            {
                ChangeCurrentScreen(8);
                switch (g_TitleModeSelection)
                {
                case 0:
                    UpdateScreen8Mode0();
                    break;
                case 1:
                    UpdateScreen8Mode123();
                    break;
                case 2:
                    UpdateScreen8Mode123();
                    break;
                case 3:
                    UpdateScreen8Mode123();
                    break;
                case 4:
                    UpdateScreen8Mode0();
                    break;
                }
                vms[136 + g_TitleDifficulty].pendingInterrupt = 10;
                return 1;
            }

            if (currentScreen == 2)
                ChangeCurrentScreen(3);
            else
                ChangeCurrentScreen(5);
            OnUpdateCharacterSelect();
            vms[136 + g_TitleDifficulty].pendingInterrupt = 9;
            return 1;
        }

        if (g_TitleModeSelection == 4 && g_SupervisorNetworkState->active == 0)
        {
            PlayMenuSound(11, 0);
            stateTimer = 0;
            if (currentScreen == 7)
            {
                ChangeCurrentScreen(6);
                return 1;
            }
            g_AnmManager->SetInterruptArray(vms, vmCount, 6);
            currentScreenState = 3;
        }

        if (g_SupervisorNetworkState->active == 0 && (g_TitleInputFlags & 0xA))
        {
            g_TitleDifficulty = (u8)keyboardSelection;
            PlayMenuSound(11, 0);
            stateTimer = 0;
            if (currentScreen == 7)
            {
                ChangeCurrentScreen(6);
                return 1;
            }
            g_AnmManager->SetInterruptArray(vms, vmCount, 6);
            currentScreenState = 3;
        }
        break;

    case 3:
        if (stateTimer >= 20)
        {
            oldScreen = currentScreen;
            ChangeCurrentScreen(1);
            keyboardSelection = oldScreen != 2;
            return 1;
        }
        break;
    }

    stateTimer++;
    screenFrameCounter++;
    stateTimer2++;
    return 1;
}


int TitleScreenView::OnUpdateModeSelect()
{
    switch (currentScreenState)
    {
    case 0:
        if (stateTimer2 == 0)
        {
            if (previousScreen != 7 && g_AnmManager->LoadSurface(0, "title/select00.png"))
                return 0;

            g_AnmManager->SetInterruptArray(vms, vmCount, 11);
            g_AnmManager->ExecuteScriptArray(vms, vmCount);

            if (resumeState != 0)
            {
                ChangeCurrentScreen(7);
                OnUpdateDifficultySelect();
                vms[131 + g_TitleModeSelection].pendingInterrupt = 9;
                return 1;
            }

            keyboardSelection = g_TitleModeSelection;
            menuVmStart = 131;
            menuItemCount = 5;
            UpdateMenuSelection();
            unknown0C918 = 1;
            currentScreenState = 0;
            stateTimer = 0;
            currentHelpTextVm = 0;
        }

        if (stateTimer2 == 8)
            currentScreenState = 1;
        break;

    case 1:
        if (MoveCursorVertical(5))
            UpdateMenuSelection();

        {
            u32 inputFlags = g_TitleInputFlags;
            if (inputFlags & 0x1001)
            {
                g_TitleModeSelection = keyboardSelection;
                if (g_TitleModeSelection == 4)
                {
                    PrepareTitleMode4Network(g_SupervisorNetworkState);
                    ResetTitleMode4Supervisor(&g_TitleSupervisor);
                    g_TitleDifficulty = 1;
                    g_GameSide0Value20 = 0;
                    g_GameSide1Value20 = 1;
                }

                PlayMenuSound(10, 0);
                ChangeCurrentScreen(7);
                OnUpdateDifficultySelect();
                vms[131 + g_TitleModeSelection].pendingInterrupt = 9;
                return 1;
            }

            if (inputFlags & 0xA)
            {
                g_TitleModeSelection = keyboardSelection;
                PlayMenuSound(11, 0);
                currentScreenState = 3;
                stateTimer = 0;
                g_AnmManager->SetInterruptArray(vms, vmCount, 6);
            }
        }
        break;

    case 3:
        if (stateTimer >= 20)
        {
            ChangeCurrentScreen(1);
            keyboardSelection = 2;
            return 1;
        }
        break;
    }

    stateTimer++;
    screenFrameCounter++;
    stateTimer2++;
    return 1;
}

int TitleSupervisorView::ClearRecordingFpsWarningState()
{
    this->recordingFpsWarning77C = 0;
    this->recordingFpsState784 = 0;
    this->recordingFpsState790 = 0;
    this->recordingFpsState788 = 0;
    this->recordingFpsState78C = 0;
    return 0;
}
