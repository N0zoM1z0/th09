#include "Controller.hpp"
#include "GameConfiguration.hpp"

#include <windows.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

typedef unsigned char u8;
typedef unsigned short u16;

enum TouhouButton
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

struct SampleNetworkInputState
{
    u16 current00;
    u16 previous02;
    u16 repeat04;
    u16 pressed06;
    u16 released08;
    u16 heldFrames0A[16];
    u8 unknown2A[0x2E];
    short mappings58[27];
};

typedef char SampleNetworkInputStateSizeIs8E[
    (sizeof(SampleNetworkInputState) == 0x8E) ? 1 : -1];

struct SampleNetworkInputSupervisorView
{
    u8 unknown000[0x10];
    IDirectInputDevice8A *keyboard10;
    u8 unknown014[0x388 - 0x14];
    GameConfiguration config388;
};

extern SampleNetworkInputState g_ReplayInputStates[3];
extern SampleNetworkInputSupervisorView g_Supervisor;
extern int g_WindowActivateWParam;

#define KEYBOARD_KEY_PRESSED(button, key) \
    ((keyboardState[(key)] & 0x80) ? (button) : 0)

short __fastcall SampleNetworkInput(int side)
{
    u8 keyboardState[256];
    u16 buttons = 0;
    SampleNetworkInputState *inputState = &g_ReplayInputStates[side];

    if (g_WindowActivateWParam != 0)
    {
        if (g_Supervisor.keyboard10 == NULL)
        {
            GetKeyboardState(keyboardState);

            if (side == 2)
            {
                buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_SHOOT, 'Z');
                buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_BOMB, 'X');
                buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_FOCUS, VK_SHIFT);
                buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_UP, VK_UP);
                buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_DOWN, VK_DOWN);
                buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_LEFT, VK_LEFT);
                buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_RIGHT, VK_RIGHT);
                buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_UP, VK_NUMPAD8);
                buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_DOWN, VK_NUMPAD2);
                buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_LEFT, VK_NUMPAD4);
                buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_RIGHT, VK_NUMPAD6);
                buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_UP_LEFT, VK_NUMPAD7);
                buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_UP_RIGHT, VK_NUMPAD9);
                buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_DOWN_LEFT, VK_NUMPAD1);
                buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_DOWN_RIGHT, VK_NUMPAD3);
            }
            else
            {
                switch ((&g_Supervisor.config388.valueB7)[side])
                {
                    case 2:
                        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_SHOOT, 'Z');
                        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_BOMB, 'X');
                        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_FOCUS, VK_SHIFT);
                        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_UP, VK_UP);
                        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_DOWN, VK_DOWN);
                        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_LEFT, VK_LEFT);
                        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_RIGHT, VK_RIGHT);
                        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_UP, VK_NUMPAD8);
                        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_DOWN, VK_NUMPAD2);
                        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_LEFT, VK_NUMPAD4);
                        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_RIGHT, VK_NUMPAD6);
                        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_UP_LEFT, VK_NUMPAD7);
                        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_UP_RIGHT, VK_NUMPAD9);
                        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_DOWN_LEFT, VK_NUMPAD1);
                        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_DOWN_RIGHT, VK_NUMPAD3);
                        break;
                    case 3:
                        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_SHOOT, 'Z');
                        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_BOMB, 'X');
                        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_FOCUS, VK_SHIFT);
                        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_UP, 'T');
                        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_DOWN, 'B');
                        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_LEFT, 'F');
                        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_RIGHT, 'H');
                        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_UP_LEFT, 'R');
                        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_UP_RIGHT, 'Y');
                        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_DOWN_LEFT, 'V');
                        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_DOWN_RIGHT, 'N');
                        break;
                    case 4:
                        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_SHOOT, VK_DOWN);
                        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_BOMB, VK_RIGHT);
                        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_FOCUS, VK_LEFT);
                        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_UP, VK_NUMPAD8);
                        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_DOWN, VK_NUMPAD2);
                        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_LEFT, VK_NUMPAD4);
                        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_RIGHT, VK_NUMPAD6);
                        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_UP_LEFT, VK_NUMPAD7);
                        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_UP_RIGHT, VK_NUMPAD9);
                        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_DOWN_LEFT, VK_NUMPAD1);
                        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_DOWN_RIGHT, VK_NUMPAD3);
                        break;
                }
            }

            buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_RESET, 'R');
            buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_D, 'D');
            buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_ENTER, VK_RETURN);
            buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_HOME, VK_HOME);
            buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_HOME, 'P');
            buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_S, 'S');
            buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_Q, 'Q');
            buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_SKIP, VK_CONTROL);
            buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_MENU, VK_ESCAPE);
        }
        else
        {
            HRESULT inputResult =
                g_Supervisor.keyboard10->GetDeviceState(
                    sizeof(keyboardState), keyboardState);
            buttons = 0;

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
                if (side == 2)
                {
                    buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_SHOOT, DIK_Z);
                    buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_BOMB, DIK_X);
                    buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_FOCUS, DIK_LSHIFT);
                    buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_UP, DIK_UP);
                    buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_DOWN, DIK_DOWN);
                    buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_LEFT, DIK_LEFT);
                    buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_RIGHT, DIK_RIGHT);
                    buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_UP, DIK_NUMPAD8);
                    buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_DOWN, DIK_NUMPAD2);
                    buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_LEFT, DIK_NUMPAD4);
                    buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_RIGHT, DIK_NUMPAD6);
                    buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_UP_LEFT, DIK_NUMPAD7);
                    buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_UP_RIGHT, DIK_NUMPAD9);
                    buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_DOWN_LEFT, DIK_NUMPAD1);
                    buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_DOWN_RIGHT, DIK_NUMPAD3);
                }
                else
                {
                    switch ((&g_Supervisor.config388.valueB7)[side])
                    {
                        case 2:
                            buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_SHOOT, DIK_Z);
                            buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_BOMB, DIK_X);
                            buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_FOCUS, DIK_LSHIFT);
                            buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_UP, DIK_UP);
                            buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_DOWN, DIK_DOWN);
                            buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_LEFT, DIK_LEFT);
                            buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_RIGHT, DIK_RIGHT);
                            buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_UP, DIK_NUMPAD8);
                            buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_DOWN, DIK_NUMPAD2);
                            buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_LEFT, DIK_NUMPAD4);
                            buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_RIGHT, DIK_NUMPAD6);
                            buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_UP_LEFT, DIK_NUMPAD7);
                            buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_UP_RIGHT, DIK_NUMPAD9);
                            buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_DOWN_LEFT, DIK_NUMPAD1);
                            buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_DOWN_RIGHT, DIK_NUMPAD3);
                            break;
                        case 3:
                            buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_SHOOT, DIK_Z);
                            buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_BOMB, DIK_X);
                            buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_FOCUS, DIK_LSHIFT);
                            buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_UP, DIK_T);
                            buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_DOWN, DIK_B);
                            buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_LEFT, DIK_F);
                            buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_RIGHT, DIK_H);
                            buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_UP_LEFT, DIK_R);
                            buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_UP_RIGHT, DIK_Y);
                            buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_DOWN_LEFT, DIK_V);
                            buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_DOWN_RIGHT, DIK_N);
                            break;
                        case 4:
                            buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_SHOOT, DIK_DOWN);
                            buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_BOMB, DIK_RIGHT);
                            buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_FOCUS, DIK_LEFT);
                            buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_UP, DIK_NUMPAD8);
                            buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_DOWN, DIK_NUMPAD2);
                            buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_LEFT, DIK_NUMPAD4);
                            buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_RIGHT, DIK_NUMPAD6);
                            buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_UP_LEFT, DIK_NUMPAD7);
                            buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_UP_RIGHT, DIK_NUMPAD9);
                            buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_DOWN_LEFT, DIK_NUMPAD1);
                            buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_DOWN_RIGHT, DIK_NUMPAD3);
                            break;
                    }
                }

                buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_RESET, DIK_R);
                buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_D, DIK_D);
                buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_ENTER, DIK_RETURN);
                buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_HOME, DIK_HOME);
                buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_HOME, DIK_P);
                buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_S, DIK_S);
                buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_Q, DIK_Q);
                buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_SKIP, DIK_LCONTROL);
                buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_MENU, DIK_ESCAPE);
            }
        }
    }

    if (side >= 2)
    {
        u8 assignment = g_Supervisor.config388.valueB7;
        if (assignment == 0 || assignment == 1)
            buttons = Controller::GetControllerInput(0, assignment != 0, buttons);

        assignment = g_Supervisor.config388.valueB8;
        if (assignment == 0 || assignment == 1)
            buttons = Controller::GetControllerInput(1, assignment != 0, buttons);
    }
    else
    {
        u8 assignment = (&g_Supervisor.config388.valueB7)[side];
        if (assignment == 0 || assignment == 1)
            buttons = Controller::GetControllerInput(side, assignment != 0, buttons);
    }

    inputState->previous02 = inputState->current00;
    inputState->current00 = buttons;

    u16 repeatBit = 1;
    u16 currentBits = buttons;
    inputState->repeat04 = 0;
    u16 *held = inputState->heldFrames0A;
    int remaining = 16;
    do
    {
        if ((currentBits & 1) != 0)
        {
            (*held)++;
            if (*held >= 26)
            {
                inputState->repeat04 |= repeatBit;
                *held -= 8;
            }
        }
        else
        {
            *held = 0;
        }
        currentBits >>= 1;
        held++;
        repeatBit <<= 1;
        remaining--;
    } while (remaining != 0);

    u16 changed = inputState->current00 ^ inputState->previous02;
    inputState->pressed06 = changed & inputState->current00;
    inputState->released08 = changed & ~inputState->current00;
    return buttons;
}

#undef KEYBOARD_KEY_PRESSED
