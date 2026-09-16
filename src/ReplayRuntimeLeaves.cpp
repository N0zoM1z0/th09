#include "ReplayRuntimeLeaves.hpp"

unsigned short ReplayRngView::GetSeed()
{
    return this->seed;
}

void ReplayRngView::SetSeed(unsigned short newSeed)
{
    this->seed = newSeed;
}

void ReplayRngView::ResetGenerationCount()
{
    this->generationCount = 0;
}

int SupervisorReplayView::IsSpeedhackDetected()
{
    return (this->flags5D4 >> 3) & 1;
}
