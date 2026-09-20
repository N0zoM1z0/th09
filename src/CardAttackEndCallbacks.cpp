#include "CardAttack.hpp"
#include "ZunMemory.hpp"

struct CardAttackEndTailView
{
    unsigned char unknown0000[0x1360];
    void *extra1360;
    void *extra1364;
};

extern ZunMemory g_ZunMemory;

int CardAttackEndFree1360(CardAttack *cardAttack)
{
    CardAttackEndTailView *view =
        reinterpret_cast<CardAttackEndTailView *>(cardAttack);
    if (view->extra1360 != 0)
        g_ZunMemory.Free(view->extra1360);
    view->extra1360 = 0;
    return 0;
}

int CardAttackEndFree1364(CardAttack *cardAttack)
{
    CardAttackEndTailView *view =
        reinterpret_cast<CardAttackEndTailView *>(cardAttack);
    if (view->extra1364 != 0)
        g_ZunMemory.Free(view->extra1364);
    view->extra1364 = 0;
    return 0;
}
