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

struct FrontMessagePortraitPair
{
    AnmVm first00;
    AnmVm second2A4;
};

typedef char FrontMessagePortraitPairSizeIs548[
    (sizeof(FrontMessagePortraitPair) == 0x548 &&
     sizeof(((FrontMessageRuntimeView *)0)->messageVms14) ==
         4 * sizeof(FrontMessagePortraitPair) &&
     offsetof(FrontMessageRuntimeView, messageVms14) == 0x14) ? 1 : -1];
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
extern ScreenEffect *g_FrontMessageScreenEffect;
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
        *output++ = (char)(decoded = *source++ ^ key);
        key += step;
        step += 0x10;
    } while (decoded != 0);
}

// The actual portrait VMs are four 0x548-byte pairs in the native eight-VM
// allocation. The wire side/index word has no retail bounds check.
static inline FrontMessagePortraitPair &NativeMessagePortrait(
    FrontMessageRuntimeView *owner, int index)
{
    return *reinterpret_cast<FrontMessagePortraitPair *>(
        reinterpret_cast<unsigned int>(&owner->messageVms14[0]) +
        static_cast<unsigned int>(index) * sizeof(FrontMessagePortraitPair));
}

// The shared GameManager side records use their original 0x38-byte
// stride, including when a wire index is outside the playable pair.
static inline AsciiGameManagerSideView &NativeFrontMessageSide(int index)
{
    return *reinterpret_cast<AsciiGameManagerSideView *>(
        reinterpret_cast<unsigned int>(&g_GameManager.sides[0]) +
        static_cast<unsigned int>(index) * sizeof(g_GameManager.sides[0]));
}

// Keep original VC7.1 scalar load scheduling: a callable palette/VM
// adapter alters this switch owner's register and jump-table layout.
// This lexical expression is the native i386 array-word address instead.
#define FRONT_MESSAGE_NATIVE_SLOT(type, array, index) \
    (*reinterpret_cast<type *>(reinterpret_cast<unsigned int>(&array[0]) + \
     static_cast<unsigned int>(index) * sizeof(array[0])))

// The packed FrontMessage payload is a variable-length native byte stream.
// Its 16-bit/8-bit operands and the shared 0x8E-byte ReplayInputState
// are addressed as actual i386 DWORD words, without a bounded shadow array.
#define FRONT_NATIVE_SHORT_ARG(arguments, index) \
    (*reinterpret_cast<short *>(reinterpret_cast<unsigned int>(arguments) + \
        static_cast<unsigned int>(index) * sizeof(short)))
#define FRONT_NATIVE_BYTE_ARG(arguments, index) \
    (*reinterpret_cast<unsigned char *>(reinterpret_cast<unsigned int>(arguments) + \
        static_cast<unsigned int>(index)))
#define FRONT_NATIVE_SHARED_INPUT \
    (*reinterpret_cast<ReplayInputState *>(reinterpret_cast<unsigned int>(&g_ReplayInputStates[0]) + \
        ReplayNativeInputGateCapacity::SharedInputStateIndex * sizeof(g_ReplayInputStates[0])))

int FrontMessageRuntimeView::Update()
{
    char textA[64];
    char textB[64];

    if (currentIndex08 < 0)
        return -1;

    if (ignoreWait1D68 > 0)
        --ignoreWait1D68;

    if (skippable1D6C && FRONT_NATIVE_SHARED_INPUT.IsHeld(0x100))
        timer1D40 = current04->time00;

    while (timer1D40 >= static_cast<int>(current04->time00))
    {
        FrontMessageInstructionView *instruction = current04;
        switch (instruction->opcode02)
        {
        case 0:
            currentIndex08 = -1;
            return -1;

        case 0x17:
        {
            NativeFrontMessageSide(current04->argument04).player04->SetUpdateState(1);
            NativeFrontMessageSide(current04->argument04).player04->timer303C8 = 0;
            break;
        }

        // Script and sprite identifiers supplied as signed native DWORDs
        // combine modulo 2^32 before the original ANM calls.
        case 0x0F:
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
                            NativeMessagePortrait(this, i).first00.pendingInterrupt = 4;
                            NativeMessagePortrait(this, i).second2A4.pendingInterrupt = 4;
                        }
                    }
                }

                NativeMessagePortrait(this, side).first00.pendingInterrupt = 3;
                NativeMessagePortrait(this, side).second2A4.pendingInterrupt = 3;
                currentPortrait1D67 = (unsigned char)side;
                colorIndex1D64 = (unsigned char)side;

                if (instruction->argument08 >= 0)
                {
                    NativeFrontMessageSide(mode10).frontSide18->auxA678.sideAnm00
                        ->SetSprite(&NativeMessagePortrait(this, mode10).first00,
                        static_cast<int>(static_cast<unsigned int>(NativeFrontMessageSide(mode10).player04->tableValue303FC) +
                        static_cast<unsigned int>(instruction->argument08)));
                    NativeFrontMessageSide(mode10).frontSide18->auxA678.sideAnm00
                        ->SetSprite(&NativeMessagePortrait(this, mode10).second2A4,
                        static_cast<int>(static_cast<unsigned int>(NativeFrontMessageSide(mode10).player04->tableValue303FC) +
                        static_cast<unsigned int>(instruction->argument08) + 9u));
                }

                if (instruction->argument0C >= 0)
                {
                    NativeFrontMessageSide(static_cast<int>(1u - static_cast<unsigned int>(mode10))).frontSide18->auxA678.sideAnm00
                        ->SetSprite(&NativeMessagePortrait(this, static_cast<int>(1u - static_cast<unsigned int>(mode10))).first00,
                        static_cast<int>(static_cast<unsigned int>(NativeFrontMessageSide(static_cast<int>(1u - static_cast<unsigned int>(mode10))).player04->tableValue303FC) +
                        static_cast<unsigned int>(instruction->argument0C)));
                    NativeFrontMessageSide(static_cast<int>(1u - static_cast<unsigned int>(mode10))).frontSide18->auxA678.sideAnm00
                        ->SetSprite(&NativeMessagePortrait(this, static_cast<int>(1u - static_cast<unsigned int>(mode10))).second2A4,
                        static_cast<int>(static_cast<unsigned int>(NativeFrontMessageSide(static_cast<int>(1u - static_cast<unsigned int>(mode10))).player04->tableValue303FC) +
                        static_cast<unsigned int>(instruction->argument0C) + 9u));
                }
            }
            else
            {
                reinterpret_cast<FrontMessagePortraitPair *>(messageVms14)[0].first00.pendingInterrupt = 4;
                reinterpret_cast<FrontMessagePortraitPair *>(messageVms14)[1].first00.pendingInterrupt = 4;
                reinterpret_cast<FrontMessagePortraitPair *>(messageVms14)[0].second2A4.pendingInterrupt = 4;
                reinterpret_cast<FrontMessagePortraitPair *>(messageVms14)[1].second2A4.pendingInterrupt = 4;
            }

            resetText1D65 = 1;
            break;
        }

        case 0x11:
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
                        NativeMessagePortrait(this, i).first00.pendingInterrupt = 4;
                        NativeMessagePortrait(this, i).second2A4.pendingInterrupt = 4;
                    }
                }
            }

            NativeMessagePortrait(this, side).first00.pendingInterrupt = 3;
            NativeMessagePortrait(this, side).second2A4.pendingInterrupt = 3;
            currentPortrait1D67 = (unsigned char)side;

            if (instruction->argument08 >= 0)
            {
                if (side < 2)
                {
                    NativeFrontMessageSide(side).frontSide18->auxA678.sideAnm00
                        ->SetSprite(&NativeMessagePortrait(this, side).first00,
                        static_cast<int>(static_cast<unsigned int>(NativeFrontMessageSide(side).player04->tableValue303FC) +
                        static_cast<unsigned int>(instruction->argument08)));
                    NativeFrontMessageSide(side).frontSide18->auxA678.sideAnm00
                        ->SetSprite(&NativeMessagePortrait(this, side).second2A4,
                        static_cast<int>(static_cast<unsigned int>(NativeFrontMessageSide(side).player04->tableValue303FC) +
                        static_cast<unsigned int>(instruction->argument08) + 9u));
                }
                else
                {
                    g_GameManager.sides[0].frontSide18->auxA678.optionalAnm04
                        ->SetSprite(&NativeMessagePortrait(this, side).first00,
                        instruction->argument08);
                    g_GameManager.sides[0].frontSide18->auxA678.optionalAnm04
                        ->SetSprite(&NativeMessagePortrait(this, side).second2A4,
                        static_cast<int>(static_cast<unsigned int>(instruction->argument08) + 6u));
                }
            }

            colorIndex1D64 = (unsigned char)side;
            resetText1D65 = 1;
            break;
        }

        case 0x1A:
        {
            unsigned char *arguments =
                reinterpret_cast<unsigned char *>(&instruction->argument04);
            resetText1D65 = 1;
            colorIndex1D64 = *arguments;
            break;
        }

        case 1:
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
                    &NativeMessagePortrait(this, side).first00, script);
                script = static_cast<int>(
                    static_cast<unsigned int>(
                        NativeFrontMessageSide(side).player04->tableValue303F8) +
                    static_cast<unsigned int>(side) + 4u);
                anm = NativeFrontMessageSide(side).frontSide18->auxA678.sideAnm00;
                anm->SetAndExecuteScriptIdx(
                    &NativeMessagePortrait(this, side).second2A4, script);
            }
            else
            {
                AnmLoaded *anm =
                    g_GameManager.sides[0].frontSide18->auxA678.optionalAnm04;
                anm->SetAndExecuteScriptIdx(
                    &NativeMessagePortrait(this, portrait).first00, portrait);
                anm = g_GameManager.sides[0].frontSide18->auxA678.optionalAnm04;
                anm->SetAndExecuteScriptIdx(
                    &NativeMessagePortrait(this, portrait).second2A4,
                    static_cast<int>(static_cast<unsigned int>(portrait) + 4u));
            }
            break;
        }

        case 2:
        {
            short *arguments =
                reinterpret_cast<short *>(&current04->argument04);
            if (FRONT_NATIVE_SHORT_ARG(arguments, 0) < 2)
            {
                int side;
                if (!mode10)
                    side = FRONT_NATIVE_SHORT_ARG(arguments, 0);
                else
                    side = 1 - FRONT_NATIVE_SHORT_ARG(arguments, 0);
                int sprite = FRONT_NATIVE_SHORT_ARG(arguments, 1);
                int spriteBase =
                    static_cast<int>(
                        static_cast<unsigned int>(
                            NativeFrontMessageSide(side).player04->tableValue303FC) +
                        static_cast<unsigned int>(sprite));
                AnmLoaded *anm =
                    NativeFrontMessageSide(side).frontSide18->auxA678.sideAnm00;
                anm->SetSprite(
                    &NativeMessagePortrait(this, side).first00, spriteBase);
                spriteBase =
                    static_cast<int>(
                        static_cast<unsigned int>(
                            NativeFrontMessageSide(side).player04->tableValue303FC) +
                        static_cast<unsigned int>(FRONT_NATIVE_SHORT_ARG(arguments, 1)) + 9u);
                anm = NativeFrontMessageSide(side).frontSide18->auxA678.sideAnm00;
                anm->SetSprite(
                    &NativeMessagePortrait(this, side).second2A4, spriteBase);
            }
            else
            {
                int portrait = FRONT_NATIVE_SHORT_ARG(arguments, 0);
                AnmLoaded *anm =
                    g_GameManager.sides[0].frontSide18->auxA678.optionalAnm04;
                anm->SetSprite(
                    &NativeMessagePortrait(this, portrait).first00, FRONT_NATIVE_SHORT_ARG(arguments, 1));
                anm = g_GameManager.sides[0].frontSide18->auxA678.optionalAnm04;
                anm->SetSprite(
                    &NativeMessagePortrait(this, portrait).second2A4, static_cast<int>(static_cast<unsigned int>(FRONT_NATIVE_SHORT_ARG(arguments, 1)) + 6u));
            }
            break;
        }

        case 3:
        {
            short *arguments =
                reinterpret_cast<short *>(&current04->argument04);
            if (FRONT_NATIVE_SHORT_ARG(arguments, 1) == 0 && textVms1534[1].scriptIndex >= 0)
            {
                g_AnmManager->DrawTextLeft(
                    &textVms1534[1],
                    FRONT_MESSAGE_NATIVE_SLOT(unsigned long, textColors1D20, FRONT_NATIVE_SHORT_ARG(arguments, 0)),
                    FRONT_MESSAGE_NATIVE_SLOT(unsigned long, shadowColors1D30, FRONT_NATIVE_SHORT_ARG(arguments, 0)),
                    " ");
            }

            g_FrontMessageAnm->SetAndExecuteScriptIdx(
                &FRONT_MESSAGE_NATIVE_SLOT(AnmVm, textVms1534, FRONT_NATIVE_SHORT_ARG(arguments, 1)), FRONT_NATIVE_SHORT_ARG(arguments, 1));
            reinterpret_cast<unsigned char *>(&FRONT_MESSAGE_NATIVE_SLOT(AnmVm, textVms1534, FRONT_NATIVE_SHORT_ARG(arguments, 1)))[0x298] =
                reinterpret_cast<unsigned char *>(&FRONT_MESSAGE_NATIVE_SLOT(AnmVm, textVms1534, FRONT_NATIVE_SHORT_ARG(arguments, 1)))[0x299] =
                (unsigned char)glyphSize1D54;

            DecodeFrontMessageString(
                textA,
                reinterpret_cast<const unsigned char *>(
                    reinterpret_cast<unsigned int>(arguments) + 4u));
            g_AnmManager->DrawTextLeft(
                &FRONT_MESSAGE_NATIVE_SLOT(AnmVm, textVms1534, FRONT_NATIVE_SHORT_ARG(arguments, 1)),
                FRONT_MESSAGE_NATIVE_SLOT(unsigned long, textColors1D20, FRONT_NATIVE_SHORT_ARG(arguments, 0)),
                FRONT_MESSAGE_NATIVE_SLOT(unsigned long, shadowColors1D30, FRONT_NATIVE_SHORT_ARG(arguments, 0)),
                textA);
            framesElapsed1D4C = 0;
            break;
        }

        case 0x10:
        {
            unsigned char *text =
                reinterpret_cast<unsigned char *>(&current04->argument04);
            if (resetText1D65)
            {
                if (textVms1534[1].scriptIndex >= 0)
                {
                    g_AnmManager->DrawTextLeft(
                        &textVms1534[1],
                        FRONT_MESSAGE_NATIVE_SLOT(unsigned long, textColors1D20, colorIndex1D64),
                        FRONT_MESSAGE_NATIVE_SLOT(unsigned long, shadowColors1D30, colorIndex1D64),
                        " ");
                }
                textLineIndex1D66 = 0;
            }

            g_FrontMessageAnm->SetAndExecuteScriptIdx(
                &FRONT_MESSAGE_NATIVE_SLOT(AnmVm, textVms1534, textLineIndex1D66),
                textLineIndex1D66);
            reinterpret_cast<unsigned char *>(
                &FRONT_MESSAGE_NATIVE_SLOT(AnmVm, textVms1534, textLineIndex1D66))[0x298] =
                reinterpret_cast<unsigned char *>(
                    &FRONT_MESSAGE_NATIVE_SLOT(AnmVm, textVms1534, textLineIndex1D66))[0x299] =
                (unsigned char)glyphSize1D54;

            DecodeFrontMessageString(
                textB,
                text);
            g_AnmManager->DrawTextLeft(
                &FRONT_MESSAGE_NATIVE_SLOT(AnmVm, textVms1534, textLineIndex1D66),
                FRONT_MESSAGE_NATIVE_SLOT(unsigned long, textColors1D20, colorIndex1D64),
                FRONT_MESSAGE_NATIVE_SLOT(unsigned long, shadowColors1D30, colorIndex1D64),
                textB);
            framesElapsed1D4C = 0;
            resetText1D65 = 0;
            ++textLineIndex1D66;
            break;
        }

        case 4:
        {
            FrontMessageInstructionView *instruction = current04;
            if (skippable1D6C && FRONT_NATIVE_SHARED_INPUT.IsHeld(0x100))
                break;

            if (FRONT_NATIVE_SHARED_INPUT.WasPressed(1) &&
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

        case 5:
        {
            short *arguments =
                reinterpret_cast<short *>(&current04->argument04);
            NativeMessagePortrait(this, FRONT_NATIVE_SHORT_ARG(arguments, 0)).first00.pendingInterrupt =
                FRONT_NATIVE_BYTE_ARG(arguments, 2);
            NativeMessagePortrait(this, FRONT_NATIVE_SHORT_ARG(arguments, 0)).second2A4.pendingInterrupt =
                FRONT_NATIVE_BYTE_ARG(arguments, 2);
            break;
        }

        case 6:
            ++ignoreWait1D68;
            break;

        case 7:
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

        case 8:
        {
            AsciiGameManagerSideView *side = &g_GameManager.sides[1];
            int script = side->player04->tableValue303F0 + 4;
            side->frontSide18->auxA678.sideAnm00
                ->SetAndExecuteScriptIdx(&introVm1A7C, script);
            framesElapsed1D4C = 0;
            break;
        }

        case 9:
            g_GameManager.frontFactoryA8->InitializeResultBonus();
            // fall through
        case 0x0C:
        {
            g_Supervisor.FadeOutMusic(4.0f);
            break;
        }

        case 0x0E:
            g_FrontMessageScreenEffect = ScreenEffect::RegisterChain(
                SCREEN_EFFECT_FULL_FADE_HOLD,
                300, 0xFFFFFF, 0, 0, 35, 2);
            g_ScreenTransitionCountdown = 302;
            break;

        case 0x0D:
            skippable1D6C =
                *reinterpret_cast<unsigned char *>(&current04->argument04);
            break;

        case 0x12:
            textBoxVisible1D6D =
                *reinterpret_cast<unsigned char *>(&current04->argument04);
            break;

        case 0x18:
            secondaryTimer1D58 = 1;
            break;

        case 0x19:
            g_GameManager.sides[0].frontSide18->LoadScript67();
            g_GameManager.sides[1].frontSide18->LoadScript67();
            break;

        case 0x1C:
            g_GameManager.sides[0].background00->pendingStageLabel18 =
                current04->argument04;
            g_GameManager.sides[1].background00->pendingStageLabel18 =
                current04->argument04;
            break;

        default:
            break;
        case 0x0A:
            goto run_scripts;

        case 0x0B:
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

        // Retail advances through the file-backed variable-width command
        // payload with an unsigned 32-bit byte-address word, not a bounded
        // C++ struct-member pointer. Keep the original live size03 read.
        FrontMessageInstructionView *next =
            reinterpret_cast<FrontMessageInstructionView *>(
                reinterpret_cast<unsigned int>(&current04->argument04) +
                static_cast<unsigned int>(current04->size03));
        current04 = next;
    }
    timer1D40++;

run_scripts:
    g_AnmManager->ExecuteScript(&reinterpret_cast<FrontMessagePortraitPair *>(messageVms14)[0].first00);
    g_AnmManager->ExecuteScript(&reinterpret_cast<FrontMessagePortraitPair *>(messageVms14)[1].first00);
    g_AnmManager->ExecuteScript(&reinterpret_cast<FrontMessagePortraitPair *>(messageVms14)[0].second2A4);
    g_AnmManager->ExecuteScript(&reinterpret_cast<FrontMessagePortraitPair *>(messageVms14)[1].second2A4);
    g_AnmManager->ExecuteScript(&reinterpret_cast<FrontMessagePortraitPair *>(messageVms14)[2].first00);
    g_AnmManager->ExecuteScript(&reinterpret_cast<FrontMessagePortraitPair *>(messageVms14)[3].first00);
    g_AnmManager->ExecuteScript(&reinterpret_cast<FrontMessagePortraitPair *>(messageVms14)[2].second2A4);
    g_AnmManager->ExecuteScript(&reinterpret_cast<FrontMessagePortraitPair *>(messageVms14)[3].second2A4);
    g_AnmManager->ExecuteScript(&textVms1534[0]);
    g_AnmManager->ExecuteScript(&textVms1534[1]);
    g_AnmManager->ExecuteScript(&introVm1A7C);

    if (timer1D40 < 60 && skippable1D6C &&
        FRONT_NATIVE_SHARED_INPUT.IsHeld(0x100))
    {
        timer1D40 = 60;
    }

    if (secondaryTimer1D58 >= 1)
    {
        if (secondaryTimer1D58 < 60 && skippable1D6C &&
            FRONT_NATIVE_SHARED_INPUT.IsHeld(0x100))
        {
            timer1D40 = 60;
        }
        secondaryTimer1D58++;
    }

    return 0;
}

#undef FRONT_MESSAGE_NATIVE_SLOT

#undef FRONT_NATIVE_SHORT_ARG
#undef FRONT_NATIVE_BYTE_ARG
#undef FRONT_NATIVE_SHARED_INPUT
