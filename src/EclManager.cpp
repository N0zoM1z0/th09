#include "EclManager.hpp"
#include "EclRunLate.inl"

#include <string.h>

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
    context = enemyState->activeContext2CE0;
    *worldPosition = *position + *positionOffset;

    if (static_cast<int>(context->secondaryTime094) > 0)
    {
        context->secondaryTime094--;
        context->time008--;
        goto th09_ecl_after_dispatch;
    }

    if (!(context->time008 == instruction->time00))
        goto th09_ecl_after_dispatch;

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

th09_ecl_after_dispatch:
    context = enemyState->activeContext2CE0;
    if (enemyState->life2E48 > 0)
    {
        Float3 positionBeforeCallbacks = *position;
        positionInterpolated = false;

        perFrameCallback =
            reinterpret_cast<Th09EclRunState::ExInstructionCallback>(
                context->perFrameCallback014);
        if (perFrameCallback != NULL)
        {
            perFrameCallback(
                enemy,
                reinterpret_cast<Th09EclRawInstructionHeaderView *>(
                    context->perFrameInstruction018));
        }

        for (interpolationIndex = 0;
             interpolationIndex < TH09_ECL_INTERPOLATION_SLOT_COUNT;
             ++interpolationIndex)
        {
            slot = &context->interpolationSlots0A0[interpolationIndex];
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
    enemy->UpdateMovement();
    enemy->UpdateShotAndAnm();
    return 0;
}
