#include <stddef.h>

typedef unsigned short u16;

#include "TitleInputView.hpp"

typedef char InputRepeatAt04[
    (offsetof(InputView, repeatOutput) == 0x04) ? 1 : -1];
typedef char InputFrontStateAt06[
    (offsetof(InputView, word06) == 0x06) ? 1 : -1];

u16 InputView::IsPressedScrolling(int mask)
{
    u16 pressed = word06 & mask;
    return pressed != 0 || ((pressed = repeatOutput & mask) != 0);
}
