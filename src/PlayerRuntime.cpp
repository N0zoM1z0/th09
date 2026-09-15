#include "PlayerLifecycleView.hpp"
#include "FileSystem.hpp"
#include "ZunMemory.hpp"

#include <stddef.h>

struct PlayerResourcePathView
{
    const char *shtPath;
    unsigned char unknown04[0x14];
};

typedef char PlayerResourcePathSizeIs18[(sizeof(PlayerResourcePathView) == 0x18) ? 1 : -1];

struct PlayerAnmManagerView
{
    void *GetAnm(int anmIdx);
    int ExecuteScript(void *vm);
    int DrawNoRotation(void *vm);
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

extern double __stdcall PlayerSin(float value);

static int LoadPlayerShtFile(PlayerShtFileView **header, const char *path)
{
    int i;
    PlayerShtDescriptorView *descriptor;

    *header = reinterpret_cast<PlayerShtFileView *>(FileSystem::OpenFile(path, NULL, 0));
    if (*header == NULL)
        return -1;

    PlayerShtFileView *shtFile = *header;
    double sine = PlayerSin(0.7853981852531433f);
    shtFile->sizeDerived0 = static_cast<float>(shtFile->sizeSource0 * sine);
    shtFile->sizeDerived1 = static_cast<float>(shtFile->sizeSource1 * sine);

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
    player->history36C.selfLink00 = player;
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
};
extern PlayerSupervisorRuntimeView g_PlayerSupervisorRuntime;

struct PlayerGameManagerRuntimeView
{
    int CheckTimingState();
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

extern int PlayerCheckState4();
extern int PlayerCheckState2();
extern void PlayerTransitionSpecial();
extern void PlayerUpdateCommon();
extern void PlayerUpdateStageA();
extern void PlayerUpdateStageB();
extern void PlayerUpdateStageC();
extern void PlayerUpdateStageD();
extern void PlayerUpdateStageE();
extern void __fastcall PlayerUpdateSelectorState(void *state);
extern void __fastcall PlayerUpdateOwnerState(void *state);

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
        if (PlayerCheckState4())
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
    PlayerTransitionSpecial();
afterTransition:
    PlayerUpdateCommon();
    if (player->selector > 0)
        PlayerUpdateSelectorState(reinterpret_cast<unsigned char *>(player) + 0x24);

    UpdatePlayerCollisionRegions(player);
    if (!g_PlayerSharedRuntime->IsBlocked())
        UpdatePlayerBombState(player);

    if (player->GetUpdateState() != 2 && player->GetUpdateState() != 4 &&
        player->GetUpdateState() != 1)
        player->UpdateStateValue();

    g_AnmManager->ExecuteScript(&player->mainVm);
    player->UpdateAfterAnimation();

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
        PlayerUpdateStageC();

    PlayerUpdateStageD();
    if ((player->sideState->flags34 & 1) == 0)
        PlayerUpdateOwnerState(&player->ownerState30410);

    player->transient1B70 = 0;
    return 1;
}


PlayerLifecycleView::PlayerLifecycleView()
{
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
    player->DrawOptionStates();
    return 1;
}

int __fastcall PlayerLifecycleView::OnDrawLowPrio(PlayerLifecycleView *player)
{
    g_PlayerSupervisorRuntime.ConfigureGameplayViewport(player->sideIndex);
    player->DrawHitShots();
    return 1;
}
