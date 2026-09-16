#include "SupervisorNetworkState.hpp"

void __fastcall SupervisorNetworkMarkActive(void *state)
{
    reinterpret_cast<SupervisorNetworkState *>(state)->active = 1;
}

void __fastcall SupervisorNetworkMarkInactive(void *state)
{
    reinterpret_cast<SupervisorNetworkState *>(state)->active = 0;
}
