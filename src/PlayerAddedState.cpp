#include "PlayerLifecycleView.hpp"

#include <stddef.h>
#include <string.h>

struct PlayerAddedTimerView
{
    unsigned char storage[0x0C];
    void operator=(int value);
};
typedef char PlayerAddedTimerSizeIsC[(sizeof(PlayerAddedTimerView) == 0x0C) ? 1 : -1];

struct PlayerAddedAnmLoadedView
{
    void ExecuteAnmIdx(PlayerAnmVmView *vm, int scriptIndex);
};

struct PlayerAddedStateSetterView
{
    int SetState(int state);
};

struct PlayerAddedRngView
{
    float SampleRange(float max);
};

struct PlayerAddedCollisionRegionView
{
    unsigned char unknown00[0x34];
    int defaultValue34;
    unsigned char unknown38[0x0C];

    int Reset();
};
typedef char PlayerAddedCollisionRegionSizeIs44[
    (sizeof(PlayerAddedCollisionRegionView) == 0x44) ? 1 : -1];

struct PlayerAddedHeaderResetView
{
    unsigned char unknown00[0x5C];
    PlayerAddedTimerView timer5C;
    PlayerAddedTimerView timer68;
    unsigned int flags74;

    void Reset();
};
typedef char PlayerAddedHeaderResetSizeIs78[
    (sizeof(PlayerAddedHeaderResetView) == 0x78) ? 1 : -1];

struct PlayerAddedHandleManagerView
{
    void ReleaseHandle(void *handle);
};

struct PlayerAddedOwnerContextView
{
    unsigned char unknown00[0x0C];
    PlayerAddedHandleManagerView *manager0C;
};

struct PlayerAddedOwnerStateView
{
    PlayerAddedOwnerContextView *owner00;
    unsigned char unknown04[0x08];
    void *field0C;
    void *handle10;
    PlayerAddedTimerView timer14;
    unsigned char unknown20[0x1C];
    int activeFrameCounter3C;
    void *callback40;

    void Reset();
};
typedef char PlayerAddedOwnerStateSizeIs44[
    (sizeof(PlayerAddedOwnerStateView) == 0x44) ? 1 : -1];

struct PlayerAddedShotView
{
    unsigned char unknown000[0x462];
    short state462;
    unsigned char unknown464[0x20];
};
typedef char PlayerAddedShotSizeIs484[(sizeof(PlayerAddedShotView) == 0x484) ? 1 : -1];

struct PlayerAddedStateLayout
{
    int state00;
    unsigned char unknown004[0x04];
    int sideIndex08;
    unsigned char unknown00C[0xB0];
    PlayerAddedAnmLoadedView *anmFileBC;
    PlayerAnmVmView mainVmC0;
    int field364;
    unsigned char unknown368[0x1B74 - 0x368];
    PlayerAddedTimerView timer1B74;
    unsigned int flags1B80;
    int addedMarker1B84;
    PlayerPositionView position1B88;
    unsigned char unknown1B94[0x1BA0 - 0x1B94];
    PlayerPositionView positionHistory1BA0[16];
    unsigned char unknown1C60[0x1CD8 - 0x1C60];
    float baseShotAngle1CD8;
    float speedMultiplier1CDC;
    float speedMultiplier1CE0;
    float speedMultiplier1CE4;
    float speedMultiplier1CE8;
    unsigned char unknown1CEC[0x28BC - 0x1CEC];
    PlayerAddedCollisionRegionView collisionRegions28BC[512];
    unsigned char embeddedCollisionB0BC[0x44];
    PlayerAddedCollisionRegionView *activeRegionsB100[514];
    PlayerAddedCollisionRegionView *releasedRegionsB908[514];
    int activeRegionCountC110;
    int releasedRegionCountC114;
    unsigned char unknownC118[0xC11C - 0xC118];
    PlayerAddedShotView shotsC11C[128];
    unsigned char unknown3031C[0x30358 - 0x3031C];
    int field30358;
    unsigned char unknown3035C[0x30384 - 0x3035C];
    float scalar30384;
    float scalar30388;
    PlayerAddedTimerView timer3038C;
    PlayerAddedTimerView timer30398;
    PlayerAddedTimerView timer303A4;
    PlayerAddedTimerView timer303B0;
    PlayerAddedTimerView timer303BC;
    PlayerAddedTimerView timer303C8;
    PlayerAddedTimerView timer303D4;
    unsigned char unknown303E0[0x08];
    int field303E8;
    unsigned char unknown303EC[0x30410 - 0x303EC];
    PlayerAddedOwnerStateView ownerState30410;
    unsigned char tailStates30454[0xB10];
    unsigned char unknown30F64[0x0C];
};
typedef char PlayerAddedStateLayoutSizeIs30F70[
    (sizeof(PlayerAddedStateLayout) == 0x30F70) ? 1 : -1];
typedef char PlayerAddedPositionAt1B88[
    (offsetof(PlayerAddedStateLayout, position1B88) == 0x1B88) ? 1 : -1];
typedef char PlayerAddedCollisionAt28BC[
    (offsetof(PlayerAddedStateLayout, collisionRegions28BC) == 0x28BC) ? 1 : -1];
typedef char PlayerAddedActiveAtB100[
    (offsetof(PlayerAddedStateLayout, activeRegionsB100) == 0xB100) ? 1 : -1];
typedef char PlayerAddedReleasedAtB908[
    (offsetof(PlayerAddedStateLayout, releasedRegionsB908) == 0xB908) ? 1 : -1];
typedef char PlayerAddedShotsAtC11C[
    (offsetof(PlayerAddedStateLayout, shotsC11C) == 0xC11C) ? 1 : -1];
typedef char PlayerAddedOwnerAt30410[
    (offsetof(PlayerAddedStateLayout, ownerState30410) == 0x30410) ? 1 : -1];
typedef char PlayerAddedTailAt30454[
    (offsetof(PlayerAddedStateLayout, tailStates30454) == 0x30454) ? 1 : -1];

extern float g_PlayerAddedStartY;
extern int g_PlayerAddedSideTable[];
extern int g_PlayerAddedClampValue;
extern int g_PlayerAddedModeSelector;
extern PlayerAddedRngView g_PlayerAddedRng;
extern void PlayerAddedOwnerPrepareReset();

int PlayerAddedCollisionRegionView::Reset()
{
    memset(this, 0, sizeof(*this));
    this->defaultValue34 = 1;
    return 0;
}

void PlayerAddedHeaderResetView::Reset()
{
    this->timer5C = 0;
    this->timer68 = 0;
    this->flags74 &= ~3u;
}

void PlayerAddedOwnerStateView::Reset()
{
    PlayerAddedOwnerPrepareReset();
    this->owner00->manager0C->ReleaseHandle(this->handle10);
    this->handle10 = NULL;
    this->field0C = NULL;
    this->timer14 = 0;
}

void PlayerLifecycleView::InitializeAddedState()
{
    PlayerAddedStateLayout *player = reinterpret_cast<PlayerAddedStateLayout *>(this);
    int i;

    player->anmFileBC->ExecuteAnmIdx(&player->mainVmC0, 0);

    float *position = player->position1B88.operator float *();
    position[0] = -160.0f;
    position[1] = g_PlayerAddedStartY - 64.0f;
    position[2] = 0.49f;

    PlayerAddedCollisionRegionView **activeSlot = player->activeRegionsB100;
    PlayerAddedCollisionRegionView *region = player->collisionRegions28BC;
    for (i = 0; i < 512; ++i, ++activeSlot, ++region)
    {
        region->Reset();
        activeSlot[514] = region;
        *activeSlot = NULL;
    }
    player->activeRegionCountC110 = 0;
    player->releasedRegionCountC114 = 512;
    player->field30358 = 0;
    player->ownerState30410.activeFrameCounter3C = 0;

    reinterpret_cast<PlayerAddedStateSetterView *>(this)->SetState(5);
    player->timer303C8 = 120;
    player->addedMarker1B84 = 1;

    for (i = 0; i < 128; ++i)
        player->shotsC11C[i].state462 = 0;

    player->speedMultiplier1CE0 = 1.0f;
    player->speedMultiplier1CDC = 1.0f;
    player->speedMultiplier1CE8 = 1.0f;
    player->speedMultiplier1CE4 = 1.0f;

    for (i = 0; i < 16; ++i)
        player->positionHistory1BA0[i] = player->position1B88;

    unsigned int addedFlags = player->flags1B80;
    addedFlags &= ~1u;
    addedFlags |= 2u;
    player->field303E8 = 0;
    player->flags1B80 = addedFlags;
    player->timer303B0 = 6;

    float randomValue = g_PlayerAddedRng.SampleRange(400.0f) + 100.0f;
    *reinterpret_cast<float *>(reinterpret_cast<unsigned char *>(player) + 0x7C) = randomValue;
    if (randomValue >= 400.0f)
        *reinterpret_cast<float *>(reinterpret_cast<unsigned char *>(player) + 0x7C) = 400.0f;

    player->ownerState30410.Reset();
    player->timer1B74 = 0;
    player->scalar30384 = 0.0f;

    int *stateTable = reinterpret_cast<int *>(reinterpret_cast<unsigned char *>(player) + 0xA0);
    stateTable[2] = g_PlayerAddedSideTable[player->sideIndex08];
    int value = g_PlayerAddedClampValue;
    if (value > 2)
        value = 2;
    switch (g_PlayerAddedModeSelector)
    {
    case 0:
    case 1:
        value += 1;
        break;
    case 2:
        value += 4;
        break;
    case 3:
        value += 8;
        break;
    case 4:
        value += 12;
        stateTable[2] = 1;
        player->scalar30388 = 400.0f;
        break;
    default:
        goto afterMode;
    }
    stateTable[0] = value;
    stateTable[1] = value;
afterMode:
    if (stateTable[2] == 1)
        player->flags1B80 |= 8u;

    player->timer303A4 = -1;
    player->baseShotAngle1CD8 = -1.57079637f;
    player->field364 = 0;
    reinterpret_cast<PlayerAddedHeaderResetView *>(
        reinterpret_cast<unsigned char *>(player) + 0x24)->Reset();
    memset(player->tailStates30454, 0, sizeof(player->tailStates30454));
}
