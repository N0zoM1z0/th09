#include "CardAttack.hpp"

int CardAttackUpdateAfter192(CardAttack *cardAttack)
{
    return (cardAttack->timer98 > 192) != 0;
}
