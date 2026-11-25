#pragma once

#include "AnmVmLifecycle.hpp"
#include "Chain.hpp"
#include "ZunTimer.hpp"

#include <stddef.h>

class AnmLoaded;

struct BackgroundPoint
{
    float x;
    float y;
    float z;
};

typedef char BackgroundPointSizeIs0C[
    (sizeof(BackgroundPoint) == 0x0C) ? 1 : -1];

struct BackgroundSpecialEffectPoint
{
    BackgroundSpecialEffectPoint();
    BackgroundPoint point;
};

typedef char BackgroundSpecialEffectPointSizeIs0C[
    (sizeof(BackgroundSpecialEffectPoint) == 0x0C) ? 1 : -1];
typedef char BackgroundSpecialEffectPointValueAt0[
    (offsetof(BackgroundSpecialEffectPoint, point) == 0) ? 1 : -1];

struct BackgroundFog
{
    float nearPlane;
    float farPlane;
    unsigned int color;
};

typedef char BackgroundFogSizeIs0C[
    (sizeof(BackgroundFog) == 0x0C) ? 1 : -1];

struct BackgroundCameraRecord
{
    BackgroundPoint position;
    BackgroundPoint lookAt;
    BackgroundPoint up;
    BackgroundPoint forward;
    BackgroundPoint right;
    BackgroundPoint positionOffset;
    float fieldOfView;
    unsigned char unknown04C[0xF0 - 0x4C];
};

typedef char BackgroundCameraRecordSizeIsF0[
    (sizeof(BackgroundCameraRecord) == 0xF0) ? 1 : -1];

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

// TH09 v1.50a stage .std script opcodes recovered from
// BackgroundRunStageScriptPhase and Background::ApplyPendingStageLabel.
enum BackgroundStageOpcode
{
    BACKGROUND_OPCODE_SET_STAGE_POSITION_KEYFRAME = 0,
    BACKGROUND_OPCODE_SET_FOG = 1,
    BACKGROUND_OPCODE_START_FOG_INTERPOLATION = 2,
    BACKGROUND_OPCODE_WAIT_FOR_STAGE_LABEL = 3,
    BACKGROUND_OPCODE_JUMP = 4,
    BACKGROUND_OPCODE_SET_CAMERA_POSITION = 5,
    BACKGROUND_OPCODE_START_CAMERA_POSITION_INTERPOLATION = 6,
    BACKGROUND_OPCODE_SET_CAMERA_LOOK_AT_OFFSET = 7,
    BACKGROUND_OPCODE_START_CAMERA_LOOK_AT_INTERPOLATION = 8,
    BACKGROUND_OPCODE_SET_CAMERA_UP = 9,
    BACKGROUND_OPCODE_START_CAMERA_UP_INTERPOLATION = 10,
    BACKGROUND_OPCODE_SET_CAMERA_FIELD_OF_VIEW = 11,
    BACKGROUND_OPCODE_START_CAMERA_FIELD_OF_VIEW_INTERPOLATION = 12,
    BACKGROUND_OPCODE_SET_CLEAR_COLOR = 13,
    BACKGROUND_OPCODE_SET_CAMERA_POSITION_START = 14,
    BACKGROUND_OPCODE_SET_CAMERA_POSITION_TARGET = 15,
    BACKGROUND_OPCODE_SET_CAMERA_POSITION_START_TANGENT = 16,
    BACKGROUND_OPCODE_SET_CAMERA_POSITION_END_TANGENT = 17,
    BACKGROUND_OPCODE_START_CAMERA_POSITION_HERMITE = 18,
    BACKGROUND_OPCODE_SET_CAMERA_LOOK_AT_START = 19,
    BACKGROUND_OPCODE_SET_CAMERA_LOOK_AT_TARGET = 20,
    BACKGROUND_OPCODE_SET_CAMERA_LOOK_AT_START_TANGENT = 21,
    BACKGROUND_OPCODE_SET_CAMERA_LOOK_AT_END_TANGENT = 22,
    BACKGROUND_OPCODE_START_CAMERA_LOOK_AT_HERMITE = 23,
    BACKGROUND_OPCODE_SET_CAMERA_UP_START = 24,
    BACKGROUND_OPCODE_SET_CAMERA_UP_TARGET = 25,
    BACKGROUND_OPCODE_SET_CAMERA_UP_START_TANGENT = 26,
    BACKGROUND_OPCODE_SET_CAMERA_UP_END_TANGENT = 27,
    BACKGROUND_OPCODE_START_CAMERA_UP_HERMITE = 28,
    BACKGROUND_OPCODE_SET_STAGE_VM_0_SCRIPT = 29,
    BACKGROUND_OPCODE_SET_STAGE_VM_1_SCRIPT = 30,
    BACKGROUND_OPCODE_LABEL = 31,
    BACKGROUND_OPCODE_SET_CAMERA_POSITION_OFFSET = 32,
    BACKGROUND_OPCODE_SET_CAMERA_MOTION_MODE = 33,
    BACKGROUND_OPCODE_SET_STAGE_VM_2_SCRIPT = 34
};

// Curve modes used by Background stage camera/FOV interpolation channels.
enum BackgroundInterpolationMode
{
    BACKGROUND_INTERPOLATION_LINEAR = 0,
    BACKGROUND_INTERPOLATION_EASE_OUT_QUADRATIC = 1,
    BACKGROUND_INTERPOLATION_EASE_OUT_CUBIC = 2,
    BACKGROUND_INTERPOLATION_EASE_OUT_QUARTIC = 3,
    BACKGROUND_INTERPOLATION_EASE_IN_QUADRATIC = 4,
    BACKGROUND_INTERPOLATION_EASE_IN_CUBIC = 5,
    BACKGROUND_INTERPOLATION_EASE_IN_QUARTIC = 6,
    BACKGROUND_INTERPOLATION_CUBIC_HERMITE = 7
};

// Procedural camera motion modes observed in BackgroundRunStageScriptPhase.
enum BackgroundCameraMotionMode
{
    BACKGROUND_CAMERA_MOTION_DISABLED = 0,
    BACKGROUND_CAMERA_MOTION_HORIZONTAL_SWAY = 1,
    BACKGROUND_CAMERA_MOTION_HORIZONTAL_SWAY_AND_ROLL = 2,
    BACKGROUND_CAMERA_MOTION_ROTATE_UP_VECTOR = 3,
    BACKGROUND_CAMERA_MOTION_ORBIT_SWAY_AND_ROLL = 4
};

struct Background
{
    Background();
    void BeginSpellBackground();

    static Background *Create(int viewportIndex);
    static int OnUpdate(Background *background);
    static int OnDrawHighPrio(Background *background);
    static int OnDrawLowPrio(Background *background);
    static int AddedCallback(Background *background);
    static int DeletedCallback(Background *background);
    void ClearSpellBackgroundState();
    void ApplyPendingStageLabel();
    int UpdateStageObjectVms();

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
    BackgroundCameraRecord cameraTarget;
    BackgroundCameraRecord cameraStart;
    BackgroundCameraRecord cameraTangentEnd;
    BackgroundCameraRecord cameraTangentStart;
    int interpolationDuration[5];
    ZunTimer interpolationTimers[5];
    int interpolationModes[5];
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
    BackgroundPoint stagePositionTarget;
    int stagePositionEndFrame;
    BackgroundPoint stagePositionInitial;
    int stagePositionStartFrame;
    unsigned char compensateCameraJump;
    unsigned char unknown6429[3];
    unsigned int tintColor;
    int retainTint;
    float cullingDistanceSq;
    unsigned char cameraMotionMode;
    unsigned char unknown6439[3];
    int specialEffectPointCount;
    int collectSpecialEffectPoints;
    BackgroundSpecialEffectPoint specialEffectPoints[32];
};

typedef char BackgroundSizeIs65C4[(sizeof(Background) == 0x65C4) ? 1 : -1];
typedef char BackgroundStageVm0At02C[(offsetof(Background, stageVm0) == 0x02C) ? 1 : -1];
typedef char BackgroundStageAnmAt818[(offsetof(Background, stageAnm) == 0x818) ? 1 : -1];
typedef char BackgroundStageDataAt820[(offsetof(Background, stageData) == 0x820) ? 1 : -1];
typedef char BackgroundCameraTargetAt834[(offsetof(Background, cameraTarget) == 0x834) ? 1 : -1];
typedef char BackgroundCameraStartAt924[(offsetof(Background, cameraStart) == 0x924) ? 1 : -1];
typedef char BackgroundCameraTangentEndAtA14[(offsetof(Background, cameraTangentEnd) == 0xA14) ? 1 : -1];
typedef char BackgroundCameraTangentStartAtB04[(offsetof(Background, cameraTangentStart) == 0xB04) ? 1 : -1];
typedef char BackgroundDurationsAtBF4[(offsetof(Background, interpolationDuration) == 0xBF4) ? 1 : -1];
typedef char BackgroundStageTimerAtC58[(offsetof(Background, stageScriptTimer) == 0xC58) ? 1 : -1];
typedef char BackgroundSpellStateAtCCC[(offsetof(Background, spellBackgroundState) == 0xCCC) ? 1 : -1];
typedef char BackgroundSpellVmsAtCE0[(offsetof(Background, spellVms) == 0xCE0) ? 1 : -1];
typedef char BackgroundCallbackAt6404[(offsetof(Background, spellBackgroundDrawCallback) == 0x6404) ? 1 : -1];
typedef char BackgroundStagePositionTargetAt6408[(offsetof(Background, stagePositionTarget) == 0x6408) ? 1 : -1];
typedef char BackgroundStageEndFrameAt6414[(offsetof(Background, stagePositionEndFrame) == 0x6414) ? 1 : -1];
typedef char BackgroundStagePositionInitialAt6418[(offsetof(Background, stagePositionInitial) == 0x6418) ? 1 : -1];
typedef char BackgroundStageStartFrameAt6424[(offsetof(Background, stagePositionStartFrame) == 0x6424) ? 1 : -1];
typedef char BackgroundCompensateCameraJumpAt6428[(offsetof(Background, compensateCameraJump) == 0x6428) ? 1 : -1];
typedef char BackgroundTintAt642C[(offsetof(Background, tintColor) == 0x642C) ? 1 : -1];
typedef char BackgroundPointsAt6444[(offsetof(Background, specialEffectPoints) == 0x6444) ? 1 : -1];
