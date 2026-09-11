#include "Supervisor.hpp"
#include "AnmManager.hpp"

#include <stddef.h>

struct SupervisorLoadingFloat3View
{
    float x;
    float y;
    float z;

    SupervisorLoadingFloat3View &operator+=(const SupervisorLoadingFloat3View &other);
    SupervisorLoadingFloat3View &operator-=(const SupervisorLoadingFloat3View &other);
};

struct SupervisorLoadingVmView
{
    unsigned char unknown000[0x208];
    SupervisorLoadingFloat3View pos;
    unsigned char unknown214[0x288 - 0x214];
    SupervisorLoadingFloat3View pos2;
    unsigned char unknown294[0x2A4 - 0x294];
};

typedef char SupervisorLoadingVmSizeIs2A4[
    (sizeof(SupervisorLoadingVmView) == 0x2A4) ? 1 : -1];
typedef char SupervisorLoadingVmPosAt208[
    (offsetof(SupervisorLoadingVmView, pos) == 0x208) ? 1 : -1];
typedef char SupervisorLoadingVmPos2At288[
    (offsetof(SupervisorLoadingVmView, pos2) == 0x288) ? 1 : -1];

struct SupervisorDrawLoadingAnmManagerView
{
    int Draw2D(void *vm);
};

extern SupervisorLoadingVmView g_SupervisorLoadingVms[3];

int __fastcall SupervisorDrawLoading(Supervisor *unused)
{
    int i = 0;
    int count = 3;
    do
    {
        g_SupervisorLoadingVms[i].pos += g_SupervisorLoadingVms[i].pos2;
        reinterpret_cast<SupervisorDrawLoadingAnmManagerView *>(g_AnmManager)->Draw2D(
            &g_SupervisorLoadingVms[i]);
        g_SupervisorLoadingVms[i].pos -= g_SupervisorLoadingVms[i].pos2;
        i++;
    } while (--count != 0);

    return 1;
}
