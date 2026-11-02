#include "Supervisor.hpp"

#include <windows.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <stddef.h>

namespace
{
// The enumeration callbacks write the same native Supervisor devices as
// the original 882-byte StartupThread and 405-byte DeletedCallback.
typedef char SupervisorEnumController0At14[
    (offsetof(Supervisor, controller014) == 0x14) ? 1 : -1];
typedef char SupervisorEnumController1At18[
    (offsetof(Supervisor, controller118) == 0x18) ? 1 : -1];
}

BOOL CALLBACK StartupEnumControllers(const DIDEVICEINSTANCEA *instance, void *unused)
{
    Supervisor *supervisor = &g_Supervisor;

    if (supervisor->controller014 == NULL)
    {
        supervisor->directInput0C->CreateDevice(
            instance->guidInstance, &supervisor->controller014, NULL);
        return TRUE;
    }

    if (supervisor->controller118 != NULL)
        return TRUE;

    return supervisor->directInput0C->CreateDevice(
               instance->guidInstance, &supervisor->controller118, NULL) < 0;
}

BOOL CALLBACK StartupEnumObjects0(const DIDEVICEOBJECTINSTANCEA *instance, void *unused)
{
    Supervisor *supervisor = &g_Supervisor;

    if ((instance->dwType & DIDFT_AXIS) != 0)
    {
        DIPROPRANGE range;
        range.diph.dwSize = sizeof(DIPROPRANGE);
        range.diph.dwHeaderSize = sizeof(DIPROPHEADER);
        range.diph.dwObj = instance->dwType;
        range.diph.dwHow = DIPH_BYID;
        range.lMin = -1000;
        range.lMax = 1000;
        if (supervisor->controller014->SetProperty(DIPROP_RANGE, &range.diph) < 0)
            return FALSE;
    }

    return TRUE;
}

BOOL CALLBACK StartupEnumObjects1(const DIDEVICEOBJECTINSTANCEA *instance, void *unused)
{
    Supervisor *supervisor = &g_Supervisor;

    if ((instance->dwType & DIDFT_AXIS) != 0)
    {
        DIPROPRANGE range;
        range.diph.dwSize = sizeof(DIPROPRANGE);
        range.diph.dwHeaderSize = sizeof(DIPROPHEADER);
        range.diph.dwObj = instance->dwType;
        range.diph.dwHow = DIPH_BYID;
        range.lMin = -1000;
        range.lMax = 1000;
        if (supervisor->controller118->SetProperty(DIPROP_RANGE, &range.diph) < 0)
            return FALSE;
    }

    return TRUE;
}
