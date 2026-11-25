#include "RuntimePolarMath.hpp"
#include "AnmManager.hpp"
#include "AsciiGameManagerView.hpp"
#include "ExAttackController.hpp"
#include "ExAttackRecordRuntimeView.hpp"
#include "ExAttackType3DataView.hpp"
#include "PlayerLifecycleView.hpp"
#include "Supervisor.hpp"


namespace Th09EclRunControl {
float __stdcall VectorAngle(float y, float x);
}

int __fastcall ExAttackInitializeCallbackType3(ExAttackRecord *base)
{
    ExAttackType3RecordView *record =
        reinterpret_cast<ExAttackType3RecordView *>(base);

    reinterpret_cast<ExAttackAllocateRecordView *>(record)
        ->AllocateDynamicData(5, sizeof(ExAttackType3Extra));

    ExAttackType3Extra *extra = record->extra34;

    reinterpret_cast<ExAttackRecordSideView *>(record)
        ->GetSideState()->player04->header24.anmFile98->ExecuteAnmIdx(
            record->vms1C + 0, 9);
    reinterpret_cast<ExAttackRecordSideView *>(record)
        ->GetSideState()->player04->header24.anmFile98->ExecuteAnmIdx(
            record->vms1C + 1, 10);
    reinterpret_cast<ExAttackRecordSideView *>(record)
        ->GetSideState()->player04->header24.anmFile98->ExecuteAnmIdx(
            record->vms1C + 2, 11);
    reinterpret_cast<ExAttackRecordSideView *>(record)
        ->GetSideState()->player04->header24.anmFile98->ExecuteAnmIdx(
            record->vms1C + 3, 12);
    reinterpret_cast<ExAttackRecordSideView *>(record)
        ->GetSideState()->player04->header24.anmFile98->ExecuteAnmIdx(
            record->vms1C + 4, 13);

    g_Supervisor.SelectSide(record->opponentSide04);
    PlayerLifecycleView *opponent =
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

    record->position20 =
        *reinterpret_cast<Float3 *>(&spawnPosition);
    record->unknown00 = 2;

    float angle = Th09EclRunControl::VectorAngle(
        opponentPosition.y - spawnPosition.y,
        opponentPosition.x - spawnPosition.x);
    record->angle3C = angle;

    SetPolar2D(reinterpret_cast<Float2 *>(&(extra->motion04)), angle, 3.5f);
    extra->state00 = 0;

    g_GameManager.exAttackControllerAC->Spawn(
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
