#pragma once

namespace Controller
{
unsigned short GetJoystickCaps();
unsigned short __fastcall GetControllerInput(
    int controllerIndex, int joystickIndex, unsigned short buttons);
unsigned char *__fastcall GetControllerState(int deviceIndex);
}
