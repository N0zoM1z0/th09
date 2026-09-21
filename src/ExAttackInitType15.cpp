#include "AnmManager.hpp"
#include "AsciiGameManagerView.hpp"
#include "ExAttackController.hpp"
#include "RngRuntimeLeaves.hpp"
#include "Supervisor.hpp"

#include <stddef.h>

struct ExAttackType2InitExtra
{
    int state00;
    Float3 motion04;
    Float3 point10;
    Float3 point1C;
    Float3 point28;
    Float3 point34;
    Float3 spawnPosition40;
};
typedef char ExAttackType2InitExtraSizeIs4C[
    (sizeof(ExAttackType2InitExtra) == 0x4C) ? 1 : -1];

struct ExAttackType2InitRecord
{
    int state00;
    int opponentSide04;
    int side08;
    int active0C;
    ZunTimer timer10;
    void *dynamicData1C;
    Float3 position20;
    int vmCount2C;
    int unknown30;
    ExAttackType2InitExtra *extra34;
};

struct ExAttackAllocateRecordView
{
    unsigned char bytes[0x38];
    void AllocateDynamicData(int vmCount, int extraBytes);
};

struct ExAttackInitAnmOwner
{
    unsigned char unknown000[0x2D4];
    AnmLoaded *anm2D4;
};

struct ExAttackGameManagerSideView
{
    unsigned char unknown00[0x0C];
    ExAttackInitAnmOwner *anmOwner0C;
    unsigned char unknown10[0x28];
};

struct ExAttackRecordSideView
{
    int unknown00;
    int opponentSide04;
    int side08;
    ExAttackGameManagerSideView *GetSideState();
};

extern RngRuntimeView g_ReplayRng;
extern float g_ExAttackPlayfieldWidth;

int __fastcall ExAttackInitializeCallbackType15(ExAttackRecord *base)
{
    ExAttackType2InitRecord *record =
        reinterpret_cast<ExAttackType2InitRecord *>(base);

    reinterpret_cast<ExAttackAllocateRecordView *>(record)->
        AllocateDynamicData(1, sizeof(ExAttackType2InitExtra));

    reinterpret_cast<ExAttackRecordSideView *>(record)
        ->GetSideState()
        ->anmOwner0C
        ->anm2D4
        ->ExecuteAnmIdx(
            reinterpret_cast<AnmVm *>(record->dynamicData1C), 37);

    ExAttackType2InitExtra *extra = record->extra34;

    float angle = g_ReplayRng.GetRandomF32InRange(-3.1415927f);
    extra->motion04.z = 0.0f;
    extra->motion04.FromAngleMagnitude(angle, 2.0f);
    extra->state00 = 0;
    record->state00 = 2;

    extra->point28.FromAngleMagnitude(
        g_ReplayRng.GetRandomF32SignedInRange(3.1415927f),
        g_ReplayRng.GetRandomF32InRange(192.0f));

    extra->point34.FromAngleMagnitude(
        g_ReplayRng.GetRandomF32SignedInRange(3.1415927f),
        g_ReplayRng.GetRandomF32InRange(192.0f));

    extra->point10.x = g_GameManager.TransformPopupX(record->position20.x);
    extra->point10.y = g_GameManager.TransformPopupY(record->position20.y);
    extra->point10.z = 0.0f;

    extra->spawnPosition40.x =
        g_ReplayRng.GetRandomF32SignedInRange(
            g_ExAttackPlayfieldWidth * 0.5f - 8.0f);
    extra->spawnPosition40.y =
        g_ReplayRng.GetRandomF32InRange(200.0f) + 128.0f;
    extra->spawnPosition40.z = 0.0f;

    g_Supervisor.SelectSide(record->opponentSide04);
    extra->point1C.x =
        g_GameManager.TransformPopupX(extra->spawnPosition40.x);
    extra->point1C.y =
        g_GameManager.TransformPopupY(extra->spawnPosition40.y);
    extra->point1C.z = 0.0f;
    g_Supervisor.SelectSide(record->side08);

    return 0;
}
