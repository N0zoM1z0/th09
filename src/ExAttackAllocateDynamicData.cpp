#include "ZunMemory.hpp"

#include <string.h>

struct ExAttackAllocateRecordView
{
    unsigned char unknown00[0x1C];
    void *dynamicData1C;
    unsigned char unknown20[0x0C];
    int vmCount2C;
    unsigned char unknown30[0x04];
    void *extraData34;

    void AllocateDynamicData(int vmCount, int extraBytes);
};

void ExAttackAllocateRecordView::AllocateDynamicData(
    int vmCount, int extraBytes)
{
    vmCount2C = vmCount;
    vmCount *= 0x2A4;
    extraBytes += vmCount;
    dynamicData1C = g_ZunMemory.Alloc(
        extraBytes, "./system\\global.h");
    memset(dynamicData1C, 0, extraBytes);
    extraData34 =
        reinterpret_cast<unsigned char *>(dynamicData1C) + vmCount;
}
