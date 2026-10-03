// This single definition is compiled with EnemyScheduleRuntimeAdvance.cpp.
// Equality remains a real call; visibility exposes its read-only memory effects.
// It overwrites EAX, ECX and EDX, so no extra register preservation is assumed.
// Other timer bodies stay opaque: exposing all of them changes save/restore code.
// This recovers required compiler context, not a unique original file partition.
#include "ZunTimer.hpp"

int ZunTimer::operator==(int value)
{
    return this->current == value;
}
