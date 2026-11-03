#pragma once
#include "Supervisor.hpp"
#include <stddef.h>

// Original CheckVersion symbol retains its standalone nominal C++ receiver;
// it operates on exactly the existing 0x7B0-byte Supervisor allocation.
struct SupervisorVersionView : Supervisor
{
    int CheckVersion(const char *version, int exeSize, int exeChecksum);
};
typedef char SupervisorNativeVersionReceiver[
    (sizeof(SupervisorVersionView) == sizeof(Supervisor) &&
     offsetof(SupervisorVersionView, versionDataSize7A8) == 0x7A8 &&
     offsetof(SupervisorVersionView, versionData7AC) == 0x7AC) ? 1 : -1];
