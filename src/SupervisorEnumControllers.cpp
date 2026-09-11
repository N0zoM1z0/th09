#include "Supervisor.hpp"

#include <windows.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <stddef.h>

namespace
{
struct SupervisorEnumControllerLayout
{
    unsigned char unknown000[0x0C];
    IDirectInput8A *directInput;
    IDirectInputDevice8A *keyboard;
    IDirectInputDevice8A *controller0;
    IDirectInputDevice8A *controller1;
};

typedef char SupervisorEnumController0At14[
    (offsetof(SupervisorEnumControllerLayout, controller0) == 0x14) ? 1 : -1];
typedef char SupervisorEnumController1At18[
    (offsetof(SupervisorEnumControllerLayout, controller1) == 0x18) ? 1 : -1];
}

BOOL CALLBACK StartupEnumControllers(const DIDEVICEINSTANCEA *instance, void *unused)
{
    SupervisorEnumControllerLayout *supervisor =
        reinterpret_cast<SupervisorEnumControllerLayout *>(&g_Supervisor);

    if (supervisor->controller0 == NULL)
    {
        supervisor->directInput->CreateDevice(
            instance->guidInstance, &supervisor->controller0, NULL);
        return TRUE;
    }

    if (supervisor->controller1 != NULL)
        return TRUE;

    return supervisor->directInput->CreateDevice(
               instance->guidInstance, &supervisor->controller1, NULL) < 0;
}
