#include "Controller.hpp"
#include "GameErrorContext.hpp"

#include <windows.h>
#include <mmsystem.h>

extern GameErrorContext g_GameErrorContext;
extern const char g_NoPadFoundMessage[];

namespace Controller
{
// Target-local relocation evidence establishes two contiguous JOYCAPSA records.
// Their defining translation unit and physical ownership remain unresolved.
extern JOYCAPSA g_JoystickCaps[2];

unsigned short GetJoystickCaps()
{
    JOYINFOEX joystickState;

    joystickState.dwSize = sizeof(joystickState);
    joystickState.dwFlags = JOY_RETURNALL;

    if (joyGetPosEx(0, &joystickState) != MMSYSERR_NOERROR &&
        joyGetPosEx(1, &joystickState) != MMSYSERR_NOERROR)
    {
        g_GameErrorContext.Log(g_NoPadFoundMessage);
        return 1;
    }

    joyGetDevCapsA(0, &g_JoystickCaps[0], sizeof(g_JoystickCaps[0]));
    joyGetDevCapsA(1, &g_JoystickCaps[1], sizeof(g_JoystickCaps[1]));
    return 0;
}
} // namespace Controller
