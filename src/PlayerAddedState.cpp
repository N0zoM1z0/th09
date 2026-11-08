#include "PlayerLifecycleView.hpp"
#include "GameManagerRuntimeView.hpp"
#include "ZunTimer.hpp"

#include <stddef.h>
#include <string.h>

struct PlayerAddedAnmLoadedView
{
    void SetAndExecuteScriptIdx(PlayerAnmVmView *vm, int scriptIndex);
};

struct PlayerAddedStateSetterView
{
    int SetState(int state);
};

struct PlayerAddedRngView
{
    float SampleRange(float max);
};

struct PlayerAddedHeaderResetView
{
    unsigned char unknown00[0x5C];
    ZunTimer timer5C;
    ZunTimer timer68;
    unsigned int flags74;

    void Reset();
};
typedef char PlayerAddedHeaderResetSizeIs78[
    (sizeof(PlayerAddedHeaderResetView) == 0x78) ? 1 : -1];
// The independently exact Player registration and this update share the
// header subobject at Player +0x24, not another byte-offset allocation.
typedef char PlayerAddedNativeHeaderAt24[
    (offsetof(PlayerLifecycleView, header24) == 0x24) ? 1 : -1];

// The original 0x41D7E0 reset receiver is already the genuine native
// 0x44-byte PlayerOwnerStateView, not an extra owner/handle allocation.
// The old ReleaseHandle label at 0x40F860 actually names the exact
// AsciiGameManagerSideView::AddScore method on Player+0x0C sideState.
typedef char PlayerAddedRealNativeOwnerAt30410[
    (sizeof(PlayerOwnerStateView) == 0x44 &&
     offsetof(PlayerLifecycleView, ownerState30410) == 0x30410 &&
     offsetof(PlayerOwnerStateView, playerOwner00) == 0x00 &&
     offsetof(PlayerOwnerStateView, value0C) == 0x0C &&
     offsetof(PlayerOwnerStateView, maxValue10) == 0x10 &&
     offsetof(PlayerOwnerStateView, timer14) == 0x14 &&
     offsetof(PlayerLifecycleView, sideState) == 0x0C) ? 1 : -1];

struct PlayerAddedStateLayout
{
    int state00;
    unsigned char unknown004[0x04];
    int sideIndex08;
    unsigned char unknown00C[0x7C - 0x0C];
    float randomRange7C;
    unsigned char unknown080[0xA0 - 0x80];
    int initialStateA0[2];
    int sideStateA8;
    unsigned char unknown0AC[0xBC - 0xAC];
    PlayerAddedAnmLoadedView *anmFileBC;
    PlayerAnmVmView mainVmC0;
    int field364;
    unsigned char unknown368[0x1B74 - 0x368];
    ZunTimer timer1B74;
    unsigned int flags1B80;
    int addedMarker1B84;
    PlayerPositionView position1B88;
    unsigned char unknown1B94[0x1BA0 - 0x1B94];
    PlayerPositionView positionHistory1BA0[16];
    unsigned char unknown1C60[0x1CD8 - 0x1C60];
    unsigned char unknown1CD8[0x1CEC - 0x1CD8];
    unsigned char unknown1CEC[0x28BC - 0x1CEC];
    PlayerCollisionRegionCreateRegionView collisionRegions28BC[512];
    PlayerCollisionRegionCreateRegionView embeddedCollisionB0BC;
    PlayerCollisionRegionCreateRegionView *activeRegionsB100[514];
    PlayerCollisionRegionCreateRegionView *releasedRegionsB908[514];
    int activeRegionCountC110;
    int releasedRegionCountC114;
    unsigned char unknownC118[0xC11C - 0xC118];
    unsigned char unknownC11C[0x3031C - 0xC11C];
    unsigned char unknown3031C[0x30358 - 0x3031C];
    int field30358;
    unsigned char unknown3035C[0x30384 - 0x3035C];
    float scalar30384;
    float scalar30388;
    ZunTimer timer3038C;
    ZunTimer timer30398;
    ZunTimer timer303A4;
    ZunTimer timer303B0;
    ZunTimer timer303BC;
    ZunTimer timer303C8;
    ZunTimer timer303D4;
    unsigned char unknown303E0[0x08];
    int field303E8;
    unsigned char unknown303EC[0x30410 - 0x303EC];
    PlayerOwnerStateView ownerState30410;
    unsigned char tailStates30454[0xB10];
    unsigned char unknown30F64[0x0C];
};
typedef char PlayerAddedStateLayoutSizeIs30F70[
    (sizeof(PlayerAddedStateLayout) == 0x30F70) ? 1 : -1];
typedef char PlayerAddedRandomRangeAt7C[
    (offsetof(PlayerAddedStateLayout, randomRange7C) == 0x7C) ? 1 : -1];
typedef char PlayerAddedInitialStateAtA0[
    (offsetof(PlayerAddedStateLayout, initialStateA0) == 0xA0) ? 1 : -1];
typedef char PlayerAddedSideStateAtA8[
    (offsetof(PlayerAddedStateLayout, sideStateA8) == 0xA8) ? 1 : -1];
typedef char PlayerAddedPositionAt1B88[
    (offsetof(PlayerAddedStateLayout, position1B88) == 0x1B88) ? 1 : -1];
typedef char PlayerAddedCollisionAt28BC[
    (offsetof(PlayerAddedStateLayout, collisionRegions28BC) == 0x28BC) ? 1 : -1];
typedef char PlayerAddedActiveAtB100[
    (offsetof(PlayerAddedStateLayout, activeRegionsB100) == 0xB100) ? 1 : -1];
typedef char PlayerAddedReleasedAtB908[
    (offsetof(PlayerAddedStateLayout, releasedRegionsB908) == 0xB908) ? 1 : -1];
typedef char PlayerAddedShotsAtC11C[
    (offsetof(PlayerLifecycleView, shots) == 0xC11C) ? 1 : -1];
typedef char PlayerAddedOwnerAt30410[
    (offsetof(PlayerAddedStateLayout, ownerState30410) == 0x30410) ? 1 : -1];
typedef char PlayerAddedTailAt30454[
    (offsetof(PlayerAddedStateLayout, tailStates30454) == 0x30454) ? 1 : -1];

typedef char PlayerAddedStartYAt35C[(offsetof(AsciiGameManagerView, coordinate35C)==0x35C)?1:-1];
extern int g_PlayerAddedSideTable[];
extern int g_PlayerAddedClampValue;
extern int g_PlayerAddedModeSelector;
extern PlayerAddedRngView g_PlayerAddedRng;

int PlayerCollisionRegionCreateRegionView::Reset()
{
    memset(this, 0, sizeof(*this));
    this->interval34 = 1;
    return 0;
}

void PlayerAddedHeaderResetView::Reset()
{
    this->timer5C = 0;
    this->timer68 = 0;
    this->flags74 &= ~3u;
}

void PlayerOwnerStateView::Reset()
{
    PrepareReset();
    playerOwner00->sideState->AddScore(maxValue10);
    maxValue10 = 0;
    value0C = 0;
    timer14 = 0;
}

void PlayerLifecycleView::InitializeAddedState()
{
    PlayerAddedStateLayout *player = reinterpret_cast<PlayerAddedStateLayout *>(this);
    int i;

    player->anmFileBC->SetAndExecuteScriptIdx(&player->mainVmC0, 0);

    PlayerPositionView *position = reinterpret_cast<PlayerPositionView *>(
        this->position1B88.operator float *());
    position->x = -160.0f;
    // Preserve the two original scalar publications in this preheader.
    // Access qualification stays local to the actual Y and Z stores.
    static_cast<volatile float &>(position->y) = g_GameManager.coordinate35C - 64.0f;
    static_cast<volatile float &>(position->z) = 0.49f;

    for (i = 0; i < sizeof(this->collisionRegions28BC) /
                 sizeof(this->collisionRegions28BC[0]); ++i)
    {
        this->collisionRegions28BC[i].Reset();
        this->releasedB908[i] = &this->collisionRegions28BC[i];
        this->activeB100[i] = NULL;
    }
    this->collisionRegionActiveCountC110 = 0;
    this->collisionRegionReleasedCountC114 = 512;
    player->field30358 = 0;
    player->ownerState30410.activeFrameCounter3C = 0;

    reinterpret_cast<PlayerAddedStateSetterView *>(this)->SetState(5);
    player->timer303C8 = 120;
    player->addedMarker1B84 = 1;

    for (i = 0; i < 128; ++i)
        this->shots[i].state462 = 0;

    this->movementFields1CCC.speedMultiplier1CE0 = 1.0f;
    this->movementFields1CCC.speedMultiplier1CDC = 1.0f;
    this->movementFields1CCC.speedMultiplier1CE8 = 1.0f;
    this->movementFields1CCC.speedMultiplier1CE4 = 1.0f;

    for (i = 0; i < 16; ++i)
        player->positionHistory1BA0[i] = player->position1B88;

    unsigned int addedFlags = this->flags1B80;
    addedFlags &= ~1u;
    addedFlags |= 2u;
    player->field303E8 = 0;
    this->flags1B80 = addedFlags;
    player->timer303B0 = 6;

    float randomValue = g_PlayerAddedRng.SampleRange(400.0f) + 100.0f;
    player->randomRange7C = randomValue;
    if (randomValue >= 400.0f)
        player->randomRange7C = 400.0f;

    this->ownerState30410.Reset();
    player->timer1B74 = 0;
    this->scalar30384 = 0.0f;

    this->header24.state84 = g_PlayerAddedSideTable[this->sideIndex];
    int value = g_PlayerAddedClampValue;
    if (value > 2)
        value = 2;
    switch (g_PlayerAddedModeSelector)
    {
    case 0:
    case 1:
        {
            const int initialState = value + 1;
            this->header24.state7C = initialState;
            this->header24.state80 = initialState;
        }
        break;
    case 2:
        {
            const int initialState = value + 4;
            this->header24.state7C = initialState;
            this->header24.state80 = initialState;
        }
        break;
    case 3:
        {
            const int initialState = value + 8;
            this->header24.state7C = initialState;
            this->header24.state80 = initialState;
        }
        break;
    case 4:
        {
            const int initialState = value + 12;
            this->header24.state84 = 1;
            this->scalar30388 = 400.0f;
            this->header24.state7C = initialState;
            this->header24.state80 = initialState;
        }
        break;
    }
    if (this->header24.state84 == 1)
        this->flags1B80 |= 8u;

    player->timer303A4 = -1;
    this->movementFields1CCC.movementAngle1CD8 = -1.57079637f;
    this->focusEffect364 = NULL;
    reinterpret_cast<PlayerAddedHeaderResetView *>(&this->header24)->Reset();
    memset(player->tailStates30454, 0, sizeof(player->tailStates30454));
}
