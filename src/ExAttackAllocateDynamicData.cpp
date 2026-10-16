#include "ZunMemory.hpp"
#include "ExAttackRecordRuntimeView.hpp"

#include <string.h>

void ExAttackAllocateRecordView::AllocateDynamicData(
    int vmCount, int extraBytes)
{
    vmCount2C = vmCount;
    extraBytes += vmCount * 0x2A4;
    dynamicData1C = g_ZunMemory.Alloc(
        extraBytes, "./system\\global.h");
    memset(dynamicData1C, 0, extraBytes);
    extraData34 =
        reinterpret_cast<unsigned char *>(dynamicData1C) + vmCount * 0x2A4;
}
