#pragma once

#include "Chain.hpp"
#include "ZunTimer.hpp"

#include <stddef.h>

struct EffectManager;
struct Effect;
typedef int (__fastcall *EffectCallback)(Effect *effect);
typedef void (__fastcall *EffectDrawCallback)(Effect *effect);

struct EffectFloat3
{
    float x;
    float y;
    float z;
    EffectFloat3 *Add(const EffectFloat3 *other);
};
typedef char EffectFloat3SizeIs0C[(sizeof(EffectFloat3) == 0x0C) ? 1 : -1];

struct EffectVmView
{
    void Initialize(int scriptIndex);
    unsigned char unknown000[0x1F0];
    unsigned int color;
    unsigned char unknown1F4[0x1F8 - 0x1F4];
    unsigned int flags;
    unsigned char unknown1FC[0x208 - 0x1FC];
    EffectFloat3 position;
    unsigned char unknown214[0x288 - 0x214];
    EffectFloat3 pos2;
    unsigned char unknown294[0x2A4 - 0x294];
};
typedef char EffectVmViewSizeIs2A4[(sizeof(EffectVmView) == 0x2A4) ? 1 : -1];

struct Effect
{
    Effect();
    EffectManager *manager;
    EffectVmView *vms;
    int vmCount;
    EffectFloat3 position;
    EffectFloat3 velocity;
    unsigned char unknown024[0x7C - 0x24];
    int value7C;
    int value80;
    unsigned char unknown084[0x88 - 0x84];
    int value88;
    int value8C;
    int fixedSlotIndex;
    int value94;
    int value98;
    float value9C;
    unsigned char unknown0A0[0xAC - 0xA0];
    ZunTimer timer;
    unsigned char unknown0B8[0xBC - 0xB8];
    EffectCallback updateCallback;
    EffectDrawCallback drawCallback;
    unsigned char active;
    unsigned char effectId;
    unsigned char unknown0C6[2];
    unsigned char alternateDrawGroup;
    unsigned char unknown0C9;
    unsigned char backgroundDrawGroup;
    unsigned char unknown0CB;
    void *vertices;
    unsigned char suppressDraw;
    unsigned char unknown0D1[3];
    Effect *nextInDrawGroup;
};
typedef char EffectSizeIsD8[(sizeof(Effect) == 0xD8) ? 1 : -1];
typedef char EffectValue7CAt7C[(offsetof(Effect, value7C) == 0x7C) ? 1 : -1];
typedef char EffectFixedSlotIndexAt90[(offsetof(Effect, fixedSlotIndex) == 0x90) ? 1 : -1];
typedef char EffectValue9CAt9C[(offsetof(Effect, value9C) == 0x9C) ? 1 : -1];
typedef char EffectTimerAtAC[(offsetof(Effect, timer) == 0xAC) ? 1 : -1];
typedef char EffectUpdateAtBC[(offsetof(Effect, updateCallback) == 0xBC) ? 1 : -1];
typedef char EffectActiveAtC4[(offsetof(Effect, active) == 0xC4) ? 1 : -1];
typedef char EffectVerticesAtCC[(offsetof(Effect, vertices) == 0xCC) ? 1 : -1];
typedef char EffectNextAtD4[(offsetof(Effect, nextInDrawGroup) == 0xD4) ? 1 : -1];

struct EffectManager
{
    EffectManager();
    void Clear();
    int ResetPool();
    Effect *InitializeEffect(Effect *effect, int effectId, const EffectFloat3 *position,
                             unsigned int color, const EffectFloat3 *velocity);
    Effect *SpawnEffect(int effectId, const EffectFloat3 *position, int count, unsigned int color);
    Effect *SpawnEffectWithVelocity(int effectId, const EffectFloat3 *position,
                                    const EffectFloat3 *velocity, int count, unsigned int color);
    Effect *SpawnEffectInFixedSlot(int effectId, const EffectFloat3 *position,
                                   int slotIndex, unsigned int color);
    int ReleasePool();
    static int OnUpdate(EffectManager *effectManager);
    static int OnDraw(EffectManager *effectManager);
    static int OnDrawShared(EffectManager *effectManager);
    static int AddedCallback(EffectManager *effectManager);
    static void Release(EffectManager *effectManager);
    static EffectManager *Create(int sideIndex, int primaryCount, int secondaryCount);

    int nextEffectIndex;
    int unknown004;
    int activeCount;
    int sideIndex;
    void *sideState;
    void *opposingSideState;
    ChainElem *calcChain;
    ChainElem *drawChain;
    float scaleX, scaleY, scaleZ, scaleW;
    Effect *effects;
    int primaryCount;
    int secondaryCount;
    Effect drawSentinel0;
    Effect drawSentinel1;
    Effect drawSentinel2;
    Effect *drawTail0;
    Effect *drawTail1;
    Effect *drawTail2;
    int updateCounter;
    void *effectAnm;
    int unknown2D8;
};
typedef char EffectManagerSizeIs2DC[(sizeof(EffectManager) == 0x2DC) ? 1 : -1];
typedef char EffectManagerSideStateAt10[(offsetof(EffectManager, sideState) == 0x10) ? 1 : -1];
typedef char EffectManagerOpposingSideAt14[(offsetof(EffectManager, opposingSideState) == 0x14) ? 1 : -1];
typedef char EffectManagerPoolAt30[(offsetof(EffectManager, effects) == 0x30) ? 1 : -1];
typedef char EffectManagerSentinel0At3C[(offsetof(EffectManager, drawSentinel0) == 0x3C) ? 1 : -1];
typedef char EffectManagerSentinel1At114[(offsetof(EffectManager, drawSentinel1) == 0x114) ? 1 : -1];
typedef char EffectManagerSentinel2At1EC[(offsetof(EffectManager, drawSentinel2) == 0x1EC) ? 1 : -1];
typedef char EffectManagerAnmAt2D4[(offsetof(EffectManager, effectAnm) == 0x2D4) ? 1 : -1];
