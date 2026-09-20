#include "Controller.hpp"

#include <windows.h>
#include <mmsystem.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <string.h>

typedef unsigned char u8;
typedef signed short i16;
typedef unsigned short u16;
typedef unsigned int u32;

struct ControllerInputSlotView
{
    u8 unknown00[0x58];
    i16 mappings58[27];
};

struct ControllerSupervisorInputView
{
    u8 unknown000[0x14];
    IDirectInputDevice8A *controllers14[2];
    u8 unknown01C[0x430 - 0x1C];
    i16 axisThresholdX430;
    i16 axisThresholdY432;
};

extern ControllerInputSlotView g_ReplayInputStates[3];
extern ControllerSupervisorInputView g_Supervisor;

namespace Controller
{
extern JOYCAPSA g_JoystickCaps[2];

static u32 SetButtonFromDirectInputState(
    u16 *outButtons, i16 buttonIndex, u16 touhouButton, u8 *inputButtons)
{
    if (buttonIndex < 0)
        return 0;

    *outButtons |= (inputButtons[buttonIndex] & 0x80)
                       ? touhouButton
                       : 0;
    return (inputButtons[buttonIndex] & 0x80) ? touhouButton : 0;
}

static u32 SetButtonFromJoystickButtons(
    u16 *outButtons, i16 buttonIndex, u16 touhouButton, u32 inputButtons)
{
    if (buttonIndex < 0)
        return 0;

    u32 mask = 1 << buttonIndex;
    *outButtons |= (inputButtons & mask) ? touhouButton : 0;
    return (inputButtons & mask) ? touhouButton : 0;
}

u16 __fastcall GetControllerInput(
    int controllerIndex, int joystickIndex, u16 buttons)
{
    DIJOYSTATE2 joystickState;
    JOYINFOEX joystickInfo;
    int savedJoystickIndex = joystickIndex;
    ControllerInputSlotView *inputSlot =
        &g_ReplayInputStates[controllerIndex];

    if (g_Supervisor.controllers14[0] != NULL &&
        g_Supervisor.controllers14[1] != NULL)
    {
        IDirectInputDevice8A *device =
            g_Supervisor.controllers14[savedJoystickIndex];

        if (device->Poll() < 0)
        {
            int acquireAttempts = 0;
            HRESULT result = device->Acquire();
            if (result == DIERR_INPUTLOST)
            {
                do
                {
                    result = device->Acquire();
                    ++acquireAttempts;
                    if (acquireAttempts >= 400)
                        return buttons;
                }
                while (result == DIERR_INPUTLOST);
            }
            return buttons;
        }

        memset(&joystickState, 0, sizeof(joystickState));
        if (device->GetDeviceState(sizeof(joystickState), &joystickState) < 0)
            return buttons;

        SetButtonFromDirectInputState(
            &buttons, inputSlot->mappings58[0], 0x001, joystickState.rgbButtons);
        SetButtonFromDirectInputState(
            &buttons, inputSlot->mappings58[1], 0x002, joystickState.rgbButtons);
        SetButtonFromDirectInputState(
            &buttons, inputSlot->mappings58[2], 0x004, joystickState.rgbButtons);
        SetButtonFromDirectInputState(
            &buttons, inputSlot->mappings58[3], 0x008, joystickState.rgbButtons);
        SetButtonFromDirectInputState(
            &buttons, inputSlot->mappings58[4], 0x010, joystickState.rgbButtons);
        SetButtonFromDirectInputState(
            &buttons, inputSlot->mappings58[5], 0x020, joystickState.rgbButtons);
        SetButtonFromDirectInputState(
            &buttons, inputSlot->mappings58[6], 0x040, joystickState.rgbButtons);
        SetButtonFromDirectInputState(
            &buttons, inputSlot->mappings58[7], 0x080, joystickState.rgbButtons);
        SetButtonFromDirectInputState(
            &buttons, inputSlot->mappings58[8], 0x100, joystickState.rgbButtons);

        buttons |= joystickState.lX < -g_Supervisor.axisThresholdX430 ? 0x40 : 0;
        buttons |= joystickState.lY < -g_Supervisor.axisThresholdY432 ? 0x10 : 0;
        buttons |= joystickState.lX > g_Supervisor.axisThresholdX430 ? 0x80 : 0;
        buttons |= joystickState.lY > g_Supervisor.axisThresholdY432 ? 0x20 : 0;
        return buttons;
    }

    memset(&joystickInfo, 0, sizeof(joystickInfo));
    joystickInfo.dwSize = sizeof(joystickInfo);
    joystickInfo.dwFlags = JOY_RETURNALL;
    if (joyGetPosEx(savedJoystickIndex != 0, &joystickInfo) != JOYERR_NOERROR)
        return buttons;

    u32 rawButtons = joystickInfo.dwButtons;
    SetButtonFromJoystickButtons(
        &buttons, inputSlot->mappings58[0], 0x001, rawButtons);
    SetButtonFromJoystickButtons(
        &buttons, inputSlot->mappings58[1], 0x002, rawButtons);
    SetButtonFromJoystickButtons(
        &buttons, inputSlot->mappings58[2], 0x004, rawButtons);
    SetButtonFromJoystickButtons(
        &buttons, inputSlot->mappings58[3], 0x008, rawButtons);
    SetButtonFromJoystickButtons(
        &buttons, inputSlot->mappings58[4], 0x010, rawButtons);
    SetButtonFromJoystickButtons(
        &buttons, inputSlot->mappings58[5], 0x020, rawButtons);
    SetButtonFromJoystickButtons(
        &buttons, inputSlot->mappings58[6], 0x040, rawButtons);
    SetButtonFromJoystickButtons(
        &buttons, inputSlot->mappings58[7], 0x080, rawButtons);
    SetButtonFromJoystickButtons(
        &buttons, inputSlot->mappings58[8], 0x100, rawButtons);

    JOYCAPSA *caps = &g_JoystickCaps[savedJoystickIndex];

    UINT xDeadzone = (caps->wXmax - caps->wXmin) >> 2;
    UINT xMidpoint = (caps->wXmin + caps->wXmax) >> 1;
    buttons |= joystickInfo.dwXpos < xMidpoint - xDeadzone ? 0x40 : 0;
    buttons |= joystickInfo.dwXpos > xMidpoint + xDeadzone ? 0x80 : 0;

    UINT yDeadzone = (caps->wYmax - caps->wYmin) >> 2;
    UINT yMidpoint = (caps->wYmin + caps->wYmax) >> 1;
    buttons |= joystickInfo.dwYpos < yMidpoint - yDeadzone ? 0x10 : 0;
    buttons |= joystickInfo.dwYpos > yMidpoint + yDeadzone ? 0x20 : 0;

    return buttons;
}
} // namespace Controller
