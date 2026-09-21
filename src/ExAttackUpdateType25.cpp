#include "AnmManager.hpp"
#include "AsciiManager.hpp"
#include "BulletManager.hpp"
#include "ExAttackController.hpp"
#include "ExAttackInterpolation.hpp"
#include "PlayerLifecycleView.hpp"
#include "RngRuntimeLeaves.hpp"

struct ExAttackType25Extra
{
    int state00;
    Float3 motion04;
    Float3 point10;
    Float3 point1C;
    Float3 control28;
    Float3 control34;
    Float3 spawn40;
};
typedef char ExAttackType25ExtraSizeIs4C[
    (sizeof(ExAttackType25Extra) == 0x4C) ? 1 : -1];

struct ExAttackType25Record
{
    int unknown00;
    int opponentSide04;
    int side08;
    int active0C;
    ZunTimer timer10;
    void *dynamicData1C;
    Float3 position20;
    int vmCount2C;
    int unknown30;
    ExAttackType25Extra *extra34;
};

struct ExAttackType25PlayerView
{
    unsigned char unknown000[0xBC];
    AnmLoaded *anmFileBC;
};

struct ExAttackType25SideView
{
    unsigned char unknown00[0x04];
    ExAttackType25PlayerView *player04;
    EtamaController *etama08;
    unsigned char unknown0C[0x2C];
};

struct ExAttackType25GameManagerView
{
    ExAttackType25SideView sides[2];
};

struct ExAttackType25BulletDescriptorStorage
{
    unsigned char storage[0x214];
    ExAttackType25BulletDescriptorStorage();
};

typedef char ExAttackType25BulletDescriptorStorageSizeIs214[
    (sizeof(ExAttackType25BulletDescriptorStorage) == 0x214) ? 1 : -1];

extern ExAttackType25GameManagerView g_GameManager;
extern RngRuntimeView g_ReplayRng;

int __fastcall ExAttackUpdateCallbackType25(ExAttackRecord *base)
{
    ExAttackType25Record *record =
        reinterpret_cast<ExAttackType25Record *>(base);
    ExAttackType25Extra *extra = record->extra34;

    switch (extra->state00)
    {
    case 2:
        if (record->timer10 > 20)
            return 1;
        break;

    case 1:
        if (record->timer10 > 20)
        {
            if (record->timer10.HasTickedEvery(3))
            {
                ExAttackType25BulletDescriptorStorage storage;
                BulletSpawnDescriptor *descriptor =
                    reinterpret_cast<BulletSpawnDescriptor *>(&storage);

                descriptor->position = record->position20;
                descriptor->position.y += 24.0f;
                descriptor->bulletType = 19;
                descriptor->aimMode = 3;
                descriptor->count1 = 1;
                descriptor->count2 = 1;
                descriptor->color = 1;
                descriptor->angle = 1.57079637f;
                descriptor->speed1 = 1.2f;

                descriptor->InstallPolarAccelerationTransform(
                    0,
                    0,
                    120,
                    g_ReplayRng.GetRandomF32SignedInRange(0.015707964f),
                    0.010833333f);

                descriptor->angleStep = 0.0f;
                descriptor->speed2 = 0.0f;
                descriptor->unknown1FA = 0;
                descriptor->transformFlags = 0x20;

                g_GameManager.sides[record->opponentSide04]
                    .etama08->SpawnBulletPatternPrimary(descriptor);
            }

            if (record->timer10 > 60)
                return 1;
        }
        break;

    case 0:
        ExAttackInterpolate2D(
            &record->position20.x,
            &extra->point1C.x,
            &extra->point10.x,
            &extra->control34.x,
            &extra->control28.x,
            record->timer10,
            90.0f);

        if (record->timer10 > 90)
        {
            g_GameManager.sides[record->side08]
                .player04->anmFileBC->ExecuteAnmIdx(
                    reinterpret_cast<AnmVm *>(record->dynamicData1C),
                    8);
            ++extra->state00;
            record->position20 = extra->spawn40;
            record->unknown00 = record->opponentSide04;
            record->timer10 = 0;
        }
        break;
    }

    return 0;
}
