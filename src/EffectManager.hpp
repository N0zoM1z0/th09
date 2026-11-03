#pragma once

#include "Chain.hpp"
#include "ZunTimer.hpp"
#include "Float3.hpp"

#include <stddef.h>

struct EffectManager;
struct Effect;
class AnmLoaded;
typedef int (__fastcall *EffectCallback)(Effect *effect);
typedef void (__fastcall *EffectDrawCallback)(Effect *effect);

struct EffectFloat3
{
    float x;
    float y;
    float z;
};
typedef char EffectFloat3SizeIs0C[(sizeof(EffectFloat3) == 0x0C) ? 1 : -1];

struct EffectVmView
{
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

struct EffectRewardPayloadA0
{
    float angularStep;
    unsigned short tier;
    unsigned short tierLimit;
    unsigned short side;
    unsigned short unknown0AA;
};
typedef char EffectRewardPayloadSizeIs0C[
    (sizeof(EffectRewardPayloadA0) == 0x0C) ? 1 : -1];

// Bullet-cancellation effects interpret the same twelve bytes as projectile
// metadata; this is a variant overlay, not an extra allocation.
struct EffectCancelPayloadA0
{
    float bulletSpeed;
    short bulletType;
    short color;
    short side;
    short extraAttribute;
};
typedef char EffectCancelPayloadSizeIs0C[
    (sizeof(EffectCancelPayloadA0) == 0x0C) ? 1 : -1];

// Player score/reward effects share +0xA0..+0xAB but assign their own
// meaning to the short slots; do not unify these with Enemy tier fields.
struct EffectPlayerRewardPayloadA0
{
    float scale;
    short valueA4;
    short valueA6;
    short side;
    short unknownAA;
};
typedef char EffectPlayerRewardPayloadSizeIs0C[
    (sizeof(EffectPlayerRewardPayloadA0) == 0x0C) ? 1 : -1];

struct Effect
{
    Effect();
    EffectManager *manager;
    EffectVmView *vms;
    int vmCount;
    EffectFloat3 position;
    EffectFloat3 velocity;
    // Recovered shared effect-geometry state. Ring/fan callbacks interpret
    // these slots as floats; the general Effect manager also writes the
    // exact same words through its VM-snapshot integer transfer.
    unsigned char unknown024[0x48 - 0x24];
    Float3 origin48;
    Float3 axis54;
    Float3 axis60;
    unsigned char unknown06C[0x7C - 0x6C];
    union { int value7C; float radius7C; };
    union { int value80; float angle80; };
    unsigned char unknown084[0x88 - 0x84];
    union { int value88; float thickness88; };
    union { int value8C; int segments8C; };
    int fixedSlotIndex;
    union { int value94; float ellipseY94; };
    union { int value98; float rotation98; };
    union { float value9C; float frequency9C; };
    // Typed 12-byte death-reward payload, overlaid with the original
    // type-dependent storage used by other effect variants.
    union
    {
        unsigned char unknown0A0[0xAC - 0xA0];
        EffectRewardPayloadA0 reward;
        EffectCancelPayloadA0 cancel;
        EffectPlayerRewardPayloadA0 playerReward;
    };
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
typedef char EffectOriginAt48[(offsetof(Effect, origin48) == 0x48) ? 1 : -1];
typedef char EffectAxis54At54[(offsetof(Effect, axis54) == 0x54) ? 1 : -1];
typedef char EffectAxis60At60[(offsetof(Effect, axis60) == 0x60) ? 1 : -1];
typedef char EffectRadiusAt7C[(offsetof(Effect, radius7C) == 0x7C) ? 1 : -1];
typedef char EffectAngleAt80[(offsetof(Effect, angle80) == 0x80) ? 1 : -1];
typedef char EffectSegmentsAt8C[(offsetof(Effect, segments8C) == 0x8C) ? 1 : -1];
typedef char EffectValue7CAt7C[(offsetof(Effect, value7C) == 0x7C) ? 1 : -1];
typedef char EffectFixedSlotIndexAt90[(offsetof(Effect, fixedSlotIndex) == 0x90) ? 1 : -1];
typedef char EffectValue9CAt9C[(offsetof(Effect, value9C) == 0x9C) ? 1 : -1];
typedef char EffectRewardPayloadAtA0[
    (offsetof(Effect, reward) == 0xA0) ? 1 : -1];
typedef char EffectCancelPayloadAtA0[
    (offsetof(Effect, cancel) == 0xA0) ? 1 : -1];
typedef char EffectPlayerRewardPayloadAtA0[
    (offsetof(Effect, playerReward) == 0xA0) ? 1 : -1];
typedef char EffectTimerAtAC[(offsetof(Effect, timer) == 0xAC) ? 1 : -1];
typedef char EffectUpdateAtBC[(offsetof(Effect, updateCallback) == 0xBC) ? 1 : -1];
typedef char EffectActiveAtC4[(offsetof(Effect, active) == 0xC4) ? 1 : -1];
typedef char EffectVerticesAtCC[(offsetof(Effect, vertices) == 0xCC) ? 1 : -1];
typedef char EffectNextAtD4[(offsetof(Effect, nextInDrawGroup) == 0xD4) ? 1 : -1];

struct EffectManager
{
    EffectManager();
    void Clear();
    void ResetPool();
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
    int DrawBulletLayerEffects();
    static int AddedCallback(void *context);
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
    AnmLoaded *effectAnm;
    int unknown2D8;
};
typedef char EffectManagerSizeIs2DC[(sizeof(EffectManager) == 0x2DC) ? 1 : -1];
typedef char EffectManagerSideStateAt10[(offsetof(EffectManager, sideState) == 0x10) ? 1 : -1];
typedef char EffectManagerBulletLayerHeadAt1E8[
    (offsetof(EffectManager, drawSentinel1) +
     offsetof(Effect, nextInDrawGroup) == 0x1E8) ? 1 : -1];
typedef char EffectManagerOpposingSideAt14[(offsetof(EffectManager, opposingSideState) == 0x14) ? 1 : -1];
typedef char EffectManagerPoolAt30[(offsetof(EffectManager, effects) == 0x30) ? 1 : -1];
typedef char EffectManagerSentinel0At3C[(offsetof(EffectManager, drawSentinel0) == 0x3C) ? 1 : -1];
typedef char EffectManagerSentinel1At114[(offsetof(EffectManager, drawSentinel1) == 0x114) ? 1 : -1];
typedef char EffectManagerSentinel2At1EC[(offsetof(EffectManager, drawSentinel2) == 0x1EC) ? 1 : -1];
typedef char EffectManagerAnmAt2D4[(offsetof(EffectManager, effectAnm) == 0x2D4) ? 1 : -1];
