#pragma once

#include "Chain.hpp"

#include <stddef.h>

struct PlayerSideStateView
{
    unsigned char unknown00[0x04];
    void *manager04;
    unsigned char unknown08[0x08];
    void *attackTarget10;
    void *attackOwner14;
    void *frontSide18;
    unsigned char unknown1C[0x04];
    int shotType20;
    int previousShotType24;
    unsigned char unknown28[0x04];
    int characterIndex2C;
    unsigned char unknown30[0x04];
    unsigned int flags34;
};

typedef char PlayerSideStateSizeIs38[(sizeof(PlayerSideStateView) == 0x38) ? 1 : -1];

struct PlayerShtDescriptorView
{
    short fireInterval;
    unsigned char unknown02[0x26];
    void *spawnCallback;
    void *updateCallback;
    void *drawCallback;
    void *collisionCallback;
};

typedef char PlayerShtDescriptorSizeIs38[(sizeof(PlayerShtDescriptorView) == 0x38) ? 1 : -1];

struct PlayerShtPowerLevelView
{
    PlayerShtDescriptorView *descriptors;
    int unknown04;
};

typedef char PlayerShtPowerLevelSizeIs8[(sizeof(PlayerShtPowerLevelView) == 8) ? 1 : -1];

struct PlayerShtFileView
{
    unsigned char unknown00[0x02];
    unsigned short shotPowerLevelCount;
    float hurtboxSize;
    float grazeBoxSize;
    unsigned char unknown0C[0x04];
    float itemCollectionBoxSize;
    float sizeSource0;
    float sizeSource1;
    float sizeDerived0;
    float sizeDerived1;
    unsigned char unknown24[0x08];
    const char *modePath0;
    unsigned char unknown30[0x3C];
    const char *modePath1;
    unsigned char unknown70[0x3C];
    const char *modePath2;
    unsigned char unknownB0[0x42C - 0xB0];
    PlayerShtPowerLevelView shotPowerLevels[1];
};

typedef char PlayerShtLevelsAt42C[(offsetof(PlayerShtFileView, shotPowerLevels) == 0x42C) ? 1 : -1];

struct PlayerPositionView
{
    float x;
    float y;
    float z;
};

typedef char PlayerPositionSizeIsC[(sizeof(PlayerPositionView) == 0x0C) ? 1 : -1];
typedef PlayerPositionView PlayerHalfSizeView;

struct PlayerHeaderStateView
{
    unsigned char unknown00[0x78];
    void *selfLink78;
    int state7C;
    int state80;
    int state84;
    int state88;
    int state8C;
    int state90;
    int state94;
    void *anmFile98;

    PlayerHeaderStateView();
};
typedef char PlayerHeaderStateSizeIs9C[(sizeof(PlayerHeaderStateView) == 0x9C) ? 1 : -1];

struct PlayerAnmVmView
{
    unsigned char unknown000[0x208];
    PlayerPositionView position208;
    unsigned char unknown214[0x2A4 - 0x214];

    PlayerAnmVmView();
};
typedef char PlayerAnmVmSizeIs2A4[(sizeof(PlayerAnmVmView) == 0x2A4) ? 1 : -1];

struct PlayerHistoryBlockView
{
    void *selfLink00;
    unsigned char unknown004[0x1800];

    PlayerHistoryBlockView();
};
typedef char PlayerHistoryBlockSizeIs1804[(sizeof(PlayerHistoryBlockView) == 0x1804) ? 1 : -1];

struct PlayerTimerCtorView
{
    unsigned char storage[0x0C];
    PlayerTimerCtorView();
};
typedef char PlayerTimerCtorSizeIsC[(sizeof(PlayerTimerCtorView) == 0x0C) ? 1 : -1];

struct PlayerConstructedVector3View
{
    float x;
    float y;
    float z;
    PlayerConstructedVector3View();
};
typedef char PlayerConstructedVector3SizeIsC[(sizeof(PlayerConstructedVector3View) == 0x0C) ? 1 : -1];

struct PlayerOptionStateCtorView
{
    unsigned char storage[0x2F4];
    PlayerOptionStateCtorView();
};
typedef char PlayerOptionStateCtorSizeIs2F4[(sizeof(PlayerOptionStateCtorView) == 0x2F4) ? 1 : -1];

struct PlayerShotCtorView
{
    unsigned char storage[0x484];
    PlayerShotCtorView();
};
typedef char PlayerShotCtorSizeIs484[(sizeof(PlayerShotCtorView) == 0x484) ? 1 : -1];

struct PlayerState3031CView
{
    unsigned char storage[0x1C];
    PlayerState3031CView();
};
typedef char PlayerState3031CSizeIs1C[(sizeof(PlayerState3031CView) == 0x1C) ? 1 : -1];

struct PlayerOwnerStateView
{
    void *owner00;
    unsigned char unknown04[0x38];
    int activeFrameCounter3C;
    void *callback40;

    PlayerOwnerStateView();
};
typedef char PlayerOwnerStateSizeIs44[(sizeof(PlayerOwnerStateView) == 0x44) ? 1 : -1];

struct PlayerTailStateCtorView
{
    unsigned char storage[0x2C4];
    PlayerTailStateCtorView();
};
typedef char PlayerTailStateCtorSizeIs2C4[(sizeof(PlayerTailStateCtorView) == 0x2C4) ? 1 : -1];

struct PlayerLifecycleView
{
    unsigned char unknown0000[0x08];
    int sideIndex;
    PlayerSideStateView *sideState;
    PlayerSideStateView *opponentState;
    ChainElem *calcChain;
    ChainElem *drawChainHighPrio;
    ChainElem *drawChainLowPrio;
    int selector;
    PlayerHeaderStateView header24;
    PlayerAnmVmView mainVm;
    unsigned char unknown0364[0x08];
    PlayerHistoryBlockView history36C;
    int transient1B70;
    PlayerTimerCtorView timer1B74;
    unsigned int flags1B80;
    unsigned int unknown1B84;
    PlayerPositionView position1B88;
    PlayerPositionView position1B94;
    PlayerConstructedVector3View positions1BA0[16];
    unsigned char unknown1C60[0x1CA8 - 0x1C60];
    PlayerHalfSizeView hurtboxHalfSize;
    PlayerHalfSizeView grazeHalfSize;
    PlayerHalfSizeView itemCollectionHalfSize;
    unsigned char unknown1CCC[0x1CEC - 0x1CCC];
    PlayerOptionStateCtorView optionStates[4];
    unsigned char unknown28BC[0xC11C - 0x28BC];
    PlayerShotCtorView shots[128];
    PlayerState3031CView state3031C;
    PlayerShtFileView *primaryShtFile;
    unsigned char unknown3033C[0x30384 - 0x3033C];
    float scalar30384;
    float scalar30388;
    PlayerTimerCtorView timer3038C;
    PlayerTimerCtorView timer30398;
    PlayerTimerCtorView timer303A4;
    PlayerTimerCtorView timer303B0;
    PlayerTimerCtorView timer303BC;
    PlayerTimerCtorView timer303C8;
    PlayerTimerCtorView timer303D4;
    unsigned char unknown303E0[0x303F0 - 0x303E0];
    int tableValue303F0;
    int tableValue303F4;
    int tableValue303F8;
    int tableValue303FC;
    int tableValue30400;
    void *callback30404;
    void *callback30408;
    unsigned char unknown3040C[0x04];
    PlayerOwnerStateView ownerState30410;
    PlayerTailStateCtorView tailStates30454[4];
    unsigned char unknown30F64[0x30F70 - 0x30F64];

    PlayerLifecycleView();
    void InitializeType(unsigned char playerType);
    void InitializeAddedState();
    void UpdateBeforeState();
    int GetUpdateState();
    void UpdateStateValue();
    void SetBombMode0();
    void SetBombMode1();
    void SetBombMode2();
    void UpdateAfterAnimation();
    void UpdateHeavyState();
    void DrawActiveShots();
    void DrawOptionStates();
    void DrawHitShots();

    static int __fastcall OnUpdate(PlayerLifecycleView *player);
    static int __fastcall AddedCallback(PlayerLifecycleView *player);
    static int __fastcall OnDrawHighPrio(PlayerLifecycleView *player);
    static int __fastcall OnDrawLowPrio(PlayerLifecycleView *player);
};

typedef char PlayerLifecycleSizeIs30F70[(sizeof(PlayerLifecycleView) == 0x30F70) ? 1 : -1];
typedef char PlayerLifecycleSideIndexAt08[(offsetof(PlayerLifecycleView, sideIndex) == 0x08) ? 1 : -1];
typedef char PlayerLifecycleHeaderAt24[(offsetof(PlayerLifecycleView, header24) == 0x24) ? 1 : -1];
typedef char PlayerLifecycleMainVmAtC0[(offsetof(PlayerLifecycleView, mainVm) == 0xC0) ? 1 : -1];
typedef char PlayerLifecycleHistoryAt36C[(offsetof(PlayerLifecycleView, history36C) == 0x36C) ? 1 : -1];
typedef char PlayerLifecycleFlagsAt1B80[(offsetof(PlayerLifecycleView, flags1B80) == 0x1B80) ? 1 : -1];
typedef char PlayerLifecyclePositionAt1B88[(offsetof(PlayerLifecycleView, position1B88) == 0x1B88) ? 1 : -1];
typedef char PlayerLifecycleOptionsAt1CEC[(offsetof(PlayerLifecycleView, optionStates) == 0x1CEC) ? 1 : -1];
typedef char PlayerLifecycleShotsAtC11C[(offsetof(PlayerLifecycleView, shots) == 0xC11C) ? 1 : -1];
typedef char PlayerLifecycleState3031CAt3031C[(offsetof(PlayerLifecycleView, state3031C) == 0x3031C) ? 1 : -1];
typedef char PlayerLifecyclePrimaryShtAt30338[(offsetof(PlayerLifecycleView, primaryShtFile) == 0x30338) ? 1 : -1];
typedef char PlayerLifecycleTimer303C8At303C8[(offsetof(PlayerLifecycleView, timer303C8) == 0x303C8) ? 1 : -1];
typedef char PlayerLifecycleOwnerAt30410[(offsetof(PlayerLifecycleView, ownerState30410) == 0x30410) ? 1 : -1];
typedef char PlayerLifecycleTailStatesAt30454[(offsetof(PlayerLifecycleView, tailStates30454) == 0x30454) ? 1 : -1];
