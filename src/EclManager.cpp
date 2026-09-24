#include "EclManager.hpp"
#include "EclRunLate.inl"

#include <string.h>

static void EnemyPostEclUpdateMovement(EnemyView *enemy);
static void EnemyPostEclUpdateShotAndAnm(EnemyView *enemy);

// Maintained non-exact reconstruction of the complete TH09 v1.50a enemy ECL
// interpreter owner at 0x004086C0.  The six lexical fragments preserve the
// target's single 187-way switch and its shared control-flow labels.  Names
// below describe target-observed helper roles; their independent source and
// exact calling conventions remain separate reconstruction work.

namespace Th09EclRunOwner
{

extern unsigned int g_DifficultyMask;
extern float g_TimeScale;

typedef void (__fastcall *InterpolationCallback)(
    EnemyView *enemy,
    Th09EclInterpolationSlotView *slot,
    float progress);

} // namespace Th09EclRunOwner

namespace Th09EclRunControl
{

static int PopContext(EnemyView *enemy)
{
    Th09EclRunState::EnemyStateView *view = Th09EclRunState::View(enemy);
    --view->activeCallDepth2D2A;
    if (view->activeCallDepth2D2A < 0)
    {
        int childIndex = view->activeContext2CE0->contextOrdinal224 - 1;
        if (view->childEclBlocks33D8[childIndex] != NULL)
            g_ZunMemory.Free(view->childEclBlocks33D8[childIndex]);
        view->childEclBlocks33D8[childIndex] = NULL;
        view->activeCallStack2CE4 = view->mainCallStack0A20;
        view->activeContext2CE0 = &view->mainContext07F4;
        view->activeCallDepth2D2A = view->mainCallDepth2D28;
        return 1;
    }
    *view->activeContext2CE0 =
        view->activeCallStack2CE4[view->activeCallDepth2D2A];
    return 0;
}

} // namespace Th09EclRunControl

namespace Th09EclRunMovement
{

static void SetPrimaryAnmScripts(
    EnemyView *enemy,
    Th09EclRawInstructionHeaderView *,
    int script0,
    int script1,
    int script2,
    int script3,
    int script4,
    int specialScript)
{
    EnemyMovementView *view = View(enemy);
    view->primaryAnmScript0338A = static_cast<short>(script0);
    view->primaryAnmScript13390 = static_cast<short>(script1);
    view->primaryAnmScript23392 = static_cast<short>(script2);
    view->primaryAnmScript3338C = static_cast<short>(script3);
    view->primaryAnmScript4338E = static_cast<short>(script4);
    view->specialAnmScript3394 = static_cast<short>(specialScript);
    view->anmDirection3386 = -1;
}

static void ConfigurePolarMotion(
    EnemyView *enemy,
    Th09EclRawInstructionHeaderView *instruction)
{
    EnemyMovementView *view = View(enemy);
    float angle = Th09EclRunControl::AddNormalizeAngle(
        Th09EclRunControl::ReadFloat(enemy, instruction, 2), 0.0f);

    view->movementDelta2E10.x =
        Th09EclRunControl::Cos(angle) *
        Th09EclRunControl::ReadFloat(enemy, instruction, 3) *
        Th09EclRunControl::ReadInt(enemy, instruction, 0);
    view->movementDelta2E10.y =
        Th09EclRunControl::Sin(angle) *
        Th09EclRunControl::ReadFloat(enemy, instruction, 3) *
        Th09EclRunControl::ReadInt(enemy, instruction, 0);
    view->movementDelta2E10.z = 0.0f;
    view->movementOrigin2E1C = view->worldPosition2DD4;

    int duration = Th09EclRunControl::ReadInt(enemy, instruction, 0);
    view->movementDuration2E34 = duration;
    *reinterpret_cast<Th09EclTimerStorageView *>(
        view->movementTimer2E28) = duration;
    view->primaryFlags337C =
        (view->primaryFlags337C & ~0x3A00U) |
        ((Th09EclRunControl::ReadInt(enemy, instruction, 1) & 7) << 11) |
        0x400U;
    if ((view->primaryFlags337C & 0x8000U) != 0)
        view->movementDelta2E10.x = -view->movementDelta2E10.x;
}

static void ConfigureRelativeMotion(
    EnemyView *enemy,
    Th09EclRawInstructionHeaderView *instruction)
{
    EnemyMovementView *view = View(enemy);
    EnemyFloat3 target;
    target.x = Th09EclRunControl::ReadFloat(enemy, instruction, 2);
    target.y = Th09EclRunControl::ReadFloat(enemy, instruction, 3);
    target.z = 0.0f;

    *reinterpret_cast<Float3 *>(&view->movementDelta2E10) =
        *reinterpret_cast<Float3 *>(&target) -
        *reinterpret_cast<Float3 *>(&view->worldPosition2DD4);
    view->movementOrigin2E1C = view->position2D74;

    int duration = Th09EclRunControl::ReadInt(enemy, instruction, 0);
    view->movementDuration2E34 = duration;
    *reinterpret_cast<Th09EclTimerStorageView *>(
        view->movementTimer2E28) = duration;
    unsigned int movementFlags =
        (view->primaryFlags337C & ~0x3A00U) |
        ((Th09EclRunControl::ReadInt(enemy, instruction, 1) & 7) << 11);
    view->primaryFlags337C = movementFlags;
    view->primaryFlags337C |= 0x400U;
    movementFlags = view->primaryFlags337C;

    *reinterpret_cast<Float3 *>(&view->velocity2D8C) =
        Float3(0.0f, 0.0f, 0.0f);
    if ((movementFlags & 0x8000U) != 0)
        view->movementDelta2E10.x = -view->movementDelta2E10.x;
}

static void StartTimedPolarDisplacement(
    EnemyView *enemy,
    Th09EclRawInstructionHeaderView *instruction,
    float angle)
{
    EnemyMovementView *view = View(enemy);

    view->movementDelta2E10.x =
        Th09EclRunControl::Cos(angle) *
        Th09EclRunControl::ReadFloat(enemy, instruction, 2) *
        Th09EclRunControl::ReadInt(enemy, instruction, 0);
    view->movementDelta2E10.y =
        Th09EclRunControl::Sin(angle) *
        Th09EclRunControl::ReadFloat(enemy, instruction, 2) *
        Th09EclRunControl::ReadInt(enemy, instruction, 0);
    view->movementDelta2E10.z = 0.0f;
    view->movementOrigin2E1C = view->worldPosition2DD4;

    int duration = Th09EclRunControl::ReadInt(enemy, instruction, 0);
    view->movementDuration2E34 = duration;
    *reinterpret_cast<Th09EclTimerStorageView *>(
        view->movementTimer2E28) = duration;
    view->primaryFlags337C =
        (view->primaryFlags337C & ~0x3A00U) |
        ((Th09EclRunControl::ReadInt(enemy, instruction, 1) & 7) << 11) |
        0x400U;
}

struct BoundaryMovePlayerView
{
    unsigned char unknown0000[0x1B88];
    EnemyFloat3 position1B88;
};

static void BeginBoundaryAwareMove(
    EnemyView *enemy,
    Th09EclRawInstructionHeaderView *instruction)
{
    EnemyMovementView *view = View(enemy);
    BoundaryMovePlayerView *player =
        reinterpret_cast<BoundaryMovePlayerView *>(Player(enemy));
    float angle;
    if (player->position1B88.x < view->position2D74.x)
        angle = Th09EclRunControl::AddNormalizeAngle(
            Th09EclRunControl::g_Rng.GetRandomF32InRange(1.5707964f) +
                2.3561945f,
            0.0f);
    else
        angle = Th09EclRunControl::g_Rng.GetRandomF32InRange(1.5707964f) -
                0.78539819f;

    float *position =
        reinterpret_cast<Float3 *>(&view->position2D74)->operator float *();

    if (position[0] < view->movementLowerBounds3398.x + 96.0f)
    {
        if (angle > 1.5707964f)
            angle = 3.1415927f - angle;
        else if (angle < -1.5707964f)
            angle = -3.1415927f - angle;
    }
    if (position[0] > view->movementUpperBounds33A0.x - 96.0f)
    {
        if (angle < 1.5707964f && angle >= 0.0f)
            angle = 3.1415927f - view->movementAngle2DE0;
        else if (angle > -1.5707964f && angle <= 0.0f)
            angle = -3.1415927f - angle;
    }
    if (position[1] < view->movementLowerBounds3398.y + 48.0f &&
        angle < 0.0f)
        angle = -angle;
    if (position[1] > view->movementUpperBounds33A0.y - 48.0f &&
        angle > 0.0f)
        angle = -angle;

    if (Th09EclRunControl::ReadInt(enemy, instruction, 0) <= 0)
    {
        view->movementAngle2DE0 = angle;
        view->speed2DF4 =
            Th09EclRunControl::ReadFloat(enemy, instruction, 2);
        view->primaryFlags337C =
            (view->primaryFlags337C & ~ENEMY_MOVEMENT_MODE_MASK) |
            ENEMY_MOVEMENT_MODE_POLAR;
        view->movementDuration2E34 = 0;
        *reinterpret_cast<Th09EclTimerStorageView *>(
            view->movementTimer2E28) = 0;
    }
    else
    {
        StartTimedPolarDisplacement(enemy, instruction, angle);
    }
}

} // namespace Th09EclRunMovement

namespace Th09EclRunLate
{

static void MoveRandomBiased(
    EnemyView *enemy,
    Th09EclRawInstructionHeaderView *instruction)
{
    Th09EclRunMovement::EnemyMovementView *view =
        Th09EclRunMovement::View(enemy);
    float angle;

    if (Th09EclRunControl::g_Rng.GetRandomU32InRange(4) != 0)
    {
        if (reinterpret_cast<Th09EclRunMovement::BoundaryMovePlayerView *>(
                Th09EclRunMovement::Player(enemy))
                ->position1B88.x < view->position2D74.x)
        {
            if (view->position2D74.x -
                    reinterpret_cast<
                        Th09EclRunMovement::BoundaryMovePlayerView *>(
                        Th09EclRunMovement::Player(enemy))
                        ->position1B88.x <
                reinterpret_cast<
                    Th09EclRunMovement::BoundaryMovePlayerView *>(
                    Th09EclRunMovement::Player(enemy))
                        ->position1B88.x +
                    384.0f - view->position2D74.x)
            {
                angle = Th09EclRunControl::AddNormalizeAngle(
                    Th09EclRunControl::g_Rng.GetRandomF32InRange(1.5707964f) +
                        2.3561945f,
                    0.0f);
            }
            else
            {
                angle = Th09EclRunControl::AddNormalizeAngle(
                    Th09EclRunControl::g_Rng.GetRandomF32InRange(1.5707964f) -
                        0.78539819f,
                    0.0f);
            }
        }
        else
        {
            if (reinterpret_cast<
                    Th09EclRunMovement::BoundaryMovePlayerView *>(
                    Th09EclRunMovement::Player(enemy))
                        ->position1B88.x -
                    view->position2D74.x <
                view->position2D74.x -
                    (reinterpret_cast<
                         Th09EclRunMovement::BoundaryMovePlayerView *>(
                         Th09EclRunMovement::Player(enemy))
                         ->position1B88.x -
                     384.0f))
            {
                angle =
                    Th09EclRunControl::g_Rng.GetRandomF32InRange(1.5707964f) -
                    0.78539819f;
            }
            else
            {
                angle = Th09EclRunControl::AddNormalizeAngle(
                    Th09EclRunControl::g_Rng.GetRandomF32InRange(1.5707964f) +
                        2.3561945f,
                    0.0f);
            }
        }
    }
    else
    {
        angle =
            Th09EclRunControl::g_Rng.GetRandomF32SignedInRange(3.1415927f);
    }

    float *position =
        reinterpret_cast<Float3 *>(&view->position2D74)->operator float *();
    if (position[1] < view->movementLowerBounds3398.y + 48.0f &&
        angle < 0.0f)
        angle = -angle;
    if (position[1] > view->movementUpperBounds33A0.y - 48.0f &&
        angle > 0.0f)
        angle = -angle;

    if (Th09EclRunControl::ReadInt(enemy, instruction, 0) <= 0)
    {
        view->movementAngle2DE0 = angle;
        view->speed2DF4 =
            Th09EclRunControl::ReadFloat(enemy, instruction, 2);
        view->primaryFlags337C =
            (view->primaryFlags337C &
             ~Th09EclRunMovement::ENEMY_MOVEMENT_MODE_MASK) |
            Th09EclRunMovement::ENEMY_MOVEMENT_MODE_POLAR;
        view->movementDuration2E34 = 0;
        *reinterpret_cast<Th09EclTimerStorageView *>(
            view->movementTimer2E28) = 0;
    }
    else
    {
        Th09EclRunMovement::StartTimedPolarDisplacement(
            enemy, instruction, angle);
    }
}


} // namespace Th09EclRunLate

namespace Th09EclRunMovement
{

static void SetExtraAnmScript(
    EnemyView *enemy,
    Th09EclRawInstructionHeaderView *instruction)
{
    int unusedIndex = Th09EclRunControl::ReadInt(enemy, instruction, 0);
    int script = Th09EclRunControl::ReadInt(enemy, instruction, 1);

    if (script >= 0)
    {
        if ((View(enemy)->primaryFlags337C & ENEMY_ALTERNATE_ANM_BANK) != 0)
        {
            reinterpret_cast<AnmLoaded *>(AlternateAnm(enemy))->
                SetAndExecuteScriptIdx(
                    reinterpret_cast<AnmVm *>(
                        reinterpret_cast<unsigned char *>(enemy) + 0x2AC +
                        Th09EclRunControl::ReadInt(enemy, instruction, 0) *
                            sizeof(AnmVm)),
                    Th09EclRunControl::ReadInt(enemy, instruction, 1));
        }
        else
        {
            reinterpret_cast<AnmLoaded *>(PrimaryAnm(enemy))->
                SetAndExecuteScriptIdx(
                    reinterpret_cast<AnmVm *>(
                        reinterpret_cast<unsigned char *>(enemy) + 0x2AC +
                        Th09EclRunControl::ReadInt(enemy, instruction, 0) *
                            sizeof(AnmVm)),
                    Th09EclRunControl::ReadInt(enemy, instruction, 1));
        }
    }
    else
    {
        reinterpret_cast<AnmVm *>(
            reinterpret_cast<unsigned char *>(enemy) + 0x2AC +
            Th09EclRunControl::ReadInt(enemy, instruction, 0) *
                sizeof(AnmVm))->scriptIndex = -1;
    }
    (void)unusedIndex;
}

} // namespace Th09EclRunMovement

namespace Th09EclRunControl
{

static Th09EclRawInstructionHeaderView *CompareOperands(
    EnemyView *enemy,
    Th09EclRawInstructionHeaderView *instruction)
{
    short opcode = static_cast<short>(instruction->opcode04);
    switch (opcode)
    {
    case TH09_ECL_OPCODE_JUMP_IF_INT_EQUAL:
        if (ReadInt(enemy, instruction, 0) == ReadInt(enemy, instruction, 1))
            goto comparison_succeeded;
        return NULL;
    case TH09_ECL_OPCODE_JUMP_IF_FLOAT_EQUAL:
        if (ReadFloat(enemy, instruction, 0) == ReadFloat(enemy, instruction, 1))
            goto comparison_succeeded;
        return NULL;
    case TH09_ECL_OPCODE_JUMP_IF_INT_NOT_EQUAL:
        if (ReadInt(enemy, instruction, 0) != ReadInt(enemy, instruction, 1))
            goto comparison_succeeded;
        return NULL;
    case TH09_ECL_OPCODE_JUMP_IF_FLOAT_NOT_EQUAL:
        if (ReadFloat(enemy, instruction, 0) != ReadFloat(enemy, instruction, 1))
            goto comparison_succeeded;
        return NULL;
    case TH09_ECL_OPCODE_JUMP_IF_INT_LESS:
        if (ReadInt(enemy, instruction, 0) < ReadInt(enemy, instruction, 1))
            goto comparison_succeeded;
        return NULL;
    case TH09_ECL_OPCODE_JUMP_IF_FLOAT_LESS:
        if (ReadFloat(enemy, instruction, 0) < ReadFloat(enemy, instruction, 1))
            goto comparison_succeeded;
        return NULL;
    case TH09_ECL_OPCODE_JUMP_IF_INT_LESS_EQUAL:
        if (ReadInt(enemy, instruction, 0) <= ReadInt(enemy, instruction, 1))
            goto comparison_succeeded;
        return NULL;
    case TH09_ECL_OPCODE_JUMP_IF_FLOAT_LESS_EQUAL:
        if (ReadFloat(enemy, instruction, 0) <= ReadFloat(enemy, instruction, 1))
            goto comparison_succeeded;
        return NULL;
    case TH09_ECL_OPCODE_JUMP_IF_INT_GREATER:
        if (ReadInt(enemy, instruction, 0) > ReadInt(enemy, instruction, 1))
            goto comparison_succeeded;
        return NULL;
    case TH09_ECL_OPCODE_JUMP_IF_FLOAT_GREATER:
        if (ReadFloat(enemy, instruction, 0) > ReadFloat(enemy, instruction, 1))
            goto comparison_succeeded;
        return NULL;
    case TH09_ECL_OPCODE_JUMP_IF_INT_GREATER_EQUAL:
        if (ReadInt(enemy, instruction, 0) >= ReadInt(enemy, instruction, 1))
            goto comparison_succeeded;
        return NULL;
    case TH09_ECL_OPCODE_JUMP_IF_FLOAT_GREATER_EQUAL:
        if (ReadFloat(enemy, instruction, 0) >= ReadFloat(enemy, instruction, 1))
            goto comparison_succeeded;
        return NULL;
    default:
        return NULL;
    }

comparison_succeeded:
    Th09EclRunState::View(enemy)->activeContext2CE0->time008.current08 =
        RawInt(instruction, 2);
    return reinterpret_cast<Th09EclRawInstructionHeaderView *>(
        reinterpret_cast<unsigned char *>(instruction) + RawInt(instruction, 3));
}

extern Th09EclRunOwner::InterpolationCallback g_EclInterpolationCallbacks[];

static void ApplyInterpolationOperation(
    EnemyView *enemy,
    Th09EclRawInstructionHeaderView *instruction)
{
    float delta = ReadFloat(enemy, instruction, 1) -
                  ReadFloat(enemy, instruction, 2);
    *WriteFloat(enemy, instruction, 0) =
        delta * ReadFloat(enemy, instruction, 3) +
        ReadFloat(enemy, instruction, 2);
}

static void InstallInterpolationSlot(
    EnemyView *enemy,
    Th09EclRawInstructionHeaderView *instruction)
{
    Th09EclInterpolationSlotView *slot =
        Th09EclRunState::View(enemy)->activeContext2CE0->interpolationSlots0A0;
    for (int index = 0; index < TH09_ECL_INTERPOLATION_SLOT_COUNT;
         ++index, ++slot)
    {
        if (slot->callback00 != NULL &&
            slot->affectedVariable2C != RawFloat(instruction, 0))
            continue;

        slot->timer04 = 0;
        slot->affectedVariable2C = RawFloat(instruction, 0);
        slot->duration10 = ReadInt(enemy, instruction, 1);
        slot->unknown14 = ReadInt(enemy, instruction, 2);
        slot->easing18 = ReadInt(enemy, instruction, 3);
        slot->callback00 = g_EclInterpolationCallbacks[slot->unknown14];
        reinterpret_cast<float *>(slot->unknown1C)[0] =
            ReadFloat(enemy, instruction, 4);
        reinterpret_cast<float *>(slot->unknown1C)[1] =
            ReadFloat(enemy, instruction, 5);
        reinterpret_cast<float *>(slot->unknown1C)[2] =
            ReadFloat(enemy, instruction, 6);
        reinterpret_cast<float *>(slot->unknown1C)[3] =
            ReadFloat(enemy, instruction, 7);
        break;
    }
}

static void CallSubroutine(
    EnemyView *enemy,
    Th09EclRawInstructionHeaderView *instruction,
    int rawSubroutineId)
{
    Th09EclRunState::EnemyStateView *view =
        Th09EclRunState::View(enemy);
    view->activeContext2CE0->currentInstruction004 =
        reinterpret_cast<Th09EclRawInstructionHeaderView *>(
            reinterpret_cast<unsigned char *>(instruction) +
            static_cast<short>(instruction->nextOffset06));

    if ((view->primaryFlags337C &
         Th09EclRunState::ENEMY_STATE_CALL_STACK_DISABLED) == 0)
        view->activeCallStack2CE4[view->activeCallDepth2D2A] =
            *view->activeContext2CE0;

    (*reinterpret_cast<Th09EclRunState::ManagerStateView **>(
        view->activeContext2CE0))
        ->InitializeSubroutine(
            view->activeContext2CE0,
            static_cast<short>(rawSubroutineId));

    memcpy(view->activeContext2CE0->unknown01C + (0x74 - 0x1C),
           reinterpret_cast<unsigned char *>(*reinterpret_cast<
               Th09EclRunState::ManagerStateView **>(
                   view->activeContext2CE0)) + 0x168,
           0x20);

    if ((view->primaryFlags337C &
         Th09EclRunState::ENEMY_STATE_CALL_STACK_DISABLED) == 0 &&
        view->activeCallDepth2D2A < TH09_ECL_MAIN_CALL_STACK_CAPACITY - 1)
        ++view->activeCallDepth2D2A;
}

} // namespace Th09EclRunControl

int EclManager::RunEcl(EnemyView *enemy)
{
    Th09EclRunState::EnemyStateView *enemyState;
    Th09EclContextView *savedMainContext;
    Th09EclContextView *savedMainCallStack;
    Th09EclContextView *context;
    Th09EclRawInstructionHeaderView *instruction;
    Th09EclRunState::ChildEclBlock *child;
    Th09EclInterpolationSlotView *slot;
    Float3 *position;
    Float3 *positionOffset;
    Float3 *worldPosition;
    Th09EclRunState::ExInstructionCallback perFrameCallback;
    Th09EclRunOwner::InterpolationCallback interpolationCallback;
    unsigned int executionMask;
    unsigned int affectedVariableBits;
    int childIndex;
    int interpolationIndex;
    int positionInterpolated;
    int lhsInt;
    float progress;
    float inverseProgress;
    float deltaX;
    float deltaY;

    enemyState = Th09EclRunState::View(enemy);
    savedMainCallStack = enemyState->mainCallStack0A20;
    enemyState->activeCallStack2CE4 = savedMainCallStack;
    savedMainContext = &enemyState->mainContext07F4;
    enemyState->activeContext2CE0 = savedMainContext;
    enemyState->activeCallDepth2D2A = enemyState->mainCallDepth2D28;
    childIndex = -1;

th09_ecl_restart_context:
    context = enemyState->activeContext2CE0;
    instruction = context->currentInstruction004;
    if (enemyState->pendingSubroutineId2D70 >= 0)
        goto th09_ecl_enter_pending_subroutine;
    position = &enemyState->position2D74;
    positionOffset = &enemyState->positionOffset2D80;
    worldPosition = &enemyState->worldPosition2DD4;
    goto th09_ecl_instruction_loop;

th09_ecl_enter_pending_subroutine:
    context->currentInstruction004 =
        reinterpret_cast<Th09EclRawInstructionHeaderView *>(
            reinterpret_cast<unsigned char *>(instruction) +
            static_cast<short>(instruction->nextOffset06));

    if ((enemyState->primaryFlags337C &
         Th09EclRunState::ENEMY_STATE_CALL_STACK_DISABLED) == 0)
    {
        memcpy(
            &enemyState->activeCallStack2CE4[enemyState->activeCallDepth2D2A],
            savedMainContext,
            sizeof(Th09EclContextView));
    }

    enemyState->manager0000->InitializeSubroutine(
        savedMainContext,
        enemyState->subroutineSlots2D30[
            enemyState->pendingSubroutineId2D70]);
    if (enemyState->activeCallDepth2D2A <
        TH09_ECL_MAIN_CALL_STACK_CAPACITY - 1)
    {
        ++enemyState->activeCallDepth2D2A;
    }
    enemyState->pendingSubroutineId2D70 = -1;
    goto th09_ecl_restart_context;

th09_ecl_instruction_loop:
    *worldPosition = *position + *positionOffset;

    for (;;)
    {
        Th09EclTimerStorageView *secondaryTimer =
            &enemyState->activeContext2CE0->secondaryTime094;
        if (static_cast<int>(*secondaryTimer) > 0)
        {
            (*secondaryTimer)--;
            enemyState->activeContext2CE0->time008--;
            break;
        }

        if (enemyState->activeContext2CE0->time008 == instruction->time00)
        {
            executionMask = Th09EclRunOwner::g_DifficultyMask |
                            enemyState->difficultyOverride3388;
            if ((instruction->difficultyMask09 & executionMask) != executionMask)
                goto th09_ecl_advance_instruction;

#define TH09_ECL_RUN_SHARED_SWITCH
            switch (instruction->opcode04)
            {
            {
#define TH09_ECL_RUN_CONTROL_BODY
#include "EclRunControl.inl"
#undef TH09_ECL_RUN_CONTROL_BODY
            }
            {
#define TH09_ECL_RUN_MOVEMENT_BODY
#include "EclRunMovement.inl"
#undef TH09_ECL_RUN_MOVEMENT_BODY
            }
            {
#define TH09_ECL_RUN_REMOTE_BODY
#include "EclRunRemote.inl"
#undef TH09_ECL_RUN_REMOTE_BODY
            }
            {
#define TH09_ECL_RUN_BULLET_BODY
#include "EclRunBullet.inl"
#undef TH09_ECL_RUN_BULLET_BODY
            }
            {
#define TH09_ECL_RUN_STATE_BODY
#include "EclRunState.inl"
#undef TH09_ECL_RUN_STATE_BODY
            }
            {
#define TH09_ECL_RUN_LATE_BODY
#include "EclRunLate.inl"
#undef TH09_ECL_RUN_LATE_BODY
            }
            default:
                break;
            }
#undef TH09_ECL_RUN_SHARED_SWITCH

th09_ecl_advance_instruction:
            instruction = reinterpret_cast<Th09EclRawInstructionHeaderView *>(
                reinterpret_cast<unsigned char *>(instruction) +
                static_cast<short>(instruction->nextOffset06));

th09_ecl_redispatch_instruction:
            goto th09_ecl_instruction_loop;
        }

        break;
    }

th09_ecl_after_dispatch:
    if (enemyState->life2E48 > 0)
    {
        context = enemyState->activeContext2CE0;
        perFrameCallback =
            reinterpret_cast<Th09EclRunState::ExInstructionCallback>(
                context->perFrameCallback014);
        slot = &context->interpolationSlots0A0[0];

        Float3 positionBeforeCallbacks = *position;
        positionInterpolated = false;

        if (perFrameCallback != NULL)
        {
            perFrameCallback(
                enemy,
                reinterpret_cast<Th09EclRawInstructionHeaderView *>(
                    context->perFrameInstruction018));
        }

        for (interpolationIndex = TH09_ECL_INTERPOLATION_SLOT_COUNT;
             interpolationIndex > 0;
             --interpolationIndex, ++slot)
        {
            if (slot->callback00 == NULL)
                continue;

            slot->timer04 += Th09EclRunOwner::g_TimeScale;
            if (slot->timer04 >= slot->duration10)
                slot->timer04 = slot->duration10;

            progress = static_cast<float>(slot->timer04) / slot->duration10;
            switch (slot->easing18)
            {
            case 1:
                progress = progress * progress;
                break;
            case 2:
                progress = progress * progress * progress;
                break;
            case 3:
                progress = progress * progress * progress * progress;
                break;
            case 4:
                inverseProgress = 1.0f - progress;
                progress = 1.0f - inverseProgress * inverseProgress;
                break;
            case 5:
                inverseProgress = 1.0f - progress;
                progress = 1.0f -
                    inverseProgress * inverseProgress * inverseProgress;
                break;
            case 6:
                inverseProgress = 1.0f - progress;
                progress = 1.0f -
                    inverseProgress * inverseProgress *
                    inverseProgress * inverseProgress;
                break;
            default:
                break;
            }
            interpolationCallback =
                reinterpret_cast<Th09EclRunOwner::InterpolationCallback>(
                    slot->callback00);
            interpolationCallback(enemy, slot, progress);

            if (slot->timer04 >= slot->duration10)
                slot->callback00 = NULL;

            affectedVariableBits =
                *reinterpret_cast<unsigned int *>(&slot->affectedVariable2C);
            if (affectedVariableBits == 0x461CE800U ||
                affectedVariableBits == 0x461CEC00U ||
                affectedVariableBits == 0x461CF000U)
            {
                positionInterpolated = true;
            }
        }

        if (positionInterpolated)
        {
            deltaX = position->x - positionBeforeCallbacks.x;
            deltaY = position->y - positionBeforeCallbacks.y;
            enemyState->velocity2D8C.x = deltaX;
            enemyState->velocity2D8C.y = deltaY;
            enemyState->movementAngle2DE0 =
                Th09EclRunControl::VectorAngle(deltaY, deltaX);
            position->x = positionBeforeCallbacks.x;
            position->y = positionBeforeCallbacks.y;
            position->z = positionBeforeCallbacks.z;
        }
    }

    if (childIndex == -1)
    {
        enemyState->mainCallDepth2D28 = enemyState->activeCallDepth2D2A;
    }
    else
    {
        child = reinterpret_cast<Th09EclRunState::ChildEclBlock *>(
            enemyState->childEclBlocks33D8[childIndex]);
        child->callStackDepth06 = enemyState->activeCallDepth2D2A;
    }

    context->currentInstruction004 = instruction;
    context->time008 += Th09EclRunOwner::g_TimeScale;

th09_ecl_select_next_context:
    for (int next = childIndex + 1;
         next < TH09_ECL_CHILD_CONTEXT_COUNT;
         ++next)
    {
        child = reinterpret_cast<Th09EclRunState::ChildEclBlock *>(
            enemyState->childEclBlocks33D8[next]);
        if (child != NULL)
        {
            enemyState->activeCallStack2CE4 = child->callStack0234;
            enemyState->activeContext2CE0 = &child->context08;
            instruction = child->context08.currentInstruction004;
            child->context08.contextOrdinal224 = next + 1;
            enemyState->activeCallDepth2D2A = child->callStackDepth06;
            childIndex = next;
            goto th09_ecl_instruction_loop;
        }
    }

    enemyState->activeCallStack2CE4 = savedMainCallStack;
    enemyState->activeContext2CE0 = savedMainContext;
    EnemyPostEclUpdateMovement(enemy);
    EnemyPostEclUpdateShotAndAnm(enemy);
    return 0;
}

// These lexical fragments are target-proven members of the same VC7.1 TU.
#include "EclPostRuntime.inl"
#include "ExAttackInitType8.inl"
