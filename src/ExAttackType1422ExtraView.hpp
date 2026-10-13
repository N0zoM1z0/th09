#pragma once

#include "AsciiManager.hpp"
#include <stddef.h>

// Shared TH09 0xF48-byte ExAttack Type14/22 dynamic-data ABI. Original
// game-class identity and any inheritance between the two attack types are
// unknown; these names designate only the independently target-reviewed
// physical layout used by four authored callback bodies.

struct ExAttackType1422Vertex
{
    Float3 pos;
    float rhw;
    unsigned long diffuse;
};
typedef char ExAttackType1422VertexSizeIs14[
    (sizeof(ExAttackType1422Vertex) == 0x14) ? 1 : -1];

struct ExAttackType1422Extra
{
    int state00;
    Float3 motion04;
    Float3 point10;
    Float3 point1C;
    Float3 point28;
    Float3 point34;
    Float3 spawn40;
    int drawPending4C;
    float value50;
    ExAttackType1422Vertex centerFan54[33];
    ExAttackType1422Vertex fanGroups2E8[4][33];
    float randomD38[4][33];
};
typedef char ExAttackType1422ExtraSizeIsF48[
    (sizeof(ExAttackType1422Extra) == 0xF48) ? 1 : -1];
typedef char ExAttackType1422CenterAt54[
    (offsetof(ExAttackType1422Extra, centerFan54) == 0x54) ? 1 : -1];
typedef char ExAttackType1422GroupsAt2E8[
    (offsetof(ExAttackType1422Extra, fanGroups2E8) == 0x2E8) ? 1 : -1];
typedef char ExAttackType1422RandomAtD38[
    (offsetof(ExAttackType1422Extra, randomD38) == 0xD38) ? 1 : -1];


// These four callback-local role names share one physical representation.
typedef ExAttackType1422Vertex ExAttackType1422DrawVertex;
typedef ExAttackType1422Extra ExAttackType1422DrawExtra;
typedef ExAttackType1422Extra ExAttackType14Extra;
typedef ExAttackType1422Extra ExAttackType22Extra;

// Independently observed target offsets and the allocated array extents.
typedef char ExAttackType1422StateAt00[
    (offsetof(ExAttackType1422Extra, state00) == 0x00) ? 1 : -1];
typedef char ExAttackType1422MotionAt04[
    (offsetof(ExAttackType1422Extra, motion04) == 0x04) ? 1 : -1];
typedef char ExAttackType1422EndPointAt10[
    (offsetof(ExAttackType1422Extra, point10) == 0x10) ? 1 : -1];
typedef char ExAttackType1422StartPointAt1C[
    (offsetof(ExAttackType1422Extra, point1C) == 0x1C) ? 1 : -1];
typedef char ExAttackType1422Control28[
    (offsetof(ExAttackType1422Extra, point28) == 0x28) ? 1 : -1];
typedef char ExAttackType1422Control34[
    (offsetof(ExAttackType1422Extra, point34) == 0x34) ? 1 : -1];
typedef char ExAttackType1422SpawnAt40[
    (offsetof(ExAttackType1422Extra, spawn40) == 0x40) ? 1 : -1];
typedef char ExAttackType1422ValueAt50[
    (offsetof(ExAttackType1422Extra, value50) == 0x50) ? 1 : -1];
typedef char ExAttackType1422DrawPendingAt4C[
    (offsetof(ExAttackType1422Extra, drawPending4C) == 0x4C) ? 1 : -1];
