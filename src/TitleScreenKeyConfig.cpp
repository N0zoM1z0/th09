// TH09 screen-10 key/controller configuration owner.
// Layouts and behavior are target-bound reconstruction views; original
// identifier spelling remains unresolved except where exact callers fix it.

#include "GameConfiguration.hpp"

#include <stddef.h>
#include <string.h>

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef int i32;

union TitleKeyColor
{
    u32 d3dColor;
    struct
    {
        u8 b;
        u8 g;
        u8 r;
        u8 a;
    };
};

struct AnmVmView
{
    u8 unknown000[0x1F0];
    TitleKeyColor color1;
    u8 unknown1F4[0x0A];
    u16 pendingInterrupt;
    u8 unknown200[0x18];
    short baseSpriteIndex;
    u8 unknown21A[0x8A];
};
typedef char KeyConfigAnmVmSizeIs2A4[(sizeof(AnmVmView) == 0x2A4) ? 1 : -1];

struct TitleKeyAnmManagerView
{
    void SetInterruptArray(AnmVmView *vms, i32 count, i32 interrupt);
    void ExecuteScriptArray(AnmVmView *vms, i32 count);
};

struct TitleKeyInputView
{
    u16 current00;
    u16 previous02;
    u16 repeatOutput04;
    u16 pressed06;
    u16 IsPressedScrolling(i32 mask);
};

struct TitleKeySoundView
{
    void PlaySoundByIdx(i32 soundId, i32 unused);
};

struct TitleKeySupervisorView
{
    u8 unknown000[0x388];
    GameConfiguration config388;
};

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

extern TitleKeyAnmManagerView *g_TitleAnmManager;
extern TitleKeyInputView g_TitleInput;
extern TitleKeySoundView g_SoundPlayer;
extern TitleKeySupervisorView g_Supervisor;
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
            g_TitleAnmManager->SetInterruptArray(vms, vmCount, 4);
            g_TitleAnmManager->ExecuteScriptArray(vms, vmCount);
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
                (&g_Supervisor.config388.valueB7)[keyConfigSide], 48);
            SetIndexedSpriteChoice(
                (&g_Supervisor.config388.valueB4)[keyConfigSide], 49);

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

        currentScreenState = 1;
        for (helpIndex = 0; helpIndex < 10; ++helpIndex)
        {
            DrawTitleHelpText(
                g_TitleAnmManager,
                &helpTextVms[helpIndex],
                0x00FFF0E0u,
                0x300000,
                g_TitleKeyConfigHelpText[helpIndex]);
        }
    case 1:
        i = MoveCursorVertical(10);
    if (i != 0)
    {
        if ((&g_Supervisor.config388.valueB7)[keyConfigSide] >= 2)
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

    if ((&g_Supervisor.config388.valueB7)[keyConfigSide] < 2)
    {
        controllerState = Controller::GetControllerState(
            (&g_Supervisor.config388.valueB7)[keyConfigSide]);

        for (keyToChange = 0; keyToChange < 32; ++keyToChange)
        {
            if ((controllerState[keyToChange] & 0x80) != 0)
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

    if (g_TitleInput.IsPressedScrolling(0x40))
    {
        switch (keyboardSelection)
        {
        case 0:
            keyConfigSide = 1 - keyConfigSide;
            SetIndexedSpriteChoice(keyConfigSide, 47);
            SetIndexedSpriteChoice(
                (&g_Supervisor.config388.valueB4)[keyConfigSide], 49);
            SetIndexedSpriteChoice(
                (&g_Supervisor.config388.valueB7)[keyConfigSide], 48);
            SetIndexedSpriteChoice(
                (&g_Supervisor.config388.valueB4)[keyConfigSide], 42);
            if ((&g_Supervisor.config388.valueB7)[keyConfigSide] >= 2)
            {
                for (i = 0; i < 5; ++i)
                    vms[i + 40].color1.d3dColor = 0xFF606060u;
                for (i = 0; i < 10; ++i)
                    vms[i + 50].color1.d3dColor = 0xFF606060u;
            }
            else
            {
                for (i = 0; i < 5; ++i)
                    vms[i + 40].color1.d3dColor = 0xFFFFFFFFu;
                for (i = 0; i < 10; ++i)
                    vms[i + 50].color1.d3dColor = 0xFFFFFFFFu;
            }
            break;
        case 1:
            do
            {
                if ((&g_Supervisor.config388.valueB7)[keyConfigSide] <= 0)
                    (&g_Supervisor.config388.valueB7)[keyConfigSide] += 5;
                --(&g_Supervisor.config388.valueB7)[keyConfigSide];
            }
            while (
                (((&g_Supervisor.config388.valueB7)[1 - keyConfigSide] == 1) &&
                 ((&g_Supervisor.config388.valueB7)[keyConfigSide] == 1)) ||
                (((&g_Supervisor.config388.valueB7)[1 - keyConfigSide] == 0) &&
                 ((&g_Supervisor.config388.valueB7)[keyConfigSide] == 0)) ||
                (((&g_Supervisor.config388.valueB7)[1 - keyConfigSide] == 2) &&
                 (((&g_Supervisor.config388.valueB7)[keyConfigSide] == 2) ||
                  ((&g_Supervisor.config388.valueB7)[keyConfigSide] == 3) ||
                  ((&g_Supervisor.config388.valueB7)[keyConfigSide] == 4))) ||
                (((&g_Supervisor.config388.valueB7)[1 - keyConfigSide] == 3) &&
                 (((&g_Supervisor.config388.valueB7)[keyConfigSide] == 2) ||
                  ((&g_Supervisor.config388.valueB7)[keyConfigSide] == 3))) ||
                (((&g_Supervisor.config388.valueB7)[1 - keyConfigSide] == 4) &&
                 (((&g_Supervisor.config388.valueB7)[keyConfigSide] == 2) ||
                  ((&g_Supervisor.config388.valueB7)[keyConfigSide] == 4))));

            SetIndexedSpriteChoice(
                (&g_Supervisor.config388.valueB7)[keyConfigSide], 48);
            if ((&g_Supervisor.config388.valueB7)[keyConfigSide] >= 2)
            {
                for (i = 0; i < 5; ++i)
                    vms[i + 40].color1.d3dColor = 0xFF606060u;
                for (i = 0; i < 10; ++i)
                    vms[i + 50].color1.d3dColor = 0xFF606060u;
            }
            else
            {
                for (i = 0; i < 5; ++i)
                    vms[i + 40].color1.d3dColor = 0xFFFFFFFFu;
                for (i = 0; i < 10; ++i)
                    vms[i + 50].color1.d3dColor = 0xFFFFFFFFu;
            }
            break;
        case 2:
            if ((&g_Supervisor.config388.valueB4)[keyConfigSide] <= 0)
                (&g_Supervisor.config388.valueB4)[keyConfigSide] += 2;
            --(&g_Supervisor.config388.valueB4)[keyConfigSide];
            SetIndexedSpriteChoice(
                (&g_Supervisor.config388.valueB4)[keyConfigSide], 49);
            SetIndexedSpriteChoice(
                (&g_Supervisor.config388.valueB4)[keyConfigSide], 42);
            break;
        default:
            goto afterLeft;
        }
        g_SoundPlayer.PlaySoundByIdx(12, 0);
    }
afterLeft:

    if (g_TitleInput.IsPressedScrolling(0x80))
    {
        switch (keyboardSelection)
        {
        case 0:
            keyConfigSide = 1 - keyConfigSide;
            SetIndexedSpriteChoice(keyConfigSide, 47);
            SetIndexedSpriteChoice(
                (&g_Supervisor.config388.valueB4)[keyConfigSide], 49);
            SetIndexedSpriteChoice(
                (&g_Supervisor.config388.valueB7)[keyConfigSide], 48);
            SetIndexedSpriteChoice(
                (&g_Supervisor.config388.valueB4)[keyConfigSide], 42);
            if ((&g_Supervisor.config388.valueB7)[keyConfigSide] >= 2)
            {
                for (i = 0; i < 5; ++i)
                    vms[i + 40].color1.d3dColor = 0xFF606060u;
                for (i = 0; i < 10; ++i)
                    vms[i + 50].color1.d3dColor = 0xFF606060u;
            }
            else
            {
                for (i = 0; i < 5; ++i)
                    vms[i + 40].color1.d3dColor = 0xFFFFFFFFu;
                for (i = 0; i < 10; ++i)
                    vms[i + 50].color1.d3dColor = 0xFFFFFFFFu;
            }
            break;
        case 1:
            do
            {
                ++(&g_Supervisor.config388.valueB7)[keyConfigSide];
                if ((&g_Supervisor.config388.valueB7)[keyConfigSide] >= 5)
                    (&g_Supervisor.config388.valueB7)[keyConfigSide] -= 5;
            }
            while (
                (((&g_Supervisor.config388.valueB7)[1 - keyConfigSide] == 1) &&
                 ((&g_Supervisor.config388.valueB7)[keyConfigSide] == 1)) ||
                (((&g_Supervisor.config388.valueB7)[1 - keyConfigSide] == 0) &&
                 ((&g_Supervisor.config388.valueB7)[keyConfigSide] == 0)) ||
                (((&g_Supervisor.config388.valueB7)[1 - keyConfigSide] == 2) &&
                 (((&g_Supervisor.config388.valueB7)[keyConfigSide] == 2) ||
                  ((&g_Supervisor.config388.valueB7)[keyConfigSide] == 3) ||
                  ((&g_Supervisor.config388.valueB7)[keyConfigSide] == 4))) ||
                (((&g_Supervisor.config388.valueB7)[1 - keyConfigSide] == 3) &&
                 (((&g_Supervisor.config388.valueB7)[keyConfigSide] == 2) ||
                  ((&g_Supervisor.config388.valueB7)[keyConfigSide] == 3))) ||
                (((&g_Supervisor.config388.valueB7)[1 - keyConfigSide] == 4) &&
                 (((&g_Supervisor.config388.valueB7)[keyConfigSide] == 2) ||
                  ((&g_Supervisor.config388.valueB7)[keyConfigSide] == 4))));

            SetIndexedSpriteChoice(
                (&g_Supervisor.config388.valueB7)[keyConfigSide], 48);
            if ((&g_Supervisor.config388.valueB7)[keyConfigSide] >= 2)
            {
                for (i = 0; i < 5; ++i)
                    vms[i + 40].color1.d3dColor = 0xFF606060u;
                for (i = 0; i < 10; ++i)
                    vms[i + 50].color1.d3dColor = 0xFF606060u;
            }
            else
            {
                for (i = 0; i < 5; ++i)
                    vms[i + 40].color1.d3dColor = 0xFFFFFFFFu;
                for (i = 0; i < 10; ++i)
                    vms[i + 50].color1.d3dColor = 0xFFFFFFFFu;
            }
            break;
        case 2:
            ++(&g_Supervisor.config388.valueB4)[keyConfigSide];
            if ((&g_Supervisor.config388.valueB4)[keyConfigSide] >= 2)
                (&g_Supervisor.config388.valueB4)[keyConfigSide] -= 2;
            SetIndexedSpriteChoice(
                (&g_Supervisor.config388.valueB4)[keyConfigSide], 49);
            SetIndexedSpriteChoice(
                (&g_Supervisor.config388.valueB4)[keyConfigSide], 42);
            break;
        default:
            goto afterRight;
        }
        g_SoundPlayer.PlaySoundByIdx(12, 0);
    }
afterRight:

    if ((g_TitleInput.pressed06 & 0x1001) != 0)
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
            (&g_Supervisor.config388.valueB4)[keyConfigSide] = 0;
            (&g_Supervisor.config388.valueB7)[keyConfigSide] =
                (u8)keyConfigSide;
            SetIndexedSpriteChoice(keyConfigSide, 47);
            SetIndexedSpriteChoice(
                (&g_Supervisor.config388.valueB4)[keyConfigSide], 49);
            SetIndexedSpriteChoice(
                (&g_Supervisor.config388.valueB7)[keyConfigSide], 48);
            SetIndexedSpriteChoice(
                (&g_Supervisor.config388.valueB4)[keyConfigSide], 42);

            if ((&g_Supervisor.config388.valueB7)[keyConfigSide] >= 2)
            {
                for (i = 0; i < 5; ++i)
                    vms[i + 40].color1.d3dColor = 0xFF606060u;
                for (i = 0; i < 10; ++i)
                    vms[i + 50].color1.d3dColor = 0xFF606060u;
            }
            else
            {
                for (i = 0; i < 5; ++i)
                    vms[i + 40].color1.d3dColor = 0xFFFFFFFFu;
                for (i = 0; i < 10; ++i)
                    vms[i + 50].color1.d3dColor = 0xFFFFFFFFu;
            }
            break;
        }
        default:
            break;
        }
    }
        break;
    }

    ++screenFrameCounter;
    ++stateTimer;
    ++stateTimer2;
    return 1;
}
