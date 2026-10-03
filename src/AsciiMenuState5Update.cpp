// The visibility of AnmVm::SetInvisible is intentionally opaque in this owner.
// TH09 keeps loop cursors across that call in preserved registers; the shared
// menu TU keeps the real helper definition for its separately verified owners.
#include "AnmManager.hpp"
#include "AsciiManager.hpp"
#include "AsciiGameManagerView.hpp"
#include "Supervisor.hpp"

#include <windows.h>
#include <mmsystem.h>

struct AsciiInputView
{
    unsigned short WasPressed(unsigned short buttons);
};

struct AsciiSoundPlayerView
{
    void PlaySoundByIdx(int soundIndex, int pan);
    void ResumeAfterPause();
};

struct AsciiState5SupervisorView
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

};

typedef char AsciiSupervisorField5A4[
    (offsetof(AsciiState5SupervisorView, field5A4) == 0x5A4) ? 1 : -1];
typedef char AsciiSupervisorFrameScalarAt5B8[
    (offsetof(AsciiState5SupervisorView, frameScalar5B8) == 0x5B8) ? 1 : -1];
typedef char AsciiSupervisorFlagsAt5D4[
    (offsetof(AsciiState5SupervisorView, flags5D4) == 0x5D4) ? 1 : -1];
typedef char AsciiSupervisorSystemTimeAt79C[
    (offsetof(AsciiState5SupervisorView, systemTime79C) == 0x79C) ? 1 : -1];

extern AsciiInputView g_AsciiInput;
extern AsciiSoundPlayerView g_SoundPlayer;
extern AsciiManager g_AsciiManager;
extern unsigned char *g_AsciiMenuConfig;

struct GameManagerSetupLayout
{
    static void CleanupGameplayState();
};

static inline AsciiState5SupervisorView *AsciiSupervisor()
{
    return reinterpret_cast<AsciiState5SupervisorView *>(&g_Supervisor);
}

static const unsigned int COLOR_MENU_ITEM_SELECTED = 0xFFFF8080u;
static const unsigned int COLOR_MENU_ITEM_NORMAL = 0xFF505050u;
static const unsigned short ASCII_INTERRUPT_SHOW = 1;

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
    case 3:
        if (this->frames >= 20) {
            if (!g_GameManager.IsGameMode1()) {
                this->state = 0;
                this->frames = 0;
                g_GameManager.menuState97C8Active = 0;
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
                AsciiSupervisor()->state590 = 13;
                this->frames = 0;
                g_GameManager.field0FC = 0;
                g_GameManager.menuState97C8Active = 0;
                AsciiSupervisor()->systemTime79C = timeGetTime();
                return 1;
            }

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

