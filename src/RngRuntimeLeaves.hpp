#pragma once

#include <stddef.h>

struct RngRuntimeView
{
    unsigned short seed;
    unsigned short unknown02;
    int generationCount;

    unsigned short GetSeed();
    void SetSeed(unsigned short seed);
    void ResetGenerationCount();
    unsigned short NextU16();
    unsigned short GetRandomU16InRange(unsigned short maximum);
    unsigned int GetRandomU32();
    float GetRandomF32();
    float GetRandomF32Signed();
    unsigned int GetRandomU32InRange(unsigned int maximum);
    float GetRandomF32InRange(float maximum);
    float GetRandomF32SignedInRange(float maximum);
};

typedef char RngRuntimeNativeState[
    (sizeof(RngRuntimeView) == 8 &&
     offsetof(RngRuntimeView, seed) == 0 &&
     offsetof(RngRuntimeView, unknown02) == 2 &&
     offsetof(RngRuntimeView, generationCount) == 4) ? 1 : -1];
