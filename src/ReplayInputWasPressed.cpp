#include <stddef.h>

typedef unsigned short u16;

struct ReplayInputPressedView
{
    u16 currentInput00;
    u16 word02;
    u16 repeatOutput04;
    u16 frontState06;
    u16 word08;
    unsigned char unknown0A[0x28];
    u16 historyPressed32;

    u16 WasPressed(u16 mask);
};

typedef char ReplayInputPressedAt32[
    (offsetof(ReplayInputPressedView, historyPressed32) == 0x32) ? 1 : -1];

u16 ReplayInputPressedView::WasPressed(u16 mask)
{
    return historyPressed32 & mask;
}
