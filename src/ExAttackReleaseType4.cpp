#include "ZunMemory.hpp"

struct ExAttackReleaseType4RecordView
{
    unsigned char unknown00[0x34];
    void *extra34;
};

extern ZunMemory g_ZunMemory;

int __fastcall ExAttackReleaseCallbackType4(
    ExAttackReleaseType4RecordView *record)
{
    if (record->extra34 != 0)
        g_ZunMemory.Free(record->extra34);
    record->extra34 = 0;
    return 0;
}
