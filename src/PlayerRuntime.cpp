#include "PlayerLifecycleView.hpp"
#include "FileSystem.hpp"
#include "ZunMemory.hpp"
#include "ZunTimer.hpp"
#include "AsciiManager.hpp"
#include "EffectManager.hpp"
#include "RngRuntimeLeaves.hpp"

#include <stddef.h>

struct PlayerResourcePathView
{
    const char *shtPath;
    unsigned char unknown04[0x14];
};

typedef char PlayerResourcePathSizeIs18[(sizeof(PlayerResourcePathView) == 0x18) ? 1 : -1];

struct PlayerTailDrawStateView
{
    PlayerPositionView position00;
    unsigned char unknown00C[0x0C];
    void *active18;
    PlayerAnmVmView vm1C;
    unsigned char trailing2C0[0x04];
};

typedef char PlayerTailDrawStateSizeIs2C4[
    (sizeof(PlayerTailDrawStateView) == 0x2C4) ? 1 : -1];
typedef char PlayerTailDrawStateActiveAt18[
    (offsetof(PlayerTailDrawStateView, active18) == 0x18) ? 1 : -1];
typedef char PlayerTailDrawStateVmAt1C[
    (offsetof(PlayerTailDrawStateView, vm1C) == 0x1C) ? 1 : -1];

struct PlayerRewardAnmView
{
    void ExecuteAnmIdx(PlayerAnmVmView *vm, int scriptIndex);
};

struct PlayerRewardAnmBankView
{
    unsigned char unknown000000[0x25E1BC];
    PlayerRewardAnmView *rewardAnm25E1BC;
};

struct PlayerRewardSideStateView
{
    unsigned char unknown00[0x08];
    PlayerRewardAnmBankView *anmBank08;
};

struct PlayerRewardTailStateView
{
    int type00;
    PlayerPositionView position04;
    PlayerPositionView velocity10;
    int active1C;
    PlayerAnmVmView vm20;
};

typedef char PlayerRewardTailStateSizeIs2C4[
    (sizeof(PlayerRewardTailStateView) == 0x2C4) ? 1 : -1];
typedef char PlayerRewardTailPositionAt04[
    (offsetof(PlayerRewardTailStateView, position04) == 0x04) ? 1 : -1];
typedef char PlayerRewardTailActiveAt1C[
    (offsetof(PlayerRewardTailStateView, active1C) == 0x1C) ? 1 : -1];
typedef char PlayerRewardTailVmAt20[
    (offsetof(PlayerRewardTailStateView, vm20) == 0x20) ? 1 : -1];

struct PlayerOwnerStateSideView
{
    void AdjustOwnerMetric(int value);
};

struct PlayerOwnerStatePlayerView
{
    unsigned char unknown00[0x0C];
    PlayerOwnerStateSideView *sideState0C;
};

struct PlayerOwnerStateUpdateView
{
    PlayerOwnerStatePlayerView *owner00;
    int state04;
    unsigned char unknown08[0x04];
    int value0C;
    int value10;
    ZunTimer timer14;
    ZunTimer timer20;
    unsigned char unknown2C[0x0C];
    int state38;
    int activeFrameCounter3C;
    void *callback40;
};

typedef char PlayerOwnerStateUpdateSizeIs44[
    (sizeof(PlayerOwnerStateUpdateView) == 0x44) ? 1 : -1];
typedef char PlayerOwnerStateTimer14At14[
    (offsetof(PlayerOwnerStateUpdateView, timer14) == 0x14) ? 1 : -1];
typedef char PlayerOwnerStateTimer20At20[
    (offsetof(PlayerOwnerStateUpdateView, timer20) == 0x20) ? 1 : -1];

struct PlayerAnmManagerView
{
    void *GetAnm(int anmIdx);
    int ExecuteScript(void *vm);
    int DrawNoRotation(void *vm);
    int Draw2D(void *vm);
    void DrawPlayerBullet(void *vm);
};

extern PlayerAnmManagerView *g_AnmManager;
extern PlayerShtFileView *g_PlayerShtFilesBySide[2];
extern PlayerResourcePathView g_PlayerResourcePaths[];
extern int g_PlayerTable303F0[];
extern int g_PlayerTable303F4[];
extern int g_PlayerTable303F8[];
extern int g_PlayerTable303FC[];
extern int g_PlayerTable30400[];
extern void *g_PlayerCallback30404[];
extern void *g_PlayerCallback30408[];
extern void *g_PlayerCallback30450[];
extern void *g_PlayerShotSpawnCallbacks[];
extern void *g_PlayerShotUpdateCallbacks[];
extern void *g_PlayerShotDrawCallbacks[];
extern void *g_PlayerShotCollisionCallbacks[];
extern ZunMemory g_ZunMemory;

extern float __fastcall PlayerSin(float value);

static int LoadPlayerShtFile(PlayerShtFileView **header, const char *path)
{
    int i;
    PlayerShtDescriptorView *descriptor;

    *header = reinterpret_cast<PlayerShtFileView *>(FileSystem::OpenFile(path, NULL, 0));
    if (*header == NULL)
        return -1;

    PlayerShtFileView *shtFile = *header;
    float sine = PlayerSin(0.7853981852531433f);
    shtFile->sizeDerived0 = static_cast<float>(sine * shtFile->sizeSource0);
    (*header)->sizeDerived1 =
        static_cast<float>(sine * (*header)->sizeSource1);

    for (i = 0; i < (*header)->shotPowerLevelCount; ++i)
    {
        reinterpret_cast<unsigned int &>((*header)->shotPowerLevels[i].descriptors) +=
            reinterpret_cast<unsigned int>(*header);
        descriptor = (*header)->shotPowerLevels[i].descriptors;
        while (descriptor->fireInterval >= 0)
        {
            descriptor->spawnCallback =
                g_PlayerShotSpawnCallbacks[reinterpret_cast<unsigned int>(descriptor->spawnCallback)];
            descriptor->updateCallback =
                g_PlayerShotUpdateCallbacks[reinterpret_cast<unsigned int>(descriptor->updateCallback)];
            descriptor->drawCallback =
                g_PlayerShotDrawCallbacks[reinterpret_cast<unsigned int>(descriptor->drawCallback)];
            descriptor->collisionCallback =
                g_PlayerShotCollisionCallbacks[reinterpret_cast<unsigned int>(descriptor->collisionCallback)];
            ++descriptor;
        }
    }
    return 0;
}

int __fastcall PlayerLifecycleView::AddedCallback(PlayerLifecycleView *player)
{
    PlayerSideStateView *sideState;
    if (g_PlayerShtFilesBySide[player->sideIndex] != NULL)
    {
        if (player->sideState->shotType20 == player->sideState->previousShotType24)
        {
            player->primaryShtFile = g_PlayerShtFilesBySide[player->sideIndex];
            goto resourceReady;
        }
    }
    if (g_PlayerShtFilesBySide[player->sideIndex] != NULL)
        g_ZunMemory.Free(g_PlayerShtFilesBySide[player->sideIndex]);

    if (LoadPlayerShtFile(&player->primaryShtFile,
                          g_PlayerResourcePaths[player->sideState->shotType20].shtPath) != 0)
        return -1;
    g_PlayerShtFilesBySide[player->sideIndex] = player->primaryShtFile;

resourceReady:
    sideState = player->sideState;
    player->tableValue303F0 = g_PlayerTable303F0[sideState->shotType20];
    player->tableValue303F4 = g_PlayerTable303F4[sideState->shotType20];
    player->tableValue303F8 = g_PlayerTable303F8[sideState->shotType20];
    player->tableValue303FC = g_PlayerTable303FC[sideState->shotType20];
    player->tableValue30400 = g_PlayerTable30400[sideState->shotType20];
    player->header24.anmFile98 = g_AnmManager->GetAnm(player->sideIndex + 5);

    PlayerShtFileView *sht = player->primaryShtFile;
    player->hurtboxHalfSize.y = sht->hurtboxSize * 0.5f;
    player->hurtboxHalfSize.x = player->hurtboxHalfSize.y;
    player->hurtboxHalfSize.z = 5.0f;
    player->grazeHalfSize.y = sht->grazeBoxSize * 0.5f;
    player->grazeHalfSize.x = player->grazeHalfSize.y;
    player->grazeHalfSize.z = 5.0f;
    player->itemCollectionHalfSize.y = sht->itemCollectionBoxSize * 0.5f;
    player->itemCollectionHalfSize.x = player->itemCollectionHalfSize.y;
    player->itemCollectionHalfSize.z = 5.0f;

    player->scalar30388 = 100.0f;
    player->ownerState30410.owner00 = player;
    player->ownerState30410.callback40 = g_PlayerCallback30450[sideState->shotType20];
    player->header24.state7C = 1;
    player->header24.state80 = 1;
    player->header24.selfLink78 = player;
    player->collisionQuery36C.player00 = player;
    player->header24.state88 = 2;
    player->header24.state8C = 0;
    player->header24.state90 = 0;
    player->header24.state94 = 0;
    player->callback30404 = g_PlayerCallback30404[sideState->shotType20];
    player->callback30408 = g_PlayerCallback30408[sideState->shotType20];
    player->InitializeAddedState();
    return 0;
}


struct PlayerCollisionRegionRuntimeView
{
    unsigned char unknown00[0x08];
    float valueX;
    float growthX;
    float valueY;
    float valueZ;
    float growthY;
    float growthZ;
    unsigned char unknown20[0x04];
    int lifetime;
    unsigned char unknown28[0x14];
    unsigned char active;
    unsigned char unknown3D[0x03];
    int delay;

    void Deactivate();
};

typedef char PlayerCollisionRegionSizeIs44[(sizeof(PlayerCollisionRegionRuntimeView) == 0x44) ? 1 : -1];

struct PlayerCollisionRuntimeView
{
    unsigned char unknown0000[0xB0BC];
    PlayerCollisionRegionRuntimeView embeddedRegion;
    PlayerCollisionRegionRuntimeView *activeRegions[(0xB908 - 0xB100) / 4];
    PlayerCollisionRegionRuntimeView *releasedRegions[(0xC110 - 0xB908) / 4];
    int activeRegionCount;
    int releasedRegionCount;
};

typedef char PlayerCollisionActiveAtB100[(offsetof(PlayerCollisionRuntimeView, activeRegions) == 0xB100) ? 1 : -1];
typedef char PlayerCollisionReleasedAtB908[(offsetof(PlayerCollisionRuntimeView, releasedRegions) == 0xB908) ? 1 : -1];
typedef char PlayerCollisionCountAtC110[(offsetof(PlayerCollisionRuntimeView, activeRegionCount) == 0xC110) ? 1 : -1];

static void UpdatePlayerCollisionRegions(PlayerLifecycleView *player)
{
    PlayerCollisionRuntimeView *runtime = reinterpret_cast<PlayerCollisionRuntimeView *>(player);
    PlayerCollisionRegionRuntimeView **slot = runtime->activeRegions;

    for (int index = 0; index < runtime->activeRegionCount; ++index)
    {
        PlayerCollisionRegionRuntimeView *region = *slot;
        if (region->delay > 0)
        {
            --region->delay;
        }
        else
        {
            --region->lifetime;
            region->valueX += region->growthX;
            region->valueY += region->growthY;
            region->valueZ += region->growthZ;
            if (region->lifetime <= 0)
            {
                region->Deactivate();
                runtime->releasedRegions[runtime->releasedRegionCount++] = region;

                PlayerCollisionRegionRuntimeView **next = slot + 1;
                *slot = *next;
                if (*slot != NULL)
                {
                    PlayerCollisionRegionRuntimeView *following;
                    do
                    {
                        PlayerCollisionRegionRuntimeView **current = next;
                        following = next[1];
                        ++next;
                        *current = following;
                    }
                    while (following != NULL);
                }
                --runtime->activeRegionCount;
                --index;
                continue;
            }
        }
        ++slot;
    }

    PlayerCollisionRegionRuntimeView *embedded = &runtime->embeddedRegion;
    if (embedded->active)
    {
        if (embedded->delay > 0)
        {
            --embedded->delay;
            return;
        }

        --embedded->lifetime;
        embedded->valueX += embedded->growthX;
        embedded->valueY += embedded->growthY;
        embedded->valueZ += embedded->growthZ;
        if (embedded->lifetime <= 0)
        {
            embedded->Deactivate();
            return;
        }
        runtime->activeRegions[runtime->activeRegionCount] = embedded;
    }
    else
    {
        runtime->activeRegions[runtime->activeRegionCount] = NULL;
    }
}

struct PlayerSharedRuntimeView
{
    int IsBlocked();
    unsigned char unknown0000[0x1095C];
    int updateBlock1095C;
    int drawCounter10960;
};
extern PlayerSharedRuntimeView *g_PlayerSharedRuntime;

struct PlayerInputRuntimeView
{
    unsigned short Check(int code);
    unsigned char opaque[0x8E];
};
typedef char PlayerInputRuntimeSizeIs8E[(sizeof(PlayerInputRuntimeView) == 0x8E) ? 1 : -1];
extern PlayerInputRuntimeView g_PlayerInputStates[2];

struct PlayerBombTargetRuntimeView
{
    int CheckMode(int mode);
};

struct PlayerBombOwnerRuntimeView
{
    void Configure(int mode0, int mode1, int zero, const char *path);
};

static void UpdatePlayerBombState(PlayerLifecycleView *player)
{
    if (g_PlayerSharedRuntime->updateBlock1095C != 0)
        return;
    if (player->GetUpdateState() == 4)
        return;
    if (g_PlayerInputStates[player->sideIndex].Check(2) == 0)
        return;

    PlayerBombTargetRuntimeView *target =
        reinterpret_cast<PlayerBombTargetRuntimeView *>(player->opponentState->attackTarget10);
    PlayerBombOwnerRuntimeView *owner =
        reinterpret_cast<PlayerBombOwnerRuntimeView *>(player->opponentState->attackOwner14);

    if (player->scalar30388 >= 400.0f)
    {
        if (target->CheckMode(3))
        {
            owner->Configure(1, 1, 0, player->primaryShtFile->modePath1);
            player->scalar30384 = 100.0f;
            player->scalar30388 = 100.0f;
            player->SetBombMode1();
        }
        else
        {
            owner->Configure(2, 2, 0, player->primaryShtFile->modePath2);
            player->scalar30384 = 0.0f;
            player->scalar30388 = 0.0f;
            player->SetBombMode2();
        }
        return;
    }

    if (player->scalar30388 >= 300.0f)
    {
        owner->Configure(1, 1, 0, player->primaryShtFile->modePath1);
        player->scalar30384 = 0.0f;
        player->scalar30388 = 0.0f;
        player->SetBombMode1();
        return;
    }

    if (player->scalar30388 >= 200.0f)
    {
        owner->Configure(0, 0, 0, player->primaryShtFile->modePath0);
        player->scalar30384 = 0.0f;
        player->scalar30388 = 0.0f;
        player->SetBombMode0();
    }
}

struct PlayerSupervisorRuntimeView
{
    void SelectSide(int sideIndex);
    void PreparePlayerDraw(int sideIndex);
    void ConfigureGameplayViewport(int sideIndex);

    unsigned char unknown000[0x5B8];
    float framerateMultiplier5B8;
};
typedef char PlayerSupervisorFramerateAt5B8[
    (offsetof(PlayerSupervisorRuntimeView, framerateMultiplier5B8) == 0x5B8) ? 1 : -1];
extern PlayerSupervisorRuntimeView g_PlayerSupervisorRuntime;

struct PlayerGameManagerRuntimeView
{
    int CheckTimingState();
    int IsWithinPlayfield(float x, float y, float extent34, float extent30);
    float TransformPopupX(float value);
    float TransformPopupY(float value);
    unsigned char unknown000[0x13D];
    unsigned char suppressPlayerDraw13D;
};
extern PlayerGameManagerRuntimeView g_PlayerGameManagerRuntime;

struct PlayerFrontSideRuntimeView
{
    void ClearState(int value);
    void SetTimingValue(int value);
    int UpdateMeterHundreds(int level);
};

struct PlayerMetricRuntimeView
{
    int GetValue();
};

struct PlayerSoundRuntimeView
{
    void PlaySoundByIdx(int soundIndex, int pan);
};
extern PlayerSoundRuntimeView g_SoundPlayer;

extern unsigned int g_PlayerUpdateGateFlags;
extern int g_PlayerTimingTable[][7];

extern RngRuntimeView g_ReplayRng;
extern EffectManager *g_PlayerRewardEffectManager;
extern int g_PlayerRewardBaseValue;
extern int g_PlayerRewardModeValue;
extern float g_PlayerRewardVelocitySpan;

struct PlayerRewardEffectRuntimeView
{
    unsigned char unknown000[0x98];
    float delay98;
    unsigned char unknown09C[0x04];
    float scaleA0;
    short valueA4;
    short valueA6;
    short sideA8;
};

typedef char PlayerRewardEffectDelayAt98[
    (offsetof(PlayerRewardEffectRuntimeView, delay98) == 0x98) ? 1 : -1];
typedef char PlayerRewardEffectScaleAtA0[
    (offsetof(PlayerRewardEffectRuntimeView, scaleA0) == 0xA0) ? 1 : -1];
typedef char PlayerRewardEffectValueA4[
    (offsetof(PlayerRewardEffectRuntimeView, valueA4) == 0xA4) ? 1 : -1];

struct PlayerState4TimerCurrentView
{
    int GetCurrent();
};

struct PlayerState4OpsView
{
    void AddRespawnResource(float value);
    void SetUpdateState(int state);
    void SpawnTransitionA(
        PlayerPositionView *position, float x, float y, int kind, int zero);
    void SpawnTransitionB(
        PlayerPositionView *position, float x, float y,
        int subtype, int kind, int zero);
};

extern float g_PlayerState4MinX;
extern float g_PlayerState4MinY;
extern float g_PlayerState4Width;
extern float g_PlayerState4Height;

void PlayerState4OpsView::AddRespawnResource(float value)
{
    PlayerLifecycleView *player = reinterpret_cast<PlayerLifecycleView *>(this);
    int previousLevel = static_cast<int>(player->scalar30388) / 100;

    player->scalar30388 += value;
    PlayerSideStateView *sideState = player->sideState;
    if (sideState->shotType20 == 1)
        player->scalar30388 += value * 0.15f;

    if (player->scalar30388 >= 400.0f)
        player->scalar30388 = 400.0f;

    int currentLevel = static_cast<int>(player->scalar30388) / 100;
    if (previousLevel != currentLevel)
    {
        reinterpret_cast<PlayerFrontSideRuntimeView *>(sideState->frontSide18)
            ->UpdateMeterHundreds(currentLevel);
    }
}

static int PlayerCheckState4(PlayerLifecycleView *player);
extern int PlayerCheckState2();
static void PlayerTransitionSpecial(PlayerLifecycleView *player);
static void PlayerUpdateCommon(PlayerLifecycleView *player);
extern void PlayerUpdateStageA();
extern void PlayerUpdateStageB();
struct PlayerCollisionOwnerStateView
{
    unsigned char unknown000[0xDBE];
    short stateDBE;
};

static int PlayerUpdateStageC(PlayerLifecycleView *player);
static void PlayerUpdateStageD(PlayerLifecycleView *player);
extern void PlayerUpdateStageE();
extern void __fastcall PlayerUpdateSelectorState(void *state);
void __fastcall PlayerUpdateOwnerState(void *opaqueState)
{
    PlayerOwnerStateUpdateView *state =
        reinterpret_cast<PlayerOwnerStateUpdateView *>(opaqueState);

    if (state->timer20 > 0)
    {
        state->activeFrameCounter3C = 0;
        state->timer20--;
        if (state->timer20 <= 0)
        {
            state->timer20 = 0;
            state->state04 = 0;
            state->state38 = 0;
        }
    }

    if (state->timer14 > 0)
    {
        state->timer14--;
        if (state->timer14 <= 0)
        {
            state->owner00->sideState0C->AdjustOwnerMetric(state->value10);
            state->value10 = 0;
            state->value0C = 0;
            state->timer14 = 0;
        }
    }
}


struct PlayerRespawnVmView
{
    unsigned char unknown000[0x18];
    float scaleX18;
    float scaleY1C;
    unsigned char unknown020[0x1F0 - 0x20];
    unsigned int color1F0;
};

struct PlayerRespawnBlendView
{
    void SetBlendModeAdditive();
    void SetBlendModeNormal();
};

static void PlayerTransitionSpecial(PlayerLifecycleView *player)
{
    *reinterpret_cast<int *>(
        reinterpret_cast<unsigned char *>(player) + 0x30334) = 60;

    float value =
        1.0f -
        (float)*reinterpret_cast<ZunTimer *>(&player->timer303C8) *
            0.033333335f;

    reinterpret_cast<PlayerRespawnVmView *>(&player->mainVm)->scaleY1C =
        value + value + 1.0f;
    reinterpret_cast<PlayerRespawnVmView *>(&player->mainVm)->scaleX18 =
        1.0f - value;
    reinterpret_cast<PlayerRespawnBlendView *>(&player->mainVm)
        ->SetBlendModeAdditive();

    *reinterpret_cast<float *>(
        reinterpret_cast<unsigned char *>(player) + 0x1CE0) = 1.0f;
    *reinterpret_cast<float *>(
        reinterpret_cast<unsigned char *>(player) + 0x1CDC) = 1.0f;

    int timerCurrent =
        reinterpret_cast<PlayerState4TimerCurrentView *>(
            &player->timer303C8)->GetCurrent();
    reinterpret_cast<PlayerRespawnVmView *>(&player->mainVm)->color1F0 =
        ((255 * timerCurrent / 30) << 24) | 0x00FFFFFF;

    *reinterpret_cast<int *>(
        reinterpret_cast<unsigned char *>(player) + 0x3032C) = 0;

    if (player->sideIndex == 0)
        player->position1B88.x = value * -176.0f;
    else
        player->position1B88.x = value * 176.0f;
    player->position1B88.y = value * 64.0f + 384.0f;

    if (reinterpret_cast<PlayerState4TimerCurrentView *>(
            &player->timer303C8)->GetCurrent() >= 30)
    {
        reinterpret_cast<PlayerState4OpsView *>(player)->SetUpdateState(3);
        reinterpret_cast<PlayerRespawnVmView *>(&player->mainVm)->scaleX18 = 1.0f;
        reinterpret_cast<PlayerRespawnVmView *>(&player->mainVm)->scaleY1C = 1.0f;
        reinterpret_cast<PlayerRespawnVmView *>(&player->mainVm)->color1F0 = 0xFFFFFFFF;
        reinterpret_cast<PlayerRespawnBlendView *>(&player->mainVm)
            ->SetBlendModeNormal();
        player->position1B88.x = 0.0f;
        player->position1B88.y = 384.0f;
    }
}


static __inline Effect *&PlayerStateEffect(PlayerLifecycleView *player)
{
    return *reinterpret_cast<Effect **>(
        reinterpret_cast<unsigned char *>(player) + 0x303E0);
}

static void PlayerUpdateCommon(PlayerLifecycleView *player)
{
    int &transitionCounter =
        *reinterpret_cast<int *>(
            reinterpret_cast<unsigned char *>(player) + 0x30334);
    if (transitionCounter != 0)
        --transitionCounter;

    if (player->GetUpdateState() == 3)
    {
        if (PlayerStateEffect(player) != NULL)
        {
            PlayerStateEffect(player)->position =
                *reinterpret_cast<EffectFloat3 *>(&player->position1B88);
        }

        reinterpret_cast<ZunTimer *>(&player->timer303C8)->operator--(0);
        if (reinterpret_cast<PlayerState4TimerCurrentView *>(
                &player->timer303C8)->GetCurrent() <= 0)
        {
            if (PlayerStateEffect(player) != NULL)
            {
                PlayerStateEffect(player)->active = 0;
                PlayerStateEffect(player) = NULL;
            }
            reinterpret_cast<PlayerState4OpsView *>(player)->SetUpdateState(0);
            *reinterpret_cast<ZunTimer *>(&player->timer303C8) = 0;
            reinterpret_cast<unsigned int &>(player->mainVm.color1F0) =
                0xFFFFFFFFu;
        }
        else
        {
            int current =
                reinterpret_cast<PlayerState4TimerCurrentView *>(
                    &player->timer303C8)->GetCurrent();
            reinterpret_cast<unsigned int &>(player->mainVm.color1F0) =
                current % 8 < 2 ? 0xFFF02020u : 0xFFFFFFFFu;
        }
    }
    else
    {
        reinterpret_cast<ZunTimer *>(&player->timer303C8)->operator++(0);
    }

    if (reinterpret_cast<ZunTimer *>(&player->timer1B74)->operator>(0))
    {
        int current =
            reinterpret_cast<PlayerState4TimerCurrentView *>(
                &player->timer303C8)->GetCurrent();
        if (current % 4 < 2)
            reinterpret_cast<unsigned int &>(player->mainVm.color1F0) =
                0xFFF02020u;
        else
            reinterpret_cast<unsigned int &>(player->mainVm.color1F0) =
                0xFFFFFFFFu;

        reinterpret_cast<ZunTimer *>(&player->timer1B74)->operator--(0);
        if (reinterpret_cast<ZunTimer *>(&player->timer1B74)->operator<=(0))
        {
            *reinterpret_cast<ZunTimer *>(&player->timer1B74) = 0;
            reinterpret_cast<unsigned int &>(player->mainVm.color1F0) =
                0xFFFFFFFFu;
        }
    }
}

static int PlayerCheckState4(PlayerLifecycleView *player)
{
    ZunTimer *timer =
        reinterpret_cast<ZunTimer *>(&player->timer303C8);

    if ((float)*timer < 60.0f)
    {
        *reinterpret_cast<Float3 *>(&player->position1B88) +=
            *reinterpret_cast<Float3 *>(
                reinterpret_cast<unsigned char *>(player) + 0x1CCC) *
            (60.0f - (float)*timer) / 20.0f;

        float *position =
            reinterpret_cast<Float3 *>(&player->position1B88)->operator float *();
        if (position[0] < g_PlayerState4MinX)
            position[0] = g_PlayerState4MinX;
        else if (position[0] > g_PlayerState4MinX + g_PlayerState4Width)
            position[0] = g_PlayerState4MinX + g_PlayerState4Width;

        if (position[1] < g_PlayerState4MinY)
            position[1] = g_PlayerState4MinY;
        else if (position[1] > g_PlayerState4MinY + g_PlayerState4Height)
            position[1] = g_PlayerState4MinY + g_PlayerState4Height;

        int timerCurrent =
            reinterpret_cast<PlayerState4TimerCurrentView *>(timer)->GetCurrent();
        reinterpret_cast<unsigned int &>(player->mainVm.color1F0) =
            ((255 * timerCurrent / 10) << 24) | 0x00FFFFFF;
        return 0;
    }

    float respawnValue;
    if (player->header24.state84 == 1)
        respawnValue = 400.0f;
    else
        respawnValue = 130.0f - player->header24.state84 * 10.0f;

    PlayerState4OpsView *ops = reinterpret_cast<PlayerState4OpsView *>(player);
    ops->AddRespawnResource(respawnValue);
    ops->SetUpdateState(3);
    *timer = 60;
    ops->SpawnTransitionA(&player->position1B88, 16.0f, 20.0f, 8, 0);
    ops->SpawnTransitionB(&player->position1B88, 16.0f, 20.0f, 10, 8, 0);
    return 0;
}

static void PlayerUpdateStageD(PlayerLifecycleView *player)
{
    *reinterpret_cast<Float3 *>(
        reinterpret_cast<unsigned char *>(player) + 0x30364) =
        Float3(-999.0f, -999.0f, 0.0f);
    *reinterpret_cast<Float3 *>(
        reinterpret_cast<unsigned char *>(player) + 0x30370) =
        Float3(-999.0f, -999.0f, 0.0f);

    *reinterpret_cast<int *>(
        reinterpret_cast<unsigned char *>(player) + 0x30380) = 0;
    *reinterpret_cast<float *>(
        reinterpret_cast<unsigned char *>(player) + 0x1CE8) = 1.0f;
    *reinterpret_cast<float *>(
        reinterpret_cast<unsigned char *>(player) + 0x1CE4) = 1.0f;
}

static int PlayerUpdateStageC(PlayerLifecycleView *player)
{
    if (!player->GetUpdateState())
    {
        if (!reinterpret_cast<ZunTimer *>(&player->timer1B74)->operator>(0))
        {
            PlayerCollisionQueryRecordView *record =
                player->collisionQuery36C.FindCollisionAtPlayer();
            if (record != NULL)
            {
                PlayerCollisionOwnerStateView *owner =
                    reinterpret_cast<PlayerCollisionOwnerStateView *>(record->unknown2C);
                if (owner != NULL)
                    owner->stateDBE = 5;

                player->EnterDeathState();
                return 1;
            }
        }
    }
    return 0;
}

int __fastcall PlayerLifecycleView::OnUpdate(PlayerLifecycleView *player)
{
    if ((g_PlayerUpdateGateFlags & 0x1800) != 0)
        return 1;

    PlayerSideStateView *sideState = player->sideState;
    if ((sideState->flags34 & 1) != 0 && sideState->shotType20 != 2)
        return 1;

    if ((sideState->flags34 & 1) == 0)
        player->UpdateBeforeState();

    if (player->GetUpdateState() == 5)
        return 1;

    g_PlayerSupervisorRuntime.SelectSide(player->sideIndex);

    if (player->GetUpdateState() == 4)
    {
        if (PlayerCheckState4(player))
            goto transitionSpecial;
        goto afterTransition;
    }
    if (player->GetUpdateState() == 2)
    {
        if (PlayerCheckState2())
            goto transitionSpecial;
        goto afterTransition;
    }
    if (player->GetUpdateState() != 1)
        goto afterTransition;

transitionSpecial:
    PlayerTransitionSpecial(player);
afterTransition:
    PlayerUpdateCommon(player);
    if (player->selector > 0)
        PlayerUpdateSelectorState(reinterpret_cast<unsigned char *>(player) + 0x24);

    UpdatePlayerCollisionRegions(player);
    if (!g_PlayerSharedRuntime->IsBlocked())
        UpdatePlayerBombState(player);

    if (player->GetUpdateState() != 2 && player->GetUpdateState() != 4 &&
        player->GetUpdateState() != 1)
        player->UpdateMovementAndOptions();

    g_AnmManager->ExecuteScript(&player->mainVm);
    player->UpdateShots();

    if (!g_PlayerSharedRuntime->IsBlocked())
    {
        ++player->ownerState30410.activeFrameCounter3C;
        if ((player->flags1B80 & 8) != 0)
        {
            g_SoundPlayer.PlaySoundByIdx(48, player->sideIndex != 0 ? 500 : -500);
            reinterpret_cast<PlayerFrontSideRuntimeView *>(player->sideState->frontSide18)->ClearState(0);
            player->flags1B80 &= ~8u;

            if (g_PlayerGameManagerRuntime.CheckTimingState() && player->sideIndex == 0)
            {
                PlayerSideStateView *opponentState = player->opponentState;
                PlayerMetricRuntimeView *metric = reinterpret_cast<PlayerMetricRuntimeView *>(
                    reinterpret_cast<unsigned char *>(opponentState->manager04) + 0x80);
                int metricValue = metric->GetValue();
                int timingValue = g_PlayerTimingTable[opponentState->characterIndex2C][0];
                timingValue *= 60;
                timingValue -= metricValue;
                PlayerSideStateView *currentSideState = player->sideState;
                reinterpret_cast<PlayerFrontSideRuntimeView *>(currentSideState->frontSide18)
                    ->SetTimingValue(timingValue);
            }
        }

        if (g_PlayerSharedRuntime->updateBlock1095C == 0)
            player->UpdateHeavyState();
    }

    if ((player->sideState->flags34 & 1) == 0 &&
        g_PlayerSharedRuntime->updateBlock1095C == 0)
        PlayerUpdateStageC(player);

    PlayerUpdateStageD(player);
    if ((player->sideState->flags34 & 1) == 0)
        PlayerUpdateOwnerState(&player->ownerState30410);

    player->collisionQuery36C.count1804 = 0;
    return 1;
}


PlayerLifecycleView::PlayerLifecycleView()
{
}

void PlayerLifecycleView::UpdateBeforeState()
{
    PlayerRewardTailStateView *state =
        reinterpret_cast<PlayerRewardTailStateView *>(tailStates30454);
    Float3 collectionSize(
        primaryShtFile->itemCollectionBoxSize,
        primaryShtFile->itemCollectionBoxSize, 16.0f);
    targetOverride30F64.x = -1000.0f;

    int i;
    for (i = 0; i < 4; ++i, ++state)
    {
        if (state->active1C == 0)
            continue;

        state->position04 += state->velocity10;
        if (state->position04.y >= 464.0f)
        {
            state->active1C = 0;
            continue;
        }

        float motionValue = state->velocity10.y;
        if (motionValue < 3.0f)
        {
            motionValue = state->velocity10.y + 0.03f;
            state->velocity10.y = motionValue;
        }
        else
            state->velocity10.y = 3.0f;

        if (CalcItemCollectionCollision(
                &state->position04,
                reinterpret_cast<PlayerPositionView *>(&collectionSize)))
        {
            switch (state->type00)
            {
            case 0:
                reinterpret_cast<PlayerState4OpsView *>(this)
                    ->AddRespawnResource(400.0f);
                break;
            case 1:
                for (i = 0;
                     i < g_PlayerRewardBaseValue +
                             2 * g_PlayerRewardModeValue + 10;
                     ++i)
                {
                    g_PlayerSupervisorRuntime.SelectSide(sideIndex);

                    EffectFloat3 position;
                    position.x =
                        g_PlayerGameManagerRuntime.TransformPopupX(position1B88.x) +
                        g_ReplayRng.GetRandomF32SignedInRange(64.0f);
                    position.y =
                        g_PlayerGameManagerRuntime.TransformPopupY(position1B88.y) +
                        g_ReplayRng.GetRandomF32SignedInRange(64.0f);
                    position.z = 0.0f;

                    g_PlayerSupervisorRuntime.SelectSide(1 - sideIndex);

                    EffectFloat3 velocity;
                    velocity.x = g_ReplayRng.GetRandomF32SignedInRange(
                        g_PlayerRewardVelocitySpan * 0.5f - 8.0f);
                    velocity.y = g_ReplayRng.GetRandomF32InRange(128.0f);
                    velocity.z = 0.0f;

                    Effect *rawEffect =
                        g_PlayerRewardEffectManager->SpawnEffectWithVelocity(
                            sideIndex + 1, &position, &velocity, 1,
                            static_cast<unsigned int>(-1));
                    PlayerRewardEffectRuntimeView *effect =
                        reinterpret_cast<PlayerRewardEffectRuntimeView *>(
                            rawEffect);
                    effect->valueA4 = 3;
                    effect->valueA6 = 4;

                    switch (g_PlayerRewardModeValue)
                    {
                    case 0:
                        effect->scaleA0 =
                            g_PlayerRewardBaseValue * 0.04f + 0.9f;
                        break;
                    case 1:
                        effect->scaleA0 =
                            g_PlayerRewardBaseValue * 0.05f + 1.1f;
                        break;
                    case 2:
                        effect->scaleA0 =
                            g_PlayerRewardBaseValue * 0.08f + 1.3f;
                        break;
                    case 3:
                        effect->scaleA0 =
                            g_PlayerRewardBaseValue * 0.1f + 1.7f;
                        break;
                    case 4:
                        effect->scaleA0 =
                            g_PlayerRewardBaseValue * 0.08f + 1.5f;
                        break;
                    default:
                        break;
                    }

                    effect->sideA8 = static_cast<short>(sideIndex);
                    effect->delay98 = static_cast<float>(i) * 2.0f;
                }
                g_PlayerSupervisorRuntime.SelectSide(sideIndex);
                break;
            case 2:
                ownerState30410.ApplyReward(
                    &position1B88, 1, 0, 400, 0);
                break;
            case 3:
                ownerState30410.ApplyReward(
                    &position1B88, 0, 0, 0, 70000);
                break;
            default:
                break;
            }

            state->active1C = 0;
            g_SoundPlayer.PlaySoundByIdx(
                31, sideIndex != 0 ? 500 : -500);
        }
        else
        {
            targetOverride30F64.x = state->position04.x;
            targetOverride30F64.y =
                400.0f - (400.0f - state->position04.y) * 0.5f;
        }
    }
}


void PlayerLifecycleView::SpawnRewardTailState(
    int type, PlayerPositionView *position)
{
    PlayerRewardTailStateView *state =
        reinterpret_cast<PlayerRewardTailStateView *>(tailStates30454);

    if (g_PlayerSharedRuntime->updateBlock1095C != 0)
        return;

    for (int index = 0; index < 4; ++index, ++state)
    {
        if (state->active1C == 0)
        {
            state->type00 = type;
            state->position04 = *position;
            state->velocity10.x = 0.0f;
            state->velocity10.y = -1.5f;
            state->velocity10.z = 0.0f;
            state->active1C = 1;

            reinterpret_cast<PlayerRewardSideStateView *>(sideState)
                ->anmBank08->rewardAnm25E1BC
                ->ExecuteAnmIdx(&state->vm20, type + 70);
            return;
        }
    }
}


void PlayerLifecycleView::DrawTailStates()
{
    PlayerTailDrawStateView *state = reinterpret_cast<PlayerTailDrawStateView *>(
        reinterpret_cast<unsigned char *>(this) + 0x30458);
    for (int i = 0; i < 4; ++i, ++state)
    {
        if (state->active18 == NULL)
            continue;
        state->vm1C.position208.x =
            g_PlayerGameManagerRuntime.TransformPopupX(state->position00.x);
        state->vm1C.position208.y =
            g_PlayerGameManagerRuntime.TransformPopupY(state->position00.y);
        state->vm1C.position208.z = 0.15f;
        g_AnmManager->DrawNoRotation(&state->vm1C);
    }
}

int __fastcall PlayerLifecycleView::OnDrawHighPrio(PlayerLifecycleView *player)
{
    g_PlayerSupervisorRuntime.PreparePlayerDraw(player->sideIndex);
    if (player->GetUpdateState() == 5)
        goto drawOptions;

    player->DrawActiveShots();
    if (player->header24.state84 == 0)
    {
        if (g_PlayerSharedRuntime->updateBlock1095C >= 2)
            return 1;
        if (g_PlayerSharedRuntime->drawCounter10960 > 30)
            return 1;
    }

    if (g_PlayerGameManagerRuntime.suppressPlayerDraw13D == 0)
    {
        player->mainVm.position208.x = g_PlayerGameManagerRuntime.TransformPopupX(player->position1B88.x);
        player->mainVm.position208.y = g_PlayerGameManagerRuntime.TransformPopupY(player->position1B88.y);
        player->mainVm.position208.z = 0.1f;
        g_AnmManager->DrawNoRotation(&player->mainVm);
    }

drawOptions:
    player->DrawTailStates();
    return 1;
}

int __fastcall PlayerLifecycleView::OnDrawLowPrio(PlayerLifecycleView *player)
{
    g_PlayerSupervisorRuntime.ConfigureGameplayViewport(player->sideIndex);
    player->DrawHitShots();
    return 1;
}

void PlayerLifecycleView::UpdateShots()
{
    if ((this->sideState->flags34 & 1) != 0)
        return;

    PlayerShotCtorView *shot = this->shots;
    for (int i = 0; i < 128; ++i, ++shot)
    {
        if (shot->state462 == 0)
            continue;

        if (shot->updateCallback474 != NULL && shot->updateCallback474(this, shot) != 0)
        {
            shot->state462 = 0;
            continue;
        }

        float *position = shot->position2A4.operator float *();
        position[0] += g_PlayerSupervisorRuntime.framerateMultiplier5B8 * shot->velocity43C.x;
        position[1] += g_PlayerSupervisorRuntime.framerateMultiplier5B8 * shot->velocity43C.y;

        if (shot->shotType464 != 2 &&
            !g_PlayerGameManagerRuntime.IsWithinPlayfield(
                position[0], position[1], shot->vm.loadedSprite224->extent34,
                shot->vm.loadedSprite224->extent30))
            shot->state462 = 0;

        if (g_AnmManager->ExecuteScript(&shot->vm) != 0)
            shot->state462 = 0;
        shot->timer454++;
    }
}

void PlayerLifecycleView::DrawActiveShots()
{
    PlayerShotCtorView *shot = this->shots;
    for (int i = 0; i < 128; ++i, ++shot)
    {
        if (shot->state462 != 1)
            continue;
        if (shot->vm.type1FC != 0)
            shot->vm.SetZRotation(shot->angle450);

        shot->vm.position208.x = g_PlayerGameManagerRuntime.TransformPopupX(shot->position2A4.x);
        shot->vm.position208.y = g_PlayerGameManagerRuntime.TransformPopupY(shot->position2A4.y);
        shot->vm.position208.z = 0.4f;
        if (shot->tintFlag46E != 0)
        {
            shot->vm.color1F0.red = 0xff;
            shot->vm.color1F0.green = 0x40;
            shot->vm.color1F0.blue = 0x40;
        }

        if (shot->drawCallback478 != NULL)
            shot->drawCallback478(this, shot);
        else
            g_AnmManager->Draw2D(&shot->vm);
    }
}

void PlayerLifecycleView::DrawHitShots()
{
    PlayerShotCtorView *shot = this->shots;
    for (int i = 0; i < 128; ++i, ++shot)
    {
        if (shot->state462 != 2)
            continue;
        if (shot->vm.type1FC != 0)
            shot->vm.SetZRotation(shot->angle450);

        shot->vm.position208.x = g_PlayerGameManagerRuntime.TransformPopupX(shot->position2A4.x);
        shot->vm.position208.y = g_PlayerGameManagerRuntime.TransformPopupY(shot->position2A4.y);
        shot->vm.position208.z = 0.2f;
        if (shot->tintFlag46E != 0)
        {
            shot->vm.color1F0.red = 0xff;
            shot->vm.color1F0.green = 0x40;
            shot->vm.color1F0.blue = 0x40;
        }
        g_AnmManager->DrawPlayerBullet(&shot->vm);
    }
}
