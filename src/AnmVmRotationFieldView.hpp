#pragma once

#include <stddef.h>

// Shared, target-observed ANM VM prefix: both Player's exact SetZRotation
// @0x0040F840 and ECL's late main-ANM rotation opcode consume +0x08.
// Flags at +0x1F8 are independently verified by the exact Player setter.
// This is a bounded storage view, not a claim about the original VM class.
struct AnmVmRotationFieldView
{
    unsigned char unknown000[0x08];
    float rotationZ08;
    unsigned char unknown00C[0x1F8 - 0x0C];
    unsigned int flags1F8;
};

typedef char AnmVmRotationZAt08[
    (offsetof(AnmVmRotationFieldView, rotationZ08) == 0x08) ? 1 : -1];
typedef char AnmVmRotationFlagsAt1F8[
    (offsetof(AnmVmRotationFieldView, flags1F8) == 0x1F8) ? 1 : -1];
