#pragma once

// Target-bound input view for g_AsciiInput (shared across AsciiInput,
// AsciiManagerMenu, AsciiMenuState5Update, Ending, and GameWindowPresent).
struct AsciiInputView
{
    unsigned short held00;
    unsigned char unknown02[4];
    unsigned short pressed06;

    unsigned short IsHeld(unsigned short buttons);
    unsigned short WasPressed(unsigned short buttons);
};

extern AsciiInputView g_AsciiInput;
