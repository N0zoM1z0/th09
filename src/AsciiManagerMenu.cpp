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

struct AsciiGameManagerSideView
{
    unsigned char unknown00[0x34];
    unsigned int flags;
};

typedef char AsciiGameManagerSideViewSizeIs38[
    (sizeof(AsciiGameManagerSideView) == 0x38) ? 1 : -1];

struct AsciiGameManagerView
{
    AsciiGameManagerSideView sides[2];
    unsigned char unknown070[0xAC];
    int difficulty;
    unsigned char unknown120[0x14];
    unsigned int flags;
    unsigned char unknown138[4];
    unsigned char inGameMenu;
    unsigned char menuState97C8Active;
    unsigned char menuStateA7A8Active;
    unsigned char unknown13F;

    int IsReplayNeutral();
    int HasFlagBit0();
};

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
    unsigned char unknown594[0x24];
    float frameScalar5B8;
    unsigned char unknown5BC[0x18];
    unsigned int flags5D4;
    unsigned char unknown5D8[0x1C4];
    unsigned int systemTime79C;

    int PrepareResultScreen();
};

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
