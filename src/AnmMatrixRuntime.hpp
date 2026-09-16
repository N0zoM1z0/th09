#pragma once

struct AnmMatrixRuntime
{
    float m[4][4];

    AnmMatrixRuntime *SetIdentity();
};

typedef char AnmMatrixRuntimeSizeIs40[
    (sizeof(AnmMatrixRuntime) == 0x40) ? 1 : -1];
