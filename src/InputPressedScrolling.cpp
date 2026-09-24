#include <stddef.h>

typedef unsigned short u16;

struct InputView
{
    u16 held00;
    u16 previous02;
    u16 repeatOutput04;
    u16 frontState06;

    u16 IsPressedScrolling(int mask);
};

typedef char InputRepeatAt04[
    (offsetof(InputView, repeatOutput04) == 0x04) ? 1 : -1];
typedef char InputFrontStateAt06[
    (offsetof(InputView, frontState06) == 0x06) ? 1 : -1];

u16 InputView::IsPressedScrolling(int mask)
{
    u16 pressed = frontState06 & mask;
    return pressed != 0 || ((pressed = repeatOutput04 & mask) != 0);
}
