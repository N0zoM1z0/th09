// SPDX-License-Identifier: GPL-3.0-only
// Adapted from Cirno-Labs/th09 src/unsorted.cpp, ReadPad,
// commit d7653152becabb8c86f2f43d49015ecc11bd0652.
// Original MIT portions retain their grant in LICENSE; see THIRD_PARTY_NOTICES.md.
#include "Controller.hpp"
#include "ReplayInputState.hpp"
#include "Supervisor.hpp"

#include <windows.h>
#include <mmsystem.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <string.h>
#include <stddef.h>

typedef unsigned char u8;
typedef signed short i16;
typedef unsigned short u16;
typedef unsigned int u32;

// The two original DirectInput device pointers reside in the native
// Supervisor's contiguous +0x14..+0x1B controller array.
typedef char ControllerNativeDeviceSlotsAt14[
    (offsetof(Supervisor, controllers14) == 0x14 &&
     sizeof(((Supervisor *)0)->controllers14) == 8 &&
     offsetof(Supervisor, controller014) == 0x14 &&
     offsetof(Supervisor, controller118) == 0x18) ? 1 : -1];
typedef char ControllerMappingsAt58[
    (offsetof(ReplayInputState, keyCodes58) == 0x58) ? 1 : -1];
typedef char ControllerThresholdXAt430[
    (offsetof(Supervisor, config388) + offsetof(GameConfiguration, padXAxis) == 0x430) ? 1 : -1];
typedef char ControllerThresholdYAt432[
    (offsetof(Supervisor, config388) + offsetof(GameConfiguration, padYAxis) == 0x432) ? 1 : -1];

namespace Controller
{
extern JOYCAPSA g_JoystickCaps[2];

// The DirectInput button number is a signed 16-bit controller mapping.
// Once the original negative-index test passes, retail reads the actual
// RGB button buffer at native Win32 base plus the signed index byte.
// These two actual Supervisor DirectInput devices reside at +0x14/+0x18;
// the input's signed pad selector is scaled in a native 32-bit word.
#define TH09_CONTROLLER_NATIVE_DEVICE(index) \
    (*reinterpret_cast<IDirectInputDevice8A **>( \
        reinterpret_cast<u32>(&g_Supervisor.controllers14[0]) + \
        static_cast<u32>(index) * sizeof(g_Supervisor.controllers14[0])))

// WinMM joystick capabilities likewise occupy two authentic JOYCAPSA
// records; do not replace or duplicate this process-global storage.
#define TH09_CONTROLLER_NATIVE_CAPS(index) \
    (*reinterpret_cast<JOYCAPSA *>( \
        reinterpret_cast<u32>(&g_JoystickCaps[0]) + \
        static_cast<u32>(index) * sizeof(g_JoystickCaps[0])))

#define TH09_CONTROLLER_NATIVE_BUTTON(index) \
    (*reinterpret_cast<const u8 *>( \
        reinterpret_cast<u32>(inputButtons) + \
        static_cast<u32>(static_cast<int>(index))))


static u32 SetButtonFromDirectInputState(
    u16 *outButtons, i16 buttonIndex, u16 touhouButton, u8 *inputButtons)
{
    if (buttonIndex < 0)
        return 0;

    *outButtons |= (TH09_CONTROLLER_NATIVE_BUTTON(buttonIndex) & 0x80)
                       ? touhouButton
                       : 0;
    return (TH09_CONTROLLER_NATIVE_BUTTON(buttonIndex) & 0x80) ? touhouButton : 0;
}

static u32 SetButtonFromJoystickButtons(
    u16 *outButtons, i16 buttonIndex, u16 touhouButton, u32 inputButtons)
{
    if (buttonIndex < 0)
        return 0;

    u32 mask = 1U << buttonIndex;
    *outButtons |= (inputButtons & mask) ? touhouButton : 0;
    return (inputButtons & mask) ? touhouButton : 0;
}

u16 __fastcall GetControllerInput(int input, int pad, u16 buttons)
{
    JOYINFOEX joyInfo;
    DWORD margin;
    HRESULT hr;
    DIJOYSTATE2 state;
    ReplayInputState *playerInput;
    int retries;

    // The three actual 0x8E-byte ReplayInputState slots are shared by
    // replay recording, Player movement and the controller pollers.
    playerInput = reinterpret_cast<ReplayInputState *>(
        reinterpret_cast<u32>(&g_ReplayInputStates[0]) +
        static_cast<u32>(input) * sizeof(g_ReplayInputStates[0]));
    if (g_Supervisor.controllers14[0] == NULL || g_Supervisor.controllers14[1] == NULL)
    {
        memset(&joyInfo, 0, sizeof(joyInfo));
        joyInfo.dwSize = sizeof(joyInfo);
        joyInfo.dwFlags = JOY_RETURNALL;
        if (joyGetPosEx(pad != 0, &joyInfo) != JOYERR_NOERROR)
        {
            return buttons;
        }
        SetButtonFromJoystickButtons(&buttons, playerInput->keyCodes58[0], 1, joyInfo.dwButtons);
        SetButtonFromJoystickButtons(&buttons, playerInput->keyCodes58[1], 2, joyInfo.dwButtons);
        SetButtonFromJoystickButtons(&buttons, playerInput->keyCodes58[2], 4, joyInfo.dwButtons);
        SetButtonFromJoystickButtons(&buttons, playerInput->keyCodes58[3], 8, joyInfo.dwButtons);
        SetButtonFromJoystickButtons(&buttons, playerInput->keyCodes58[4], 0x10, joyInfo.dwButtons);
        SetButtonFromJoystickButtons(&buttons, playerInput->keyCodes58[5], 0x20, joyInfo.dwButtons);
        SetButtonFromJoystickButtons(&buttons, playerInput->keyCodes58[6], 0x40, joyInfo.dwButtons);
        SetButtonFromJoystickButtons(&buttons, playerInput->keyCodes58[7], 0x80, joyInfo.dwButtons);
        SetButtonFromJoystickButtons(&buttons, playerInput->keyCodes58[8], 0x100, joyInfo.dwButtons);
        margin = (TH09_CONTROLLER_NATIVE_CAPS(pad).wXmax - TH09_CONTROLLER_NATIVE_CAPS(pad).wXmin) / 2 / 2;
        buttons |= ((TH09_CONTROLLER_NATIVE_CAPS(pad).wXmin + TH09_CONTROLLER_NATIVE_CAPS(pad).wXmax) / 2 + margin < joyInfo.dwXpos) ? 0x80 : 0;
        buttons |= (joyInfo.dwXpos < (TH09_CONTROLLER_NATIVE_CAPS(pad).wXmin + TH09_CONTROLLER_NATIVE_CAPS(pad).wXmax) / 2 - margin) ? 0x40 : 0;
        margin = (TH09_CONTROLLER_NATIVE_CAPS(pad).wYmax - TH09_CONTROLLER_NATIVE_CAPS(pad).wYmin) / 2 / 2;
        buttons |= ((TH09_CONTROLLER_NATIVE_CAPS(pad).wYmin + TH09_CONTROLLER_NATIVE_CAPS(pad).wYmax) / 2 + margin < joyInfo.dwYpos) ? 0x20 : 0;
        buttons |= (joyInfo.dwYpos < (TH09_CONTROLLER_NATIVE_CAPS(pad).wYmin + TH09_CONTROLLER_NATIVE_CAPS(pad).wYmax) / 2 - margin) ? 0x10 : 0;
        return buttons;
    }
    hr = TH09_CONTROLLER_NATIVE_DEVICE(pad)->Poll();
    if (FAILED(hr))
    {
        retries = 0;
        hr = TH09_CONTROLLER_NATIVE_DEVICE(pad)->Acquire();
        while (hr == DIERR_INPUTLOST)
        {
            hr = TH09_CONTROLLER_NATIVE_DEVICE(pad)->Acquire();
            retries++;
            if (retries >= 400)
            {
                return buttons;
            }
        }
        return buttons;
    }
    memset(&state, 0, sizeof(state));
    hr = TH09_CONTROLLER_NATIVE_DEVICE(pad)->GetDeviceState(sizeof(DIJOYSTATE2), &state);
    if (FAILED(hr))
    {
        return buttons;
    }
    SetButtonFromDirectInputState(&buttons, playerInput->keyCodes58[0], 1, state.rgbButtons);
    SetButtonFromDirectInputState(&buttons, playerInput->keyCodes58[1], 2, state.rgbButtons);
    SetButtonFromDirectInputState(&buttons, playerInput->keyCodes58[2], 4, state.rgbButtons);
    SetButtonFromDirectInputState(&buttons, playerInput->keyCodes58[3], 8, state.rgbButtons);
    SetButtonFromDirectInputState(&buttons, playerInput->keyCodes58[4], 0x10, state.rgbButtons);
    SetButtonFromDirectInputState(&buttons, playerInput->keyCodes58[5], 0x20, state.rgbButtons);
    SetButtonFromDirectInputState(&buttons, playerInput->keyCodes58[6], 0x40, state.rgbButtons);
    SetButtonFromDirectInputState(&buttons, playerInput->keyCodes58[7], 0x80, state.rgbButtons);
    SetButtonFromDirectInputState(&buttons, playerInput->keyCodes58[8], 0x100, state.rgbButtons);
    buttons |= (state.lX > static_cast<i16>(g_Supervisor.config388.padXAxis)) ? 0x80 : 0;
    buttons |= (state.lX < -static_cast<i16>(g_Supervisor.config388.padXAxis)) ? 0x40 : 0;
    buttons |= (state.lY > static_cast<i16>(g_Supervisor.config388.padYAxis)) ? 0x20 : 0;
    buttons |= (state.lY < -static_cast<i16>(g_Supervisor.config388.padYAxis)) ? 0x10 : 0;
    return buttons;
}

#undef TH09_CONTROLLER_NATIVE_BUTTON
#undef TH09_CONTROLLER_NATIVE_DEVICE
#undef TH09_CONTROLLER_NATIVE_CAPS

} // namespace Controller
