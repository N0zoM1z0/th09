#include "AnmManager.hpp"
#include "ExAttackController.hpp"
#include "PlayerLifecycleView.hpp"
#include "Supervisor.hpp"

struct ExAttackType3Extra
{
    int state00;
    Float3 motion04;
    Float3 history10[32];
};
typedef char ExAttackType3ExtraSizeIs190[
    (sizeof(ExAttackType3Extra) == 0x190) ? 1 : -1];

struct ExAttackType3RecordView
{
    int unknown00;
    int opponentSide04;
    int side08;
    int active0C;
    ZunTimer timer10;
    void *dynamicData1C;
    Float3 position20;
    int vmCount2C;
    int unknown30;
    ExAttackType3Extra *extra34;
    int parameter38;
    float angle3C;
};

struct ExAttackType3PlayerView
{
    unsigned char unknown000[0xBC];
    AnmLoaded *anmFileBC;
    unsigned char unknown0C0[0x1B88 - 0x0C0];
    Float3 position1B88;
};

struct ExAttackType3SideView
{
    unsigned char unknown00[0x04];
    ExAttackType3PlayerView *player04;
    unsigned char unknown08[0x30];
};

struct ExAttackType3GameManagerView
{
    ExAttackType3SideView sides[2];

    float TransformPopupX(float value);
    float TransformPopupY(float value);
};

struct ExAttackType3SideLookupView
{
    int unknown00;
    int opponentSide04;
    int side08;
    ExAttackType3SideView *GetSideState();
};

struct ExAttackType3AllocateView
{
    unsigned char bytes[0x38];
    void AllocateDynamicData(int vmCount, int extraBytes);
};

extern ExAttackType3GameManagerView g_GameManager;
extern ExAttackController *g_ExAttackController;

namespace Th09EclRunControl {
float __stdcall VectorAngle(float y, float x);
}

int __fastcall ExAttackInitializeCallbackType3(ExAttackRecord *base)
{
    ExAttackType3RecordView *record =
        reinterpret_cast<ExAttackType3RecordView *>(base);

    reinterpret_cast<ExAttackType3AllocateView *>(record)
        ->AllocateDynamicData(5, sizeof(ExAttackType3Extra));

    ExAttackType3Extra *extra = record->extra34;

    reinterpret_cast<ExAttackType3SideLookupView *>(record)
        ->GetSideState()->player04->anmFileBC->ExecuteAnmIdx(
            reinterpret_cast<AnmVm *>(record->dynamicData1C) + 0, 9);
    reinterpret_cast<ExAttackType3SideLookupView *>(record)
        ->GetSideState()->player04->anmFileBC->ExecuteAnmIdx(
            reinterpret_cast<AnmVm *>(record->dynamicData1C) + 1, 10);
    reinterpret_cast<ExAttackType3SideLookupView *>(record)
        ->GetSideState()->player04->anmFileBC->ExecuteAnmIdx(
            reinterpret_cast<AnmVm *>(record->dynamicData1C) + 2, 11);
    reinterpret_cast<ExAttackType3SideLookupView *>(record)
        ->GetSideState()->player04->anmFileBC->ExecuteAnmIdx(
            reinterpret_cast<AnmVm *>(record->dynamicData1C) + 3, 12);
    reinterpret_cast<ExAttackType3SideLookupView *>(record)
        ->GetSideState()->player04->anmFileBC->ExecuteAnmIdx(
            reinterpret_cast<AnmVm *>(record->dynamicData1C) + 4, 13);

    g_Supervisor.SelectSide(record->opponentSide04);
    ExAttackType3PlayerView *opponent =
        g_GameManager.sides[record->opponentSide04].player04;
    ExAttackVec3 opponentPosition;
    opponentPosition.x =
        g_GameManager.TransformPopupX(opponent->position1B88.x);
    opponentPosition.y =
        g_GameManager.TransformPopupY(opponent->position1B88.y);

    g_Supervisor.SelectSide(record->side08);
    ExAttackVec3 spawnPosition;
    spawnPosition.x = g_GameManager.TransformPopupX(record->position20.x);
    spawnPosition.y = g_GameManager.TransformPopupY(record->position20.y);
    spawnPosition.z = 0.0f;

    record->position20.x = spawnPosition.x;
    record->position20.y = spawnPosition.y;
    record->position20.z = spawnPosition.z;
    record->unknown00 = 2;

    float angle = Th09EclRunControl::VectorAngle(
        opponentPosition.y - spawnPosition.y,
        opponentPosition.x - spawnPosition.x);
    record->angle3C = angle;

    extra->motion04.FromAngleMagnitude(angle, 3.5f);
    extra->state00 = 0;

    g_ExAttackController->Spawn(
        4,
        reinterpret_cast<const Float3 *>(&spawnPosition),
        record->side08,
        reinterpret_cast<int>(base));

    Float3 *history = extra->history10;
    int remaining = 32;
    do
    {
        *history++ = record->position20;
    }
    while (--remaining != 0);

    return 0;
}
