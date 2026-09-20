#include "CardAttack.hpp"
#include "ZunMemory.hpp"

struct CardAttackSpawnPosition
{
    float x;
    float y;
    float z;
};

struct EnemyManagerCardAttackSpawnView
{
    void *SpawnFromCardAttack(
        int subroutine,
        CardAttackSpawnPosition *position,
        int valueA,
        int valueB,
        int valueC,
        int mirrored);
};

struct CardAttackBeginSideView
{
    unsigned char unknown00[0x10];
    EnemyManagerCardAttackSpawnView *enemyManager10;
    unsigned char unknown14[0x38 - 0x14];
};

struct CardAttackBeginGameManagerView
{
    CardAttackBeginSideView sides[2];
};

struct CardAttackBeginTailView
{
    unsigned char unknown0000[0x1360];
    void *extra1360;
    void *extra1364;
};

extern CardAttackBeginGameManagerView g_GameManager;
extern ZunMemory g_ZunMemory;

int CardAttackBeginPl00Type0(CardAttack *cardAttack)
{
    CardAttackSpawnPosition position;
    reinterpret_cast<CardAttackBeginTailView *>(cardAttack)->extra1360 =
        g_ZunMemory.Alloc(0x2B8, ".\\pl\\pl00.cpp");
    g_GameManager.sides[cardAttack->sideIndex00]
        .enemyManager10
        ->SpawnFromCardAttack(0, &position, 30, -2, 1000, 0);
    return 0;
}

int CardAttackBeginPl00Type1(CardAttack *cardAttack)
{
    CardAttackSpawnPosition position;
    reinterpret_cast<CardAttackBeginTailView *>(cardAttack)->extra1360 =
        g_ZunMemory.Alloc(0x2B8, ".\\pl\\pl00.cpp");
    g_GameManager.sides[cardAttack->sideIndex00]
        .enemyManager10
        ->SpawnFromCardAttack(1, &position, 30, -2, 10000, 0);
    return 0;
}

int CardAttackBeginPl00Type2(CardAttack *cardAttack)
{
    CardAttackSpawnPosition position;
    reinterpret_cast<CardAttackBeginTailView *>(cardAttack)->extra1364 =
        g_ZunMemory.Alloc(0x2B8, ".\\pl\\pl00.cpp");
    g_GameManager.sides[cardAttack->sideIndex00]
        .enemyManager10
        ->SpawnFromCardAttack(2, &position, 1400, -2, 1000, 0);
    return 0;
}

int CardAttackBeginPl01Type0(CardAttack *cardAttack)
{
    CardAttackSpawnPosition position;
    reinterpret_cast<CardAttackBeginTailView *>(cardAttack)->extra1360 =
        g_ZunMemory.Alloc(0x2B8, ".\\pl\\pl01.cpp");
    g_GameManager.sides[cardAttack->sideIndex00]
        .enemyManager10
        ->SpawnFromCardAttack(0, &position, 30, -2, 1000, 0);
    return 0;
}

int CardAttackBeginPl01Type1(CardAttack *cardAttack)
{
    CardAttackSpawnPosition position;
    reinterpret_cast<CardAttackBeginTailView *>(cardAttack)->extra1360 =
        g_ZunMemory.Alloc(0x2B8, ".\\pl\\pl01.cpp");
    g_GameManager.sides[cardAttack->sideIndex00]
        .enemyManager10
        ->SpawnFromCardAttack(1, &position, 30, -2, 10000, 0);
    return 0;
}

int CardAttackBeginPl01Type2(CardAttack *cardAttack)
{
    CardAttackSpawnPosition position;
    reinterpret_cast<CardAttackBeginTailView *>(cardAttack)->extra1364 =
        g_ZunMemory.Alloc(0x2B8, ".\\pl\\pl01.cpp");
    g_GameManager.sides[cardAttack->sideIndex00]
        .enemyManager10
        ->SpawnFromCardAttack(2, &position, 1400, -2, 1000, 0);
    return 0;
}
