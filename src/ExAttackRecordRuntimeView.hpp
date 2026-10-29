#pragma once
#include <stddef.h>
#include "EffectManager.hpp"

// Reviewed TH09 0x38-byte side with the real EffectManager at +0x0C.
struct ExAttackGameManagerSideView
{
    unsigned char unknown00[0x0C];
    EffectManager *effectManager0C;
    unsigned char unknown10[0x28];
};
typedef char ExAttackGameSideSize38[(sizeof(ExAttackGameManagerSideView)==0x38)?1:-1];
typedef char ExAttackGameSideEffectAt0C[(offsetof(ExAttackGameManagerSideView,effectManager0C)==0x0C)?1:-1];
typedef char ExAttackEffectManagerAnmAt2D4[(offsetof(EffectManager,effectAnm)==0x2D4)?1:-1];

struct ExAttackRecordSideView
{
    int unknown00;
    int opponentSide04;
    int side08;
    ExAttackGameManagerSideView *GetSideState();
};
typedef char ExAttackRecordSideIndexAt08[(offsetof(ExAttackRecordSideView,side08)==8)?1:-1];
typedef char ExAttackRecordSideViewSizeIs0C[(sizeof(ExAttackRecordSideView)==0x0C)?1:-1];

struct ExAttackAllocateRecordView
{
    unsigned char unknown00[0x1C];
    void *dynamicData1C;
    unsigned char unknown20[0x0C];
    int vmCount2C;
    unsigned char unknown30[4];
    void *extraData34;
    void AllocateDynamicData(int vmCount, int extraBytes);
};
typedef char ExAttackAllocateSize38[(sizeof(ExAttackAllocateRecordView)==0x38)?1:-1];
typedef char ExAttackAllocateExtraAt34[(offsetof(ExAttackAllocateRecordView,extraData34)==0x34)?1:-1];
typedef char ExAttackAllocateVmCountAt2C[(offsetof(ExAttackAllocateRecordView,vmCount2C)==0x2C)?1:-1];
typedef char ExAttackAllocateDynamicAt1C[(offsetof(ExAttackAllocateRecordView,dynamicData1C)==0x1C)?1:-1];
