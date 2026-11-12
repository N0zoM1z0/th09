// TH09 screen-10 key/controller configuration owner.
// Layouts and behavior are target-bound reconstruction views; original
// identifier spelling remains unresolved except where exact callers fix it.

#include "GameConfiguration.hpp"
#include "AnmManager.hpp"
#include "AnmVmLifecycle.hpp"
#include "Supervisor.hpp"
#include "SoundPlayer.hpp"
#include "TitleInputView.hpp"

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

extern const char *g_TitleKeyConfigHelpText[10];
extern short g_TitleLastControllerButton;

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

// The real Supervisor settings occupy adjacent native byte addresses.
// The existing Title runtime has no added side-selection bounds gate.
#define TH09_KEY_NATIVE_BYTE(first, index) \
    (*reinterpret_cast<unsigned char *>( \
        reinterpret_cast<unsigned int>(first) + \
        static_cast<unsigned int>(index)))

#define TH09_KEY_NATIVE_HELP_VM(index) \
    (*reinterpret_cast<AnmVmView *>( \
        reinterpret_cast<unsigned int>(&helpTextVms[0]) + \
        static_cast<unsigned int>(index) * sizeof(helpTextVms[0])))

#define TH09_KEY_NATIVE_CONTROLLER_BYTE(index) \
    (*reinterpret_cast<unsigned char *>( \
        reinterpret_cast<unsigned int>(controllerState) + \
        static_cast<unsigned int>(index)))

// The key-config page's help pointers are real four-byte global slots;
// help VMs and paired digit VMs are canonical 0x2A4-byte ANM records.
// Retain the native Win32 byte-base arithmetic under the original /O1 ABI.
#define TH09_KEY_HELP_TEXT_NATIVE(index) \
    (*reinterpret_cast<const char *const *>( \
        reinterpret_cast<const u8 *>(&g_TitleKeyConfigHelpText[0]) + \
        static_cast<unsigned int>(index) * sizeof(g_TitleKeyConfigHelpText[0])))
#define TH09_KEY_HELP_VM_PTR(index) \
    reinterpret_cast<AnmVmView *>( \
        reinterpret_cast<u8 *>(&helpTextVms[0]) + \
        static_cast<unsigned int>(index) * sizeof(helpTextVms[0]))

i32 TitleScreenView::OnUpdateKeyConfig()
{
    AnmVmView *vmPair;
    i32 i;
    u32 helpIndex;
    unsigned char *controllerState;
    short keyToChange;

    switch (currentScreenState)
    {
    case 0:
        if (stateTimer2 == 0)
        {
            g_AnmManager->SetInterruptArray(vms, vmCount, 4);
            g_AnmManager->ExecuteScriptArray(vms, vmCount);
            menuVmStart = 37;
            menuItemCount = 10;
            UpdateMenuSelection();
            currentScreenState = 0;
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
                TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.sideFocusMode[0], keyConfigSide), 49);

            vmPair = &vms[50];
            SetTwoDigitVmValue(vmPair, keyMapping.sides[keyConfigSide].keys[0]);
            vmPair = reinterpret_cast<AnmVmView *>(
                reinterpret_cast<u8 *>(vmPair) + 2u * sizeof(*vmPair));
            SetTwoDigitVmValue(vmPair, keyMapping.sides[keyConfigSide].keys[1]);
            vmPair = reinterpret_cast<AnmVmView *>(
                reinterpret_cast<u8 *>(vmPair) + 2u * sizeof(*vmPair));
            SetTwoDigitVmValue(vmPair, keyMapping.sides[keyConfigSide].keys[2]);
            vmPair = reinterpret_cast<AnmVmView *>(
                reinterpret_cast<u8 *>(vmPair) + 2u * sizeof(*vmPair));
            SetTwoDigitVmValue(vmPair, keyMapping.sides[keyConfigSide].keys[8]);
            vmPair = reinterpret_cast<AnmVmView *>(
                reinterpret_cast<u8 *>(vmPair) + 2u * sizeof(*vmPair));
            SetTwoDigitVmValue(vmPair, keyMapping.sides[keyConfigSide].keys[3]);
            previousCursor = -1;
        }

        currentScreenState = 1;
        for (helpIndex = 0; helpIndex < 10; ++helpIndex)
        {
            DrawTitleHelpText(
                static_cast<TitleKeyAnmManagerView *>(g_AnmManager),
                TH09_KEY_HELP_VM_PTR(helpIndex),
                0x00FFF0E0u,
                0x300000,
                TH09_KEY_HELP_TEXT_NATIVE(helpIndex));
        }
    case 1:
        i = MoveCursorVertical(10);
    if (i != 0)
    {
        if (TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, keyConfigSide) >= 2)
        {
            while (keyboardSelection >= 3 && keyboardSelection <= 7)
                *reinterpret_cast<u32 *>(&keyboardSelection) += static_cast<u32>(i);
        }
        UpdateMenuSelection();
    }

    if (previousCursor != keyboardSelection)
    {
        currentHelpTextVm = &TH09_KEY_NATIVE_HELP_VM(keyboardSelection);
        currentHelpTextVm->pendingInterrupt = 1;
    }
    previousCursor = keyboardSelection;

    vmPair = &vms[50];
    SetTwoDigitVmValue(vmPair, keyMapping.sides[keyConfigSide].keys[0]);
    vmPair = reinterpret_cast<AnmVmView *>(
                reinterpret_cast<u8 *>(vmPair) + 2u * sizeof(*vmPair));
    SetTwoDigitVmValue(vmPair, keyMapping.sides[keyConfigSide].keys[1]);
    vmPair = reinterpret_cast<AnmVmView *>(
                reinterpret_cast<u8 *>(vmPair) + 2u * sizeof(*vmPair));
    SetTwoDigitVmValue(vmPair, keyMapping.sides[keyConfigSide].keys[2]);
    vmPair = reinterpret_cast<AnmVmView *>(
                reinterpret_cast<u8 *>(vmPair) + 2u * sizeof(*vmPair));
    SetTwoDigitVmValue(vmPair, keyMapping.sides[keyConfigSide].keys[8]);
    vmPair = reinterpret_cast<AnmVmView *>(
                reinterpret_cast<u8 *>(vmPair) + 2u * sizeof(*vmPair));
    SetTwoDigitVmValue(vmPair, keyMapping.sides[keyConfigSide].keys[3]);

    if (TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, keyConfigSide) < 2)
    {
        controllerState = Controller::GetControllerState(
            TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, keyConfigSide));

        for (keyToChange = 0; keyToChange < 32; ++keyToChange)
        {
            if ((TH09_KEY_NATIVE_CONTROLLER_BYTE(keyToChange) & 0x80) != 0)
                break;
        }

        if (keyToChange < 32 && g_TitleLastControllerButton != keyToChange)
        {
            switch (keyboardSelection)
            {
            case 3:
                SwapDuplicateCharacterIndex(
                    keyMapping.sides[keyConfigSide].keys,
                    keyToChange,
                    keyMapping.sides[keyConfigSide].keys[0],
                    1);
                keyMapping.sides[keyConfigSide].keys[0] = keyToChange;
                break;
            case 4:
                SwapDuplicateCharacterIndex(
                    keyMapping.sides[keyConfigSide].keys,
                    keyToChange,
                    keyMapping.sides[keyConfigSide].keys[1],
                    0);
                keyMapping.sides[keyConfigSide].keys[1] = keyToChange;
                break;
            case 5:
                SwapDuplicateCharacterIndex(
                    keyMapping.sides[keyConfigSide].keys,
                    keyToChange,
                    keyMapping.sides[keyConfigSide].keys[2],
                    1);
                keyMapping.sides[keyConfigSide].keys[2] = keyToChange;
                break;
            case 6:
                SwapDuplicateCharacterIndex(
                    keyMapping.sides[keyConfigSide].keys,
                    keyToChange,
                    keyMapping.sides[keyConfigSide].keys[8],
                    0);
                keyMapping.sides[keyConfigSide].keys[8] = keyToChange;
                break;
            case 7:
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
            PlayMenuSound(10, 0);
        }
afterControllerBinding:
        g_TitleLastControllerButton = keyToChange;
    }

    // Both side-toggle branches perform original i386 DWORD subtraction
    // directly on the signed +0xC920 selector; the input cursor step above
    // also adds in the same native 32-bit word before the signed 3..7 test.
    if (g_TitleInput.IsPressedScrolling(0x40))
    {
        switch (keyboardSelection)
        {
        case 0:
            *reinterpret_cast<u32 *>(&keyConfigSide) = 1u - static_cast<u32>(keyConfigSide);
            SetIndexedSpriteChoice(keyConfigSide, 47);
            SetIndexedSpriteChoice(
                TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.sideFocusMode[0], keyConfigSide), 49);
            SetIndexedSpriteChoice(
                TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, keyConfigSide), 48);
            SetIndexedSpriteChoice(
                TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.sideFocusMode[0], keyConfigSide), 42);
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
            g_SoundPlayer.PlaySoundByIdx(12, 0);
            break;
        case 1:
            do
            {
                if (TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, keyConfigSide) <= 0)
                    TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, keyConfigSide) += 5;
                --TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, keyConfigSide);
            }
            while (
                ((TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, static_cast<int>(1u - static_cast<unsigned int>(keyConfigSide))) == 1) &&
                 (TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, keyConfigSide) == 1)) ||
                ((TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, static_cast<int>(1u - static_cast<unsigned int>(keyConfigSide))) == 0) &&
                 (TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, keyConfigSide) == 0)) ||
                ((TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, static_cast<int>(1u - static_cast<unsigned int>(keyConfigSide))) == 2) &&
                 ((TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, keyConfigSide) == 2) ||
                  (TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, keyConfigSide) == 3) ||
                  (TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, keyConfigSide) == 4))) ||
                ((TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, static_cast<int>(1u - static_cast<unsigned int>(keyConfigSide))) == 3) &&
                 ((TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, keyConfigSide) == 2) ||
                  (TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, keyConfigSide) == 3))) ||
                ((TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, static_cast<int>(1u - static_cast<unsigned int>(keyConfigSide))) == 4) &&
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
            g_SoundPlayer.PlaySoundByIdx(12, 0);
            break;
        case 2:
            if (TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.sideFocusMode[0], keyConfigSide) <= 0)
                TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.sideFocusMode[0], keyConfigSide) += 2;
            --TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.sideFocusMode[0], keyConfigSide);
            SetIndexedSpriteChoice(
                TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.sideFocusMode[0], keyConfigSide), 49);
            SetIndexedSpriteChoice(
                TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.sideFocusMode[0], keyConfigSide), 42);
            g_SoundPlayer.PlaySoundByIdx(12, 0);
            break;
        default:
            break;
        }
    }

    if (g_TitleInput.IsPressedScrolling(0x80))
    {
        switch (keyboardSelection)
        {
        case 0:
            *reinterpret_cast<u32 *>(&keyConfigSide) = 1u - static_cast<u32>(keyConfigSide);
            SetIndexedSpriteChoice(keyConfigSide, 47);
            SetIndexedSpriteChoice(
                TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.sideFocusMode[0], keyConfigSide), 49);
            SetIndexedSpriteChoice(
                TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, keyConfigSide), 48);
            SetIndexedSpriteChoice(
                TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.sideFocusMode[0], keyConfigSide), 42);
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
            g_SoundPlayer.PlaySoundByIdx(12, 0);
            break;
        case 1:
            do
            {
                ++TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, keyConfigSide);
                if (TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, keyConfigSide) >= 5)
                    TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, keyConfigSide) -= 5;
            }
            while (
                ((TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, static_cast<int>(1u - static_cast<unsigned int>(keyConfigSide))) == 1) &&
                 (TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, keyConfigSide) == 1)) ||
                ((TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, static_cast<int>(1u - static_cast<unsigned int>(keyConfigSide))) == 0) &&
                 (TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, keyConfigSide) == 0)) ||
                ((TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, static_cast<int>(1u - static_cast<unsigned int>(keyConfigSide))) == 2) &&
                 ((TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, keyConfigSide) == 2) ||
                  (TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, keyConfigSide) == 3) ||
                  (TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, keyConfigSide) == 4))) ||
                ((TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, static_cast<int>(1u - static_cast<unsigned int>(keyConfigSide))) == 3) &&
                 ((TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, keyConfigSide) == 2) ||
                  (TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, keyConfigSide) == 3))) ||
                ((TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, static_cast<int>(1u - static_cast<unsigned int>(keyConfigSide))) == 4) &&
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
            g_SoundPlayer.PlaySoundByIdx(12, 0);
            break;
        case 2:
            ++TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.sideFocusMode[0], keyConfigSide);
            if (TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.sideFocusMode[0], keyConfigSide) >= 2)
                TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.sideFocusMode[0], keyConfigSide) -= 2;
            SetIndexedSpriteChoice(
                TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.sideFocusMode[0], keyConfigSide), 49);
            SetIndexedSpriteChoice(
                TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.sideFocusMode[0], keyConfigSide), 42);
            g_SoundPlayer.PlaySoundByIdx(12, 0);
            break;
        default:
            break;
        }
    }

    if ((g_TitleInput.word06 & 0x1001) != 0)
    {
        switch (keyboardSelection)
        {
        case 9:
        {
            PlayMenuSound(11, 0);
            ChangeCurrentScreen(9);
            memcpy(
                &g_Supervisor.config388.controllerMapping,
                &keyMapping.serialized,
                sizeof(keyMapping.serialized));
            g_ControllerMapping.primaryBindings[0] =
                g_Supervisor.config388.controllerMapping.bindings[0];
            g_ControllerMapping.secondaryBindings[0] =
                g_Supervisor.config388.controllerMapping.bindings[3];
            keyboardSelection = 7;
            return 1;
        }
        case 8:
        {
            PlayMenuSound(10, 0);
            memcpy(
                &keyMapping.serialized,
                &g_Supervisor.config388.controllerMapping,
                sizeof(keyMapping.serialized));
            TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.sideFocusMode[0], keyConfigSide) = 0;
            TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, keyConfigSide) =
                (u8)keyConfigSide;
            SetIndexedSpriteChoice(keyConfigSide, 47);
            SetIndexedSpriteChoice(
                TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.sideFocusMode[0], keyConfigSide), 49);
            SetIndexedSpriteChoice(
                TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.valueB7, keyConfigSide), 48);
            SetIndexedSpriteChoice(
                TH09_KEY_NATIVE_BYTE(&g_Supervisor.config388.sideFocusMode[0], keyConfigSide), 42);

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

    // The original title key-config update advances three signed runtime
    // frame/phase words with native wrapping i386 DWORD increments.
    ++*reinterpret_cast<u32 *>(&screenFrameCounter);
    ++*reinterpret_cast<u32 *>(&stateTimer);
    ++*reinterpret_cast<u32 *>(&stateTimer2);
    return 1;
}

#undef TH09_KEY_NATIVE_BYTE
#undef TH09_KEY_NATIVE_HELP_VM
#undef TH09_KEY_NATIVE_CONTROLLER_BYTE

#undef TH09_KEY_HELP_TEXT_NATIVE
#undef TH09_KEY_HELP_VM_PTR
