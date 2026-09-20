#include "CardAttack.hpp"
#include "EnemyManager.hpp"

struct CardAttackEnemySideView
{
    unsigned char unknown00[0x10];
    EnemyManagerView *enemyManager10;
    unsigned char unknown14[0x38 - 0x14];
};

struct CardAttackEnemyGameManagerView
{
    CardAttackEnemySideView sides[2];
};

extern CardAttackEnemyGameManagerView g_GameManager;

int CardAttackUpdateNoCategory3(CardAttack *cardAttack)
{
    return g_GameManager.sides[cardAttack->sideIndex00]
                   .enemyManager10
                   ->FindActiveEnemyBySideCategory(3) == NULL;
}
