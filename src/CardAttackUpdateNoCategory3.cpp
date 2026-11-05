#include "CardAttack.hpp"
#include "EnemyManager.hpp"

extern AsciiGameManagerView g_GameManager;

int CardAttackUpdateNoCategory3(CardAttack *cardAttack)
{
    return g_GameManager.sides[cardAttack->sideIndex00]
                   .enemyManager10
                   ->FindActiveEnemyBySideCategory(3) == NULL;
}
