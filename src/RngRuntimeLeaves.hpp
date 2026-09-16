#pragma once

#include <stddef.h>

struct RngRuntimeView
{
    unsigned short seed;
    unsigned short unknown02;
    int generationCount;

    unsigned short NextU16();
    unsigned int GetRandomU32();
    float GetRandomF32();
    float GetRandomF32Signed();
    unsigned int GetRandomU32InRange(unsigned int maximum);
    float GetRandomF32InRange(float maximum);
    float GetRandomF32SignedInRange(float maximum);
};

typedef char RngRuntimeGenerationCountAt04[
    (offsetof(RngRuntimeView, generationCount) == 0x04) ? 1 : -1];
