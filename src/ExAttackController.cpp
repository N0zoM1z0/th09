#include "ExAttackController.hpp"
#include "ZunMemory.hpp"
#include "SoundPlayer.hpp"

#include <new>
#include <string.h>

extern Chain g_Chain;
extern ZunMemory g_ZunMemory;

struct ExAttackReleaseType4RecordView;
extern int __fastcall PlayerCheckState2();
extern int __fastcall ExAttackInitializeCallbackType0(ExAttackRecord *record);
extern int __fastcall ExAttackInitializeCallbackType1(ExAttackRecord *record);
extern int __fastcall ExAttackInitializeCallbackType2(ExAttackRecord *record);
extern int __fastcall ExAttackInitializeCallbackType3(ExAttackRecord *record);
extern int __fastcall ExAttackInitializeCallbackType4(ExAttackRecord *record);
extern int __fastcall ExAttackInitializeCallbackType5(ExAttackRecord *record);
extern int __fastcall ExAttackInitializeCallbackType6(ExAttackRecord *record);
extern int __fastcall ExAttackInitializeCallbackType7(ExAttackRecord *record);
extern int __fastcall ExAttackInitializeCallbackType8(ExAttackRecord *record);
extern int __fastcall ExAttackInitializeCallbackType9(ExAttackRecord *record);
extern int __fastcall ExAttackInitializeCallbackType10(ExAttackRecord *record);
extern int __fastcall ExAttackInitializeCallbackType11And12(ExAttackRecord *record);
extern int __fastcall ExAttackInitializeCallbackType13(ExAttackRecord *record);
extern int __fastcall ExAttackInitializeCallbackType14(ExAttackRecord *record);
extern int __fastcall ExAttackInitializeCallbackType15(ExAttackRecord *record);
extern int __fastcall ExAttackInitializeCallbackType16(ExAttackRecord *record);
extern int __fastcall ExAttackInitializeCallbackType17(ExAttackRecord *record);
extern int __fastcall ExAttackInitializeCallbackType18(ExAttackRecord *record);
extern int __fastcall ExAttackInitializeCallbackType19(ExAttackRecord *record);
extern int __fastcall ExAttackInitializeCallbackType20(ExAttackRecord *record);
extern int __fastcall ExAttackInitializeCallbackType21(ExAttackRecord *record);
extern int __fastcall ExAttackInitializeCallbackType22(ExAttackRecord *record);
extern int __fastcall ExAttackInitializeCallbackType23(ExAttackRecord *record);
extern int __fastcall ExAttackInitializeCallbackType24(ExAttackRecord *record);
extern int __fastcall ExAttackInitializeCallbackType25(ExAttackRecord *record);
extern int __fastcall ExAttackInitializeCallbackType26(ExAttackRecord *record);
extern int __fastcall ExAttackUpdateCallbackType01(ExAttackRecord *record);
extern int __fastcall ExAttackUpdateCallbackType2(ExAttackRecord *record);
extern int __fastcall ExAttackUpdateCallbackType3(ExAttackRecord *record);
extern int __fastcall ExAttackUpdateCallbackType4(ExAttackRecord *record);
extern int __fastcall ExAttackUpdateCallbackType5(ExAttackRecord *record);
extern int __fastcall ExAttackUpdateCallbackType6(ExAttackRecord *record);
extern int __fastcall ExAttackUpdateCallbackType7(ExAttackRecord *record);
extern int __fastcall ExAttackUpdateCallbackType8_9(ExAttackRecord *record);
extern int __fastcall ExAttackUpdateCallbackType11(ExAttackRecord *record);
extern int __fastcall ExAttackUpdateCallbackType12(ExAttackRecord *record);
extern int __fastcall ExAttackUpdateCallbackType13(ExAttackRecord *record);
extern int __fastcall ExAttackUpdateCallbackType14_22(ExAttackRecord *record);
extern int __fastcall ExAttackUpdateCallbackType15(ExAttackRecord *record);
extern int __fastcall ExAttackUpdateCallbackType16_23(ExAttackRecord *record);
extern int __fastcall ExAttackUpdateCallbackType17(ExAttackRecord *record);
extern int __fastcall ExAttackUpdateCallbackType18_24(ExAttackRecord *record);
extern int __fastcall ExAttackUpdateCallbackType19(ExAttackRecord *record);
extern int __fastcall ExAttackUpdateCallbackType20(ExAttackRecord *record);
extern int __fastcall ExAttackUpdateCallbackType21(ExAttackRecord *record);
extern int __fastcall ExAttackUpdateCallbackType25(ExAttackRecord *record);
extern int __fastcall ExAttackUpdateCallbackType26(ExAttackRecord *record);
extern int __fastcall ExAttackDrawCallbackType14_22(ExAttackRecord *record);
extern int __fastcall ExAttackDrawCallbackType19_21(ExAttackRecord *record);
extern int __fastcall ExAttackReleaseCallbackType4(ExAttackReleaseType4RecordView *record);

ExAttackTemplate g_ExAttackTemplates[27] = {
    { ExAttackInitializeCallbackType0, ExAttackUpdateCallbackType01,
      reinterpret_cast<ExAttackRecordCallback>(PlayerCheckState2),
      reinterpret_cast<ExAttackRecordCallback>(PlayerCheckState2) },
    { ExAttackInitializeCallbackType1, ExAttackUpdateCallbackType01,
      reinterpret_cast<ExAttackRecordCallback>(PlayerCheckState2),
      reinterpret_cast<ExAttackRecordCallback>(PlayerCheckState2) },
    { ExAttackInitializeCallbackType2, ExAttackUpdateCallbackType2,
      reinterpret_cast<ExAttackRecordCallback>(PlayerCheckState2),
      reinterpret_cast<ExAttackRecordCallback>(PlayerCheckState2) },
    { ExAttackInitializeCallbackType3, ExAttackUpdateCallbackType3,
      reinterpret_cast<ExAttackRecordCallback>(PlayerCheckState2),
      reinterpret_cast<ExAttackRecordCallback>(PlayerCheckState2) },
    { ExAttackInitializeCallbackType4, ExAttackUpdateCallbackType4,
      reinterpret_cast<ExAttackRecordCallback>(PlayerCheckState2),
      reinterpret_cast<ExAttackRecordCallback>(ExAttackReleaseCallbackType4) },
    { ExAttackInitializeCallbackType5, ExAttackUpdateCallbackType5,
      reinterpret_cast<ExAttackRecordCallback>(PlayerCheckState2),
      reinterpret_cast<ExAttackRecordCallback>(PlayerCheckState2) },
    { ExAttackInitializeCallbackType6, ExAttackUpdateCallbackType6,
      reinterpret_cast<ExAttackRecordCallback>(PlayerCheckState2),
      reinterpret_cast<ExAttackRecordCallback>(PlayerCheckState2) },
    { ExAttackInitializeCallbackType7, ExAttackUpdateCallbackType7,
      reinterpret_cast<ExAttackRecordCallback>(PlayerCheckState2),
      reinterpret_cast<ExAttackRecordCallback>(PlayerCheckState2) },
    { ExAttackInitializeCallbackType8, ExAttackUpdateCallbackType8_9,
      reinterpret_cast<ExAttackRecordCallback>(PlayerCheckState2),
      reinterpret_cast<ExAttackRecordCallback>(PlayerCheckState2) },
    { ExAttackInitializeCallbackType9, ExAttackUpdateCallbackType8_9,
      reinterpret_cast<ExAttackRecordCallback>(PlayerCheckState2),
      reinterpret_cast<ExAttackRecordCallback>(PlayerCheckState2) },
    { ExAttackInitializeCallbackType10, ExAttackUpdateCallbackType5,
      reinterpret_cast<ExAttackRecordCallback>(PlayerCheckState2),
      reinterpret_cast<ExAttackRecordCallback>(PlayerCheckState2) },
    { ExAttackInitializeCallbackType11And12, ExAttackUpdateCallbackType11,
      reinterpret_cast<ExAttackRecordCallback>(PlayerCheckState2),
      reinterpret_cast<ExAttackRecordCallback>(PlayerCheckState2) },
    { ExAttackInitializeCallbackType11And12, ExAttackUpdateCallbackType12,
      reinterpret_cast<ExAttackRecordCallback>(PlayerCheckState2),
      reinterpret_cast<ExAttackRecordCallback>(PlayerCheckState2) },
    { ExAttackInitializeCallbackType13, ExAttackUpdateCallbackType13,
      reinterpret_cast<ExAttackRecordCallback>(PlayerCheckState2),
      reinterpret_cast<ExAttackRecordCallback>(PlayerCheckState2) },
    { ExAttackInitializeCallbackType14, ExAttackUpdateCallbackType14_22,
      ExAttackDrawCallbackType14_22,
      reinterpret_cast<ExAttackRecordCallback>(PlayerCheckState2) },
    { ExAttackInitializeCallbackType15, ExAttackUpdateCallbackType15,
      reinterpret_cast<ExAttackRecordCallback>(PlayerCheckState2),
      reinterpret_cast<ExAttackRecordCallback>(PlayerCheckState2) },
    { ExAttackInitializeCallbackType16, ExAttackUpdateCallbackType16_23,
      reinterpret_cast<ExAttackRecordCallback>(PlayerCheckState2),
      reinterpret_cast<ExAttackRecordCallback>(PlayerCheckState2) },
    { ExAttackInitializeCallbackType17, ExAttackUpdateCallbackType17,
      reinterpret_cast<ExAttackRecordCallback>(PlayerCheckState2),
      reinterpret_cast<ExAttackRecordCallback>(PlayerCheckState2) },
    { ExAttackInitializeCallbackType18, ExAttackUpdateCallbackType18_24,
      reinterpret_cast<ExAttackRecordCallback>(PlayerCheckState2),
      reinterpret_cast<ExAttackRecordCallback>(PlayerCheckState2) },
    { ExAttackInitializeCallbackType19, ExAttackUpdateCallbackType19,
      ExAttackDrawCallbackType19_21,
      reinterpret_cast<ExAttackRecordCallback>(PlayerCheckState2) },
    { ExAttackInitializeCallbackType20, ExAttackUpdateCallbackType20,
      reinterpret_cast<ExAttackRecordCallback>(PlayerCheckState2),
      reinterpret_cast<ExAttackRecordCallback>(PlayerCheckState2) },
    { ExAttackInitializeCallbackType21, ExAttackUpdateCallbackType21,
      ExAttackDrawCallbackType19_21,
      reinterpret_cast<ExAttackRecordCallback>(PlayerCheckState2) },
    { ExAttackInitializeCallbackType22, ExAttackUpdateCallbackType14_22,
      ExAttackDrawCallbackType14_22,
      reinterpret_cast<ExAttackRecordCallback>(PlayerCheckState2) },
    { ExAttackInitializeCallbackType23, ExAttackUpdateCallbackType16_23,
      reinterpret_cast<ExAttackRecordCallback>(PlayerCheckState2),
      reinterpret_cast<ExAttackRecordCallback>(PlayerCheckState2) },
    { ExAttackInitializeCallbackType24, ExAttackUpdateCallbackType18_24,
      reinterpret_cast<ExAttackRecordCallback>(PlayerCheckState2),
      reinterpret_cast<ExAttackRecordCallback>(PlayerCheckState2) },
    { ExAttackInitializeCallbackType25, ExAttackUpdateCallbackType25,
      reinterpret_cast<ExAttackRecordCallback>(PlayerCheckState2),
      reinterpret_cast<ExAttackRecordCallback>(PlayerCheckState2) },
    { ExAttackInitializeCallbackType26, ExAttackUpdateCallbackType26,
      reinterpret_cast<ExAttackRecordCallback>(PlayerCheckState2),
      reinterpret_cast<ExAttackRecordCallback>(PlayerCheckState2) },
};

// The original callback table is indexed as a signed native DWORD with
// 0x10-byte template stride; a C++ array bound is not a retail condition.
static inline ExAttackTemplate &NativeExAttackTemplate(int type)
{
    return *reinterpret_cast<ExAttackTemplate *>(
        reinterpret_cast<unsigned int>(&g_ExAttackTemplates[0]) +
        static_cast<unsigned int>(type) * sizeof(g_ExAttackTemplates[0]));
}


ExAttackRecord::ExAttackRecord()
{
}

ExAttackController::ExAttackController()
{
}

static int ReleaseExAttackRecords(ExAttackController *controller)
{
    ExAttackRecord *record = controller->records;
    for (int i = 0; i < 256; ++i, ++record)
    {
        if (record->active)
        {
            if (record->releaseCallback != NULL)
                record->releaseCallback(record);
            if (record->dynamicData != NULL)
            {
                g_ZunMemory.Free(record->dynamicData);
                record->dynamicData = NULL;
            }
        }
    }
    return 0;
}

void ExAttackController::Release(ExAttackController *controller)
{
    if (controller == NULL)
        return;
    ReleaseExAttackRecords(controller);
    g_Chain.Cut(controller->drawChain1);
    g_Chain.Cut(controller->drawChain0);
    g_Chain.Cut(controller->calcChain);
    delete controller;
}

ExAttackRecord *ExAttackController::Spawn(
    int type, const Float3 *position, int side, int parameter)
{
    ExAttackRecord *record = this->records;
    int activeForSide = 0;
    int i;
    for (i = 0; i < 256; ++i, ++record)
    {
        if (record->active && record->side == side)
            activeForSide++;
    }
    if (activeForSide >= *reinterpret_cast<const int *>(
        reinterpret_cast<unsigned int>(&this->sideLimits[0]) +
        static_cast<unsigned int>(side) * sizeof(this->sideLimits[0])))
        return &this->records[256];

    g_SoundPlayer.PlaySoundByIdx(SOUND_EX_ATTACK, side != 0 ? 500 : -500);
    record = this->records;
    i = 0;
    do
    {
        if (!record->active)
            goto found_slot;
        i++;
        record++;
    }
    while (i < 256);
    return record;

found_slot:
    memset(record, 0, sizeof(ExAttackRecord));
    record->active = 1;
    record->position = *reinterpret_cast<const ExAttackVec3 *>(position);
    record->side = side;
    record->opponentSide = static_cast<int>(
        1u - static_cast<unsigned int>(side));
    record->parameter38 = parameter;
    record->updateCallback = NativeExAttackTemplate(type).updateCallback;
    record->drawCallback = NativeExAttackTemplate(type).drawCallback;
    record->releaseCallback = NativeExAttackTemplate(type).releaseCallback;
    record->timer = 0;
    if (NativeExAttackTemplate(type).initializeCallback(record) != 0)
        record->active = 0;
    return record;
}

ExAttackController *ExAttackController::Register()
{
    ExAttackController *controller = new ExAttackController;
    controller = static_cast<ExAttackController *>(g_ZunMemory.AddToRegistry(
        controller, sizeof(ExAttackController), const_cast<char *>("ExAttackCtrlInf")));
    memset(controller, 0, sizeof(ExAttackController));

    controller->calcChain = g_Chain.CreateElem(
        reinterpret_cast<ChainCallback>(ExAttackController::OnUpdate));
    controller->calcChain->arg = controller;
    controller->calcChain->addedCallback =
        reinterpret_cast<ChainLifetimeCallback>(ExAttackController::AddedCallback);
    if (g_Chain.AddToCalcChain(controller->calcChain, 14) != 0)
        return NULL;

    controller->drawChain0 = g_Chain.CreateElem(
        reinterpret_cast<ChainCallback>(ExAttackController::OnDraw0));
    controller->drawChain0->arg = controller;
    g_Chain.AddToDrawChain(controller->drawChain0, 16);

    controller->drawChain1 = g_Chain.CreateElem(
        reinterpret_cast<ChainCallback>(ExAttackController::OnDraw1));
    controller->drawChain1->arg = controller;
    g_Chain.AddToDrawChain(controller->drawChain1, 29);
    return controller;
}
