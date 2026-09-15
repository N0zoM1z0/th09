#pragma once

#include <stddef.h>

#include "Chain.hpp"
#include "ZunTimer.hpp"
#include "AsciiManager.hpp"

struct ExAttackRecord;

struct ExAttackVec3
{
    float x;
    float y;
    float z;
};
typedef char ExAttackVec3SizeIs0C[(sizeof(ExAttackVec3) == 0x0C) ? 1 : -1];

typedef int (__fastcall *ExAttackRecordCallback)(ExAttackRecord *record);

struct ExAttackRecord
{
    int unknown00;
    int opponentSide;
    int side;
    int active;
    ZunTimer timer;
    void *dynamicData;
    ExAttackVec3 position;
    unsigned char unknown2C[0x0C];
    int parameter38;
    int unknown3C;
    ExAttackRecordCallback updateCallback;
    ExAttackRecordCallback drawCallback;
    ExAttackRecordCallback releaseCallback;

    ExAttackRecord();
};

typedef char ExAttackRecordSizeIs4C[(sizeof(ExAttackRecord) == 0x4C) ? 1 : -1];
typedef char ExAttackRecordTimerAt10[(offsetof(ExAttackRecord, timer) == 0x10) ? 1 : -1];
typedef char ExAttackRecordDynamicAt1C[(offsetof(ExAttackRecord, dynamicData) == 0x1C) ? 1 : -1];
typedef char ExAttackRecordPositionAt20[(offsetof(ExAttackRecord, position) == 0x20) ? 1 : -1];
typedef char ExAttackRecordActiveAt0C[(offsetof(ExAttackRecord, active) == 0x0C) ? 1 : -1];
typedef char ExAttackRecordReleaseAt48[(offsetof(ExAttackRecord, releaseCallback) == 0x48) ? 1 : -1];

struct ExAttackTemplate
{
    ExAttackRecordCallback initializeCallback;
    ExAttackRecordCallback updateCallback;
    ExAttackRecordCallback drawCallback;
    ExAttackRecordCallback releaseCallback;
};
typedef char ExAttackTemplateSizeIs10[(sizeof(ExAttackTemplate) == 0x10) ? 1 : -1];

struct ExAttackController
{
    ChainElem *calcChain;
    ChainElem *drawChain0;
    ChainElem *drawChain1;
    int unknown0C;
    int unknown10;
    int sideLimits[2];
    ExAttackRecord records[257];
    unsigned char tail4C68[0x18];

    ExAttackController();
    ExAttackRecord *Spawn(int type, const Float3 *position, int side, int parameter);

    static int OnUpdate(ExAttackController *controller);
    static int OnDraw0(ExAttackController *controller);
    static int OnDraw1(ExAttackController *controller);
    static int AddedCallback(ExAttackController *controller);

    static ExAttackController *Register();
    static void Release(ExAttackController *controller);
};

typedef char ExAttackControllerSizeIs4C80[(sizeof(ExAttackController) == 0x4C80) ? 1 : -1];
typedef char ExAttackControllerLimitsAt14[(offsetof(ExAttackController, sideLimits) == 0x14) ? 1 : -1];
typedef char ExAttackControllerRecordsAt1C[(offsetof(ExAttackController, records) == 0x1C) ? 1 : -1];
typedef char ExAttackControllerSentinelAt4C1C[(offsetof(ExAttackController, records) + 256 * sizeof(ExAttackRecord) == 0x4C1C) ? 1 : -1];
