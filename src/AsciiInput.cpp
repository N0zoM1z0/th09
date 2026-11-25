#include "AsciiInputView.hpp"

unsigned short AsciiInputView::IsHeld(unsigned short buttons)
{
    return held00 & buttons;
}

unsigned short AsciiInputView::WasPressed(unsigned short buttons)
{
    return pressed06 & buttons;
}
