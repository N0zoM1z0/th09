#include "EclRunState.inl"

#include <string.h>

// Maintained natural reconstruction of the substantial helper owners around
// EclManager::RunEcl.  The target compiler passes several of these through
// private live-register conventions; the public C++ signatures intentionally
// retain ordinary /Gr lowering and therefore do not claim exactness.

namespace
{

struct EclHelperEnemyView
{
    EnemyManagerView *manager0000;
    unsigned char unknown0004[0x07F4 - 0x0004];
    Th09EclContextView mainContext07F4;
    Th09EclContextView mainCallStack0A20[TH09_ECL_MAIN_CALL_STACK_CAPACITY];
    Th09EclContextView *activeContext2CE0;
    Th09EclContextView *activeCallStack2CE4;
    int intVariables2CE8[8];
    float floatVariables2D08[8];
    short mainCallDepth2D28;
    short activeCallDepth2D2A;
    unsigned char unknown2D2C[0x04];
    short subroutineSlots2D30[32];
    short pendingSubroutineId2D70;
    unsigned char unknown2D72[0x02];
    Float3 position2D74;
    Float3 positionOffset2D80;
    Float3 velocity2D8C;
    unsigned char unknown2D98[0x2DD4 - 0x2D98];
    Float3 worldPosition2DD4;
    float movementAngle2DE0;
    float angularVelocity2DE4;
    float orbitAngle2DE8;
    float orbitAngularVelocity2DEC;
    unsigned char unknown2DF0[0x04];
    float speed2DF4;
    float acceleration2DF8;
    float orbitRadius2DFC;
    float radialVelocity2E00;
    Float3 shootOffset2E04;
    Float3 movementDelta2E10;
    Float3 movementOrigin2E1C;
    Th09EclTimerStorageView movementTimer2E28;
    int movementDuration2E34;
    unsigned char unknown2E38[0x2E74 - 0x2E38];
    BulletSpawnDescriptor bulletDescriptor2E74;
    unsigned char unknown3088[0x337C - 0x3088];
    unsigned int primaryFlags337C;
    unsigned int secondaryFlags3380;
    unsigned char unknown3384[0x02];
    signed char anmDirection3386;
    unsigned char unknown3387[0x03];
    short idleAnm338A;
    short idleFromLeftAnm338C;
    short idleFromRightAnm338E;
    short moveLeftAnm3390;
    short moveRightAnm3392;
    short specialAnm3394;
    unsigned char unknown3396[0x02];
    Th09EclRunMovement::EnemyFloat2 movementLowerBounds3398;
    Th09EclRunMovement::EnemyFloat2 movementUpperBounds33A0;
    float minimumPlayerDistanceSquared33A8;
    unsigned char unknown33AC[0x2C];
    void *childEclBlocks33D8[4];
};

typedef char EclHelperActiveContextAt2CE0[
    (offsetof(EclHelperEnemyView, activeContext2CE0) == 0x2CE0) ? 1 : -1];
typedef char EclHelperPositionAt2D74[
    (offsetof(EclHelperEnemyView, position2D74) == 0x2D74) ? 1 : -1];
typedef char EclHelperMovementAt2DE0[
    (offsetof(EclHelperEnemyView, movementAngle2DE0) == 0x2DE0) ? 1 : -1];
typedef char EclHelperBulletAt2E74[
    (offsetof(EclHelperEnemyView, bulletDescriptor2E74) == 0x2E74) ? 1 : -1];
typedef char EclHelperFlagsAt337C[
    (offsetof(EclHelperEnemyView, primaryFlags337C) == 0x337C) ? 1 : -1];
typedef char EclHelperChildrenAt33D8[
    (offsetof(EclHelperEnemyView, childEclBlocks33D8) == 0x33D8) ? 1 : -1];

inline EclHelperEnemyView *HelperView(EnemyView *enemy)
{
    return reinterpret_cast<EclHelperEnemyView *>(enemy);
}

inline float *SlotParameters(Th09EclInterpolationSlotView *slot)
{
    return reinterpret_cast<float *>(slot->unknown1C);
}

void __fastcall InterpolateLinear(
    EnemyView *enemy, Th09EclInterpolationSlotView *slot, float progress)
{
    float start = enemy->ResolveFloat(SlotParameters(slot)[0]);
    float end = enemy->ResolveFloat(SlotParameters(slot)[1]);
    *Th09EclRunControl::ResolveFloatLValue(
        enemy, &slot->affectedVariable2C, 0, -1) =
        (end - start) * progress + start;
}

void __fastcall InterpolateHermite(
    EnemyView *enemy, Th09EclInterpolationSlotView *slot, float progress)
{
    float parameter0 = enemy->ResolveFloat(SlotParameters(slot)[0]);
    float parameter1 = enemy->ResolveFloat(SlotParameters(slot)[1]);
    float parameter2 = enemy->ResolveFloat(SlotParameters(slot)[2]);
    float parameter3 = enemy->ResolveFloat(SlotParameters(slot)[3]);
    float twice = progress + progress;
    float minusOne = progress - 1.0f;
    float inverse = 1.0f - progress;

    *Th09EclRunControl::ResolveFloatLValue(
        enemy, &slot->affectedVariable2C, 0, -1) =
        (twice + 1.0f) * minusOne * minusOne * parameter0 +
        (3.0f - twice) * progress * progress * parameter1 +
        inverse * inverse * progress * parameter2 +
        minusOne * progress * progress * parameter3;
}


} // namespace

namespace Th09EclRunControl
{

Th09EclRawInstructionHeaderView *__fastcall CompareOperands(
    EnemyView *enemy,
    Th09EclRawInstructionHeaderView *instruction)
{
    bool success = false;
    switch (instruction->opcode04)
    {
    case TH09_ECL_OPCODE_JUMP_IF_INT_EQUAL:
        success = ReadInt(enemy, instruction, 0) == ReadInt(enemy, instruction, 1); break;
    case TH09_ECL_OPCODE_JUMP_IF_FLOAT_EQUAL:
        success = ReadFloat(enemy, instruction, 0) == ReadFloat(enemy, instruction, 1); break;
    case TH09_ECL_OPCODE_JUMP_IF_INT_NOT_EQUAL:
        success = ReadInt(enemy, instruction, 0) != ReadInt(enemy, instruction, 1); break;
    case TH09_ECL_OPCODE_JUMP_IF_FLOAT_NOT_EQUAL:
        success = ReadFloat(enemy, instruction, 0) != ReadFloat(enemy, instruction, 1); break;
    case TH09_ECL_OPCODE_JUMP_IF_INT_LESS:
        success = ReadInt(enemy, instruction, 0) < ReadInt(enemy, instruction, 1); break;
    case TH09_ECL_OPCODE_JUMP_IF_FLOAT_LESS:
        success = ReadFloat(enemy, instruction, 0) < ReadFloat(enemy, instruction, 1); break;
    case TH09_ECL_OPCODE_JUMP_IF_INT_LESS_EQUAL:
        success = ReadInt(enemy, instruction, 0) <= ReadInt(enemy, instruction, 1); break;
    case TH09_ECL_OPCODE_JUMP_IF_FLOAT_LESS_EQUAL:
        success = ReadFloat(enemy, instruction, 0) <= ReadFloat(enemy, instruction, 1); break;
    case TH09_ECL_OPCODE_JUMP_IF_INT_GREATER:
        success = ReadInt(enemy, instruction, 0) > ReadInt(enemy, instruction, 1); break;
    case TH09_ECL_OPCODE_JUMP_IF_FLOAT_GREATER:
        success = ReadFloat(enemy, instruction, 0) > ReadFloat(enemy, instruction, 1); break;
    case TH09_ECL_OPCODE_JUMP_IF_INT_GREATER_EQUAL:
        success = ReadInt(enemy, instruction, 0) >= ReadInt(enemy, instruction, 1); break;
    case TH09_ECL_OPCODE_JUMP_IF_FLOAT_GREATER_EQUAL:
        success = ReadFloat(enemy, instruction, 0) >= ReadFloat(enemy, instruction, 1); break;
    default:
        break;
    }
    if (!success)
        return NULL;

    HelperView(enemy)->activeContext2CE0->time008.current08 = RawInt(instruction, 2);
    return reinterpret_cast<Th09EclRawInstructionHeaderView *>(
        reinterpret_cast<unsigned char *>(instruction) + RawInt(instruction, 3));
}

} // namespace Th09EclRunControl

namespace Th09EclRunMovement
{


} // namespace Th09EclRunMovement

namespace Th09EclRunLate
{


} // namespace Th09EclRunLate
