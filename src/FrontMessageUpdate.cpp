#include "AnmManager.hpp"
#include "AsciiManager.hpp"
#include "ScreenEffect.hpp"
#include "ZunTimer.hpp"
#include "PlayerLifecycleView.hpp"
#include "FrontSide.hpp"
#include "GameManagerRuntimeView.hpp"
#include "Background.hpp"
#include "ReplayNativeInputGateCapacity.hpp"
#include "Supervisor.hpp"
#include "FrontMessageRuntimeView.hpp"
#include "FrontFactoryConstructView.hpp"

#include <stddef.h>

// Extended arguments exist only for opcodes carrying this larger payload.
// The shared wire header retains its true variable-length 8-byte prefix.
struct FrontMessageLongInstructionView : FrontMessageInstructionView
{
    int argument08;
    int argument0C;
};

// The four portrait pairs are eight consecutive native ANM VMs.
typedef char FrontMessageNativePortraitPairs[
    (sizeof(((FrontMessageRuntimeView *)0)->messageVms14) ==
     8 * sizeof(AnmVm) &&
     offsetof(FrontMessageRuntimeView, messageVms14) == 0x14)
         ? 1 : -1];

typedef char FrontMessageLongWireArgsAt08And0C[
    (sizeof(FrontMessageLongInstructionView) == 0x10 &&
     offsetof(FrontMessageLongInstructionView, argument08) == 0x08 &&
     offsetof(FrontMessageLongInstructionView, argument0C) == 0x0C) ? 1 : -1];






typedef char FrontMessageFrontSideAuxAtA678[
    (offsetof(FrontSide, auxA678) == 0xA678) ? 1 : -1];

// The real g_GameManager contains two 0x38-byte playable side allocations.
// Opcode 0x1C writes Background::pendingStageLabel18 through the side's
// +0x00 Background pointer (not a separate 0x1C-byte score-buffer view).
// The script skip/confirm input is not another global input allocation.
// This is the third 0x8E-byte member of the same native input array used
// by Replay recording, playback, networking and Player movement.
// Audio opcodes use the actual 0x7B0-byte Supervisor allocation, not a
// separate 3-method opaque receiver. All original music target VAs remain.
typedef char FrontMessageNativeSupervisorAudio[
    (sizeof(Supervisor) == 0x7B0 &&
     offsetof(Supervisor, config388) == 0x388) ? 1 : -1];

typedef char FrontMessageNativeReplayInputSlot[
    (ReplayNativeInputGateCapacity::StateCount == 3 &&
     ReplayNativeInputGateCapacity::SharedInputStateIndex == 2 &&
     sizeof(((ReplayInputState *)0)->heldFrames) == 0x20 &&
     sizeof(g_ReplayInputStates[0]) == 0x8E &&
     ReplayNativeInputGateCapacity::SharedInputStateIndex * sizeof(ReplayInputState) == 0x11C) ? 1 : -1];

typedef char FrontMessageNativeGameManagerOwners[
    (sizeof(AsciiGameManagerView) == 0x390 &&
     sizeof(FrontSideAuxView) == 0x558 &&
     sizeof(AsciiGameManagerSideView) == 0x38 &&
     sizeof(((AsciiGameManagerView *)0)->sides) == 0x70 &&
     offsetof(AsciiGameManagerSideView, background00) == 0x00 &&
     offsetof(AsciiGameManagerSideView, player04) == 0x04 &&
     offsetof(AsciiGameManagerSideView, frontSide18) == 0x18 &&
     offsetof(AsciiGameManagerSideView, shotType20) == 0x20 &&
     offsetof(AsciiGameManagerView, gameMode) == 0x118 &&
     offsetof(Background, pendingStageLabel18) == 0x18 &&
     sizeof(PlayerLifecycleView) == 0x30F70 &&
     offsetof(PlayerLifecycleView, updateState00) == 0x00 &&
     offsetof(PlayerLifecycleView, timer303C8) == 0x303C8 &&
     offsetof(PlayerLifecycleView, tableValue303F0) == 0x303F0 &&
     offsetof(PlayerLifecycleView, tableValue303F8) == 0x303F8 &&
     offsetof(PlayerLifecycleView, tableValue303FC) == 0x303FC &&
     offsetof(FrontSide, auxA678) == 0xA678) ? 1 : -1];



struct FrontMessageNeutralSourceView
{
    unsigned char unknown00[0x0C];
    short musicId0C;
};





extern AsciiGameManagerView g_GameManager;
extern FrontMessageNeutralSourceView *g_FrontMessageSource;
extern Supervisor g_Supervisor;
extern AnmLoaded *g_FrontMessageAnm;
extern AnmManager *g_AnmManager;
extern ScreenEffect *g_TitleFadeReleaseEffect;
extern int g_ScreenTransitionCountdown;
extern int g_StageTransitionState;
extern unsigned char g_FrontMode0Done;
extern unsigned char g_FrontOtherModeDone;

static void DecodeFrontMessageString(
    char (&destination)[64],
    const unsigned char *source)
{
    unsigned char key = 0x77;
    unsigned char step = 7;
    unsigned char decoded;
    char *output = destination;
    do
    {
        *output = static_cast<char>(decoded = *source ^ key);
        ++output;
        ++source;
        key += step;
        step += 0x10;
    } while (decoded != 0);
}

// The shared GameManager side records use their original 0x38-byte
// stride, including when a wire index is outside the playable pair.
static inline AsciiGameManagerSideView &NativeFrontMessageSide(int index)
{
    // The wire index selects a real 0x38-byte GameManager side. Retain
    // its original unsigned i386 scaling and do not add a bounds gate.
    return g_GameManager.sides[static_cast<unsigned int>(index)];
}

// Text-VM and palette selection now use the message runtime's native
// arrays directly; wire indices retain the original signed/unsigned
// expressions and the interpreter's original access ordering.
// The packed FrontMessage payload is a variable-length native byte stream.
// Its 16-bit/8-bit wire operands still require original native byte offsets;
// the shared ReplayInputState is instead indexed as a real array member.
// The message payload is a variable-length file record. Its signed short
// words and byte operands use the original byte cursor, not a shadow table.
// Keep native i386 unsigned index scaling for malformed/signed script input.
// Instructions carry short and byte operands in their real on-disk payload.
// Use the native typed pointer already selected for each opcode; the wire
// index is not normalized or range-checked by the interpreter.
// Shared input is the third record in the native replay-input array.

int FrontMessageRuntimeView::Update()
{
    char textA[64];
    char textB[64];

    if (currentIndex08 < 0)
        return -1;

    if (ignoreWait1D68 > 0)
        --ignoreWait1D68;

    if (skippable1D6C && g_ReplayInputStates[ReplayNativeInputGateCapacity::SharedInputStateIndex].IsHeld(0x100))
        timer1D40 = current04->time00;

    while (timer1D40 >= static_cast<int>(current04->time00))
    {
        FrontMessageInstructionView *instruction = current04;
        switch (instruction->opcode02)
        {
        case FRONT_MESSAGE_OPCODE_END:
            currentIndex08 = -1;
            return -1;

        case FRONT_MESSAGE_OPCODE_SET_PLAYER_UPDATE_STATE_1:
        {
            NativeFrontMessageSide(current04->argument04).player04->SetUpdateState(1);
            NativeFrontMessageSide(current04->argument04).player04->timer303C8 = 0;
            break;
        }

        // Script and sprite identifiers supplied as signed native DWORDs
        // combine modulo 2^32 before the original ANM calls.
        case FRONT_MESSAGE_OPCODE_FOCUS_SIDE_AND_SET_BOTH_PORTRAITS:
        {
            FrontMessageLongInstructionView *instruction =
                reinterpret_cast<FrontMessageLongInstructionView *>(current04);
            int requestedSide = instruction->argument04;
            if (requestedSide >= 0)
            {
                int side;
                if (!mode10)
                    side = requestedSide;
                else
                    side = static_cast<int>(1u - static_cast<unsigned int>(requestedSide));
                if (currentPortrait1D67 != side)
                {
                    for (int i = 0; i < 4; ++i)
                    {
                        if (i != side)
                        {
                            this->messageVms14[static_cast<unsigned int>(i) * 2u + 0u].pendingInterrupt = 4;
                            this->messageVms14[static_cast<unsigned int>(i) * 2u + 1u].pendingInterrupt = 4;
                        }
                    }
                }

                this->messageVms14[static_cast<unsigned int>(side) * 2u + 0u].pendingInterrupt = 3;
                this->messageVms14[static_cast<unsigned int>(side) * 2u + 1u].pendingInterrupt = 3;
                currentPortrait1D67 = (unsigned char)side;
                colorIndex1D64 = (unsigned char)side;

                if (instruction->argument08 >= 0)
                {
                    NativeFrontMessageSide(mode10).frontSide18->auxA678.sideAnm00
                        ->SetSprite(&this->messageVms14[static_cast<unsigned int>(mode10) * 2u + 0u],
                        static_cast<int>(static_cast<unsigned int>(NativeFrontMessageSide(mode10).player04->tableValue303FC) +
                        static_cast<unsigned int>(instruction->argument08)));
                    NativeFrontMessageSide(mode10).frontSide18->auxA678.sideAnm00
                        ->SetSprite(&this->messageVms14[static_cast<unsigned int>(mode10) * 2u + 1u],
                        static_cast<int>(static_cast<unsigned int>(NativeFrontMessageSide(mode10).player04->tableValue303FC) +
                        static_cast<unsigned int>(instruction->argument08) + 9u));
                }

                if (instruction->argument0C >= 0)
                {
                    NativeFrontMessageSide(static_cast<int>(1u - static_cast<unsigned int>(mode10))).frontSide18->auxA678.sideAnm00
                        ->SetSprite(&this->messageVms14[static_cast<unsigned int>(static_cast<int>(1u - static_cast<unsigned int>(mode10))) * 2u + 0u],
                        static_cast<int>(static_cast<unsigned int>(NativeFrontMessageSide(static_cast<int>(1u - static_cast<unsigned int>(mode10))).player04->tableValue303FC) +
                        static_cast<unsigned int>(instruction->argument0C)));
                    NativeFrontMessageSide(static_cast<int>(1u - static_cast<unsigned int>(mode10))).frontSide18->auxA678.sideAnm00
                        ->SetSprite(&this->messageVms14[static_cast<unsigned int>(static_cast<int>(1u - static_cast<unsigned int>(mode10))) * 2u + 1u],
                        static_cast<int>(static_cast<unsigned int>(NativeFrontMessageSide(static_cast<int>(1u - static_cast<unsigned int>(mode10))).player04->tableValue303FC) +
                        static_cast<unsigned int>(instruction->argument0C) + 9u));
                }
            }
            else
            {
                this->messageVms14[0].pendingInterrupt = 4;
                this->messageVms14[2].pendingInterrupt = 4;
                this->messageVms14[1].pendingInterrupt = 4;
                this->messageVms14[3].pendingInterrupt = 4;
            }

            resetText1D65 = 1;
            break;
        }

        case FRONT_MESSAGE_OPCODE_FOCUS_PORTRAIT_AND_SET_SPRITE:
        {
            FrontMessageLongInstructionView *instruction =
                reinterpret_cast<FrontMessageLongInstructionView *>(current04);
            int requested = instruction->argument04;
            int side;
            if (requested < 2)
            {
                if (!mode10)
                    side = requested;
                else
                    side = static_cast<int>(1u - static_cast<unsigned int>(requested));
            }
            else
                side = requested;

            if (currentPortrait1D67 != side)
            {
                for (int i = 0; i < 4; ++i)
                {
                    if (i != side)
                    {
                        this->messageVms14[static_cast<unsigned int>(i) * 2u + 0u].pendingInterrupt = 4;
                        this->messageVms14[static_cast<unsigned int>(i) * 2u + 1u].pendingInterrupt = 4;
                    }
                }
            }

            this->messageVms14[static_cast<unsigned int>(side) * 2u + 0u].pendingInterrupt = 3;
            this->messageVms14[static_cast<unsigned int>(side) * 2u + 1u].pendingInterrupt = 3;
            currentPortrait1D67 = (unsigned char)side;

            if (instruction->argument08 >= 0)
            {
                if (side < 2)
                {
                    NativeFrontMessageSide(side).frontSide18->auxA678.sideAnm00
                        ->SetSprite(&this->messageVms14[static_cast<unsigned int>(side) * 2u + 0u],
                        static_cast<int>(static_cast<unsigned int>(NativeFrontMessageSide(side).player04->tableValue303FC) +
                        static_cast<unsigned int>(instruction->argument08)));
                    NativeFrontMessageSide(side).frontSide18->auxA678.sideAnm00
                        ->SetSprite(&this->messageVms14[static_cast<unsigned int>(side) * 2u + 1u],
                        static_cast<int>(static_cast<unsigned int>(NativeFrontMessageSide(side).player04->tableValue303FC) +
                        static_cast<unsigned int>(instruction->argument08) + 9u));
                }
                else
                {
                    g_GameManager.sides[0].frontSide18->auxA678.optionalAnm04
                        ->SetSprite(&this->messageVms14[static_cast<unsigned int>(side) * 2u + 0u],
                        instruction->argument08);
                    g_GameManager.sides[0].frontSide18->auxA678.optionalAnm04
                        ->SetSprite(&this->messageVms14[static_cast<unsigned int>(side) * 2u + 1u],
                        static_cast<int>(static_cast<unsigned int>(instruction->argument08) + 6u));
                }
            }

            colorIndex1D64 = (unsigned char)side;
            resetText1D65 = 1;
            break;
        }

        case FRONT_MESSAGE_OPCODE_SET_TEXT_COLOR_AND_RESET:
        {
            unsigned char *arguments =
                reinterpret_cast<unsigned char *>(&instruction->argument04);
            resetText1D65 = 1;
            colorIndex1D64 = *arguments;
            break;
        }

        case FRONT_MESSAGE_OPCODE_LOAD_PORTRAIT_SCRIPTS:
        {
            short portrait = *reinterpret_cast<short *>(&current04->argument04);
            if (portrait < 2)
            {
                int side;
                if (!mode10)
                    side = portrait;
                else
                    side = static_cast<int>(1u - static_cast<unsigned int>(portrait));
                // ECL-side signed script ids combine as i386 DWORD sums;
                // preserve wrap before passing the resulting signed index.
                int script =
                    static_cast<int>(
                    static_cast<unsigned int>(
                        NativeFrontMessageSide(side).player04->tableValue303F8) +
                    static_cast<unsigned int>(side));
                AnmLoaded *anm =
                    NativeFrontMessageSide(side).frontSide18->auxA678.sideAnm00;
                anm->SetAndExecuteScriptIdx(
                    &this->messageVms14[static_cast<unsigned int>(side) * 2u + 0u], script);
                script = static_cast<int>(
                    static_cast<unsigned int>(
                        NativeFrontMessageSide(side).player04->tableValue303F8) +
                    static_cast<unsigned int>(side) + 4u);
                anm = NativeFrontMessageSide(side).frontSide18->auxA678.sideAnm00;
                anm->SetAndExecuteScriptIdx(
                    &this->messageVms14[static_cast<unsigned int>(side) * 2u + 1u], script);
            }
            else
            {
                AnmLoaded *anm =
                    g_GameManager.sides[0].frontSide18->auxA678.optionalAnm04;
                anm->SetAndExecuteScriptIdx(
                    &this->messageVms14[static_cast<unsigned int>(portrait) * 2u + 0u], portrait);
                anm = g_GameManager.sides[0].frontSide18->auxA678.optionalAnm04;
                anm->SetAndExecuteScriptIdx(
                    &this->messageVms14[static_cast<unsigned int>(portrait) * 2u + 1u],
                    static_cast<int>(static_cast<unsigned int>(portrait) + 4u));
            }
            break;
        }

        case FRONT_MESSAGE_OPCODE_SET_PORTRAIT_SPRITE:
        {
            short *arguments =
                reinterpret_cast<short *>(&current04->argument04);
            if (arguments[0] < 2)
            {
                int side;
                if (!mode10)
                    side = arguments[0];
                else
                    side = 1 - arguments[0];
                int sprite = arguments[1];
                int spriteBase =
                    static_cast<int>(
                        static_cast<unsigned int>(
                            NativeFrontMessageSide(side).player04->tableValue303FC) +
                        static_cast<unsigned int>(sprite));
                AnmLoaded *anm =
                    NativeFrontMessageSide(side).frontSide18->auxA678.sideAnm00;
                anm->SetSprite(
                    &this->messageVms14[static_cast<unsigned int>(side) * 2u + 0u], spriteBase);
                spriteBase =
                    static_cast<int>(
                        static_cast<unsigned int>(
                            NativeFrontMessageSide(side).player04->tableValue303FC) +
                        static_cast<unsigned int>(arguments[1]) + 9u);
                anm = NativeFrontMessageSide(side).frontSide18->auxA678.sideAnm00;
                anm->SetSprite(
                    &this->messageVms14[static_cast<unsigned int>(side) * 2u + 1u], spriteBase);
            }
            else
            {
                int portrait = arguments[0];
                AnmLoaded *anm =
                    g_GameManager.sides[0].frontSide18->auxA678.optionalAnm04;
                anm->SetSprite(
                    &this->messageVms14[static_cast<unsigned int>(portrait) * 2u + 0u], arguments[1]);
                anm = g_GameManager.sides[0].frontSide18->auxA678.optionalAnm04;
                anm->SetSprite(
                    &this->messageVms14[static_cast<unsigned int>(portrait) * 2u + 1u], static_cast<int>(static_cast<unsigned int>(arguments[1]) + 6u));
            }
            break;
        }

        case FRONT_MESSAGE_OPCODE_DRAW_TEXT_LINE:
        {
            short *arguments =
                reinterpret_cast<short *>(&current04->argument04);
            if (arguments[1] == 0 && textVms1534[1].scriptIndex >= 0)
            {
                g_AnmManager->DrawTextLeft(
                    &textVms1534[1],
                    textColors1D20[arguments[0]],
                    shadowColors1D30[arguments[0]],
                    " ");
            }

            g_FrontMessageAnm->SetAndExecuteScriptIdx(
                &textVms1534[arguments[1]], arguments[1]);
            textVms1534[arguments[1]].fontWidth =
                textVms1534[arguments[1]].fontHeight =
                (unsigned char)glyphSize1D54;

            DecodeFrontMessageString(
                textA,
                reinterpret_cast<const unsigned char *>(arguments) + 4u);
            g_AnmManager->DrawTextLeft(
                &textVms1534[arguments[1]],
                textColors1D20[arguments[0]],
                shadowColors1D30[arguments[0]],
                textA);
            framesElapsed1D4C = 0;
            break;
        }

        case FRONT_MESSAGE_OPCODE_DRAW_NEXT_TEXT_LINE:
        {
            unsigned char *text =
                reinterpret_cast<unsigned char *>(&current04->argument04);
            if (resetText1D65)
            {
                if (textVms1534[1].scriptIndex >= 0)
                {
                    g_AnmManager->DrawTextLeft(
                        &textVms1534[1],
                        textColors1D20[colorIndex1D64],
                        shadowColors1D30[colorIndex1D64],
                        " ");
                }
                textLineIndex1D66 = 0;
            }

            g_FrontMessageAnm->SetAndExecuteScriptIdx(
                &textVms1534[textLineIndex1D66],
                textLineIndex1D66);
            textVms1534[textLineIndex1D66].fontWidth =
                textVms1534[textLineIndex1D66].fontHeight =
                (unsigned char)glyphSize1D54;

            DecodeFrontMessageString(
                textB,
                text);
            g_AnmManager->DrawTextLeft(
                &textVms1534[textLineIndex1D66],
                textColors1D20[colorIndex1D64],
                shadowColors1D30[colorIndex1D64],
                textB);
            framesElapsed1D4C = 0;
            resetText1D65 = 0;
            ++textLineIndex1D66;
            break;
        }

        case FRONT_MESSAGE_OPCODE_WAIT:
        {
            FrontMessageInstructionView *instruction = current04;
            if (skippable1D6C && g_ReplayInputStates[ReplayNativeInputGateCapacity::SharedInputStateIndex].IsHeld(0x100))
                break;

            if (g_ReplayInputStates[ReplayNativeInputGateCapacity::SharedInputStateIndex].WasPressed(1) &&
                framesElapsed1D4C >= waitThreshold1D50)
            {
                resetText1D65 = 1;
                waitThreshold1D50 = 8;
            }
            else
            {
                if (framesElapsed1D4C < instruction->argument04)
                {
                    ++*reinterpret_cast<unsigned int *>(&framesElapsed1D4C);
                    goto run_scripts;
                }

                resetText1D65 = 1;
                waitThreshold1D50 = 30;
            }
            break;
        }

        case FRONT_MESSAGE_OPCODE_INTERRUPT_PORTRAIT:
        {
            short *arguments =
                reinterpret_cast<short *>(&current04->argument04);
            this->messageVms14[static_cast<unsigned int>(arguments[0]) * 2u + 0u].pendingInterrupt =
                reinterpret_cast<unsigned char *>(arguments)[2];
            this->messageVms14[static_cast<unsigned int>(arguments[0]) * 2u + 1u].pendingInterrupt =
                reinterpret_cast<unsigned char *>(arguments)[2];
            break;
        }

        case FRONT_MESSAGE_OPCODE_INCREMENT_IGNORE_WAIT:
            ++ignoreWait1D68;
            break;

        case FRONT_MESSAGE_OPCODE_SET_MUSIC:
        {
            int music = current04->argument04;
            if (music < 0)
            {
                g_Supervisor.StopAudio();
            }
            else
            {
                if (!g_GameManager.IsGameMode2())
                    g_Supervisor.PlayMusic(music, 0);
                else
                    g_Supervisor.PlayMusic(
                        g_FrontMessageSource->musicId0C, 0);
            }
            break;
        }

        case FRONT_MESSAGE_OPCODE_SHOW_INTRO_GRAPHIC:
        {
            AsciiGameManagerSideView *side = &g_GameManager.sides[1];
            // This signed Player table word is combined with the opcode-8
            // script offset as a native modulo-32 i386 DWORD before the
            // original ANM call, without any new script bounds gate.
            int script = static_cast<int>(
                static_cast<unsigned int>(side->player04->tableValue303F0) + 4u);
            side->frontSide18->auxA678.sideAnm00
                ->SetAndExecuteScriptIdx(&introVm1A7C, script);
            framesElapsed1D4C = 0;
            break;
        }

        case FRONT_MESSAGE_OPCODE_INIT_RESULT_BONUS_AND_FADE_MUSIC:
            g_GameManager.frontFactoryA8->InitializeResultBonus();
            // fall through
        case FRONT_MESSAGE_OPCODE_FADE_OUT_MUSIC:
        {
            g_Supervisor.FadeOutMusic(4.0f);
            break;
        }

        case FRONT_MESSAGE_OPCODE_START_SCREEN_FADE:
            g_TitleFadeReleaseEffect = ScreenEffect::RegisterChain(
                SCREEN_EFFECT_FULL_FADE_HOLD,
                300, 0xFFFFFF, 0, 0, 35, 2);
            g_ScreenTransitionCountdown = 302;
            break;

        case FRONT_MESSAGE_OPCODE_SET_SKIPPABLE:
            skippable1D6C =
                *reinterpret_cast<unsigned char *>(&current04->argument04);
            break;

        case FRONT_MESSAGE_OPCODE_SET_TEXT_BOX_VISIBLE:
            textBoxVisible1D6D =
                *reinterpret_cast<unsigned char *>(&current04->argument04);
            break;

        case FRONT_MESSAGE_OPCODE_START_SECONDARY_TIMER:
            secondaryTimer1D58 = 1;
            break;

        case FRONT_MESSAGE_OPCODE_LOAD_BOTH_FRONT_SIDE_SCRIPT_67:
            g_GameManager.sides[0].frontSide18->LoadScript67();
            g_GameManager.sides[1].frontSide18->LoadScript67();
            break;

        case FRONT_MESSAGE_OPCODE_SET_PENDING_STAGE_LABEL:
            g_GameManager.sides[0].background00->pendingStageLabel18 =
                current04->argument04;
            g_GameManager.sides[1].background00->pendingStageLabel18 =
                current04->argument04;
            break;

        default:
            break;
        case FRONT_MESSAGE_OPCODE_PAUSE_SCRIPT:
            goto run_scripts;

        case FRONT_MESSAGE_OPCODE_TRIGGER_STAGE_TRANSITION:
            if (g_GameManager.IsGameMode0())
            {
                if (!mode10)
                    g_StageTransitionState = 3;
                else
                    g_FrontMode0Done = 1;
            }
            else if (g_GameManager.IsGameMode1())
            {
                g_StageTransitionState = 3;
            }
            else
            {
                g_FrontOtherModeDone = 1;
            }
            goto run_scripts;

        }

        // Advance through the actual variable-length instruction byte
        // buffer using its live encoded payload size; do not invent a
        // fixed-size command stride or a new bounds check.
        FrontMessageInstructionView *next =
            reinterpret_cast<FrontMessageInstructionView *>(
                reinterpret_cast<unsigned char *>(&current04->argument04) +
                static_cast<unsigned int>(current04->size03));
        current04 = next;
    }
    timer1D40++;

run_scripts:
    // The four portrait pairs are eight actual contiguous ANM VMs;
    // retain the original per-element script execution order.
    g_AnmManager->ExecuteScript(&this->messageVms14[0]);
    g_AnmManager->ExecuteScript(&this->messageVms14[2]);
    g_AnmManager->ExecuteScript(&this->messageVms14[1]);
    g_AnmManager->ExecuteScript(&this->messageVms14[3]);
    g_AnmManager->ExecuteScript(&this->messageVms14[4]);
    g_AnmManager->ExecuteScript(&this->messageVms14[6]);
    g_AnmManager->ExecuteScript(&this->messageVms14[5]);
    g_AnmManager->ExecuteScript(&this->messageVms14[7]);
    g_AnmManager->ExecuteScript(&textVms1534[0]);
    g_AnmManager->ExecuteScript(&textVms1534[1]);
    g_AnmManager->ExecuteScript(&introVm1A7C);

    if (timer1D40 < 60 && skippable1D6C &&
        g_ReplayInputStates[ReplayNativeInputGateCapacity::SharedInputStateIndex].IsHeld(0x100))
    {
        timer1D40 = 60;
    }

    if (secondaryTimer1D58 >= 1)
    {
        if (secondaryTimer1D58 < 60 && skippable1D6C &&
            g_ReplayInputStates[ReplayNativeInputGateCapacity::SharedInputStateIndex].IsHeld(0x100))
        {
            timer1D40 = 60;
        }
        secondaryTimer1D58++;
    }

    return 0;
}
