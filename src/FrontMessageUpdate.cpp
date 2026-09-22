#include "AnmManager.hpp"
#include "AsciiManager.hpp"
#include "ScreenEffect.hpp"
#include "ZunTimer.hpp"

#include <stddef.h>

struct FrontMessageInstructionView
{
    unsigned short time00;
    unsigned char opcode02;
    unsigned char size03;
    int argument04;
    int argument08;
    int argument0C;
};

struct FrontMessagePortraitPair
{
    AnmVm first00;
    AnmVm second2A4;
};

typedef char FrontMessagePortraitPairSizeIs548[
    (sizeof(FrontMessagePortraitPair) == 0x548) ? 1 : -1];

struct FrontMessagePlayerView
{
    unsigned char unknown00000[0x303C8];
    ZunTimer stateTimer303C8;
    unsigned char unknown303D4[0x303F0 - 0x303D4];
    int tableValue303F0;
    int tableValue303F4;
    int tableValue303F8;
    int tableValue303FC;

    void SetUpdateState(int state);
};


struct FrontMessageFrontSideAuxView
{
    AnmLoaded *sideAnm00;
    AnmLoaded *optionalAnm04;
    unsigned char unknown08[0x550];
};

typedef char FrontMessageFrontSideAuxSizeIs558[
    (sizeof(FrontMessageFrontSideAuxView) == 0x558) ? 1 : -1];

struct FrontMessageFrontSideView
{
    unsigned char unknown0000[0xA678];
    FrontMessageFrontSideAuxView auxA678;

    void LoadScript67();
};

typedef char FrontMessageFrontSideAuxAtA678[
    (offsetof(FrontMessageFrontSideView, auxA678) == 0xA678) ? 1 : -1];

struct FrontMessageSideStateView
{
    unsigned char unknown00[0x18];
    int value18;
};

struct FrontMessageSideView
{
    FrontMessageSideStateView *state00;
    FrontMessagePlayerView *player04;
    unsigned char unknown08[0x10];
    FrontMessageFrontSideView *frontSide18;
    unsigned char unknown1C[0x04];
    int shotType20;
    unsigned char unknown24[0x14];
};

typedef char FrontMessageSideSizeIs38[
    (sizeof(FrontMessageSideView) == 0x38) ? 1 : -1];

struct FrontMessageGameManagerView
{
    FrontMessageSideView sides[2];
    unsigned char unknown70[0x118 - 0x70];
    int gameMode118;

    int IsGameMode0();
    int IsGameMode1();
    int IsGameMode2();
};

struct FrontMessageNeutralSourceView
{
    unsigned char unknown00[0x0C];
    short musicId0C;
};

struct FrontMessageInputView
{
    unsigned short IsHeld(unsigned short mask);
    unsigned short WasPressed(unsigned short mask);
};

struct FrontMessageSupervisorView
{
    int StopAudio();
    int PlayMusic(int track, int unused);
    int FadeOutMusic(float seconds);
};

struct FrontMessageResultOwnerView
{
    void InitializeResultBonus();
};

struct FrontMessageRuntimeView
{
    void *primary00;
    FrontMessageInstructionView *current04;
    int currentIndex08;
    void *secondary0C;
    int mode10;
    FrontMessagePortraitPair portraits14[4];
    AnmVm textVms1534[2];
    AnmVm introVm1A7C;
    unsigned long textColors1D20[4];
    unsigned long shadowColors1D30[4];
    ZunTimer timer1D40;
    int framesElapsed1D4C;
    int waitThreshold1D50;
    int glyphSize1D54;
    ZunTimer secondaryTimer1D58;
    unsigned char colorIndex1D64;
    unsigned char resetText1D65;
    unsigned char textLineIndex1D66;
    unsigned char currentPortrait1D67;
    unsigned int ignoreWait1D68;
    unsigned char skippable1D6C;
    unsigned char textBoxVisible1D6D;
    unsigned char unknown1D6E[2];

    int Update();
};

typedef char FrontMessageRuntimeSizeIs1D70[
    (sizeof(FrontMessageRuntimeView) == 0x1D70) ? 1 : -1];
typedef char FrontMessagePortraitsAt14[
    (offsetof(FrontMessageRuntimeView, portraits14) == 0x14) ? 1 : -1];
typedef char FrontMessageTextAt1534[
    (offsetof(FrontMessageRuntimeView, textVms1534) == 0x1534) ? 1 : -1];
typedef char FrontMessageIntroAt1A7C[
    (offsetof(FrontMessageRuntimeView, introVm1A7C) == 0x1A7C) ? 1 : -1];
typedef char FrontMessageTimerAt1D40[
    (offsetof(FrontMessageRuntimeView, timer1D40) == 0x1D40) ? 1 : -1];
typedef char FrontMessageTailAt1D64[
    (offsetof(FrontMessageRuntimeView, colorIndex1D64) == 0x1D64) ? 1 : -1];

extern FrontMessageGameManagerView g_GameManager;
extern FrontMessageInputView g_FrontMessageInput;
extern FrontMessageNeutralSourceView *g_FrontMessageSource;
extern FrontMessageResultOwnerView *g_FrontMessageOwner;
extern FrontMessageSupervisorView g_Supervisor;
extern AnmLoaded *g_FrontMessageAnm;
extern AnmManager *g_AnmManager;
extern ScreenEffect *g_FrontMessageScreenEffect;
extern int g_ScreenTransitionCountdown;
extern int g_StageTransitionState;
extern unsigned char g_FrontMode0Done;
extern unsigned char g_FrontOtherModeDone;

static void DecodeFrontMessageString(
    unsigned char *destination,
    const unsigned char *source)
{
    unsigned char key = 0x77;
    unsigned char step = 7;
    unsigned char decoded;
    do
    {
        decoded = *source ^ key;
        *destination = decoded;
        ++destination;
        key += step;
        ++source;
        step += 0x10;
    } while (decoded != 0);
}

static __inline AnmVm *FrontMessagePortraitVm(
    FrontMessageRuntimeView *runtime, int index, int second)
{
    return second ? &runtime->portraits14[index].second2A4
                  : &runtime->portraits14[index].first00;
}

int FrontMessageRuntimeView::Update()
{
    char textA[64];
    char textB[64];

    if (currentIndex08 < 0)
        return -1;

    if (ignoreWait1D68 > 0)
        --ignoreWait1D68;

    if (skippable1D6C && g_FrontMessageInput.IsHeld(0x100))
        timer1D40 = current04->time00;

    if (timer1D40 >= static_cast<int>(current04->time00))
    {
        do
        {
            switch (current04->opcode02)
            {
            case 0:
                currentIndex08 = -1;
                return -1;

            case 1:
            {
                int portrait = *reinterpret_cast<short *>(&current04->argument04);
                if (portrait >= 2)
                {
                    AnmLoaded *anm =
                        g_GameManager.sides[0].frontSide18->auxA678.optionalAnm04;
                    anm->SetAndExecuteScriptIdx(
                        FrontMessagePortraitVm(this, portrait, 0), portrait);
                    anm->SetAndExecuteScriptIdx(
                        FrontMessagePortraitVm(this, portrait, 1), portrait + 4);
                }
                else
                {
                    int side = mode10 ? 1 - portrait : portrait;
                    FrontMessageSideView *sideView = &g_GameManager.sides[side];
                    int script = sideView->player04->tableValue303F8 + side;
                    AnmLoaded *anm = sideView->frontSide18->auxA678.sideAnm00;
                    anm->SetAndExecuteScriptIdx(
                        FrontMessagePortraitVm(this, side, 0), script);
                    anm->SetAndExecuteScriptIdx(
                        FrontMessagePortraitVm(this, side, 1), script + 4);
                }
                break;
            }

            case 2:
            {
                int portrait = *reinterpret_cast<short *>(&current04->argument04);
                int sprite = *reinterpret_cast<short *>(
                    reinterpret_cast<unsigned char *>(&current04->argument04) + 2);
                if (portrait >= 2)
                {
                    AnmLoaded *anm =
                        g_GameManager.sides[0].frontSide18->auxA678.optionalAnm04;
                    anm->SetSprite(
                        FrontMessagePortraitVm(this, portrait, 0), sprite);
                    anm->SetSprite(
                        FrontMessagePortraitVm(this, portrait, 1), sprite + 6);
                }
                else
                {
                    int side = mode10 ? 1 - portrait : portrait;
                    FrontMessageSideView *sideView = &g_GameManager.sides[side];
                    int spriteBase = sideView->player04->tableValue303FC + sprite;
                    AnmLoaded *anm = sideView->frontSide18->auxA678.sideAnm00;
                    anm->SetSprite(
                        FrontMessagePortraitVm(this, side, 0), spriteBase);
                    anm->SetSprite(
                        FrontMessagePortraitVm(this, side, 1), spriteBase + 9);
                }
                break;
            }

            case 3:
            {
                int color =
                    *reinterpret_cast<short *>(&current04->argument04);
                int line = *reinterpret_cast<short *>(
                    reinterpret_cast<unsigned char *>(&current04->argument04) + 2);
                if (line == 0 && textVms1534[1].scriptIndex >= 0)
                {
                    g_AnmManager->DrawTextLeft(
                        &textVms1534[1],
                        textColors1D20[color],
                        shadowColors1D30[color],
                        " ");
                }

                g_FrontMessageAnm->SetAndExecuteScriptIdx(
                    &textVms1534[line], line);
                unsigned char glyph = (unsigned char)glyphSize1D54;
                reinterpret_cast<unsigned char *>(&textVms1534[line])[0x299] =
                    glyph;
                reinterpret_cast<unsigned char *>(&textVms1534[line])[0x298] =
                    glyph;

                DecodeFrontMessageString(
                    reinterpret_cast<unsigned char *>(textA),
                    reinterpret_cast<unsigned char *>(current04) + 8);
                g_AnmManager->DrawTextLeft(
                    &textVms1534[line],
                    textColors1D20[color],
                    shadowColors1D30[color],
                    textA);
                framesElapsed1D4C = 0;
                break;
            }

            case 4:
                if (skippable1D6C && g_FrontMessageInput.IsHeld(0x100))
                    break;

                if (g_FrontMessageInput.WasPressed(1) &&
                    framesElapsed1D4C >= waitThreshold1D50)
                {
                    resetText1D65 = 1;
                    waitThreshold1D50 = 8;
                }
                else
                {
                    if (framesElapsed1D4C < current04->argument04)
                    {
                        ++framesElapsed1D4C;
                        goto run_scripts;
                    }

                    resetText1D65 = 1;
                    waitThreshold1D50 = 30;
                }
                break;

            case 5:
            {
                int portrait =
                    *reinterpret_cast<short *>(&current04->argument04);
                unsigned char interrupt =
                    *(reinterpret_cast<unsigned char *>(&current04->argument04) + 2);
                FrontMessagePortraitVm(this, portrait, 0)->pendingInterrupt =
                    interrupt;
                FrontMessagePortraitVm(this, portrait, 1)->pendingInterrupt =
                    interrupt;
                break;
            }

            case 6:
                ++ignoreWait1D68;
                break;

            case 7:
                if (current04->argument04 < 0)
                {
                    g_Supervisor.StopAudio();
                }
                else
                {
                    if (g_GameManager.IsGameMode2())
                        g_Supervisor.PlayMusic(
                            g_FrontMessageSource->musicId0C, 0);
                    else
                        g_Supervisor.PlayMusic(current04->argument04, 0);
                }
                break;

            case 8:
            {
                FrontMessageSideView *side = &g_GameManager.sides[1];
                int script = side->player04->tableValue303F0 + 4;
                side->frontSide18->auxA678.sideAnm00
                    ->SetAndExecuteScriptIdx(&introVm1A7C, script);
                framesElapsed1D4C = 0;
                break;
            }

            case 9:
                g_FrontMessageOwner->InitializeResultBonus();
                // fall through
            case 0x0C:
            {
                g_Supervisor.FadeOutMusic(4.0f);
                break;
            }

            case 0x0A:
                goto run_scripts;

            case 0x0B:
                if (g_GameManager.IsGameMode0())
                {
                    if (mode10)
                        g_FrontMode0Done = 1;
                    else
                        g_StageTransitionState = 3;
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

            case 0x0D:
                skippable1D6C =
                    *reinterpret_cast<unsigned char *>(&current04->argument04);
                break;

            case 0x0E:
                g_FrontMessageScreenEffect = ScreenEffect::RegisterChain(
                    SCREEN_EFFECT_FULL_FADE_HOLD,
                    300, 0xFFFFFF, 0, 0, 35, 2);
                g_ScreenTransitionCountdown = 302;
                break;

            case 0x0F:
            {
                int requestedSide = current04->argument04;
                if (requestedSide < 0)
                {
                    portraits14[0].first00.pendingInterrupt = 4;
                    portraits14[1].first00.pendingInterrupt = 4;
                    portraits14[0].second2A4.pendingInterrupt = 4;
                    portraits14[1].second2A4.pendingInterrupt = 4;
                }
                else
                {
                    int side = mode10 ? 1 - requestedSide : requestedSide;
                    if (currentPortrait1D67 != side)
                    {
                        for (int i = 0; i < 4; ++i)
                        {
                            if (i != side)
                            {
                                portraits14[i].first00.pendingInterrupt = 4;
                                portraits14[i].second2A4.pendingInterrupt = 4;
                            }
                        }
                    }

                    portraits14[side].first00.pendingInterrupt = 3;
                    portraits14[side].second2A4.pendingInterrupt = 3;
                    currentPortrait1D67 = (unsigned char)side;
                    colorIndex1D64 = (unsigned char)side;

                    if (current04->argument08 >= 0)
                    {
                        FrontMessageSideView *displaySide =
                            &g_GameManager.sides[mode10];
                        int sprite =
                            displaySide->player04->tableValue303FC +
                            current04->argument08;
                        displaySide->frontSide18->auxA678.sideAnm00->SetSprite(
                            &portraits14[mode10].first00, sprite);
                        displaySide->frontSide18->auxA678.sideAnm00->SetSprite(
                            &portraits14[mode10].second2A4, sprite + 9);
                    }

                    if (current04->argument0C >= 0)
                    {
                        int other = 1 - mode10;
                        FrontMessageSideView *displaySide =
                            &g_GameManager.sides[other];
                        int sprite =
                            displaySide->player04->tableValue303FC +
                            current04->argument0C;
                        displaySide->frontSide18->auxA678.sideAnm00->SetSprite(
                            &portraits14[other].first00, sprite);
                        displaySide->frontSide18->auxA678.sideAnm00->SetSprite(
                            &portraits14[other].second2A4, sprite + 9);
                    }
                }

                resetText1D65 = 1;
                break;
            }

            case 0x10:
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
                {
                    unsigned char glyph = (unsigned char)glyphSize1D54;
                    reinterpret_cast<unsigned char *>(
                        &textVms1534[textLineIndex1D66])[0x299] = glyph;
                    reinterpret_cast<unsigned char *>(
                        &textVms1534[textLineIndex1D66])[0x298] = glyph;
                }

                DecodeFrontMessageString(
                    reinterpret_cast<unsigned char *>(textB),
                    reinterpret_cast<unsigned char *>(current04) + 8);
                g_AnmManager->DrawTextLeft(
                    &textVms1534[textLineIndex1D66],
                    textColors1D20[colorIndex1D64],
                    shadowColors1D30[colorIndex1D64],
                    textB);
                framesElapsed1D4C = 0;
                resetText1D65 = 0;
                ++textLineIndex1D66;
                break;

            case 0x11:
            {
                int requested = current04->argument04;
                int side;
                if (requested >= 2)
                    side = requested;
                else if (!mode10)
                    side = requested;
                else
                    side = 1 - requested;

                if (currentPortrait1D67 != side)
                {
                    for (int i = 0; i < 4; ++i)
                    {
                        if (i != side)
                        {
                            portraits14[i].first00.pendingInterrupt = 4;
                            portraits14[i].second2A4.pendingInterrupt = 4;
                        }
                    }
                }

                portraits14[side].first00.pendingInterrupt = 3;
                portraits14[side].second2A4.pendingInterrupt = 3;
                currentPortrait1D67 = (unsigned char)side;

                if (current04->argument08 >= 0)
                {
                    AnmLoaded *anm;
                    int secondSprite;
                    if (side >= 2)
                    {
                        anm = g_GameManager.sides[0].frontSide18
                            ->auxA678.optionalAnm04;
                        anm->SetSprite(
                            &portraits14[side].first00,
                            current04->argument08);
                        secondSprite = current04->argument08 + 6;
                    }
                    else
                    {
                        FrontMessageSideView *displaySide =
                            &g_GameManager.sides[side];
                        int sprite =
                            displaySide->player04->tableValue303FC +
                            current04->argument08;
                        anm = displaySide->frontSide18->auxA678.sideAnm00;
                        anm->SetSprite(
                            &portraits14[side].first00, sprite);
                        secondSprite = sprite + 9;
                    }

                    anm->SetSprite(
                        &portraits14[side].second2A4, secondSprite);
                }

                colorIndex1D64 = (unsigned char)side;
                resetText1D65 = 1;
                break;
            }

            case 0x12:
                textBoxVisible1D6D =
                    *reinterpret_cast<unsigned char *>(&current04->argument04);
                break;

            case 0x17:
            {
                int side = current04->argument04;
                g_GameManager.sides[side].player04->SetUpdateState(1);
                g_GameManager.sides[side].player04->stateTimer303C8 = 0;
                break;
            }

            case 0x18:
                secondaryTimer1D58 = 1;
                break;

            case 0x19:
                g_GameManager.sides[0].frontSide18->LoadScript67();
                g_GameManager.sides[1].frontSide18->LoadScript67();
                break;

            case 0x1A:
                resetText1D65 = 1;
                colorIndex1D64 =
                    *reinterpret_cast<unsigned char *>(&current04->argument04);
                break;

            case 0x1C:
                g_GameManager.sides[0].state00->value18 =
                    current04->argument04;
                g_GameManager.sides[1].state00->value18 =
                    current04->argument04;
                break;

            default:
                break;
            }

            FrontMessageInstructionView *next =
                reinterpret_cast<FrontMessageInstructionView *>(
                    reinterpret_cast<unsigned char *>(&current04->argument04) +
                    current04->size03);
            current04 = next;
        }
        while (timer1D40 >= static_cast<int>(current04->time00));

        timer1D40++;
    }

run_scripts:
    g_AnmManager->ExecuteScript(&portraits14[0].first00);
    g_AnmManager->ExecuteScript(&portraits14[1].first00);
    g_AnmManager->ExecuteScript(&portraits14[0].second2A4);
    g_AnmManager->ExecuteScript(&portraits14[1].second2A4);
    g_AnmManager->ExecuteScript(&portraits14[2].first00);
    g_AnmManager->ExecuteScript(&portraits14[3].first00);
    g_AnmManager->ExecuteScript(&portraits14[2].second2A4);
    g_AnmManager->ExecuteScript(&portraits14[3].second2A4);
    g_AnmManager->ExecuteScript(&textVms1534[0]);
    g_AnmManager->ExecuteScript(&textVms1534[1]);
    g_AnmManager->ExecuteScript(&introVm1A7C);

    if (timer1D40 < 60 && skippable1D6C &&
        g_FrontMessageInput.IsHeld(0x100))
    {
        timer1D40 = 60;
    }

    if (secondaryTimer1D58 >= 1)
    {
        if (secondaryTimer1D58 < 60 && skippable1D6C &&
            g_FrontMessageInput.IsHeld(0x100))
        {
            timer1D40 = 60;
        }
        secondaryTimer1D58++;
    }

    return 0;
}
