#include "AnmManager.hpp"
#include "AsciiManager.hpp"
#include "BulletManager.hpp"
#include "ExAttackController.hpp"
#include "ExAttackInterpolation.hpp"
#include "PlayerLifecycleView.hpp"

struct ExAttackType26Extra
{
    int state00;
    Float3 motion04;
    Float3 point10;
    Float3 point1C;
    Float3 control28;
    Float3 control34;
    Float3 spawn40;
};
typedef char ExAttackType26ExtraSizeIs4C[
    (sizeof(ExAttackType26Extra) == 0x4C) ? 1 : -1];

struct ExAttackType26Record
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
    ExAttackType26Extra *extra34;
};

struct ExAttackType26PlayerView
{
    unsigned char unknown000[0xBC];
    AnmLoaded *anmFileBC;
};

struct ExAttackType26SideView
{
    unsigned char unknown00[0x04];
    ExAttackType26PlayerView *player04;
    EtamaController *etama08;
    unsigned char unknown0C[0x2C];
};

struct ExAttackType26GameManagerView
{
    ExAttackType26SideView sides[2];
};

struct ExAttackType26BulletDescriptorStorage
{
    unsigned char storage[0x214];
    ExAttackType26BulletDescriptorStorage();
};

typedef char ExAttackType26BulletDescriptorStorageSizeIs214[
    (sizeof(ExAttackType26BulletDescriptorStorage) == 0x214) ? 1 : -1];

extern ExAttackType26GameManagerView g_GameManager;

int __fastcall ExAttackUpdateCallbackType26(ExAttackRecord *base)
{
    ExAttackType26Record *record =
        reinterpret_cast<ExAttackType26Record *>(base);
    ExAttackType26Extra *extra = record->extra34;

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
                ExAttackType26BulletDescriptorStorage storage;
                BulletSpawnDescriptor *descriptor =
                    reinterpret_cast<BulletSpawnDescriptor *>(&storage);

                descriptor->position = record->position20;
                descriptor->bulletType = 19;
                descriptor->aimMode = 2;
                descriptor->count1 = 1;
                descriptor->count2 = 1;
                descriptor->color = 2;
                descriptor->angle = 0.0f;
                descriptor->speed1 = 0.2f;

                descriptor->InstallVectorAccelerationTransform(
                    0,
                    0,
                    120,
                    0.0066666668f,
                    -999.0f);

                descriptor->angleStep = 0.0f;
                descriptor->speed2 = 0.0f;
                descriptor->unknown1FA = 0;
                descriptor->transformFlags = 0x10;

                g_GameManager.sides[record->opponentSide04]
                    .etama08->SpawnBulletPatternPrimary(descriptor);
            }

            if (record->timer10 > 50)
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
