#include "ZunMemory.hpp"

void __fastcall ReleaseScoreFileBuffer(void *scoreFile)
{
    g_ZunMemory.Free(scoreFile);
}
