#include "Controller.hpp"
#include "GameConfiguration.hpp"

#include <windows.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

typedef unsigned char u8;
typedef unsigned short u16;

enum ControllerTouhouButton
{
    TH_BUTTON_SHOOT = 0x0001,
    TH_BUTTON_BOMB = 0x0002,
    TH_BUTTON_FOCUS = 0x0004,
    TH_BUTTON_MENU = 0x0008,
    TH_BUTTON_UP = 0x0010,
    TH_BUTTON_DOWN = 0x0020,
    TH_BUTTON_LEFT = 0x0040,
    TH_BUTTON_RIGHT = 0x0080,
    TH_BUTTON_SKIP = 0x0100,
    TH_BUTTON_Q = 0x0200,
    TH_BUTTON_S = 0x0400,
    TH_BUTTON_HOME = 0x0800,
    TH_BUTTON_ENTER = 0x1000,
    TH_BUTTON_D = 0x2000,
    TH_BUTTON_RESET = 0x4000,

    TH_BUTTON_UP_LEFT = TH_BUTTON_UP | TH_BUTTON_LEFT,
    TH_BUTTON_UP_RIGHT = TH_BUTTON_UP | TH_BUTTON_RIGHT,
    TH_BUTTON_DOWN_LEFT = TH_BUTTON_DOWN | TH_BUTTON_LEFT,
    TH_BUTTON_DOWN_RIGHT = TH_BUTTON_DOWN | TH_BUTTON_RIGHT
};

struct ControllerGetInputSupervisorView
{
    unsigned char unknown000[0x10];
    IDirectInputDevice8A *keyboard10;
    unsigned char unknown014[0x388 - 0x14];
    GameConfiguration config388;
};

extern ControllerGetInputSupervisorView g_Supervisor;
extern int g_WindowActivateWParam;

#define KEYBOARD_KEY_PRESSED(button, key) \
    ((keyboardState[(key)] & 0x80) ? (button) : 0)

namespace Controller
{
u16 __fastcall GetInput(int inputIndex)
{
    u8 keyboardState[256];
    u16 inputButtons = 0;

    if (g_WindowActivateWParam != 0)
    {
        if (g_Supervisor.keyboard10 == NULL)
        {
            GetKeyboardState(keyboardState);

            inputButtons |= KEYBOARD_KEY_PRESSED(
                TH_BUTTON_SHOOT | TH_BUTTON_FOCUS, VK_SHIFT);
            inputButtons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_SHOOT, 'X');
            inputButtons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_BOMB, 'Z');
            inputButtons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_MENU, VK_ESCAPE);
            inputButtons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_UP, VK_NUMPAD8);
            inputButtons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_UP, VK_UP);
            inputButtons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_DOWN, VK_NUMPAD2);
            inputButtons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_DOWN, VK_DOWN);
            inputButtons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_LEFT, VK_NUMPAD4);
            inputButtons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_LEFT, VK_LEFT);
            inputButtons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_RIGHT, VK_NUMPAD6);
            inputButtons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_RIGHT, VK_RIGHT);
            inputButtons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_RESET, 'R');
            inputButtons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_D, 'D');
            inputButtons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_ENTER, VK_RETURN);
            inputButtons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_HOME, 'P');
            inputButtons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_HOME, VK_HOME);
            inputButtons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_S, 'S');
            inputButtons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_Q, 'Q');
            inputButtons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_SKIP, VK_CONTROL);
            inputButtons |= KEYBOARD_KEY_PRESSED(
                TH_BUTTON_DOWN_RIGHT, VK_NUMPAD3);
            inputButtons |= KEYBOARD_KEY_PRESSED(
                TH_BUTTON_UP_RIGHT, VK_NUMPAD9);
            inputButtons |= KEYBOARD_KEY_PRESSED(
                TH_BUTTON_DOWN_LEFT, VK_NUMPAD1);
            inputButtons |= KEYBOARD_KEY_PRESSED(
                TH_BUTTON_UP_LEFT, VK_NUMPAD7);
        }
        else
        {
            HRESULT inputResult =
                g_Supervisor.keyboard10->GetDeviceState(
                    sizeof(keyboardState), keyboardState);
            inputButtons = 0;

            if (inputResult == DIERR_INPUTLOST)
            {
                g_Supervisor.keyboard10->Acquire();
            }
            else if (inputResult != DI_OK)
            {
                g_Supervisor.keyboard10->Acquire();
            }
            else
            {
                inputButtons |= KEYBOARD_KEY_PRESSED(
                    TH_BUTTON_SHOOT | TH_BUTTON_FOCUS, DIK_LSHIFT);
                inputButtons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_SHOOT, DIK_X);
                inputButtons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_BOMB, DIK_Z);
                inputButtons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_MENU, DIK_ESCAPE);
                inputButtons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_UP, DIK_NUMPAD8);
                inputButtons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_UP, DIK_UP);
                inputButtons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_DOWN, DIK_NUMPAD2);
                inputButtons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_DOWN, DIK_DOWN);
                inputButtons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_LEFT, DIK_NUMPAD4);
                inputButtons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_LEFT, DIK_LEFT);
                inputButtons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_RIGHT, DIK_NUMPAD6);
                inputButtons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_RIGHT, DIK_RIGHT);
                inputButtons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_RESET, DIK_R);
                inputButtons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_D, DIK_D);
                inputButtons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_ENTER, DIK_RETURN);
                inputButtons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_HOME, DIK_P);
                inputButtons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_HOME, DIK_HOME);
                inputButtons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_S, DIK_S);
                inputButtons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_Q, DIK_Q);
                inputButtons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_SKIP, DIK_LCONTROL);
                inputButtons |= KEYBOARD_KEY_PRESSED(
                    TH_BUTTON_DOWN_RIGHT, DIK_NUMPAD3);
                inputButtons |= KEYBOARD_KEY_PRESSED(
                    TH_BUTTON_UP_RIGHT, DIK_NUMPAD9);
                inputButtons |= KEYBOARD_KEY_PRESSED(
                    TH_BUTTON_DOWN_LEFT, DIK_NUMPAD1);
                inputButtons |= KEYBOARD_KEY_PRESSED(
                    TH_BUTTON_UP_LEFT, DIK_NUMPAD7);
            }
        }
    }

    u8 assignment = g_Supervisor.config388.valueB7;
    if (assignment == 0 || assignment == 1)
    {
        inputButtons = GetControllerInput(
            0, assignment != 0, inputButtons);
    }

    assignment = g_Supervisor.config388.valueB8;
    if (assignment == 0 || assignment == 1)
    {
        inputButtons = GetControllerInput(
            1, assignment != 0, inputButtons);
    }

    return inputButtons;
}
} // namespace Controller
