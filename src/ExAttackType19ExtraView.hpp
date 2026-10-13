#pragma once

#include "AsciiManager.hpp"
#include "ZunTimer.hpp"
#include <stddef.h>

// TH09 ExAttack Type19 uses the same 0x688-byte dynamic data block in
// initialization and update. Both code paths address the same target-proven
// vertex, history, radius, angular-step and drift fields. Keep that wire
// structure in one real C++ declaration rather than two independent layouts.

struct ExAttackType19Vertex
{
    Float3 pos;
    float rhw;
    unsigned long diffuse;
    float u;
    float v;
};
typedef char ExAttackType19VertexSizeIs1C[
    (sizeof(ExAttackType19Vertex) == 0x1C) ? 1 : -1];

struct ExAttackType19Extra
{
    int state00;
    Float3 motion04;
    Float3 point10;
    Float3 point1C;
    Float3 control28;
    Float3 control34;
    Float3 spawn40;
    ExAttackType19Vertex vertices4C[33];
    // Center sample plus the 32 samples written by initialization/update.
    Float3 history3E8[33];
    unsigned char unknown574[0x578 - 0x574];
    float magnitudes578[32];
    float unknown5F8;
    float angularSteps5FC[32];
    float drift67C;
    float drift680;
    int drawPending684;
};
typedef char ExAttackType19ExtraHistoryEndIs574[
    (offsetof(ExAttackType19Extra, history3E8) +
     sizeof(((ExAttackType19Extra *)0)->history3E8) == 0x574) ? 1 : -1];

typedef char ExAttackType19ExtraSizeIs688[
    (sizeof(ExAttackType19Extra) == 0x688) ? 1 : -1];
typedef char ExAttackType19SpawnAt40[
    (offsetof(ExAttackType19Extra, spawn40) == 0x40) ? 1 : -1];
typedef char ExAttackType19VerticesAt4C[
    (offsetof(ExAttackType19Extra, vertices4C) == 0x4C) ? 1 : -1];
typedef char ExAttackType19HistoryAt3E8[
    (offsetof(ExAttackType19Extra, history3E8) == 0x3E8) ? 1 : -1];
typedef char ExAttackType19MagnitudesAt578[
    (offsetof(ExAttackType19Extra, magnitudes578) == 0x578) ? 1 : -1];
typedef char ExAttackType19StepsAt5FC[
    (offsetof(ExAttackType19Extra, angularSteps5FC) == 0x5FC) ? 1 : -1];
typedef char ExAttackType19DriftXAt67C[
    (offsetof(ExAttackType19Extra, drift67C) == 0x67C) ? 1 : -1];
typedef char ExAttackType19DriftYAt680[
    (offsetof(ExAttackType19Extra, drift680) == 0x680) ? 1 : -1];
typedef char ExAttackType19PendingAt684[
    (offsetof(ExAttackType19Extra, drawPending684) == 0x684) ? 1 : -1];

// Historical update-specific source names are aliases, not separate
// competing definitions of the same object representation.
typedef ExAttackType19Vertex ExAttackType19UpdateVertex;
typedef ExAttackType19Extra ExAttackType19UpdateExtra;

// The same 0x38-byte record owns this dynamic block for both callbacks.
// Only the first dword is given different historical source names.
struct ExAttackType19RecordView
{
    union { int state00; int unknown00; };
    int opponentSide04;
    int side08;
    int active0C;
    ZunTimer timer10;
    void *dynamicData1C;
    Float3 position20;
    int vmCount2C;
    int unknown30;
    ExAttackType19Extra *extra34;
};

typedef char ExAttackType19RecordDynamicAt1C[
    (offsetof(ExAttackType19RecordView, dynamicData1C) == 0x1C) ? 1 : -1];
typedef char ExAttackType19RecordPositionAt20[
    (offsetof(ExAttackType19RecordView, position20) == 0x20) ? 1 : -1];
typedef char ExAttackType19RecordExtraAt34[
    (offsetof(ExAttackType19RecordView, extra34) == 0x34) ? 1 : -1];
typedef char ExAttackType19RecordSizeIs38[
    (sizeof(ExAttackType19RecordView) == 0x38) ? 1 : -1];
typedef ExAttackType19RecordView ExAttackType19UpdateRecord;
