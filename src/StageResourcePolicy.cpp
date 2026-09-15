#include "StageResourcePolicy.hpp"

extern int g_StageResourceReleasePolicy;

int ShouldReleaseStageResources()
{
    return g_StageResourceReleasePolicy;
}
