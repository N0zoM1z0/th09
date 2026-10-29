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

// The shared Supervisor keeps its device prefix opaque; the published slots
// are independently observed at +0x14/+0x18 in TH09.
struct ControllerSupervisorDevicesView
{
    u8 unknown000[0x14];
    IDirectInputDevice8A *controllers14[2];
};

typedef char ControllerDevicesAt14[
    (offsetof(ControllerSupervisorDevicesView, controllers14) == 0x14) ? 1 : -1];
typedef char ControllerMappingsAt58[
    (offsetof(ReplayInputState, keyCodes58) == 0x58) ? 1 : -1];
typedef char ControllerThresholdXAt430[
    (offsetof(Supervisor, config388) + offsetof(GameConfiguration, padXAxis) == 0x430) ? 1 : -1];
typedef char ControllerThresholdYAt432[
    (offsetof(Supervisor, config388) + offsetof(GameConfiguration, padYAxis) == 0x432) ? 1 : -1];

static inline ControllerSupervisorDevicesView &ControllerDevices()
{
    return *reinterpret_cast<ControllerSupervisorDevicesView *>(&g_Supervisor);
}

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

    u32 mask = 1U << buttonIndex;
    *outButtons |= (inputButtons & mask) ? touhouButton : 0;
    return (inputButtons & mask) ? touhouButton : 0;
}

// Native JOYCAPSA min/max words form one eight-byte range. Keep the
// low-edge snapshot separate from the source range used for the high edge.
struct JoystickAxisRangeView
{
    UINT minimum;
    UINT maximum;

    u32 GetDirectionButtons(DWORD position, u16 lowerBit, u16 upperBit) const
    {
        UINT quarterWidth = (maximum - minimum) >> 2;
        JoystickAxisRangeView lower(*this);
        UINT lowerThreshold = (lower.maximum + lower.minimum) >> 1;
        lowerThreshold -= quarterWidth;
        u32 result = position < lowerThreshold ? lowerBit : 0;
        UINT upper = (minimum + maximum) >> 1;
        upper += quarterWidth;
        return result | (position > upper ? upperBit : 0);
    }
};

typedef char JoystickAxisRangeViewSizeIs8[
    (sizeof(JoystickAxisRangeView) == 8) ? 1 : -1];
typedef char ControllerAxisNativeXPair[
    (offsetof(JOYCAPSA, wXmax) - offsetof(JOYCAPSA, wXmin) == sizeof(UINT)) ? 1 : -1];
typedef char ControllerAxisNativeYPair[
    (offsetof(JOYCAPSA, wYmax) - offsetof(JOYCAPSA, wYmin) == sizeof(UINT)) ? 1 : -1];

u16 __fastcall GetControllerInput(
    int controllerIndex, int joystickIndex, u16 buttons)
{
    DIJOYSTATE2 joystickState;
    JOYINFOEX joystickInfo;
    int savedJoystickIndex = joystickIndex;
    ReplayInputState *inputSlot =
        &g_ReplayInputStates[controllerIndex];

    if (ControllerDevices().controllers14[0] == NULL ||
        ControllerDevices().controllers14[1] == NULL)
    {
        memset(&joystickInfo, 0, sizeof(joystickInfo));
        joystickInfo.dwSize = sizeof(joystickInfo);
        joystickInfo.dwFlags = JOY_RETURNALL;
        if (joyGetPosEx(savedJoystickIndex != 0, &joystickInfo) != JOYERR_NOERROR)
            return buttons;

        u32 rawButtons = joystickInfo.dwButtons;
        SetButtonFromJoystickButtons(
            &buttons, inputSlot->keyCodes58[0], 0x001, rawButtons);
        SetButtonFromJoystickButtons(
            &buttons, inputSlot->keyCodes58[1], 0x002, rawButtons);
        SetButtonFromJoystickButtons(
            &buttons, inputSlot->keyCodes58[2], 0x004, rawButtons);
        SetButtonFromJoystickButtons(
            &buttons, inputSlot->keyCodes58[3], 0x008, rawButtons);
        SetButtonFromJoystickButtons(
            &buttons, inputSlot->keyCodes58[4], 0x010, rawButtons);
        SetButtonFromJoystickButtons(
            &buttons, inputSlot->keyCodes58[5], 0x020, rawButtons);
        SetButtonFromJoystickButtons(
            &buttons, inputSlot->keyCodes58[6], 0x040, rawButtons);
        SetButtonFromJoystickButtons(
            &buttons, inputSlot->keyCodes58[7], 0x080, rawButtons);
        SetButtonFromJoystickButtons(
            &buttons, inputSlot->keyCodes58[8], 0x100, rawButtons);

        buttons |= reinterpret_cast<const JoystickAxisRangeView *>(
            &g_JoystickCaps[savedJoystickIndex].wXmin)->GetDirectionButtons(
                joystickInfo.dwXpos, 0x40, 0x80);
        buttons |= reinterpret_cast<const JoystickAxisRangeView *>(
            &g_JoystickCaps[savedJoystickIndex].wYmin)->GetDirectionButtons(
                joystickInfo.dwYpos, 0x10, 0x20);
        return buttons;
    }

    // Poll and Acquire may change the published device slot. The target
    // reloads it for every call, then retains it only for GetDeviceState.
    if (ControllerDevices().controllers14[savedJoystickIndex]->Poll() < 0)
    {
        int acquireAttempts = 0;
        HRESULT result = ControllerDevices().controllers14[savedJoystickIndex]->Acquire();
        if (result == DIERR_INPUTLOST)
        {
            do
            {
                result = ControllerDevices().controllers14[savedJoystickIndex]->Acquire();
                ++acquireAttempts;
                if (acquireAttempts >= 400)
                    return buttons;
            }
            while (result == DIERR_INPUTLOST);
        }
        return buttons;
    }

    IDirectInputDevice8A *device =
        ControllerDevices().controllers14[savedJoystickIndex];
    memset(&joystickState, 0, sizeof(joystickState));
    if (device->GetDeviceState(sizeof(joystickState), &joystickState) < 0)
        return buttons;

    SetButtonFromDirectInputState(
        &buttons, inputSlot->keyCodes58[0], 0x001, joystickState.rgbButtons);
    SetButtonFromDirectInputState(
        &buttons, inputSlot->keyCodes58[1], 0x002, joystickState.rgbButtons);
    SetButtonFromDirectInputState(
        &buttons, inputSlot->keyCodes58[2], 0x004, joystickState.rgbButtons);
    SetButtonFromDirectInputState(
        &buttons, inputSlot->keyCodes58[3], 0x008, joystickState.rgbButtons);
    SetButtonFromDirectInputState(
        &buttons, inputSlot->keyCodes58[4], 0x010, joystickState.rgbButtons);
    SetButtonFromDirectInputState(
        &buttons, inputSlot->keyCodes58[5], 0x020, joystickState.rgbButtons);
    SetButtonFromDirectInputState(
        &buttons, inputSlot->keyCodes58[6], 0x040, joystickState.rgbButtons);
    SetButtonFromDirectInputState(
        &buttons, inputSlot->keyCodes58[7], 0x080, joystickState.rgbButtons);
    SetButtonFromDirectInputState(
        &buttons, inputSlot->keyCodes58[8], 0x100, joystickState.rgbButtons);

    // Configuration stores unsigned words; this target path reads signed words.
    buttons |= joystickState.lX < -static_cast<i16>(g_Supervisor.config388.padXAxis) ? 0x40 : 0;
    buttons |= joystickState.lY < -static_cast<i16>(g_Supervisor.config388.padYAxis) ? 0x10 : 0;
    buttons |= joystickState.lX > static_cast<i16>(g_Supervisor.config388.padXAxis) ? 0x80 : 0;
    buttons |= joystickState.lY > static_cast<i16>(g_Supervisor.config388.padYAxis) ? 0x20 : 0;
    return buttons;

}
} // namespace Controller
