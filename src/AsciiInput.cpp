struct AsciiInputView
{
    unsigned short held00;
    unsigned char unknown02[4];
    unsigned short pressed06;

    unsigned short IsHeld(unsigned short buttons);
    unsigned short WasPressed(unsigned short buttons);
};

unsigned short AsciiInputView::IsHeld(unsigned short buttons)
{
    return held00 & buttons;
}

unsigned short AsciiInputView::WasPressed(unsigned short buttons)
{
    return pressed06 & buttons;
}
