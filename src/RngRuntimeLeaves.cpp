#include "RngRuntimeLeaves.hpp"

#include <limits.h>

unsigned short RngRuntimeView::NextU16()
{
    unsigned short value = (this->seed ^ 0x9630) - 0x6553;
    this->seed = (unsigned short)((value >> 14) + value * 4);
    this->generationCount++;
    return this->seed;
}

unsigned int RngRuntimeView::GetRandomU32()
{
    return ((unsigned int)this->NextU16() << 16) | this->NextU16();
}

float RngRuntimeView::GetRandomF32()
{
    return (float)this->GetRandomU32() / (float)UINT_MAX;
}

float RngRuntimeView::GetRandomF32Signed()
{
    return (float)this->GetRandomU32() / (float)INT_MAX - 1.0f;
}

unsigned int RngRuntimeView::GetRandomU32InRange(unsigned int maximum)
{
    return maximum != 0 ? this->GetRandomU32() % maximum : 0;
}

float RngRuntimeView::GetRandomF32InRange(float maximum)
{
    return this->GetRandomF32() * maximum;
}

float RngRuntimeView::GetRandomF32SignedInRange(float maximum)
{
    return this->GetRandomF32Signed() * maximum;
}
