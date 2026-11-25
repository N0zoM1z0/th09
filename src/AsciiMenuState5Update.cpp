// The visibility of AnmVm::SetInvisible is intentionally opaque in this owner.
// TH09 keeps loop cursors across that call in preserved registers; the shared
// menu TU keeps the real helper definition for its separately verified owners.
#include "AnmManager.hpp"
#include "AsciiManager.hpp"
#include "SoundPlayer.hpp"
#include "AsciiGameManagerView.hpp"
#include "Supervisor.hpp"
#include "GameManagerSetupView.hpp"

#include <windows.h>
#include <mmsystem.h>

struct AsciiInputView
{
    unsigned short WasPressed(unsigned short buttons);
};

// The menu updates live state in the shared native Supervisor allocation.
typedef char AsciiState5NativeSupervisorFields[
    (offsetof(Supervisor, state590) == 0x590 &&
     offsetof(Supervisor, screenTransitionCountdown) == 0x5A4 &&
     offsetof(Supervisor, framerateMultiplier5B8) == 0x5B8 &&
     offsetof(Supervisor, flags) == 0x5D4 &&
     offsetof(Supervisor, systemTime) == 0x79C) ? 1 : -1];

extern AsciiInputView g_AsciiInput;
extern AsciiManager g_AsciiManager;
extern unsigned char *g_AsciiMenuConfig;



static const unsigned int COLOR_MENU_ITEM_SELECTED = 0xFFFF8080u;
static const unsigned int COLOR_MENU_ITEM_NORMAL = 0xFF505050u;
static const unsigned short ASCII_INTERRUPT_SHOW = 1;

// State5 owns the five native AnmVm menu records; each update/read
// uses the real indexed slot with its original iteration order.


// Gameover updates its own state/frame counters and the actual
// one-byte GameManager retry counter, without integer-pointer aliases.
int AsciiMenuState5::OnUpdate()
{
    unsigned int i;

    if (g_GameManager.IsReplayNeutral()) {
        g_GameManager.menuState97C8Active = 0;
        g_Supervisor.state590 = 1;
        return 1;
    }

    if (g_GameManager.sides[0].runtime->counter14 >= 3) {
        g_GameManager.menuState97C8Active = 0;
        g_Supervisor.state590 = 6;
        return 1;
    }

    switch (this->state) {
    case 0:
        if (!g_GameManager.IsGameMode1()) {
            for (i = 0; i < 5; i++) {
                g_AsciiManager.asciiAnm->SetAndExecuteScriptIdx(&this->menuSprites[i], i + 8);
            }
            for (i = 0; i < 5; i++) {
                this->menuSprites[i].pendingInterrupt = ASCII_INTERRUPT_SHOW;
            }
            g_AsciiManager.asciiAnm->SetSprite(
                &this->menuSprites[4],
                120 - g_GameManager.sides[0].runtime->counter14);
        } else {
            for (i = 0; i < 3; i++) {
                g_AsciiManager.asciiAnm->SetAndExecuteScriptIdx(&this->menuSprites[i], i + 17);
            }
            for (i = 0; i < 3; i++) {
                this->menuSprites[i].pendingInterrupt = ASCII_INTERRUPT_SHOW;
            }
        }
        ++this->state;
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
                g_SoundPlayer.PlaySoundByIdx(SOUND_SHOOT, 0);
            }
            if (g_AsciiInput.WasPressed(0x1001)) {
                g_SoundPlayer.PlaySoundByIdx(SOUND_SELECT, 0);
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
                g_SoundPlayer.PlaySoundByIdx(SOUND_SHOOT, 0);
            }
            if (g_AsciiInput.WasPressed(0x1001)) {
                g_SoundPlayer.PlaySoundByIdx(SOUND_SELECT, 0);
                for (i = 0; i < 5; i++) {
                    this->menuSprites[i].pendingInterrupt = ASCII_INTERRUPT_SHOW;
                }
                this->state = 3;
                this->frames = 0;
            }
        }
        break;

    case 4:
        if (this->frames >= 20) {
            this->state = 0;
            this->frames = 0;
            g_GameManager.menuState97C8Active = 0;
            g_Supervisor.state590 = 6;
            for (i = 0; i < 5; i++) {
                this->menuSprites[i].SetInvisible();
            }
            g_Supervisor.systemTime = timeGetTime();
            return 1;
        }
        break;
    case 3:
        if (this->frames >= 20) {
            if (!g_GameManager.IsGameMode1()) {
                this->state = 0;
                this->frames = 0;
                g_GameManager.menuState97C8Active = 0;
                for (i = 0; i < 5; i++) {
                    this->menuSprites[i].SetInvisible();
                }
                ++g_GameManager.sides[0].runtime->counter14;
                g_GameManager.flags |= 0x2000u;
                g_GameManager.sides[0].runtime->value04 = 0;
                g_GameManager.sides[0].runtime->value08 = 0;
                g_GameManager.sides[0].runtime->value0C = 0;
                g_GameManager.sides[1].runtime->value04 = 0;
                g_GameManager.sides[1].runtime->value08 = 0;
                g_GameManager.sides[1].runtime->value0C = 0;
                g_Supervisor.screenTransitionCountdown = 8;
                g_Supervisor.systemTime = timeGetTime();
                g_Supervisor.StopAudio();
                g_Supervisor.StartLoadedMusic();
                GameManagerSetupLayout::CleanupGameplayState();
                g_GameManager.field108 = 0;
                g_GameManager.field10C = 0;
                g_GameManager.sides[0].runtime->value00 =
                    (float)g_AsciiMenuConfig[0xAC] + 2.0f;
                g_GameManager.flags |= 0x4000u;
                return 1;
            } else {
                this->state = 0;
                g_Supervisor.state590 = 13;
                this->frames = 0;
                g_GameManager.field0FC = 0;
                g_GameManager.menuState97C8Active = 0;
                g_Supervisor.systemTime = timeGetTime();
                return 1;
            }

        }
        break;


    }

    for (i = 0; i < 5; i++) {
        g_AnmManager->ExecuteScript(&this->menuSprites[i]);
    }
    if (g_Supervisor.flags & 2) {
        g_AnmManager->ExecuteScript(&this->menuBackground);
    }
    ++this->frames;
    return 0;
}
