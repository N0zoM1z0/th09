#pragma once

#include "AsciiManager.hpp"
#include "ZunTimer.hpp"
#include <stddef.h>

// TH09 ExAttack Type21 init and exact update share one 0x68C dynamic object:
// the 0x688 Type19-like common trail prefix plus Type21's angle at +0x688.
// Keep this a separate complete type rather than assuming inheritance from
// Type19: no original C++ inheritance/RTTI identity has been proven.

struct ExAttackType21Vertex
{
    Float3 pos;
    float rhw;
    unsigned long diffuse;
    float u;
    float v;
};
typedef char ExAttackType21VertexSizeIs1C[
    (sizeof(ExAttackType21Vertex) == 0x1C) ? 1 : -1];

struct ExAttackType21Extra
{
    int state00;
    Float3 motion04;
    Float3 point10;
    Float3 point1C;
    Float3 control28;
    Float3 control34;
    Float3 spawn40;
    ExAttackType21Vertex vertices4C[33];
    // Center sample plus the 32 samples written by initialization/update.
    Float3 history3E8[33];
    unsigned char unknown574[0x578 - 0x574];
    float magnitudes578[32];
    float unknown5F8;
    float angularSteps5FC[32];
    float drift67C;
    float drift680;
    int drawPending684;
    float angle688;
};
typedef char ExAttackType21ExtraHistoryEndIs574[
    (offsetof(ExAttackType21Extra, history3E8) +
     sizeof(((ExAttackType21Extra *)0)->history3E8) == 0x574) ? 1 : -1];

typedef char ExAttackType21ExtraSizeIs68C[
    (sizeof(ExAttackType21Extra) == 0x68C) ? 1 : -1];
typedef char ExAttackType21VerticesAt4C[
    (offsetof(ExAttackType21Extra, vertices4C) == 0x4C) ? 1 : -1];
typedef char ExAttackType21HistoryAt3E8[
    (offsetof(ExAttackType21Extra, history3E8) == 0x3E8) ? 1 : -1];
typedef char ExAttackType21MagnitudesAt578[
    (offsetof(ExAttackType21Extra, magnitudes578) == 0x578) ? 1 : -1];
typedef char ExAttackType21StepsAt5FC[
    (offsetof(ExAttackType21Extra, angularSteps5FC) == 0x5FC) ? 1 : -1];
typedef char ExAttackType21PendingAt684[
    (offsetof(ExAttackType21Extra, drawPending684) == 0x684) ? 1 : -1];
typedef char ExAttackType21AngleAt688[
    (offsetof(ExAttackType21Extra, angle688) == 0x688) ? 1 : -1];

struct ExAttackType21RecordView
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
    ExAttackType21Extra *extra34;
};


// Update-only role names alias this same target-proven physical layout.
typedef ExAttackType21Vertex ExAttackType21UpdateVertex;
typedef ExAttackType21Extra ExAttackType21UpdateExtra;
typedef ExAttackType21RecordView ExAttackType21UpdateRecord;
typedef char ExAttackType21RecordDynamicAt1C[
    (offsetof(ExAttackType21RecordView, dynamicData1C) == 0x1C) ? 1 : -1];
typedef char ExAttackType21RecordExtraAt34[
    (offsetof(ExAttackType21RecordView, extra34) == 0x34) ? 1 : -1];
typedef char ExAttackType21RecordSizeIs38[
    (sizeof(ExAttackType21RecordView) == 0x38) ? 1 : -1];
