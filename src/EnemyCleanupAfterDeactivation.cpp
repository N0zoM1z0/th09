#include <stddef.h>

struct EnemyView;
struct EnemyCleanupPlayerView
{
    unsigned char unknown00000[0x3037C];
    EnemyView *homingTarget3037C;
};
struct EnemyCleanupSideView
{
    unsigned char unknown00[4];
    EnemyCleanupPlayerView *player04;
};
struct EnemyCleanupManagerView
{
    unsigned char unknown000000[0x320];
    EnemyCleanupSideView *sideState320;
    unsigned char unknown000324[0x2AC388 - 0x324];
    EnemyView *remoteSlots2AC388[9];
};
typedef char EnemyCleanupManagerSideAt320[
    (offsetof(EnemyCleanupManagerView, sideState320)==0x320)?1:-1];
typedef char EnemyCleanupManagerRemoteAt2AC388[
    (offsetof(EnemyCleanupManagerView, remoteSlots2AC388)==0x2AC388)?1:-1];

struct EnemyCleanupView
{
    EnemyCleanupManagerView *manager00;
    unsigned char unknown0004[0x336B - 0x04];
    unsigned char bossSlot336B;
    unsigned char unknown336C[0x10];
    unsigned int flags337C;
    unsigned char unknown3380[0x30];
    int lifeCallbackThresholds33B0[4];
    unsigned char unknown33C0[0x10];
    int timerCallbackThreshold33D0;
    unsigned char unknown33D4[0x2040];
    int attachedEffectCount5414;
};
typedef char EnemyCleanupBossSlotAt336B[
    (offsetof(EnemyCleanupView,bossSlot336B)==0x336B)?1:-1];
typedef char EnemyCleanupFlagsAt337C[
    (offsetof(EnemyCleanupView,flags337C)==0x337C)?1:-1];
typedef char EnemyCleanupLifeAt33B0[
    (offsetof(EnemyCleanupView,lifeCallbackThresholds33B0)==0x33B0)?1:-1];
typedef char EnemyCleanupTimerAt33D0[
    (offsetof(EnemyCleanupView,timerCallbackThreshold33D0)==0x33D0)?1:-1];
typedef char EnemyCleanupEffectCountAt5414[
    (offsetof(EnemyCleanupView,attachedEffectCount5414)==0x5414)?1:-1];

namespace Th09EclRunState
{
void __fastcall ReleaseAttachedEffects(EnemyView *enemy);
}

struct EnemyCoreView;
void __fastcall EnemyCoreReleaseChildEclBlocks(EnemyCoreView *enemy);

struct EnemyView
{
    unsigned char storage[0x5430];

    void CleanupAfterDeactivation();
};

void EnemyView::CleanupAfterDeactivation()
{
    EnemyCleanupView *enemy = reinterpret_cast<EnemyCleanupView *>(this);

    unsigned int flags = enemy->flags337C;
    if ((flags & 0x000E0000u) == 0)
        enemy->flags337C = flags & ~1u;

    if ((enemy->flags337C & 2u) != 0)
    {
        unsigned char slot = enemy->bossSlot336B;
        if (slot < 4)
        {
            enemy->manager00->remoteSlots2AC388[slot] = 0;
            enemy->flags337C &= ~2u;
        }
    }

    if (enemy->attachedEffectCount5414 != 0)
        Th09EclRunState::ReleaseAttachedEffects(this);

    if ((enemy->flags337C & 2u) != 0)
        enemy->manager00->remoteSlots2AC388[enemy->bossSlot336B] = 0;

    enemy->lifeCallbackThresholds33B0[0] = -1;
    enemy->lifeCallbackThresholds33B0[1] = -1;
    enemy->lifeCallbackThresholds33B0[2] = -1;
    enemy->lifeCallbackThresholds33B0[3] = -1;
    enemy->timerCallbackThreshold33D0 = -1;

    EnemyCoreReleaseChildEclBlocks(reinterpret_cast<EnemyCoreView *>(enemy));

    EnemyCleanupPlayerView *player =
        enemy->manager00->sideState320->player04;
    if (player->homingTarget3037C == this)
        player->homingTarget3037C = 0;
}
