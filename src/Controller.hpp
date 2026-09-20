#pragma once

namespace Controller
{
unsigned short GetJoystickCaps();
unsigned short __fastcall GetControllerInput(
    int controllerIndex, int joystickIndex, unsigned short buttons);
}
