// SPDX-License-Identifier: GPL-3.0-only
// Includes the Cirno-Labs/th09 d7653152-adapted Type14/22 draw owner;
// original MIT portions retain their grant in LICENSE. See THIRD_PARTY_NOTICES.md.
#include "EclManager.hpp"
#include "EclRunLate.inl"
#include "ExAttackType8GameManagerView.hpp"
#include "GameManagerRuntimeView.hpp"

#include <string.h>
#include <d3dx8math.h>

// The SDK coordinate conversion and native Enemy position share one storage ABI.
typedef char EclSdkPositionStorageMatchesNative[
    (sizeof(D3DXVECTOR3) == sizeof(EnemyFloat3) && sizeof(EnemyFloat3) == 12 &&
     offsetof(D3DXVECTOR3, x) == offsetof(EnemyFloat3, x) &&
     offsetof(D3DXVECTOR3, y) == offsetof(EnemyFloat3, y) &&
     offsetof(D3DXVECTOR3, z) == offsetof(EnemyFloat3, z)) ? 1 : -1];

static void EnemyPostEclUpdateMovement(EnemyView *enemy);
static void EnemyPostEclUpdateShotAndAnm(EnemyView *enemy);

// Maintained non-exact reconstruction of the complete TH09 v1.50a enemy ECL
// interpreter owner at 0x004086C0.  The six lexical fragments preserve the
// target's single 187-way switch and its shared control-flow labels.  Names
// below describe target-observed helper roles; their independent source and
// exact calling conventions remain separate reconstruction work.

namespace Th09EclRunControl
{

static int PopContext(EnemyView *enemy)
{
    --enemy->activeCallDepth2D2A;
    if (enemy->activeCallDepth2D2A < 0)
    {
        int childIndex = enemy->activeContext2CE0->contextOrdinal224 - 1;
        if (enemy->childEclBlocks33D8[childIndex] != NULL)
            g_ZunMemory.Free(enemy->childEclBlocks33D8[childIndex]);
        enemy->childEclBlocks33D8[childIndex] = NULL;
        enemy->activeCallStack2CE4 = enemy->mainCallStack0A20;
        enemy->activeContext2CE0 = &enemy->mainContext07F4;
        enemy->activeCallDepth2D2A = enemy->mainCallDepth2D28;
        return 1;
    }
    *enemy->activeContext2CE0 =
        enemy->activeCallStack2CE4[enemy->activeCallDepth2D2A];
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
    view->movementTimer2E28 = duration;
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
    view->movementTimer2E28 = duration;
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
    EnemyMovementView *view = enemy;
    float (*cosineOperation)(float) = ::cosf;
    float (*sineOperation)(float) = ::sinf;

    view->movementDelta2E10.x =
        cosineOperation(angle) *
        ((instruction->parameterMask0A & 4U)
             ? enemy->ResolveFloat(instruction->arguments[2].real)
             : instruction->arguments[2].real) *
        ((instruction->parameterMask0A & 1U)
             ? Th09EclRunControl::ResolveInt(enemy, instruction->arguments[0].integer)
             : instruction->arguments[0].integer);
    view->movementDelta2E10.y =
        sineOperation(angle) *
        ((instruction->parameterMask0A & 4U)
             ? enemy->ResolveFloat(instruction->arguments[2].real)
             : instruction->arguments[2].real) *
        ((instruction->parameterMask0A & 1U)
             ? Th09EclRunControl::ResolveInt(enemy, instruction->arguments[0].integer)
             : instruction->arguments[0].integer);
    view->movementDelta2E10.z = 0.0f;
    view->movementOrigin2E1C = view->worldPosition2DD4;

    int duration = ((instruction->parameterMask0A & 1U)
             ? Th09EclRunControl::ResolveInt(enemy, instruction->arguments[0].integer)
             : instruction->arguments[0].integer);
    view->movementDuration2E34 = duration;
    view->movementTimer2E28 = duration;
    view->primaryFlags337C =
        (view->primaryFlags337C & ~0x3A00U) |
        ((((instruction->parameterMask0A & 2U)
             ? Th09EclRunControl::ResolveInt(enemy, instruction->arguments[1].integer)
             : instruction->arguments[1].integer) & 7) << 11) |
        0x400U;
}

static void BeginBoundaryAwareMove(
    EnemyView *enemy,
    Th09EclRawInstructionHeaderView *instruction)
{
    EnemyMovementView *view = View(enemy);
    EnemyRewardPlayerView *player = Player(enemy);
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
        view->movementTimer2E28 = 0;
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
        enemy;
    float angle;

    if (Th09EclRunControl::g_Rng.GetRandomU32InRange(4) != 0)
    {
        if (enemy->manager00->sideState320->player04->position1B88.x < view->position2D74.x)
        {
            if (view->position2D74.x -
                    enemy->manager00->sideState320->player04->position1B88.x <
                enemy->manager00->sideState320->player04->position1B88.x +
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
            if (enemy->manager00->sideState320->player04->position1B88.x -
                    view->position2D74.x <
                view->position2D74.x -
                    (enemy->manager00->sideState320->player04->position1B88.x -
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

    if (reinterpret_cast<D3DXVECTOR3 &>(view->position2D74)[1] <
            view->movementLowerBounds3398.y + 48.0f && angle < 0.0f)
        angle = -angle;
    if (reinterpret_cast<D3DXVECTOR3 &>(view->position2D74)[1] >
            view->movementUpperBounds33A0.y - 48.0f && angle > 0.0f)
        angle = -angle;

    if (((instruction->parameterMask0A & 1U)
             ? Th09EclRunControl::ResolveInt(enemy, instruction->arguments[0].integer)
             : instruction->arguments[0].integer) <= 0)
    {
        view->movementAngle2DE0 = angle;
        view->speed2DF4 =
            ((instruction->parameterMask0A & 4U)
             ? enemy->ResolveFloat(instruction->arguments[2].real)
             : instruction->arguments[2].real);
        view->primaryFlags337C =
            (view->primaryFlags337C &
             ~Th09EclRunMovement::ENEMY_MOVEMENT_MODE_MASK) |
            Th09EclRunMovement::ENEMY_MOVEMENT_MODE_POLAR;
        view->movementDuration2E34 = 0;
        view->movementTimer2E28 = 0;
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
                    &enemy->secondaryVms2AC[
                        Th09EclRunControl::ReadInt(enemy, instruction, 0)],
                    Th09EclRunControl::ReadInt(enemy, instruction, 1));
        }
        else
        {
            reinterpret_cast<AnmLoaded *>(PrimaryAnm(enemy))->
                SetAndExecuteScriptIdx(
                    &enemy->secondaryVms2AC[
                        Th09EclRunControl::ReadInt(enemy, instruction, 0)],
                    Th09EclRunControl::ReadInt(enemy, instruction, 1));
        }
    }
    else
    {
        enemy->secondaryVms2AC[
            Th09EclRunControl::ReadInt(enemy, instruction, 0)].scriptIndex = -1;
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
    enemy->activeContext2CE0->time008.current =
        RawInt(instruction, 2);
    return reinterpret_cast<Th09EclRawInstructionHeaderView *>(
        reinterpret_cast<unsigned char *>(instruction) + RawInt(instruction, 3));
}

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
        enemy->activeContext2CE0->interpolationSlots0A0;
    for (int index = 0; index < TH09_ECL_INTERPOLATION_SLOT_COUNT;
         ++index, ++slot)
    {
        if (slot->callback00 != NULL &&
            slot->affectedVariable2C != RawFloat(instruction, 0))
            continue;

        slot->timer04 = 0;
        slot->affectedVariable2C = RawFloat(instruction, 0);
        slot->duration10 = ReadInt(enemy, instruction, 1);
        slot->callbackIndex14 = ReadInt(enemy, instruction, 2);
        slot->easing18 = ReadInt(enemy, instruction, 3);
        slot->callback00 = g_EclInterpolationCallbacks[slot->callbackIndex14];
        slot->parameters1C[0] =
            ReadFloat(enemy, instruction, 4);
        slot->parameters1C[1] =
            ReadFloat(enemy, instruction, 5);
        slot->parameters1C[2] =
            ReadFloat(enemy, instruction, 6);
        slot->parameters1C[3] =
            ReadFloat(enemy, instruction, 7);
        break;
    }
}

static void CallSubroutine(
    EnemyView *enemy,
    Th09EclRawInstructionHeaderView *instruction,
    int rawSubroutineId)
{
    enemy->activeContext2CE0->currentInstruction004 =
        reinterpret_cast<Th09EclRawInstructionHeaderView *>(
            reinterpret_cast<unsigned char *>(instruction) +
            static_cast<short>(instruction->nextOffset06));

    if ((enemy->primaryFlags337C &
         Th09EclRunState::ENEMY_STATE_CALL_STACK_DISABLED) == 0)
        enemy->activeCallStack2CE4[enemy->activeCallDepth2D2A] =
            *enemy->activeContext2CE0;

    enemy->activeContext2CE0->selectedManager000->InitializeSubroutine(
            enemy->activeContext2CE0,
            static_cast<short>(rawSubroutineId));

    memcpy(&enemy->activeContext2CE0->intVariables74[0],
           &enemy->activeContext2CE0->selectedManager000->timelineState08.sharedInts160[0],
           0x20);

    if ((enemy->primaryFlags337C &
         Th09EclRunState::ENEMY_STATE_CALL_STACK_DISABLED) == 0 &&
        enemy->activeCallDepth2D2A < TH09_ECL_MAIN_CALL_STACK_CAPACITY - 1)
        ++enemy->activeCallDepth2D2A;
}

} // namespace Th09EclRunControl

int EclManager::RunEcl(EnemyView *enemy)
{
    Th09EclContextView *savedMainContext;
    Th09EclContextView *savedMainCallStack;
    Th09EclRawInstructionHeaderView *instruction;
    Th09EclRunState::ChildEclBlock *child;
    Th09EclInterpolationSlotView *slot;
    Float3 *position;
    Float3 *positionOffset;
    EnemyEclExInstruction perFrameCallback;
    Th09EclInterpolationCallback interpolationCallback;
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

    savedMainCallStack = enemy->mainCallStack0A20;
    enemy->activeCallStack2CE4 = savedMainCallStack;
    savedMainContext = &enemy->mainContext07F4;
    enemy->activeContext2CE0 = savedMainContext;
    childIndex = -1;
    enemy->activeCallDepth2D2A = enemy->mainCallDepth2D28;

th09_ecl_restart_context:
    instruction = enemy->activeContext2CE0->currentInstruction004;
    if (enemy->pendingSubroutineId2D70 >= 0)
        goto th09_ecl_enter_pending_subroutine;
    positionOffset = reinterpret_cast<Float3 *>(&enemy->positionOffset2D80);
    position = reinterpret_cast<Float3 *>(&enemy->position2D74);
    goto th09_ecl_instruction_loop;

th09_ecl_enter_pending_subroutine:
    enemy->activeContext2CE0->currentInstruction004 =
        reinterpret_cast<Th09EclRawInstructionHeaderView *>(
            reinterpret_cast<unsigned char *>(instruction) +
            static_cast<short>(instruction->nextOffset06));

    if ((enemy->primaryFlags337C &
         Th09EclRunState::ENEMY_STATE_CALL_STACK_DISABLED) == 0)
    {
        memcpy(
            &enemy->activeCallStack2CE4[enemy->activeCallDepth2D2A],
            savedMainContext,
            sizeof(Th09EclContextView));
    }

    enemy->manager00->primaryEclManager000.InitializeSubroutine(
        savedMainContext,
        enemy->subroutineSlots2D30[
            enemy->pendingSubroutineId2D70]);
    if (enemy->activeCallDepth2D2A <
        TH09_ECL_MAIN_CALL_STACK_CAPACITY - 1)
    {
        ++enemy->activeCallDepth2D2A;
    }
    enemy->pendingSubroutineId2D70 = -1;
    goto th09_ecl_restart_context;

th09_ecl_instruction_loop:
    reinterpret_cast<Float3 &>(enemy->worldPosition2DD4) = *position + *positionOffset;

    for (;;)
    {
        Th09EclTimerStorageView *secondaryTimer =
            &enemy->activeContext2CE0->secondaryTime094;
        if (static_cast<int>(*secondaryTimer) > 0)
        {
            (*secondaryTimer)--;
            enemy->activeContext2CE0->time008--;
            break;
        }

        // Bind the time field for the immediate by-value comparison.
        const int &instructionTime = instruction->time00;
        Th09EclContextView *comparisonContext =
            enemy->activeContext2CE0;
        if (comparisonContext->time008 == instructionTime)
        {
            executionMask = g_GameManager.instructionMask120 |
                            enemy->difficultyOverride3388;
            if ((instruction->difficultyMask09 & executionMask) != executionMask)
                goto th09_ecl_advance_instruction;

#define TH09_ECL_RUN_SHARED_SWITCH
            switch (instruction->opcode04)
            {
#define context (enemy->activeContext2CE0)
#define TH09_ECL_RUN_CONTROL_BODY
#include "EclRunControl.inl"
#undef TH09_ECL_RUN_CONTROL_BODY
#undef context
#define TH09_ECL_RUN_MOVEMENT_BODY
#include "EclRunMovement.inl"
#undef TH09_ECL_RUN_MOVEMENT_BODY
#define TH09_ECL_RUN_REMOTE_BODY
#include "EclRunRemote.inl"
#undef TH09_ECL_RUN_REMOTE_BODY
#define TH09_ECL_RUN_BULLET_BODY
#include "EclRunBullet.inl"
#undef TH09_ECL_RUN_BULLET_BODY
#define TH09_ECL_RUN_STATE_BODY
#include "EclRunState.inl"
#undef TH09_ECL_RUN_STATE_BODY
#define TH09_ECL_RUN_LATE_BODY
#include "EclRunLate.inl"
#undef TH09_ECL_RUN_LATE_BODY
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
    if (enemy->life2E48 > 0)
    {
        perFrameCallback =
            reinterpret_cast<EnemyEclCallbackContextView *>(
                enemy->activeContext2CE0)->callback014;
        slot = &enemy->activeContext2CE0->interpolationSlots0A0[0];

        positionInterpolated = false;
        Float3 positionBeforeCallbacks = *position;

        if (perFrameCallback != NULL)
        {
            perFrameCallback(
                enemy,
                reinterpret_cast<EnemyEclCallbackContextView *>(
                    enemy->activeContext2CE0)->instruction018);
        }

        for (interpolationIndex = TH09_ECL_INTERPOLATION_SLOT_COUNT;
             interpolationIndex > 0;
             --interpolationIndex, ++slot)
        {
            if (slot->callback00 == NULL)
                continue;

            slot->timer04 += g_GameManager.speedEC;
            // Use the shared read-only comparator for this 12-byte timer view.
            // This bridge follows the pinned VC7.1 target layout; it is not a
            // claim of portable C++ aliasing or a uniquely recovered original TU.
            if (reinterpret_cast<ZunTimer *>(&slot->timer04)->operator>=(slot->duration10))
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
            interpolationCallback = slot->callback00;
            interpolationCallback(enemy, slot, progress);

            if (reinterpret_cast<ZunTimer *>(&slot->timer04)->operator>=(slot->duration10))
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
            enemy->velocity2D8C.x = deltaX;
            deltaY =
                enemy->position2D74.y - positionBeforeCallbacks.y;
            enemy->velocity2D8C.y = deltaY;
            enemy->movementAngle2DE0 =
                Th09EclRunControl::VectorAngle(deltaY, deltaX);
            position->x = positionBeforeCallbacks.x;
            position->y = positionBeforeCallbacks.y;
            position->z = positionBeforeCallbacks.z;
        }
    }

    if (childIndex == -1)
    {
        enemy->mainCallDepth2D28 = enemy->activeCallDepth2D2A;
    }
    else
    {
        child = reinterpret_cast<Th09EclRunState::ChildEclBlock *>(
            enemy->childEclBlocks33D8[childIndex]);
        child->callStackDepth06 = enemy->activeCallDepth2D2A;
    }

    enemy->activeContext2CE0->currentInstruction004 = instruction;
    enemy->activeContext2CE0->time008 += g_GameManager.speedEC;

th09_ecl_select_next_context:
    for (int next = childIndex + 1;
         next < TH09_ECL_CHILD_CONTEXT_COUNT;
         ++next)
    {
        if (enemy->childEclBlocks33D8[next] != NULL)
        {
            child = reinterpret_cast<Th09EclRunState::ChildEclBlock *>(
                enemy->childEclBlocks33D8[next]);
            enemy->activeCallStack2CE4 = child->callStack0234;
            enemy->activeContext2CE0 = &child->context08;
            instruction = enemy->activeContext2CE0->currentInstruction004;
            enemy->activeContext2CE0->contextOrdinal224 = next + 1;
            enemy->activeCallDepth2D2A = child->callStackDepth06;
            childIndex = next;
            goto th09_ecl_instruction_loop;
        }
    }

    enemy->activeCallStack2CE4 = savedMainCallStack;
    enemy->activeContext2CE0 = savedMainContext;
    EnemyPostEclUpdateMovement(enemy);
    EnemyPostEclUpdateShotAndAnm(enemy);
    return 0;
}

// These lexical groups preserve the reviewed VC7.1 compiler context;
// their unique original translation-unit partition remains unknown.
#include "EclPostRuntime.inl"
#include "ExAttackInitType8.inl"
#include "ExAttackUpdateType8Type9.inl"
#include "ExAttackUpdateType11Type12.inl"

#include "ExAttackUpdateType7.inl"

#include "ExAttackUpdateType21.inl"

#include "ExAttackInitType19.inl"
#include "ExAttackInitType21.inl"
#include "ExAttackUpdateType19.inl"

// Genuine co-visible consumers preserve each reviewed original owner.
// The unique original TU partition remains unknown. Keep operator+= in its
// independent Float3 TU: exposing it here changes the exact trail owners.
#include "Float3Core.inl"
#include "EnemyManagerCore.inl"
#include "PlayerPositionCallback30404Type11.inl"
#include "PlayerPositionCallback30404Type4Type8.inl"
#include "EffectType34Family.inl"

#include "EffectPolarScale.inl"
#include "EffectRingFamily.inl"
#include "EffectInitializeOrientedOffset.inl"
#include "EffectRadialFanFamily.inl"
#include "EffectType16Family.inl"

#include "EffectDoubleFanFamily.inl"

#include "EffectFanVariants.inl"
#include "EffectFanFinalCallbacks.inl"

#include "EffectDrawType29.inl"

#include "ExAttackDrawType14Type22.inl"
