#include "ReplayRuntimeLeaves.hpp"

unsigned short RngRuntimeView::GetSeed()
{
    return this->seed;
}

void RngRuntimeView::SetSeed(unsigned short newSeed)
{
    this->seed = newSeed;
}

void RngRuntimeView::ResetGenerationCount()
{
    this->generationCount = 0;
}

int SupervisorReplayView::IsSpeedhackDetected()
{
    return (this->flags5D4 >> 3) & 1;
}
