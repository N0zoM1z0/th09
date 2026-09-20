#include <stddef.h>

struct GameManagerValueCCView
{
    unsigned char unknown00[0x1C];
    int value1C;

    void SubtractSaturating(int amount);
};

typedef char GameManagerValueCCAt1C[
    (offsetof(GameManagerValueCCView, value1C) == 0x1C) ? 1 : -1];

void GameManagerValueCCView::SubtractSaturating(int amount)
{
    value1C -= amount;
    if (value1C < 0)
        value1C = 0;
}
