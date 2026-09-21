#include "AnmManager.hpp"
#include "AsciiManager.hpp"
#include "ExAttackController.hpp"
#include "Supervisor.hpp"
#include "ZunMemory.hpp"

struct ExAttackType4Extra
{
    int state00;
    Float3 parameterVector04;
    Float3 history10[32];
};
typedef char ExAttackType4ExtraSizeIs190[
    (sizeof(ExAttackType4Extra) == 0x190) ? 1 : -1];

struct ExAttackType4ParameterSource
{
    unsigned char unknown00[0x34];
    unsigned char *vectorOwner34;
    unsigned char unknown38[4];
    int value3C;
};

struct ExAttackType4RecordView
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
    ExAttackType4Extra *extra34;
    ExAttackType4ParameterSource *parameter38;
    int unknown3C;
};

struct ExAttackType4PlayerView
{
    unsigned char unknown000[0xBC];
    AnmLoaded *anmFileBC;
};

struct ExAttackType4SideView
{
    unsigned char unknown00[0x04];
    ExAttackType4PlayerView *player04;
    unsigned char unknown08[0x30];
};

struct ExAttackType4GameManagerView
{
    ExAttackType4SideView sides[2];

    float InverseTransformPopupX(float value);
    float InverseTransformPopupY(float value);
};

extern ExAttackType4GameManagerView g_GameManager;

int __fastcall ExAttackInitializeCallbackType4(ExAttackRecord *base)
{
    ExAttackType4RecordView *record =
        reinterpret_cast<ExAttackType4RecordView *>(base);

    record->vmCount2C = 5;
    record->dynamicData1C =
        g_ZunMemory.Alloc(5 * sizeof(AnmVm), "./system\global.h");

    g_GameManager.sides[record->side08].player04->anmFileBC->ExecuteAnmIdx(
        reinterpret_cast<AnmVm *>(record->dynamicData1C) + 0, 14);
    g_GameManager.sides[record->side08].player04->anmFileBC->ExecuteAnmIdx(
        reinterpret_cast<AnmVm *>(record->dynamicData1C) + 1, 15);
    g_GameManager.sides[record->side08].player04->anmFileBC->ExecuteAnmIdx(
        reinterpret_cast<AnmVm *>(record->dynamicData1C) + 2, 16);
    g_GameManager.sides[record->side08].player04->anmFileBC->ExecuteAnmIdx(
        reinterpret_cast<AnmVm *>(record->dynamicData1C) + 3, 17);
    g_GameManager.sides[record->side08].player04->anmFileBC->ExecuteAnmIdx(
        reinterpret_cast<AnmVm *>(record->dynamicData1C) + 4, 18);

    Float3 *position;
    ExAttackType4Extra *extra = static_cast<ExAttackType4Extra *>(
        g_ZunMemory.Alloc(sizeof(ExAttackType4Extra), "./system\global.h"));
    record->extra34 = extra;

    g_Supervisor.SelectSide(record->opponentSide04);
    position = &record->position20;
    position->x = g_GameManager.InverseTransformPopupX(position->x);
    position->y = g_GameManager.InverseTransformPopupY(position->y);

    record->unknown00 = record->opponentSide04;
    record->unknown3C = record->parameter38->value3C;

    Float3 *parameterVector =
        reinterpret_cast<Float3 *>(record->parameter38->vectorOwner34 + 4);
    extra->parameterVector04 = *parameterVector;
    extra->state00 = 0;

    Float3 *history = extra->history10;
    int remaining = 32;
    do
    {
        *history++ = *position;
    }
    while (--remaining != 0);

    return 0;
}
