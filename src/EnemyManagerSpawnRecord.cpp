#include "EnemyManagerCardAttackSpawn.hpp"
#include "EnemyManager.hpp"

struct EnemySpawnRecordEclManagerBoundary
{
    unsigned char storage[0x188];

    int InitializeSubroutine(void *context, short subroutineId);
    int RunEcl(EnemyView *enemy);
};
typedef char EnemySpawnRecordEclManagerBoundarySizeIs188[
    (sizeof(EnemySpawnRecordEclManagerBoundary) == 0x188) ? 1 : -1];

struct EnemySpawnRecordFlagBits
{
    unsigned int active : 1;
    unsigned int unknown01_14 : 14;
    unsigned int mirrorMovementX : 1;
    unsigned int unknown16_31 : 16;
};

struct EnemySpawnRecordView
{
    EnemyManagerView *manager00;
    unsigned char unknown004[0x1F8 - 0x004];
    unsigned int vmColor1F8;
    unsigned char unknown1FC[0x7F4 - 0x1FC];
    unsigned char mainContext7F4[0x22C];
    unsigned char unknownA20[0x2D74 - 0xA20];
    CardAttackSpawnPosition position2D74;
    unsigned char unknown2D80[0x2E48 - 0x2D80];
    int life2E48;
    int maxLife2E4C;
    int phase2E50;
    int score2E54;
    int sequenceIndex2E58;
    unsigned char unknown2E5C[0x2E70 - 0x2E5C];
    unsigned int displayColor2E70;
    unsigned char unknown2E74[0x335C - 0x2E74];
    int defeatTokenType335C;
    unsigned char unknown3360[0x337C - 0x3360];
    unsigned int flags337C;
    unsigned char unknown3380[0x5430 - 0x3380];
};
typedef char EnemySpawnRecordViewSizeIs5430[
    (sizeof(EnemySpawnRecordView) == 0x5430) ? 1 : -1];

struct EnemySpawnRecordManagerView
{
    EnemySpawnRecordEclManagerBoundary primaryEcl000;
    EnemySpawnRecordEclManagerBoundary opposingEcl188;
    unsigned char unknown310[0x328 - 0x310];
    EnemySpawnRecordView spawnTemplate328;
    EnemySpawnRecordView enemies5758[129];
    unsigned char unknown2AC388[0x2AC42C - 0x2AC388];
    int spawnOverflow2AC42C;
};

void *EnemyManagerCardAttackSpawnView::SpawnEnemyRecord(
    short subroutine,
    CardAttackSpawnPosition *position,
    int life,
    char defeatTokenType,
    int score,
    int mirrorMovementX,
    int useOpposingEclManager)
{
    EnemySpawnRecordManagerView *manager =
        reinterpret_cast<EnemySpawnRecordManagerView *>(this);
    int enemyIndex;
    EnemySpawnRecordView *enemy;

    enemy = &manager->enemies5758[0];
    for (enemyIndex = 0; enemyIndex < 128; ++enemyIndex, ++enemy)
    {
        if ((enemy->flags337C & 1) != 0)
            continue;

        *enemy = manager->spawnTemplate328;
        enemy->sequenceIndex2E58 = enemyIndex;
        reinterpret_cast<EnemySpawnRecordFlagBits *>(&enemy->flags337C)
            ->mirrorMovementX = mirrorMovementX;

        if (life >= 0)
            enemy->life2E48 = life;

        enemy->position2D74 = *position;
        if (mirrorMovementX)
            enemy->position2D74.x = -enemy->position2D74.x;

        if (useOpposingEclManager == 0)
        {
            manager->primaryEcl000.InitializeSubroutine(
                enemy->mainContext7F4,
                subroutine);
            if (manager->primaryEcl000.RunEcl(
                    reinterpret_cast<EnemyView *>(enemy)) == -1)
            {
                enemy->flags337C &= ~1U;
                enemyIndex = 128;
                break;
            }
        }
        else
        {
            manager->opposingEcl188.InitializeSubroutine(
                enemy->mainContext7F4,
                subroutine);
            if (manager->opposingEcl188.RunEcl(
                    reinterpret_cast<EnemyView *>(enemy)) == -1)
            {
                enemy->flags337C &= ~1U;
                enemyIndex = 128;
                break;
            }
        }

        enemy->displayColor2E70 = enemy->vmColor1F8;
        enemy->defeatTokenType335C = defeatTokenType;
        if (score >= 0)
            enemy->score2E54 = score;
        enemy->maxLife2E4C = enemy->life2E48;
        enemy->phase2E50 = enemy->life2E48;
        break;
    }

    manager->spawnOverflow2AC42C = enemyIndex == 128;
    return enemy;
}
