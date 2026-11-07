#include "CardAttack.hpp"
#include "ZunMemory.hpp"

#include "EnemyManagerCardAttackSpawn.hpp"

// The original +0x10 per-side Enemy manager is the same native 0x38-byte
// GameManager side slot shared by setup, ECL, and exact Enemy spawn owners.
typedef char CardAttackBeginNativeEnemySideOwner[
    (offsetof(AsciiGameManagerSideView, enemyManager10) == 0x10 &&
     sizeof(AsciiGameManagerSideView) == 0x38 &&
     offsetof(AsciiGameManagerView, sides) == 0) ? 1 : -1];

extern ZunMemory g_ZunMemory;

int CardAttackBeginPl00Type0(CardAttack *cardAttack)
{
    CardAttackSpawnPosition position;
    cardAttack->extra1360 =
        g_ZunMemory.Alloc(0x2B8, ".\\pl\\pl00.cpp");
    static_cast<EnemyManagerCardAttackSpawnView *>(
        g_GameManager.sides[cardAttack->sideIndex00].enemyManager10)
        ->SpawnFromCardAttack(0, &position, 30, -2, 1000, 0);
    return 0;
}

int CardAttackBeginPl00Type1(CardAttack *cardAttack)
{
    CardAttackSpawnPosition position;
    cardAttack->extra1360 =
        g_ZunMemory.Alloc(0x2B8, ".\\pl\\pl00.cpp");
    static_cast<EnemyManagerCardAttackSpawnView *>(
        g_GameManager.sides[cardAttack->sideIndex00].enemyManager10)
        ->SpawnFromCardAttack(1, &position, 30, -2, 10000, 0);
    return 0;
}

int CardAttackBeginPl00Type2(CardAttack *cardAttack)
{
    CardAttackSpawnPosition position;
    cardAttack->extra1364 =
        g_ZunMemory.Alloc(0x2B8, ".\\pl\\pl00.cpp");
    static_cast<EnemyManagerCardAttackSpawnView *>(
        g_GameManager.sides[cardAttack->sideIndex00].enemyManager10)
        ->SpawnFromCardAttack(2, &position, 1400, -2, 1000, 0);
    return 0;
}

int CardAttackBeginPl01Type0(CardAttack *cardAttack)
{
    CardAttackSpawnPosition position;
    cardAttack->extra1360 =
        g_ZunMemory.Alloc(0x2B8, ".\\pl\\pl01.cpp");
    static_cast<EnemyManagerCardAttackSpawnView *>(
        g_GameManager.sides[cardAttack->sideIndex00].enemyManager10)
        ->SpawnFromCardAttack(0, &position, 30, -2, 1000, 0);
    return 0;
}

int CardAttackBeginPl01Type1(CardAttack *cardAttack)
{
    CardAttackSpawnPosition position;
    cardAttack->extra1360 =
        g_ZunMemory.Alloc(0x2B8, ".\\pl\\pl01.cpp");
    static_cast<EnemyManagerCardAttackSpawnView *>(
        g_GameManager.sides[cardAttack->sideIndex00].enemyManager10)
        ->SpawnFromCardAttack(1, &position, 30, -2, 10000, 0);
    return 0;
}

int CardAttackBeginPl01Type2(CardAttack *cardAttack)
{
    CardAttackSpawnPosition position;
    cardAttack->extra1364 =
        g_ZunMemory.Alloc(0x2B8, ".\\pl\\pl01.cpp");
    static_cast<EnemyManagerCardAttackSpawnView *>(
        g_GameManager.sides[cardAttack->sideIndex00].enemyManager10)
        ->SpawnFromCardAttack(2, &position, 1400, -2, 1000, 0);
    return 0;
}
