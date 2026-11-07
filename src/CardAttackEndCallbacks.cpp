#include "CardAttack.hpp"
#include "ZunMemory.hpp"

extern ZunMemory g_ZunMemory;

int CardAttackEndFree1360(CardAttack *cardAttack)
{
    if (cardAttack->extra1360 != 0)
        g_ZunMemory.Free(cardAttack->extra1360);
    cardAttack->extra1360 = 0;
    return 0;
}

int CardAttackEndFree1364(CardAttack *cardAttack)
{
    if (cardAttack->extra1364 != 0)
        g_ZunMemory.Free(cardAttack->extra1364);
    cardAttack->extra1364 = 0;
    return 0;
}
