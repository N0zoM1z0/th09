#pragma once

#include "AnmVmLifecycle.hpp"
#include "Chain.hpp"
#include "ZunTimer.hpp"

#include <stddef.h>

class AnmLoaded;

struct BackgroundPoint
{
    BackgroundPoint();
    float x;
    float y;
    float z;
};

typedef char BackgroundPointSizeIs0C[
    (sizeof(BackgroundPoint) == 0x0C) ? 1 : -1];

struct BackgroundFog
{
    float nearPlane;
    float farPlane;
    unsigned int color;
};

typedef char BackgroundFogSizeIs0C[
    (sizeof(BackgroundFog) == 0x0C) ? 1 : -1];

struct RawStageHeader
{
    short objectCount;
    short quadCount;
    int objectInstancesOffset;
    int scriptOffset;
    int unknown00C;
    char stageName[128];
    char songNames[4][128];
    char songPaths[4][128];
};

typedef char RawStageHeaderSizeIs490[(sizeof(RawStageHeader) == 0x490) ? 1 : -1];

struct RawStageQuadBasic
{
    short type;
    short byteSize;
    short anmScript;
    short vmIndex;
    BackgroundPoint position;
    float width;
    float height;
};

typedef char RawStageQuadBasicSizeIs1C[(sizeof(RawStageQuadBasic) == 0x1C) ? 1 : -1];

struct RawStageObject
{
    short id;
    signed char zLevel;
    unsigned char flags;
    BackgroundPoint position;
    BackgroundPoint size;
    RawStageQuadBasic firstQuad;
};

typedef char RawStageObjectSizeIs38[(sizeof(RawStageObject) == 0x38) ? 1 : -1];

struct RawStageObjectInstance
{
    short objectId;
    short reserved02;
    BackgroundPoint position;
};

typedef char RawStageObjectInstanceSizeIs10[
    (sizeof(RawStageObjectInstance) == 0x10) ? 1 : -1];

struct RawStageInstr
{
    int frame;
    short opcode;
    short size;
    int args[3];
};

typedef char RawStageInstrSizeIs14[(sizeof(RawStageInstr) == 0x14) ? 1 : -1];

struct Background
{
    Background();

    static Background *Create(int viewportIndex);
    static int OnUpdate(Background *background);
    static int OnDrawHighPrio(Background *background);
    static int OnDrawLowPrio(Background *background);
    static int AddedCallback(Background *background);
    static int DeletedCallback(Background *background);
    void ClearSpellBackgroundState();
    void ApplyPendingStageLabel();

    int LoadStageData(const char *path);
    int RenderObjects(int mode);

    ChainElem *calcChain;
    ChainElem *drawHighChain;
    ChainElem *drawLowChain;
    int viewportIndex;
    void *sideState;
    void *otherSideState;
    int pendingStageLabel18;
    int transitionMode;
    int transitionFrames;
    AnmVm *stageObjectVms;
    RawStageInstr *stageScript;
    AnmVm stageVm0;
    AnmVm stageVm1;
    AnmVm stageVm2;
    AnmLoaded *stageAnm;
    AnmLoaded *sideAnm;
    RawStageHeader *stageData;
    int stageQuadCount;
    int stageObjectCount;
    RawStageObject **stageObjects;
    RawStageObjectInstance *stageObjectInstances;
    unsigned char unknown834[0xBF4 - 0x834];
    int interpolationDuration[5];
    ZunTimer interpolationTimers[5];
    unsigned char unknownC44[0xC58 - 0xC44];
    ZunTimer stageScriptTimer;
    int stageScriptInstructionIndex;
    int frameCounter;
    int unknownC6C;
    BackgroundPoint stagePosition;
    unsigned int clearColor;
    unsigned char unknownC80[4];
    ZunTimer stageTextTimer;
    unsigned char unknownC90[4];
    BackgroundFog skyFog;
    BackgroundFog skyFogInitial;
    BackgroundFog skyFogFinal;
    int skyFogInterpolationDuration;
    ZunTimer skyFogInterpolationTimer;
    unsigned char skyFogNeedsSetup;
    unsigned char unknownCC9[3];
    int spellBackgroundState;
    int spellBackgroundFrame;
    int clearPending;
    int spellVmCount;
    int spellVmScriptBase;
    AnmVm spellVms[32];
    AnmVm spellAuxVm;
    void (__fastcall *spellBackgroundDrawCallback)();
    int pendingStageScriptLabel;
    unsigned char unknown640C[0x642C - 0x640C];
    unsigned int tintColor;
    int retainTint;
    float cullingDistanceSq;
    unsigned char cameraMotionMode;
    unsigned char unknown6439[3];
    int specialEffectPointCount;
    int collectSpecialEffectPoints;
    BackgroundPoint specialEffectPoints[32];
};

typedef char BackgroundSizeIs65C4[(sizeof(Background) == 0x65C4) ? 1 : -1];
typedef char BackgroundStageVm0At02C[(offsetof(Background, stageVm0) == 0x02C) ? 1 : -1];
typedef char BackgroundStageAnmAt818[(offsetof(Background, stageAnm) == 0x818) ? 1 : -1];
typedef char BackgroundStageDataAt820[(offsetof(Background, stageData) == 0x820) ? 1 : -1];
typedef char BackgroundDurationsAtBF4[(offsetof(Background, interpolationDuration) == 0xBF4) ? 1 : -1];
typedef char BackgroundStageTimerAtC58[(offsetof(Background, stageScriptTimer) == 0xC58) ? 1 : -1];
typedef char BackgroundSpellStateAtCCC[(offsetof(Background, spellBackgroundState) == 0xCCC) ? 1 : -1];
typedef char BackgroundSpellVmsAtCE0[(offsetof(Background, spellVms) == 0xCE0) ? 1 : -1];
typedef char BackgroundCallbackAt6404[(offsetof(Background, spellBackgroundDrawCallback) == 0x6404) ? 1 : -1];
typedef char BackgroundTintAt642C[(offsetof(Background, tintColor) == 0x642C) ? 1 : -1];
typedef char BackgroundPointsAt6444[(offsetof(Background, specialEffectPoints) == 0x6444) ? 1 : -1];
