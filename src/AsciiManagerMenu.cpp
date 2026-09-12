#include "AnmManager.hpp"
#include "Chain.hpp"
#include "AsciiManager.hpp"
#include "Supervisor.hpp"

#include <windows.h>
#include <mmsystem.h>

struct AsciiInputView
{
    unsigned short WasPressed(unsigned int buttons);
};

struct AsciiPlayerPopupPositionView
{
    unsigned char unknown0000[0x1B88];
    Float3 position;
};

struct AsciiGameSideRuntimeView
{
    float value00;
    int value04;
    int value08;
    int value0C;
    unsigned char unknown10[4];
    unsigned char counter14;
};

struct AsciiGameManagerSideView
{
    unsigned char unknown00[4];
    AsciiPlayerPopupPositionView *player;
    unsigned char unknown08[0x14];
    AsciiGameSideRuntimeView *runtime;
    unsigned char unknown20[0x14];
    unsigned int flags;
};

typedef char AsciiGameManagerSideViewSizeIs38[
    (sizeof(AsciiGameManagerSideView) == 0x38) ? 1 : -1];

struct AsciiGameManagerView
{
    AsciiGameManagerSideView sides[2];
    unsigned char unknown070[0x8C];
    int field0FC;
    unsigned char unknown100[8];
    int field108;
    int field10C;
    unsigned char unknown110[8];
    int gameMode;
    int difficulty;
    unsigned char unknown120[0x14];
    unsigned int flags;
    unsigned char unknown138[4];
    unsigned char inGameMenu;
    unsigned char menuState97C8Active;
    unsigned char menuStateA7A8Active;
    unsigned char unknown13F;

    float TransformPopupX(float value);
    float TransformPopupY(float value);
    int IsGameMode1();
    int IsReplayNeutral();
    int HasFlagBit0();
};

typedef char AsciiGameManagerSidePlayerAt04[
    (offsetof(AsciiGameManagerSideView, player) == 0x04) ? 1 : -1];
typedef char AsciiGameManagerSideRuntimeAt1C[
    (offsetof(AsciiGameManagerSideView, runtime) == 0x1C) ? 1 : -1];
typedef char AsciiGameManagerField0FC[
    (offsetof(AsciiGameManagerView, field0FC) == 0x0FC) ? 1 : -1];
typedef char AsciiGameManagerGameModeAt118[
    (offsetof(AsciiGameManagerView, gameMode) == 0x118) ? 1 : -1];
typedef char AsciiGameManagerDifficultyAt11C[
    (offsetof(AsciiGameManagerView, difficulty) == 0x11C) ? 1 : -1];
typedef char AsciiGameManagerFlagsAt134[
    (offsetof(AsciiGameManagerView, flags) == 0x134) ? 1 : -1];
typedef char AsciiGameManagerMenuAt13C[
    (offsetof(AsciiGameManagerView, inGameMenu) == 0x13C) ? 1 : -1];

struct AsciiSoundPlayerView
{
    void PlaySoundByIdx(int soundIndex, int pan);
    void ResumeAfterPause();
};

struct AsciiSupervisorView
{
    unsigned char unknown000[0x590];
    int state590;
    unsigned char unknown594[0x10];
    int field5A4;
    unsigned char unknown5A8[0x10];
    float frameScalar5B8;
    unsigned char unknown5BC[0x18];
    unsigned int flags5D4;
    unsigned char unknown5D8[0x1C4];
    unsigned int systemTime79C;

    int IsFogDisabled();
    int DisableFog();
    int SetRenderState(int state, int value);
    int PrepareResultScreen();
    int FinalizeResultScreen();
};

typedef char AsciiSupervisorField5A4[
    (offsetof(AsciiSupervisorView, field5A4) == 0x5A4) ? 1 : -1];
typedef char AsciiSupervisorFrameScalarAt5B8[
    (offsetof(AsciiSupervisorView, frameScalar5B8) == 0x5B8) ? 1 : -1];
typedef char AsciiSupervisorFlagsAt5D4[
    (offsetof(AsciiSupervisorView, flags5D4) == 0x5D4) ? 1 : -1];
typedef char AsciiSupervisorSystemTimeAt79C[
    (offsetof(AsciiSupervisorView, systemTime79C) == 0x79C) ? 1 : -1];

extern AsciiInputView g_AsciiInput;
extern AsciiGameManagerView g_GameManager;
extern AsciiSoundPlayerView g_SoundPlayer;
extern AsciiManager g_AsciiManager;
extern unsigned char *g_AsciiMenuConfig;
extern Chain g_Chain;
extern ChainElem g_AsciiManagerCalcChain;
extern ChainElem g_AsciiManagerDrawChainLowPrio;
extern ChainElem g_AsciiManagerDrawChainHighPrio;

struct GameManagerSetupLayout
{
    static void CleanupGameplayState();
};

struct AsciiAnmLoadedSpriteView
{
    unsigned char unknown00[0x34];
    float widthPx;
};

typedef char AsciiAnmLoadedSpriteWidthAt34[
    (offsetof(AsciiAnmLoadedSpriteView, widthPx) == 0x34) ? 1 : -1];

static inline AsciiSupervisorView *AsciiSupervisor()
{
    return reinterpret_cast<AsciiSupervisorView *>(&g_Supervisor);
}

enum PauseMenuState
{
    PAUSE_MENU_STATE_INIT = 0,
    PAUSE_MENU_STATE_PRIMARY_SELECTED = 1,
    PAUSE_MENU_STATE_SECONDARY_SELECTED = 2,
    PAUSE_MENU_STATE_CLOSING = 3,
    PAUSE_MENU_STATE_CONFIRM_A_YES = 4,
    PAUSE_MENU_STATE_CONFIRM_A_NO = 5,
    PAUSE_MENU_STATE_CONFIRM_B_YES = 6,
    PAUSE_MENU_STATE_CONFIRM_B_NO = 7,
    PAUSE_MENU_STATE_ACTION_A = 8,
    PAUSE_MENU_STATE_ACTION_B = 9,
};

enum
{
    ASCII_INTERRUPT_SHOW = 1,
    ASCII_INTERRUPT_HIDE = 2,
    PAUSE_SPRITE_HEADER = 0,
    PAUSE_SPRITE_PRIMARY = 1,
    PAUSE_SPRITE_SECONDARY = 2,
    PAUSE_SPRITE_CONFIRM = 3,
    PAUSE_SPRITE_YES = 4,
    PAUSE_SPRITE_NO = 5,
    PAUSE_SPRITE_DIFFICULTY = 6,
};

static const unsigned int COLOR_WHITE = 0xFFFFFFFFu;
static const unsigned int COLOR_MENU_ITEM_SELECTED = 0xFFFF8080u;
static const unsigned int COLOR_MENU_ITEM_NORMAL = 0xFF505050u;

int AnmVm::IsVisible()
{
    return this->flagsWord & 1;
}

void AnmVm::SetInvisible()
{
    this->flagsWord &= ~1u;
}

int AsciiGameManagerView::HasFlagBit0()
{
    return this->flags & 1;
}

int PauseMenu::OnUpdate()
{
    int i;

    if (g_AsciiInput.WasPressed(8) && this->state != PAUSE_MENU_STATE_CLOSING) {
        g_SoundPlayer.PlaySoundByIdx(10, 0);
        this->state = PAUSE_MENU_STATE_CLOSING;
        for (i = 0; i < 7; i++) {
            if (this->menuSprites[i].IsVisible()) {
                this->menuSprites[i].pendingInterrupt = ASCII_INTERRUPT_HIDE;
            }
        }
        this->frames = 0;
        this->menuBackground.pendingInterrupt = ASCII_INTERRUPT_SHOW;
    }

    if (g_AsciiInput.WasPressed(0x200) && this->state != PAUSE_MENU_STATE_ACTION_A) {
        g_SoundPlayer.PlaySoundByIdx(10, 0);
        this->state = PAUSE_MENU_STATE_ACTION_A;
        for (i = 0; i < 7; i++) {
            if (this->menuSprites[i].IsVisible()) {
                this->menuSprites[i].pendingInterrupt = ASCII_INTERRUPT_HIDE;
            }
        }
        this->frames = 0;
    }

    switch (this->state) {
    case PAUSE_MENU_STATE_INIT:
        for (i = 0; i < 7; i++) {
            g_AsciiManager.asciiAnm->ExecuteAnmIdx(&this->menuSprites[i], i + 1);
        }
        for (i = 0; i < 3; i++) {
            this->menuSprites[i].pendingInterrupt = ASCII_INTERRUPT_SHOW;
        }
        g_AsciiManager.asciiAnm->SetSprite(
            &this->menuSprites[PAUSE_SPRITE_DIFFICULTY], g_GameManager.difficulty + 125);
        this->state++;
        this->frames = 0;
        // fallthrough
    case PAUSE_MENU_STATE_PRIMARY_SELECTED:
        this->menuSprites[PAUSE_SPRITE_PRIMARY].color1 = COLOR_WHITE;
        this->menuSprites[PAUSE_SPRITE_SECONDARY].color1 = COLOR_MENU_ITEM_NORMAL;
        this->menuSprites[PAUSE_SPRITE_PRIMARY].pos2 = Float3(-4.0f, -4.0f, 0.0f);
        this->menuSprites[PAUSE_SPRITE_SECONDARY].pos2 = Float3(0.0f, 0.0f, 0.0f);

        if (this->frames >= 4) {
            if (!g_GameManager.IsReplayNeutral()) {
                if (g_AsciiInput.WasPressed(0x10)) {
                    this->state = PAUSE_MENU_STATE_SECONDARY_SELECTED;
                    g_SoundPlayer.PlaySoundByIdx(0, 0);
                }
            } else if (g_AsciiInput.WasPressed(0x10)) {
                this->state = PAUSE_MENU_STATE_SECONDARY_SELECTED;
                g_SoundPlayer.PlaySoundByIdx(0, 0);
            }

            if (g_AsciiInput.WasPressed(0x20)) {
                this->state = PAUSE_MENU_STATE_SECONDARY_SELECTED;
                g_SoundPlayer.PlaySoundByIdx(0, 0);
            }

            if (g_AsciiInput.WasPressed(0x1001)) {
                g_SoundPlayer.PlaySoundByIdx(10, 0);
                for (i = 0; i < 3; i++) {
                    this->menuSprites[i].pendingInterrupt = ASCII_INTERRUPT_HIDE;
                }
                this->state = PAUSE_MENU_STATE_CLOSING;
                this->frames = 0;
                this->menuBackground.pendingInterrupt = ASCII_INTERRUPT_SHOW;
            }
        }
        break;

    case PAUSE_MENU_STATE_SECONDARY_SELECTED:
        this->menuSprites[PAUSE_SPRITE_PRIMARY].color1 = COLOR_MENU_ITEM_NORMAL;
        this->menuSprites[PAUSE_SPRITE_SECONDARY].color1 = COLOR_WHITE;
        this->menuSprites[PAUSE_SPRITE_PRIMARY].pos2 = Float3(0.0f, 0.0f, 0.0f);
        this->menuSprites[PAUSE_SPRITE_SECONDARY].pos2 = Float3(-4.0f, -4.0f, 0.0f);

        if (this->frames >= 4) {
            if (g_AsciiInput.WasPressed(0x10)) {
                this->state = PAUSE_MENU_STATE_PRIMARY_SELECTED;
                g_SoundPlayer.PlaySoundByIdx(0, 0);
            }

            if (g_GameManager.IsReplayNeutral()) {
                if (g_AsciiInput.WasPressed(0x20)) {
                    this->state = PAUSE_MENU_STATE_PRIMARY_SELECTED;
                    g_SoundPlayer.PlaySoundByIdx(0, 0);
                }
            } else if (g_AsciiInput.WasPressed(0x20)) {
                this->state = PAUSE_MENU_STATE_PRIMARY_SELECTED;
                g_SoundPlayer.PlaySoundByIdx(0, 0);
            }

            if (g_AsciiInput.WasPressed(0x1001)) {
                g_SoundPlayer.PlaySoundByIdx(10, 0);
                for (i = 0; i < 3; i++) {
                    this->menuSprites[i].pendingInterrupt = ASCII_INTERRUPT_HIDE;
                }
                for (; i < 6; i++) {
                    this->menuSprites[i].pendingInterrupt = ASCII_INTERRUPT_SHOW;
                }
                this->state = PAUSE_MENU_STATE_CONFIRM_A_NO;
                this->frames = 0;
            }
        }
        break;

    case PAUSE_MENU_STATE_CLOSING:
        if (this->frames >= 20) {
            this->state = PAUSE_MENU_STATE_INIT;
            reinterpret_cast<unsigned char *>(&g_GameManager)[0x13C] = 0;
            for (i = 0; i < 7; i++) {
                this->menuSprites[i].SetInvisible();
            }
            g_SoundPlayer.ResumeAfterPause();
            AsciiSupervisor()->systemTime79C = timeGetTime();
        }
        break;

    case PAUSE_MENU_STATE_CONFIRM_A_YES:
    case PAUSE_MENU_STATE_CONFIRM_B_YES:
        this->menuSprites[PAUSE_SPRITE_YES].color1 = COLOR_MENU_ITEM_SELECTED;
        this->menuSprites[PAUSE_SPRITE_NO].color1 = COLOR_MENU_ITEM_NORMAL;
        this->menuSprites[PAUSE_SPRITE_YES].pos2 = Float3(-4.0f, -4.0f, 0.0f);
        this->menuSprites[PAUSE_SPRITE_NO].pos2 = Float3(0.0f, 0.0f, 0.0f);

        if (this->frames >= 4) {
            if (g_AsciiInput.WasPressed(0x10) || g_AsciiInput.WasPressed(0x20)) {
                this->state = this->state == PAUSE_MENU_STATE_CONFIRM_A_YES
                                  ? PAUSE_MENU_STATE_CONFIRM_A_NO
                                  : PAUSE_MENU_STATE_CONFIRM_B_NO;
                g_SoundPlayer.PlaySoundByIdx(0, 0);
            }

            if (g_AsciiInput.WasPressed(0x1001)) {
                g_SoundPlayer.PlaySoundByIdx(10, 0);
                for (i = 3; i < 6; i++) {
                    this->menuSprites[i].pendingInterrupt = ASCII_INTERRUPT_HIDE;
                }
                this->state = this->state == PAUSE_MENU_STATE_CONFIRM_A_YES
                                  ? PAUSE_MENU_STATE_ACTION_A
                                  : PAUSE_MENU_STATE_ACTION_B;
                this->frames = 0;
            }
        }
        break;

    case PAUSE_MENU_STATE_CONFIRM_A_NO:
    case PAUSE_MENU_STATE_CONFIRM_B_NO:
        this->menuSprites[PAUSE_SPRITE_YES].color1 = COLOR_MENU_ITEM_NORMAL;
        this->menuSprites[PAUSE_SPRITE_NO].color1 = COLOR_MENU_ITEM_SELECTED;
        this->menuSprites[PAUSE_SPRITE_YES].pos2 = Float3(0.0f, 0.0f, 0.0f);
        this->menuSprites[PAUSE_SPRITE_NO].pos2 = Float3(-4.0f, -4.0f, 0.0f);

        if (this->frames >= 4) {
            if (g_AsciiInput.WasPressed(0x10) || g_AsciiInput.WasPressed(0x20)) {
                this->state = this->state == PAUSE_MENU_STATE_CONFIRM_A_NO
                                  ? PAUSE_MENU_STATE_CONFIRM_A_YES
                                  : PAUSE_MENU_STATE_CONFIRM_B_YES;
                g_SoundPlayer.PlaySoundByIdx(0, 0);
            }

            if (g_AsciiInput.WasPressed(0x1001)) {
                g_SoundPlayer.PlaySoundByIdx(10, 0);
                for (i = 0; i < 3; i++) {
                    this->menuSprites[i].pendingInterrupt = ASCII_INTERRUPT_SHOW;
                }
                for (; i < 6; i++) {
                    this->menuSprites[i].pendingInterrupt = ASCII_INTERRUPT_HIDE;
                }
                if (this->state == PAUSE_MENU_STATE_CONFIRM_A_NO) {
                    this->state = PAUSE_MENU_STATE_SECONDARY_SELECTED;
                }
                this->frames = 0;
            }
        }
        break;

    case PAUSE_MENU_STATE_ACTION_A:
        if (this->frames >= 20) {
            this->state = PAUSE_MENU_STATE_INIT;
            AsciiSupervisor()->state590 = 1;
            reinterpret_cast<unsigned char *>(&g_GameManager)[0x13C] = 0;
            AsciiSupervisor()->systemTime79C = timeGetTime();
        }
        break;

    case PAUSE_MENU_STATE_ACTION_B:
        if (this->frames >= 20) {
            if (!g_GameManager.HasFlagBit0() && g_GameManager.difficulty != 4) {
                this->state = PAUSE_MENU_STATE_INIT;
                AsciiSupervisor()->state590 = 10;
                reinterpret_cast<unsigned char *>(&g_GameManager)[0x13C] = 0;
                AsciiSupervisor()->systemTime79C = timeGetTime();
                break;
            }

            AsciiSupervisor()->PrepareResultScreen();
            AsciiSupervisor()->state590 = 11;
            reinterpret_cast<unsigned char *>(&g_GameManager)[0x13C] = 0;
            AsciiSupervisor()->systemTime79C = timeGetTime();
            return 0;
        }
        break;
    }

    for (i = 0; i < 7; i++) {
        g_AnmManager->ExecuteScript(&this->menuSprites[i]);
    }
    if (AsciiSupervisor()->flags5D4 & 2) {
        g_AnmManager->ExecuteScript(&this->menuBackground);
    }
    this->frames++;
    return 0;
}

int AsciiMenuState5::OnUpdate()
{
    unsigned int i;

    if (g_GameManager.IsReplayNeutral()) {
        g_GameManager.menuState97C8Active = 0;
        AsciiSupervisor()->state590 = 1;
        return 1;
    }

    if (g_GameManager.sides[0].runtime->counter14 >= 3) {
        g_GameManager.menuState97C8Active = 0;
        AsciiSupervisor()->state590 = 6;
        return 1;
    }

    switch (this->state) {
    case 0:
        if (!g_GameManager.IsGameMode1()) {
            for (i = 0; i < 5; i++) {
                g_AsciiManager.asciiAnm->ExecuteAnmIdx(&this->menuSprites[i], i + 8);
            }
            for (i = 0; i < 5; i++) {
                this->menuSprites[i].pendingInterrupt = ASCII_INTERRUPT_SHOW;
            }
            g_AsciiManager.asciiAnm->SetSprite(
                &this->menuSprites[4],
                120 - g_GameManager.sides[0].runtime->counter14);
        } else {
            for (i = 0; i < 3; i++) {
                g_AsciiManager.asciiAnm->ExecuteAnmIdx(&this->menuSprites[i], i + 17);
            }
            for (i = 0; i < 3; i++) {
                this->menuSprites[i].pendingInterrupt = ASCII_INTERRUPT_SHOW;
            }
        }
        this->state++;
        this->frames = 0;
        // fallthrough

    case 1:
        this->menuSprites[1].color1 = COLOR_MENU_ITEM_NORMAL;
        this->menuSprites[2].color1 = COLOR_MENU_ITEM_SELECTED;
        this->menuSprites[1].pos2 = Float3(0.0f, 0.0f, 0.0f);
        this->menuSprites[2].pos2 = Float3(-4.0f, -4.0f, 0.0f);

        if (this->frames >= 4) {
            if (g_AsciiInput.WasPressed(0x10) || g_AsciiInput.WasPressed(0x20)) {
                this->state = 2;
                g_SoundPlayer.PlaySoundByIdx(0, 0);
            }
            if (g_AsciiInput.WasPressed(0x1001)) {
                g_SoundPlayer.PlaySoundByIdx(10, 0);
                for (i = 0; i < 5; i++) {
                    this->menuSprites[i].pendingInterrupt = ASCII_INTERRUPT_SHOW;
                }
                this->state = 4;
                this->frames = 0;
            }
        }
        break;

    case 2:
        this->menuSprites[2].color1 = COLOR_MENU_ITEM_NORMAL;
        this->menuSprites[1].color1 = COLOR_MENU_ITEM_SELECTED;
        this->menuSprites[2].pos2 = Float3(0.0f, 0.0f, 0.0f);
        this->menuSprites[1].pos2 = Float3(-4.0f, -4.0f, 0.0f);

        if (this->frames >= 4) {
            if (g_AsciiInput.WasPressed(0x10) || g_AsciiInput.WasPressed(0x20)) {
                this->state = 1;
                g_SoundPlayer.PlaySoundByIdx(0, 0);
            }
            if (g_AsciiInput.WasPressed(0x1001)) {
                g_SoundPlayer.PlaySoundByIdx(10, 0);
                for (i = 0; i < 5; i++) {
                    this->menuSprites[i].pendingInterrupt = ASCII_INTERRUPT_SHOW;
                }
                this->state = 3;
                this->frames = 0;
            }
        }
        break;

    case 3:
        if (this->frames >= 20) {
            this->state = 0;
            this->frames = 0;
            g_GameManager.menuState97C8Active = 0;

            if (g_GameManager.IsGameMode1()) {
                AsciiSupervisor()->state590 = 13;
                g_GameManager.field0FC = 0;
                AsciiSupervisor()->systemTime79C = timeGetTime();
                return 1;
            }

            for (i = 0; i < 5; i++) {
                this->menuSprites[i].SetInvisible();
            }
            g_GameManager.sides[0].runtime->counter14++;
            g_GameManager.flags |= 0x2000u;
            g_GameManager.sides[0].runtime->value04 = 0;
            g_GameManager.sides[0].runtime->value08 = 0;
            g_GameManager.sides[0].runtime->value0C = 0;
            g_GameManager.sides[1].runtime->value04 = 0;
            g_GameManager.sides[1].runtime->value08 = 0;
            g_GameManager.sides[1].runtime->value0C = 0;
            AsciiSupervisor()->field5A4 = 8;
            AsciiSupervisor()->systemTime79C = timeGetTime();
            AsciiSupervisor()->PrepareResultScreen();
            AsciiSupervisor()->FinalizeResultScreen();
            GameManagerSetupLayout::CleanupGameplayState();
            g_GameManager.field108 = 0;
            g_GameManager.field10C = 0;
            g_GameManager.sides[0].runtime->value00 =
                (float)g_AsciiMenuConfig[0xAC] + 2.0f;
            g_GameManager.flags |= 0x4000u;
            return 1;
        }
        break;

    case 4:
        if (this->frames >= 20) {
            this->state = 0;
            this->frames = 0;
            g_GameManager.menuState97C8Active = 0;
            AsciiSupervisor()->state590 = 6;
            for (i = 0; i < 5; i++) {
                this->menuSprites[i].SetInvisible();
            }
            AsciiSupervisor()->systemTime79C = timeGetTime();
            return 1;
        }
        break;
    }

    for (i = 0; i < 5; i++) {
        g_AnmManager->ExecuteScript(&this->menuSprites[i]);
    }
    if (AsciiSupervisor()->flags5D4 & 2) {
        g_AnmManager->ExecuteScript(&this->menuBackground);
    }
    this->frames++;
    return 0;
}

int AsciiMenuState4::OnUpdate()
{
    unsigned int i;

    if (g_GameManager.IsReplayNeutral()) {
        g_GameManager.menuStateA7A8Active = 0;
        AsciiSupervisor()->state590 = 1;
        return 1;
    }

    switch (this->state) {
    case 0:
        for (i = 0; i < 3; i++) {
            g_AsciiManager.asciiAnm->ExecuteAnmIdx(&this->menuSprites[i], i + 13);
            this->menuSprites[i].pendingInterrupt = ASCII_INTERRUPT_SHOW;
        }
        this->state++;
        this->frames = 0;
        // fallthrough

    case 1:
        this->menuSprites[0].color1 = COLOR_WHITE;
        this->menuSprites[2].color1 = COLOR_MENU_ITEM_NORMAL;
        this->menuSprites[1].color1 = COLOR_MENU_ITEM_NORMAL;
        this->menuSprites[0].pos2 = Float3(-4.0f, -4.0f, 0.0f);
        this->menuSprites[2].pos2 = Float3(0.0f, 0.0f, 0.0f);
        this->menuSprites[1].pos2 = this->menuSprites[2].pos2;

        if (this->frames >= 4) {
            if (g_AsciiInput.WasPressed(0x10)) {
                this->state = 3;
                g_SoundPlayer.PlaySoundByIdx(0, 0);
            }
            if (g_AsciiInput.WasPressed(0x20)) {
                this->state = 2;
                g_SoundPlayer.PlaySoundByIdx(0, 0);
            }
            if (g_AsciiInput.WasPressed(0x1001)) {
                g_SoundPlayer.PlaySoundByIdx(10, 0);
                for (i = 0; i < 3; i++) {
                    this->menuSprites[i].pendingInterrupt = ASCII_INTERRUPT_HIDE;
                }
                this->state = 4;
                this->frames = 0;
            }
        }
        break;

    case 2:
        this->menuSprites[2].color1 = COLOR_MENU_ITEM_NORMAL;
        this->menuSprites[0].color1 = COLOR_MENU_ITEM_NORMAL;
        this->menuSprites[1].color1 = COLOR_WHITE;
        this->menuSprites[2].pos2 = Float3(0.0f, 0.0f, 0.0f);
        this->menuSprites[0].pos2 = this->menuSprites[2].pos2;
        this->menuSprites[1].pos2 = Float3(-4.0f, -4.0f, 0.0f);

        if (this->frames >= 4) {
            if (g_AsciiInput.WasPressed(0x10)) {
                this->state = 1;
                g_SoundPlayer.PlaySoundByIdx(0, 0);
            }
            if (g_AsciiInput.WasPressed(0x20)) {
                this->state = 3;
                g_SoundPlayer.PlaySoundByIdx(0, 0);
            }
            if (g_AsciiInput.WasPressed(0x1001)) {
                g_SoundPlayer.PlaySoundByIdx(10, 0);
                for (i = 0; i < 3; i++) {
                    this->menuSprites[i].pendingInterrupt = ASCII_INTERRUPT_HIDE;
                }
                this->state = 5;
                this->frames = 0;
            }
        }
        break;

    case 3:
        this->menuSprites[1].color1 = COLOR_MENU_ITEM_NORMAL;
        this->menuSprites[0].color1 = COLOR_MENU_ITEM_NORMAL;
        this->menuSprites[2].color1 = COLOR_WHITE;
        this->menuSprites[1].pos2 = Float3(0.0f, 0.0f, 0.0f);
        this->menuSprites[0].pos2 = this->menuSprites[1].pos2;
        this->menuSprites[2].pos2 = Float3(-4.0f, -4.0f, 0.0f);

        if (this->frames >= 4) {
            if (g_AsciiInput.WasPressed(0x10)) {
                this->state = 2;
                g_SoundPlayer.PlaySoundByIdx(0, 0);
            }
            if (g_AsciiInput.WasPressed(0x20)) {
                this->state = 1;
                g_SoundPlayer.PlaySoundByIdx(0, 0);
            }
            if (g_AsciiInput.WasPressed(0x1001)) {
                g_SoundPlayer.PlaySoundByIdx(10, 0);
                for (i = 0; i < 3; i++) {
                    this->menuSprites[i].pendingInterrupt = ASCII_INTERRUPT_HIDE;
                }
                this->state = 6;
                this->frames = 0;
            }
        }
        break;

    case 5:
        if (this->frames >= 20) {
            this->state = 0;
            AsciiSupervisor()->state590 = 1;
            g_GameManager.menuStateA7A8Active = 0;
            AsciiSupervisor()->systemTime79C = timeGetTime();
            return 1;
        }
        break;

    case 4:
        if (this->frames >= 20) {
            this->state = 0;
            AsciiSupervisor()->state590 = 10;
            g_GameManager.menuStateA7A8Active = 0;
            AsciiSupervisor()->systemTime79C = timeGetTime();
            return 1;
        }
        break;

    case 6:
        if (this->frames >= 20) {
            this->state = 0;
            AsciiSupervisor()->state590 = 6;
            g_GameManager.menuStateA7A8Active = 0;
            AsciiSupervisor()->systemTime79C = timeGetTime();
            return 1;
        }
        break;
    }

    for (i = 0; i < 3; i++) {
        g_AnmManager->ExecuteScript(&this->menuSprites[i]);
    }
    this->frames++;
    return 0;
}

int AsciiManager::OnUpdate(AsciiManager *ascii)
{
    AsciiManagerPopup *popup;
    AsciiManagerPopup *popupBank;
    unsigned char *sideFlags;
    int popupIndex;

    if (reinterpret_cast<unsigned char *>(&g_GameManager)[0x13C] == 0) {
        if (reinterpret_cast<unsigned char *>(&g_GameManager)[0x13D] == 0 &&
            (g_GameManager.flags & 0x1800u) == 0) {
            popupBank = &ascii->scorePopups[0][0];
            for (sideFlags = reinterpret_cast<unsigned char *>(&g_GameManager.sides[0].flags);
                 sideFlags < reinterpret_cast<unsigned char *>(&g_GameManager.sides[2].flags);
                 popupBank += 100, sideFlags += sizeof(AsciiGameManagerSideView)) {
                if ((*reinterpret_cast<unsigned int *>(sideFlags) & 1u) != 0) {
                    continue;
                }

                popup = popupBank;
                for (popupIndex = 0; popupIndex < 100; popupIndex++, popup++) {
                    if (!popup->inUse) {
                        continue;
                    }
                    popup->position.y -= AsciiSupervisor()->frameScalar5B8 * 0.2f;
                    popup->timer++;
                    if (popup->timer >= 60) {
                        popup->inUse = 0;
                    }
                }
            }
        }
    } else {
        ascii->pauseMenu.OnUpdate();
    }

    if (reinterpret_cast<unsigned char *>(&g_GameManager)[0x13D] != 0 &&
        ascii->menuState97C8.OnUpdate()) {
        return CHAIN_CALLBACK_RESULT_BREAK;
    }
    if (reinterpret_cast<unsigned char *>(&g_GameManager)[0x13E] != 0 &&
        ascii->menuStateA7A8.OnUpdate()) {
        return CHAIN_CALLBACK_RESULT_BREAK;
    }

    ascii->frameCounter++;
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

void AsciiMenuState5::OnDraw()
{
    unsigned int i;

    if (!g_GameManager.menuState97C8Active) {
        return;
    }

    g_AnmManager->FlushVertexBuffer();
    g_Supervisor.ConfigureGameplayViewport(2);
    if ((AsciiSupervisor()->flags5D4 & 2) && (this->state != 0 || this->frames > 2)) {
        g_AnmManager->DrawNoRotation(&this->menuBackground);
    }
    for (i = 0; i < 5; i++) {
        if (this->menuSprites[i].IsVisible()) {
            g_AnmManager->DrawNoRotation(&this->menuSprites[i]);
        }
    }
}

void AsciiMenuState4::OnDraw()
{
    unsigned int i;

    if (!g_GameManager.menuStateA7A8Active) {
        return;
    }

    g_AnmManager->FlushVertexBuffer();
    g_Supervisor.ConfigureGameplayViewport(2);
    for (i = 0; i < 3; i++) {
        if (this->menuSprites[i].IsVisible()) {
            g_AnmManager->DrawNoRotation(&this->menuSprites[i]);
        }
    }
}

int AsciiManager::OnDrawLowPrio(AsciiManager *ascii)
{
    ascii->DrawStrings();
    ascii->ResetStrings();
    ascii->pauseMenu.OnDraw();
    ascii->menuState97C8.OnDraw();
    ascii->menuStateA7A8.OnDraw();
    if (ascii->menuStateA7A8.menuSprites[3].scriptIndex != 0) {
        g_AnmManager->DrawNoRotation(&ascii->menuStateA7A8.menuSprites[3]);
    }
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

int AsciiManager::OnDrawHighPrio(AsciiManager *ascii)
{
    g_Supervisor.ConfigureGameplayViewport(0);
    ascii->OnDrawHighPrioImpl(0);
    g_Supervisor.ConfigureGameplayViewport(1);
    ascii->OnDrawHighPrioImpl(1);
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

int RegisterAsciiManagerStartup()
{
    AsciiManager *ascii = &g_AsciiManager;

    g_AsciiManagerCalcChain.SetCallback(
        reinterpret_cast<ChainCallback>(AsciiManager::OnUpdate));
    g_AsciiManagerCalcChain.addedCallback =
        reinterpret_cast<ChainLifetimeCallback>(AsciiManager::AddedCallback);
    g_AsciiManagerCalcChain.deletedCallback =
        reinterpret_cast<ChainLifetimeCallback>(AsciiManager::DeletedCallback);
    g_AsciiManagerCalcChain.arg = ascii;
    if (g_Chain.AddToCalcChain(&g_AsciiManagerCalcChain, 1) != 0) {
        return -1;
    }

    g_AsciiManagerDrawChainLowPrio.SetCallback(
        reinterpret_cast<ChainCallback>(AsciiManager::OnDrawLowPrio));
    g_AsciiManagerDrawChainLowPrio.arg = ascii;
    g_Chain.AddToDrawChain(&g_AsciiManagerDrawChainLowPrio, 34);

    g_AsciiManagerDrawChainHighPrio.SetCallback(
        reinterpret_cast<ChainCallback>(AsciiManager::OnDrawHighPrio));
    g_AsciiManagerDrawChainHighPrio.arg = ascii;
    g_Chain.AddToDrawChain(&g_AsciiManagerDrawChainHighPrio, 23);
    return 0;
}

void AsciiManager::OnDrawHighPrioImpl(int playerIndex)
{
    AsciiManagerPopup *popup;
    unsigned char *character;
    int alpha;
    float dx;
    float dy;
    int i;
    int j;

    popup = &this->scorePopups[playerIndex][0];

    if (!AsciiSupervisor()->IsFogDisabled()) {
        AsciiSupervisor()->DisableFog();
    }
    AsciiSupervisor()->SetRenderState(23, 8);

    for (j = 0; j < 100; j++, popup++) {
        if (!popup->inUse) {
            continue;
        }

        this->smallScoreText.pos.x =
            g_GameManager.TransformPopupX(
                popup->position.x - (float)(popup->characterCount * 4));
        this->smallScoreText.pos.y =
            g_GameManager.TransformPopupY(popup->position.y);
        this->smallScoreText.color1 = popup->color;

        dx = g_GameManager.sides[playerIndex].player->position.x - popup->position.x;
        dy = g_GameManager.sides[playerIndex].player->position.y - popup->position.y;
        alpha = (int)(dx * dx + dy * dy);
        if (alpha > 4096) {
            alpha = 208;
        } else if (alpha > 1024) {
            alpha = ((alpha - 1024) << 7) / 3072 + 80;
        } else {
            alpha = 80;
        }

        character = reinterpret_cast<unsigned char *>(
            &popup->text[popup->characterCount - 1]);
        for (i = popup->characterCount; i > 0; i--) {
            if (popup->timer < 52) {
                this->smallScoreText.loadedSprite =
                    this->asciiAnm->GetSprite(*character + 97);
                reinterpret_cast<unsigned char *>(&this->smallScoreText.color1)[3] =
                    (unsigned char)alpha;
            } else if (popup->timer < 56) {
                this->smallScoreText.loadedSprite =
                    this->asciiAnm->GetSprite(*character + 97);
                reinterpret_cast<unsigned char *>(&this->smallScoreText.color1)[3] =
                    (unsigned char)alpha;
            } else {
                this->smallScoreText.loadedSprite =
                    this->asciiAnm->GetSprite(*character + 97);
                reinterpret_cast<unsigned char *>(&this->smallScoreText.color1)[3] =
                    (unsigned char)alpha;
            }
            this->smallScoreText.spriteSize.x =
                reinterpret_cast<AsciiAnmLoadedSpriteView *>(
                    this->smallScoreText.loadedSprite)->widthPx;
            g_AnmManager->DrawNoRotation(&this->smallScoreText);
            this->smallScoreText.pos.x += 8.0f;
            character--;
        }
    }
}
