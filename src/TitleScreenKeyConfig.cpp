// TH09 screen-10 key/controller configuration owner.
// Layouts and behavior are target-bound reconstruction views; original
// identifier spelling remains unresolved except where exact callers fix it.

#include "GameConfiguration.hpp"
#include "AnmManager.hpp"
#include "AnmVmLifecycle.hpp"
#include "Supervisor.hpp"
#include "SoundPlayer.hpp"
#include "TitleInputView.hpp"
#include "TitleScreenStates.hpp"

#include <stddef.h>
#include <string.h>

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef int i32;

// The key/controller page's 0x2A4-byte VMs have the same native ANM
// allocation as other title menus; preserve only its source-facing ABI name.
struct AnmVmView : AnmVm {};
typedef char KeyConfigNativeAnmVmStorage[
    (sizeof(AnmVmView) == sizeof(AnmVm) &&
     offsetof(AnmVmView, color1Script) == 0x1F0 &&
     offsetof(AnmVmView, pendingInterrupt) == 0x1FE &&
     offsetof(AnmVmView, baseSpriteIndex) == 0x218) ? 1 : -1];

// The independently exact Title key-config update uses the same native
// AnmManager allocation as the menu and ANM script owners. Keep the nominal
// TitleKey type only for the not-yet-proven external help-text function ABI.
struct TitleKeyAnmManagerView : AnmManager {};
typedef char TitleKeyNativeManagerStorage[
    (sizeof(TitleKeyAnmManagerView) == sizeof(AnmManager) &&
     offsetof(TitleKeyAnmManagerView, captureSurfaceIndex08) == 0x08) ? 1 : -1];

// Key/controller configuration reads the real Supervisor's native
// GameConfiguration at physical offset +0x388, not a separate prefix view.
typedef char TitleKeyNativeSupervisorConfiguration[
    (offsetof(Supervisor, config388) == 0x388) ? 1 : -1];

struct TitleKeySideMapping
{
    short keys[9];
    u8 unknown12[0x24];
};
typedef char TitleKeySideMappingSizeIs36[
    (sizeof(TitleKeySideMapping) == 0x36) ? 1 : -1];

union TitleKeyMapping
{
    SerializedControllerMapping serialized;
    TitleKeySideMapping sides[2];
};
typedef char TitleKeyMappingSizeIs6C[(sizeof(TitleKeyMapping) == 0x6C) ? 1 : -1];

const char *g_TitleKeyConfigHelpText[10] = {
    "\x82\x50\x82\x6F\x81\x41\x82\x51\x82\x6F\x82\xF0\x91\x49\x91\xF0\x82\xB5\x82\xDC\x82\xB7", // [0] １Ｐ、２Ｐを選択します
    "PAD1,2:\x83\x70\x83\x62\x83\x68\x8E\x67\x97\x70\x81\x41" "Full,Left,Right:\x83\x4C\x81\x5B\x83\x7B\x81\x5B\x83\x68\x8E\x67\x97\x70", // [1] PAD1,2:パッド使用、Full,Left,Right:キーボード使用
    "\x83\x60\x83\x83\x81\x5B\x83\x57\x83\x7B\x83\x5E\x83\x93\x82\xCC\x93\xAE\x8D\xEC\x81\x41Slow:\x92\xE1\x91\xAC\x88\xDA\x93\xAE\x81\x41" "Charge:\x83\x60\x83\x83\x81\x5B\x83\x57", // [2] チャージボタンの動作、Slow:低速移動、Charge:チャージ
    "\x83\x56\x83\x87\x83\x62\x83\x67\x83\x7B\x83\x5E\x83\x93\x82\xF0\x90\xDD\x92\xE8\x82\xB5\x82\xDC\x82\xB7", // [3] ショットボタンを設定します
    "\x83\x7B\x83\x80\x83\x7B\x83\x5E\x83\x93\x82\xF0\x90\xDD\x92\xE8\x82\xB5\x82\xDC\x82\xB7", // [4] ボムボタンを設定します
    "\x92\xE1\x91\xAC\x81\x41\x8B\x79\x82\xD1\x83\x60\x83\x83\x81\x5B\x83\x57\x83\x7B\x83\x5E\x83\x93\x82\xF0\x90\xDD\x92\xE8\x82\xB5\x82\xDC\x82\xB7", // [5] 低速、及びチャージボタンを設定します
    "\x83\x81\x83\x62\x83\x5A\x81\x5B\x83\x57\x83\x58\x83\x4C\x83\x62\x83\x76\x83\x7B\x83\x5E\x83\x93\x82\xF0\x90\xDD\x92\xE8\x82\xB5\x82\xDC\x82\xB7", // [6] メッセージスキップボタンを設定します
    "\x83\x7C\x81\x5B\x83\x59\x83\x7B\x83\x5E\x83\x93\x82\xF0\x90\xDD\x92\xE8\x82\xB5\x82\xDC\x82\xB7", // [7] ポーズボタンを設定します
    "\x8F\x89\x8A\xFA\x90\xDD\x92\xE8\x82\xC9\x96\xDF\x82\xB5\x82\xDC\x82\xB7", // [8] 初期設定に戻します
    "\x82\xA8\x82\xA8\x82\xE6\x82\xBB\x8F\x49\x97\xB9\x82\xB5\x82\xDC\x82\xB7", // [9] おおよそ終了します
};
short g_TitleLastControllerButton = 32;

namespace Controller
{
unsigned char *__fastcall GetControllerState(int deviceIndex);
}

extern void __cdecl DrawTitleHelpText(
    TitleKeyAnmManagerView *manager,
    AnmVmView *vm,
    u32 color,
    i32 width,
    const char *text);

struct TitleScreenView
{
    i32 keyboardSelection;
    u8 unknown004[0x20];
    i32 previousCursor;
    i32 currentScreenState;
    i32 stateTimer;
    u8 unknown030[0xC8E0];
    i32 screenFrameCounter;
    u8 unknownC914[4];
    i32 keyConfigUiReady;
    u8 unknownC91C[4];
    i32 keyConfigSide;
    u8 unknownC924[0x526C];
    void *titleAnm;
    void *resultAnm;
    AnmVmView *vms;
    AnmVmView *currentHelpTextVm;
    AnmVmView helpTextVms[57];
    i32 vmCount;
    i32 currentScreen;
    i32 registrationContext;
    i32 stateTimer2;
    i32 idleFrames;
    i32 chainState;
    i32 resumeState;
    i32 menuVmStart;
    i32 menuItemCount;
    void *calcChain;
    void *drawChain;
    TitleKeyMapping keyMapping;
    u8 unknown1B2BC[0xCC];

    i32 MoveCursorVertical(i32 count);
    void UpdateMenuSelection();
    i32 SetTwoDigitVmValue(AnmVmView *digitVms, short value);
    void SetIndexedSpriteChoice(i32 choice, i32 vmIndex);
    i32 PlayMenuSound(i32 soundId, i32 unused);
    i32 ChangeCurrentScreen(i32 screen);
    short *SwapDuplicateCharacterIndex(
        short *slots, short incoming, short replacement, int unusedMode);
    i32 OnUpdateKeyConfig();
};

typedef char KeyConfigSideAtC920[
    (offsetof(TitleScreenView, keyConfigSide) == 0xC920) ? 1 : -1];
typedef char KeyConfigVmsAt11B98[
    (offsetof(TitleScreenView, vms) == 0x11B98) ? 1 : -1];
typedef char KeyConfigHelpAt11BA0[
    (offsetof(TitleScreenView, helpTextVms) == 0x11BA0) ? 1 : -1];
typedef char KeyConfigMappingAt1B250[
    (offsetof(TitleScreenView, keyMapping) == 0x1B250) ? 1 : -1];


short *TitleScreenView::SwapDuplicateCharacterIndex(
    short *slots, short incoming, short replacement, int unusedMode)
{
    if (slots[0] == incoming)
        slots[0] = replacement;
    if (slots[1] == incoming)
        slots[1] = replacement;
    if (slots[2] == incoming)
        slots[2] = replacement;
    if (slots[4] == incoming)
        slots[4] = replacement;
    if (slots[5] == incoming)
        slots[5] = replacement;
    if (slots[6] == incoming)
        slots[6] = replacement;
    if (slots[7] == incoming)
        slots[7] = replacement;
    if (slots[3] == incoming)
        slots[3] = replacement;
    if (slots[8] == incoming)
        slots[8] = replacement;
    return slots;
}

// Focus mode uses the native two-side configuration array directly.
// Adjacent device-mode bytes valueB7/valueB8 still use their original
// raw address expression: a new C++ array union changes four bytes in
// the exact 2,803-byte owner and therefore is not adopted.
#define TH09_KEY_NATIVE_BYTE(first, index) \
    (*reinterpret_cast<unsigned char *>( \
        reinterpret_cast<unsigned int>(first) + \
        static_cast<unsigned int>(index)))

// Native help-text VMs, controller state bytes, help strings, and
// the two serialized input-mapping rows are real C++ arrays.

i32 TitleScreenView::OnUpdateKeyConfig()
{
    AnmVmView *vmPair;
    i32 i;
    u32 helpIndex;
    unsigned char *controllerState;
    short keyToChange;

    switch (currentScreenState)
    {
    case TITLE_KEYCONFIG_STATE_INIT:
        if (stateTimer2 == 0)
        {
            g_AnmManager->SetInterruptArray(vms, vmCount, 4);
            g_AnmManager->ExecuteScriptArray(vms, vmCount);
            menuVmStart = 37;
            menuItemCount = 10;
            UpdateMenuSelection();
            currentScreenState = TITLE_KEYCONFIG_STATE_INIT;
            stateTimer = 0;
            keyConfigUiReady = 1;
            memcpy(
                &keyMapping.serialized,
                &g_Supervisor.config388.controllerMapping,
                sizeof(keyMapping.serialized));

            SetIndexedSpriteChoice(keyConfigSide, 47);
            SetIndexedSpriteChoice(
                TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, keyConfigSide), 48);
            SetIndexedSpriteChoice(
                g_Supervisor.config388.sideFocusMode[keyConfigSide], 49);

            vmPair = &vms[50];
            SetTwoDigitVmValue(vmPair, keyMapping.sides[keyConfigSide].keys[0]);
            vmPair += 2;
            SetTwoDigitVmValue(vmPair, keyMapping.sides[keyConfigSide].keys[1]);
            vmPair += 2;
            SetTwoDigitVmValue(vmPair, keyMapping.sides[keyConfigSide].keys[2]);
            vmPair += 2;
            SetTwoDigitVmValue(vmPair, keyMapping.sides[keyConfigSide].keys[8]);
            vmPair += 2;
            SetTwoDigitVmValue(vmPair, keyMapping.sides[keyConfigSide].keys[3]);
            previousCursor = -1;
        }

        currentScreenState = TITLE_KEYCONFIG_STATE_ACTIVE;
        for (helpIndex = 0; helpIndex < 10; ++helpIndex)
        {
            DrawTitleHelpText(
                static_cast<TitleKeyAnmManagerView *>(g_AnmManager),
                &helpTextVms[helpIndex],
                0x00FFF0E0u,
                0x300000,
                g_TitleKeyConfigHelpText[helpIndex]);
        }
    case TITLE_KEYCONFIG_STATE_ACTIVE:
        i = MoveCursorVertical(10);
    if (i != 0)
    {
        if (TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, keyConfigSide) >= 2)
        {
            while (keyboardSelection >= 3 && keyboardSelection <= 7)
                keyboardSelection += i;
        }
        UpdateMenuSelection();
    }

    if (previousCursor != keyboardSelection)
    {
        currentHelpTextVm = &helpTextVms[keyboardSelection];
        currentHelpTextVm->pendingInterrupt = 1;
    }
    previousCursor = keyboardSelection;

    vmPair = &vms[50];
    SetTwoDigitVmValue(vmPair, keyMapping.sides[keyConfigSide].keys[0]);
    vmPair += 2;
    SetTwoDigitVmValue(vmPair, keyMapping.sides[keyConfigSide].keys[1]);
    vmPair += 2;
    SetTwoDigitVmValue(vmPair, keyMapping.sides[keyConfigSide].keys[2]);
    vmPair += 2;
    SetTwoDigitVmValue(vmPair, keyMapping.sides[keyConfigSide].keys[8]);
    vmPair += 2;
    SetTwoDigitVmValue(vmPair, keyMapping.sides[keyConfigSide].keys[3]);

    if (TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, keyConfigSide) < 2)
    {
        controllerState = Controller::GetControllerState(
            TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, keyConfigSide));

        for (keyToChange = 0; keyToChange < 32; ++keyToChange)
        {
            if ((controllerState[keyToChange] & 0x80) != 0)
                break;
        }

        if (keyToChange < 32 && g_TitleLastControllerButton != keyToChange)
        {
            switch (keyboardSelection)
            {
            case TITLE_KEYCONFIG_ITEM_SHOT:
                SwapDuplicateCharacterIndex(
                    keyMapping.sides[keyConfigSide].keys,
                    keyToChange,
                    keyMapping.sides[keyConfigSide].keys[0],
                    1);
                keyMapping.sides[keyConfigSide].keys[0] = keyToChange;
                break;
            case TITLE_KEYCONFIG_ITEM_BOMB:
                SwapDuplicateCharacterIndex(
                    keyMapping.sides[keyConfigSide].keys,
                    keyToChange,
                    keyMapping.sides[keyConfigSide].keys[1],
                    0);
                keyMapping.sides[keyConfigSide].keys[1] = keyToChange;
                break;
            case TITLE_KEYCONFIG_ITEM_SLOW_OR_CHARGE:
                SwapDuplicateCharacterIndex(
                    keyMapping.sides[keyConfigSide].keys,
                    keyToChange,
                    keyMapping.sides[keyConfigSide].keys[2],
                    1);
                keyMapping.sides[keyConfigSide].keys[2] = keyToChange;
                break;
            case TITLE_KEYCONFIG_ITEM_MESSAGE_SKIP:
                SwapDuplicateCharacterIndex(
                    keyMapping.sides[keyConfigSide].keys,
                    keyToChange,
                    keyMapping.sides[keyConfigSide].keys[8],
                    0);
                keyMapping.sides[keyConfigSide].keys[8] = keyToChange;
                break;
            case TITLE_KEYCONFIG_ITEM_PAUSE:
                SwapDuplicateCharacterIndex(
                    keyMapping.sides[keyConfigSide].keys,
                    keyToChange,
                    keyMapping.sides[keyConfigSide].keys[3],
                    0);
                keyMapping.sides[keyConfigSide].keys[3] = keyToChange;
                break;
            default:
                goto afterControllerBinding;
            }
            PlayMenuSound(SOUND_SELECT, 0);
        }
afterControllerBinding:
        g_TitleLastControllerButton = keyToChange;
    }

    // Both side-toggle branches use the original unsigned word
    // arithmetic on the signed selector. The cursor update now uses
    // its natural C++ addition while preserving the 3..7 range gate.
    if (g_TitleInput.IsPressedScrolling(0x40))
    {
        switch (keyboardSelection)
        {
        case TITLE_KEYCONFIG_ITEM_PLAYER_SIDE:
            keyConfigSide = static_cast<int>(1u - static_cast<u32>(keyConfigSide));
            SetIndexedSpriteChoice(keyConfigSide, 47);
            SetIndexedSpriteChoice(
                g_Supervisor.config388.sideFocusMode[keyConfigSide], 49);
            SetIndexedSpriteChoice(
                TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, keyConfigSide), 48);
            SetIndexedSpriteChoice(
                g_Supervisor.config388.sideFocusMode[keyConfigSide], 42);
            if (TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, keyConfigSide) >= 2)
            {
                for (i = 0; i < 5; ++i)
                    vms[i + 40].color1Script.d3dColor = 0xFF606060u;
                for (i = 0; i < 10; ++i)
                    vms[i + 50].color1Script.d3dColor = 0xFF606060u;
            }
            else
            {
                for (i = 0; i < 5; ++i)
                    vms[i + 40].color1Script.d3dColor = 0xFFFFFFFFu;
                for (i = 0; i < 10; ++i)
                    vms[i + 50].color1Script.d3dColor = 0xFFFFFFFFu;
            }
            g_SoundPlayer.PlaySoundByIdx(SOUND_MOVE_MENU, 0);
            break;
        case TITLE_KEYCONFIG_ITEM_INPUT_DEVICE:
            do
            {
                if (TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, keyConfigSide) <= 0)
                    TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, keyConfigSide) += 5;
                --TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, keyConfigSide);
            }
            while (
                ((TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, static_cast<int>(1u - keyConfigSide)) == 1) &&
                 (TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, keyConfigSide) == 1)) ||
                ((TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, static_cast<int>(1u - keyConfigSide)) == 0) &&
                 (TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, keyConfigSide) == 0)) ||
                ((TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, static_cast<int>(1u - keyConfigSide)) == 2) &&
                 ((TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, keyConfigSide) == 2) ||
                  (TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, keyConfigSide) == 3) ||
                  (TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, keyConfigSide) == 4))) ||
                ((TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, static_cast<int>(1u - keyConfigSide)) == 3) &&
                 ((TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, keyConfigSide) == 2) ||
                  (TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, keyConfigSide) == 3))) ||
                ((TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, static_cast<int>(1u - keyConfigSide)) == 4) &&
                 ((TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, keyConfigSide) == 2) ||
                  (TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, keyConfigSide) == 4))));

            SetIndexedSpriteChoice(
                TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, keyConfigSide), 48);
            if (TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, keyConfigSide) >= 2)
            {
                for (i = 0; i < 5; ++i)
                    vms[i + 40].color1Script.d3dColor = 0xFF606060u;
                for (i = 0; i < 10; ++i)
                    vms[i + 50].color1Script.d3dColor = 0xFF606060u;
            }
            else
            {
                for (i = 0; i < 5; ++i)
                    vms[i + 40].color1Script.d3dColor = 0xFFFFFFFFu;
                for (i = 0; i < 10; ++i)
                    vms[i + 50].color1Script.d3dColor = 0xFFFFFFFFu;
            }
            g_SoundPlayer.PlaySoundByIdx(SOUND_MOVE_MENU, 0);
            break;
        case TITLE_KEYCONFIG_ITEM_CHARGE_MODE:
            if (g_Supervisor.config388.sideFocusMode[keyConfigSide] <= 0)
                g_Supervisor.config388.sideFocusMode[keyConfigSide] += 2;
            --g_Supervisor.config388.sideFocusMode[keyConfigSide];
            SetIndexedSpriteChoice(
                g_Supervisor.config388.sideFocusMode[keyConfigSide], 49);
            SetIndexedSpriteChoice(
                g_Supervisor.config388.sideFocusMode[keyConfigSide], 42);
            g_SoundPlayer.PlaySoundByIdx(SOUND_MOVE_MENU, 0);
            break;
        default:
            break;
        }
    }

    if (g_TitleInput.IsPressedScrolling(0x80))
    {
        switch (keyboardSelection)
        {
        case TITLE_KEYCONFIG_ITEM_PLAYER_SIDE:
            keyConfigSide = static_cast<int>(1u - static_cast<u32>(keyConfigSide));
            SetIndexedSpriteChoice(keyConfigSide, 47);
            SetIndexedSpriteChoice(
                g_Supervisor.config388.sideFocusMode[keyConfigSide], 49);
            SetIndexedSpriteChoice(
                TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, keyConfigSide), 48);
            SetIndexedSpriteChoice(
                g_Supervisor.config388.sideFocusMode[keyConfigSide], 42);
            if (TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, keyConfigSide) >= 2)
            {
                for (i = 0; i < 5; ++i)
                    vms[i + 40].color1Script.d3dColor = 0xFF606060u;
                for (i = 0; i < 10; ++i)
                    vms[i + 50].color1Script.d3dColor = 0xFF606060u;
            }
            else
            {
                for (i = 0; i < 5; ++i)
                    vms[i + 40].color1Script.d3dColor = 0xFFFFFFFFu;
                for (i = 0; i < 10; ++i)
                    vms[i + 50].color1Script.d3dColor = 0xFFFFFFFFu;
            }
            g_SoundPlayer.PlaySoundByIdx(SOUND_MOVE_MENU, 0);
            break;
        case TITLE_KEYCONFIG_ITEM_INPUT_DEVICE:
            do
            {
                ++TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, keyConfigSide);
                if (TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, keyConfigSide) >= 5)
                    TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, keyConfigSide) -= 5;
            }
            while (
                ((TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, static_cast<int>(1u - keyConfigSide)) == 1) &&
                 (TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, keyConfigSide) == 1)) ||
                ((TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, static_cast<int>(1u - keyConfigSide)) == 0) &&
                 (TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, keyConfigSide) == 0)) ||
                ((TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, static_cast<int>(1u - keyConfigSide)) == 2) &&
                 ((TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, keyConfigSide) == 2) ||
                  (TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, keyConfigSide) == 3) ||
                  (TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, keyConfigSide) == 4))) ||
                ((TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, static_cast<int>(1u - keyConfigSide)) == 3) &&
                 ((TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, keyConfigSide) == 2) ||
                  (TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, keyConfigSide) == 3))) ||
                ((TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, static_cast<int>(1u - keyConfigSide)) == 4) &&
                 ((TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, keyConfigSide) == 2) ||
                  (TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, keyConfigSide) == 4))));

            SetIndexedSpriteChoice(
                TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, keyConfigSide), 48);
            if (TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, keyConfigSide) >= 2)
            {
                for (i = 0; i < 5; ++i)
                    vms[i + 40].color1Script.d3dColor = 0xFF606060u;
                for (i = 0; i < 10; ++i)
                    vms[i + 50].color1Script.d3dColor = 0xFF606060u;
            }
            else
            {
                for (i = 0; i < 5; ++i)
                    vms[i + 40].color1Script.d3dColor = 0xFFFFFFFFu;
                for (i = 0; i < 10; ++i)
                    vms[i + 50].color1Script.d3dColor = 0xFFFFFFFFu;
            }
            g_SoundPlayer.PlaySoundByIdx(SOUND_MOVE_MENU, 0);
            break;
        case TITLE_KEYCONFIG_ITEM_CHARGE_MODE:
            ++g_Supervisor.config388.sideFocusMode[keyConfigSide];
            if (g_Supervisor.config388.sideFocusMode[keyConfigSide] >= 2)
                g_Supervisor.config388.sideFocusMode[keyConfigSide] -= 2;
            SetIndexedSpriteChoice(
                g_Supervisor.config388.sideFocusMode[keyConfigSide], 49);
            SetIndexedSpriteChoice(
                g_Supervisor.config388.sideFocusMode[keyConfigSide], 42);
            g_SoundPlayer.PlaySoundByIdx(SOUND_MOVE_MENU, 0);
            break;
        default:
            break;
        }
    }

    if ((g_TitleInput.word06 & 0x1001) != 0)
    {
        switch (keyboardSelection)
        {
        case TITLE_KEYCONFIG_ITEM_QUIT:
        {
            PlayMenuSound(SOUND_BACK, 0);
            ChangeCurrentScreen(TITLE_SCREEN_OPTIONS);
            memcpy(
                &g_Supervisor.config388.controllerMapping,
                &keyMapping.serialized,
                sizeof(keyMapping.serialized));
            g_ControllerMapping.primaryBindings[0] =
                g_Supervisor.config388.controllerMapping.bindings[0];
            g_ControllerMapping.secondaryBindings[0] =
                g_Supervisor.config388.controllerMapping.bindings[3];
            keyboardSelection = TITLE_OPTIONS_ITEM_KEY_CONFIG;
            return 1;
        }
        case TITLE_KEYCONFIG_ITEM_RESET_DEFAULTS:
        {
            PlayMenuSound(SOUND_SELECT, 0);
            memcpy(
                &keyMapping.serialized,
                &g_Supervisor.config388.controllerMapping,
                sizeof(keyMapping.serialized));
            g_Supervisor.config388.sideFocusMode[keyConfigSide] = 0;
            TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, keyConfigSide) =
                (u8)keyConfigSide;
            SetIndexedSpriteChoice(keyConfigSide, 47);
            SetIndexedSpriteChoice(
                g_Supervisor.config388.sideFocusMode[keyConfigSide], 49);
            SetIndexedSpriteChoice(
                TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, keyConfigSide), 48);
            SetIndexedSpriteChoice(
                g_Supervisor.config388.sideFocusMode[keyConfigSide], 42);

            if (TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, keyConfigSide) >= 2)
            {
                for (i = 0; i < 5; ++i)
                    vms[i + 40].color1Script.d3dColor = 0xFF606060u;
                for (i = 0; i < 10; ++i)
                    vms[i + 50].color1Script.d3dColor = 0xFF606060u;
            }
            else
            {
                for (i = 0; i < 5; ++i)
                    vms[i + 40].color1Script.d3dColor = 0xFFFFFFFFu;
                for (i = 0; i < 10; ++i)
                    vms[i + 50].color1Script.d3dColor = 0xFFFFFFFFu;
            }
            break;
        }
        default:
            break;
        }
    }
        break;
    }

    // The original title key-config update advances its three real
    // frame/phase counter members directly.
    ++screenFrameCounter;
    ++stateTimer;
    ++stateTimer2;
    return 1;
}

#undef TH09_KEY_NATIVE_BYTE
