#include <windows.h>

void SanitizeKeyboardState()
{
    BYTE keyboardState[256];
    GetKeyboardState(keyboardState);

    for (int i = 0; i < 256; ++i)
        keyboardState[i] &= 0x7F;

    SetKeyboardState(keyboardState);
}
