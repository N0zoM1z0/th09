#include <windows.h>
#include <mmsystem.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <string.h>
#include "Controller.hpp"

struct ControllerStateSupervisorView
{
    unsigned char unknown000[0x14];
    IDirectInputDevice8A *controllers14[2];
};

extern ControllerStateSupervisorView g_Supervisor;
extern unsigned char g_ControllerRawButtons[128];

unsigned char *__fastcall Controller::GetControllerState(int deviceIndex)
{
    DIJOYSTATE2 joystickState;
    JOYINFOEX joystickInfo;

    memset(g_ControllerRawButtons, 0, sizeof(g_ControllerRawButtons));

    if (g_Supervisor.controllers14[deviceIndex] == NULL)
    {
        memset(&joystickInfo, 0, sizeof(joystickInfo));
        joystickInfo.dwSize = sizeof(joystickInfo);
        joystickInfo.dwFlags = JOY_RETURNALL;
        if (joyGetPosEx(0, &joystickInfo) != JOYERR_NOERROR)
            return g_ControllerRawButtons;

        DWORD buttons = joystickInfo.dwButtons;
        for (unsigned int buttonIndex = 0; buttonIndex < 32;
             ++buttonIndex, buttons >>= 1)
        {
            if (buttons & 1)
                g_ControllerRawButtons[buttonIndex] = 0x80;
        }
        return g_ControllerRawButtons;
    }

    if (g_Supervisor.controllers14[deviceIndex]->Poll() < 0)
    {
        int acquireAttempts = 0;
        HRESULT result =
            g_Supervisor.controllers14[deviceIndex]->Acquire();
        if (result != DIERR_INPUTLOST)
            return g_ControllerRawButtons;

        do
        {
            result = g_Supervisor.controllers14[deviceIndex]->Acquire();
            ++acquireAttempts;
            if (acquireAttempts >= 400)
                return g_ControllerRawButtons;
        }
        while (result == DIERR_INPUTLOST);

        return g_ControllerRawButtons;
    }

    IDirectInputDevice8A *device =
        g_Supervisor.controllers14[deviceIndex];
    device->GetDeviceState(sizeof(joystickState), &joystickState);
    memcpy(
        g_ControllerRawButtons,
        joystickState.rgbButtons,
        sizeof(g_ControllerRawButtons));
    return g_ControllerRawButtons;
}
