#include "Controller.hpp"
#include "GameErrorContext.hpp"

#include <windows.h>
#include <mmsystem.h>

extern GameErrorContext g_GameErrorContext;
extern const char g_NoPadFoundMessage[] = "\x8E\x67\x82\xA6\x82\xE9\x83\x70\x83\x62\x83\x68\x82\xAA\x91\xB6\x8D\xDD\x82\xB5\x82\xC8\x82\xA2\x82\xE6\x82\xA4\x82\xC5\x82\xB7\x81\x41\x8E\x63\x94\x4F\r\n"; // 使えるパッドが存在しないようです、残念\r\n

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
