#include "EclRunState.inl"

#include <string.h>

// Maintained natural reconstruction of the substantial helper owners around
// EclManager::RunEcl.  The target compiler passes several of these through
// private live-register conventions; the public C++ signatures intentionally
// retain ordinary /Gr lowering and therefore do not claim exactness.

namespace
{

struct EclHelperManagerView
{
    unsigned char unknown000[0x0168];
    int sharedInts168[4];
    float sharedFloats178[4];
};

struct EclHelperPlayerView
{
    unsigned char unknown0000[0x1B88];
    Float3 position1B88;
};

struct EclHelperRngView
{
    float GetRandomF32SignedInRange(float maximum);
};

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

inline EclHelperPlayerView *HelperPlayer(EnemyView *enemy)
{
    return reinterpret_cast<EclHelperPlayerView *>(
        enemy->manager00->sideState320->player04);
}

inline float *SlotParameters(Th09EclInterpolationSlotView *slot)
{
    return reinterpret_cast<float *>(slot->unknown1C);
}

inline int &SlotCallbackIndex(Th09EclInterpolationSlotView *slot)
{
    return slot->unknown14;
}

typedef void (__fastcall *EclInterpolationCallback)(
    EnemyView *, Th09EclInterpolationSlotView *, float);

extern EclInterpolationCallback g_EclInterpolationCallbacks[];

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

void StartTimedPolarDisplacement(
    EnemyView *enemy,
    Th09EclRawInstructionHeaderView *instruction,
    float angle)
{
    EclHelperEnemyView *view = HelperView(enemy);
    int duration = Th09EclRunControl::ReadInt(enemy, instruction, 0);
    float speed = Th09EclRunControl::ReadFloat(enemy, instruction, 2);

    view->movementDelta2E10.x =
        Th09EclRunControl::Cos(angle) * duration * speed;
    view->movementDelta2E10.y =
        Th09EclRunControl::Sin(angle) * duration * speed;
    view->movementDelta2E10.z = 0.0f;
    view->movementOrigin2E1C = view->worldPosition2DD4;
    view->movementDuration2E34 = duration;
    view->movementTimer2E28 = duration;
    view->primaryFlags337C =
        (view->primaryFlags337C & ~0x3A00U) |
        ((Th09EclRunControl::ReadInt(enemy, instruction, 1) & 7) << 11) |
        0x400U;
}

} // namespace

namespace Th09EclRunControl
{

void __fastcall ApplyInterpolationOperation(
    EnemyView *enemy,
    Th09EclRawInstructionHeaderView *instruction)
{
    float delta = ReadFloat(enemy, instruction, 1) -
                  ReadFloat(enemy, instruction, 2);
    *WriteFloat(enemy, instruction, 0) =
        delta * ReadFloat(enemy, instruction, 3) +
        ReadFloat(enemy, instruction, 2);
}

void __fastcall InstallInterpolationSlot(
    EnemyView *enemy,
    Th09EclRawInstructionHeaderView *instruction)
{
    Th09EclContextView *context = HelperView(enemy)->activeContext2CE0;
    for (int index = 0; index < TH09_ECL_INTERPOLATION_SLOT_COUNT; ++index)
    {
        Th09EclInterpolationSlotView *slot =
            &context->interpolationSlots0A0[index];
        if (slot->callback00 != NULL &&
            slot->affectedVariable2C != RawFloat(instruction, 0))
            continue;

        slot->timer04 = 0;
        slot->affectedVariable2C = RawFloat(instruction, 0);
        slot->duration10 = ReadInt(enemy, instruction, 1);
        SlotCallbackIndex(slot) = ReadInt(enemy, instruction, 2);
        slot->easing18 = ReadInt(enemy, instruction, 3);
        slot->callback00 = g_EclInterpolationCallbacks[SlotCallbackIndex(slot)];
        for (int parameter = 0; parameter < 4; ++parameter)
            SlotParameters(slot)[parameter] =
                ReadFloat(enemy, instruction, parameter + 4);
        break;
    }
}

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

void __fastcall CallSubroutine(
    EnemyView *enemy,
    Th09EclRawInstructionHeaderView *instruction,
    int rawSubroutineId)
{
    EclHelperEnemyView *view = HelperView(enemy);
    Th09EclContextView *context = view->activeContext2CE0;
    context->currentInstruction004 =
        reinterpret_cast<Th09EclRawInstructionHeaderView *>(
            reinterpret_cast<unsigned char *>(instruction) +
            static_cast<short>(instruction->nextOffset06));

    if ((view->primaryFlags337C &
         Th09EclRunState::ENEMY_STATE_CALL_STACK_DISABLED) == 0)
    {
        view->activeCallStack2CE4[view->activeCallDepth2D2A] = *context;
    }

    reinterpret_cast<Th09EclRunState::ManagerStateView *>(view->manager0000)
        ->InitializeSubroutine(context, static_cast<short>(rawSubroutineId));

    EclHelperManagerView *manager =
        reinterpret_cast<EclHelperManagerView *>(view->manager0000);
    memcpy(context->unknown01C + (0x74 - 0x1C),
           manager->sharedInts168, 0x20);

    if ((view->primaryFlags337C &
         Th09EclRunState::ENEMY_STATE_CALL_STACK_DISABLED) == 0 &&
        view->activeCallDepth2D2A < TH09_ECL_MAIN_CALL_STACK_CAPACITY - 1)
        ++view->activeCallDepth2D2A;
}


} // namespace Th09EclRunControl

namespace Th09EclRunMovement
{

void ConfigureRelativeMotion(
    EnemyView *enemy,
    Th09EclRawInstructionHeaderView *instruction)
{
    EclHelperEnemyView *view = HelperView(enemy);
    Float3 target(
        Th09EclRunControl::ReadFloat(enemy, instruction, 2),
        Th09EclRunControl::ReadFloat(enemy, instruction, 3),
        0.0f);
    view->movementDelta2E10 = target - view->position2D74;
    view->movementOrigin2E1C = view->position2D74;
    int duration = Th09EclRunControl::ReadInt(enemy, instruction, 0);
    view->movementDuration2E34 = duration;
    view->movementTimer2E28 = duration;
    view->primaryFlags337C =
        (view->primaryFlags337C & ~0x3A00U) |
        ((Th09EclRunControl::ReadInt(enemy, instruction, 1) & 7) << 11) |
        0x400U;
    Float3 zero(0.0f, 0.0f, 0.0f);
    view->positionOffset2D80 = zero;
    if ((view->primaryFlags337C & 0x8000U) != 0)
        view->movementDelta2E10.x = -view->movementDelta2E10.x;
}

void BeginBoundaryAwareMove(
    EnemyView *enemy,
    Th09EclRawInstructionHeaderView *instruction)
{
    EclHelperEnemyView *view = HelperView(enemy);
    float angle;
    if (HelperPlayer(enemy)->position1B88.x < view->position2D74.x)
        angle = Th09EclRunControl::AddNormalizeAngle(
            Th09EclRunControl::g_Rng.GetRandomF32InRange(1.5707964f) +
                2.3561945f, 0.0f);
    else
        angle = Th09EclRunControl::g_Rng.GetRandomF32InRange(1.5707964f) -
                0.78539819f;

    if (view->position2D74.x < view->movementLowerBounds3398.x + 96.0f)
    {
        if (angle > 1.5707964f) angle = 3.1415927f - angle;
        else if (angle < -1.5707964f) angle = -3.1415927f - angle;
    }
    if (view->position2D74.x > view->movementUpperBounds33A0.x - 96.0f)
    {
        if (angle < 1.5707964f && angle >= 0.0f)
            angle = 3.1415927f - view->movementAngle2DE0;
        else if (angle > -1.5707964f && angle <= 0.0f)
            angle = -3.1415927f - angle;
    }
    if (view->position2D74.y < view->movementLowerBounds3398.y + 48.0f && angle < 0.0f)
        angle = -angle;
    if (view->position2D74.y > view->movementUpperBounds33A0.y - 48.0f && angle > 0.0f)
        angle = -angle;

    if (Th09EclRunControl::ReadInt(enemy, instruction, 0) <= 0)
    {
        view->movementAngle2DE0 = angle;
        view->speed2DF4 = Th09EclRunControl::ReadFloat(enemy, instruction, 2);
        view->primaryFlags337C =
            (view->primaryFlags337C & ~ENEMY_MOVEMENT_MODE_MASK) |
            ENEMY_MOVEMENT_MODE_POLAR;
        view->movementDuration2E34 = 0;
        view->movementTimer2E28 = 0;
    }
    else
        StartTimedPolarDisplacement(enemy, instruction, angle);
}

} // namespace Th09EclRunMovement

namespace Th09EclRunLate
{

void MoveRandomBiased(
    EnemyView *enemy,
    Th09EclRawInstructionHeaderView *instruction)
{
    EclHelperEnemyView *view = HelperView(enemy);
    float playerX = HelperPlayer(enemy)->position1B88.x;
    float enemyX = view->position2D74.x;
    float angle;

    if (Th09EclRunControl::g_Rng.GetRandomU32InRange(4) != 0)
    {
        if (playerX >= enemyX)
        {
            if (enemyX - (playerX - 384.0f) <= playerX - enemyX)
                angle = Th09EclRunControl::AddNormalizeAngle(
                    Th09EclRunControl::g_Rng.GetRandomF32InRange(1.5707964f) +
                        2.3561945f,
                    0.0f);
            else
                angle =
                    Th09EclRunControl::g_Rng.GetRandomF32InRange(1.5707964f) -
                    0.78539819f;
        }
        else
        {
            if (playerX + 384.0f - enemyX <= enemyX - playerX)
                angle =
                    Th09EclRunControl::g_Rng.GetRandomF32InRange(1.5707964f) -
                    0.78539819f;
            else
                angle = Th09EclRunControl::AddNormalizeAngle(
                    Th09EclRunControl::g_Rng.GetRandomF32InRange(1.5707964f) +
                        2.3561945f,
                    0.0f);
        }
    }
    else
    {
        angle = reinterpret_cast<EclHelperRngView *>(
                    &Th09EclRunControl::g_Rng)
                    ->GetRandomF32SignedInRange(3.1415927f);
    }

    if (view->position2D74.y < view->movementLowerBounds3398.y + 48.0f &&
        angle < 0.0f)
        angle = -angle;
    if (view->position2D74.y > view->movementUpperBounds33A0.y - 48.0f &&
        angle > 0.0f)
        angle = -angle;

    if (Th09EclRunControl::ReadInt(enemy, instruction, 0) <= 0)
    {
        view->movementAngle2DE0 = angle;
        view->speed2DF4 = Th09EclRunControl::ReadFloat(enemy, instruction, 2);
        view->primaryFlags337C =
            (view->primaryFlags337C &
             ~Th09EclRunMovement::ENEMY_MOVEMENT_MODE_MASK) |
            Th09EclRunMovement::ENEMY_MOVEMENT_MODE_POLAR;
        view->movementDuration2E34 = 0;
        view->movementTimer2E28 = 0;
    }
    else
    {
        StartTimedPolarDisplacement(enemy, instruction, angle);
    }
}

} // namespace Th09EclRunLate

namespace Th09EclRunBullet
{

struct ShotInstructionArgs
{
    short bulletType00;
    short color02;
    int count104;
    int count208;
    float speed10C;
    float speed210;
    float angle14;
    float angleStep18;
    unsigned int transformFlags1C;
};

void DispatchShotInstruction(
    EnemyView *enemy,
    Th09EclRawInstructionHeaderView *instruction)
{
    EclHelperEnemyView *view = HelperView(enemy);
    ShotInstructionArgs *args = reinterpret_cast<ShotInstructionArgs *>(
        reinterpret_cast<unsigned char *>(instruction) + 0x0C);
    BulletSpawnDescriptor *descriptor = &view->bulletDescriptor2E74;

    descriptor->position = view->worldPosition2DD4 + view->shootOffset2E04;
    descriptor->bulletType = static_cast<short>(
        (instruction->parameterMask0A & 1)
            ? Th09EclRunControl::ResolveInt(enemy, args->bulletType00)
            : args->bulletType00);
    descriptor->aimMode = instruction->opcode04 - TH09_ECL_OPCODE_SHOOT_FAN_AIMED;
    descriptor->count1 = static_cast<short>(
        (instruction->parameterMask0A & 4)
            ? Th09EclRunControl::ResolveInt(enemy, args->count104)
            : args->count104);
    descriptor->count2 = static_cast<short>(
        (instruction->parameterMask0A & 8)
            ? Th09EclRunControl::ResolveInt(enemy, args->count208)
            : args->count208);
    descriptor->angle = (instruction->parameterMask0A & 0x40)
                            ? enemy->ResolveFloat(args->angle14)
                            : args->angle14;
    descriptor->speed1 = (instruction->parameterMask0A & 0x10)
                             ? enemy->ResolveFloat(args->speed10C)
                             : args->speed10C;
    descriptor->angleStep = (instruction->parameterMask0A & 0x80)
                                ? enemy->ResolveFloat(args->angleStep18)
                                : args->angleStep18;
    descriptor->speed2 = (instruction->parameterMask0A & 0x20)
                             ? enemy->ResolveFloat(args->speed210)
                             : args->speed210;
    descriptor->unknown1FA = 0;
    descriptor->transformFlags = args->transformFlags1C;
    descriptor->color = static_cast<short>(
        (instruction->parameterMask0A & 2)
            ? Th09EclRunControl::ResolveInt(enemy, args->color02)
            : args->color02);
    Controller(enemy)->SpawnBulletPatternSecondary(descriptor);
}

} // namespace Th09EclRunBullet
