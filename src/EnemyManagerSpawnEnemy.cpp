#include "EnemyManager.hpp"

#include <string.h>

struct EnemySpawnEclManagerBoundary
{
    unsigned char storage[0x188];

    int InitializeSubroutine(void *context, short subroutineId);
    int RunEcl(EnemyView *enemy);
};
typedef char EnemySpawnEclManagerBoundarySizeIs188[
    (sizeof(EnemySpawnEclManagerBoundary) == 0x188) ? 1 : -1];

struct EnemySpawnRecordView
{
    EnemyManagerView *manager00;
    unsigned char unknown004[0x1F8 - 0x004];
    unsigned int vmColor1F8;
    unsigned char unknown1FC[0x7F4 - 0x1FC];
    unsigned char mainContext7F4[0x22C];
    unsigned char unknownA20[0x2D74 - 0xA20];
    EnemyFloat3 position2D74;
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
typedef char EnemySpawnRecordSizeIs5430[
    (sizeof(EnemySpawnRecordView) == 0x5430) ? 1 : -1];

struct EnemySpawnManagerView
{
    EnemySpawnEclManagerBoundary primaryEcl000;
    EnemySpawnEclManagerBoundary opposingEcl188;
    unsigned char unknown310[0x328 - 0x310];
    EnemySpawnRecordView spawnTemplate328;
    EnemySpawnRecordView enemies5758[129];
    unsigned char unknown2AC388[0x2AC42C - 0x2AC388];
    int spawnOverflow2AC42C;
};

EnemyView *EnemyManagerView::SpawnEnemy(
    short eclSubroutineId,
    EnemyFloat3 *position,
    int life,
    signed char defeatTokenType,
    int score,
    int *initialVariables,
    int useOpposingEclManager)
{
    EnemySpawnManagerView *manager =
        reinterpret_cast<EnemySpawnManagerView *>(this);
    EnemySpawnRecordView *enemy = &manager->enemies5758[0];
    int enemyIndex = 0;

    while ((enemy->flags337C & 1) != 0)
    {
        ++enemyIndex;
        ++enemy;
        if (enemyIndex >= 128)
            break;
    }

    if (enemyIndex < 128)
    {
        *enemy = manager->spawnTemplate328;
        enemy->sequenceIndex2E58 = enemyIndex;

        if (life >= 0)
            enemy->life2E48 = life;

        enemy->position2D74 = *position;

        int runResult;
        if (useOpposingEclManager == 0)
        {
            manager->primaryEcl000.InitializeSubroutine(
                enemy->mainContext7F4,
                eclSubroutineId);
            memcpy(
                enemy->mainContext7F4 + 0x1C,
                initialVariables,
                0x78);
            runResult = manager->primaryEcl000.RunEcl(
                reinterpret_cast<EnemyView *>(enemy));
        }
        else
        {
            manager->opposingEcl188.InitializeSubroutine(
                enemy->mainContext7F4,
                eclSubroutineId);
            memcpy(
                enemy->mainContext7F4 + 0x1C,
                initialVariables,
                0x78);
            runResult = manager->opposingEcl188.RunEcl(
                reinterpret_cast<EnemyView *>(enemy));
        }

        if (runResult == -1)
        {
            enemy->flags337C &= ~1U;
            enemyIndex = 128;
        }
        else
        {
            enemy->displayColor2E70 = enemy->vmColor1F8;
            enemy->defeatTokenType335C = defeatTokenType;

            if (life >= 0)
                enemy->life2E48 = life;
            if (score >= 0)
                enemy->score2E54 = score;

            enemy->maxLife2E4C = enemy->life2E48;
            enemy->phase2E50 = enemy->life2E48;
        }
    }

    manager->spawnOverflow2AC42C = enemyIndex == 128;
    return reinterpret_cast<EnemyView *>(enemy);
}
