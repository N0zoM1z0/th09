#include "AnmManager.hpp"
#include "AsciiManager.hpp"
#include "BulletManager.hpp"
#include "ExAttackController.hpp"
#include "ExAttackInterpolation.hpp"
#include "PlayerLifecycleView.hpp"

struct ExAttackType6Extra
{
    int state00;
    float unknown04;
    float angle08;
    float angle0C;
    float angularVelocity10;
    Float3 motion14;
    Float3 point20;
    Float3 point2C;
    Float3 control38;
    Float3 control44;
    Float3 spawn50;
};
typedef char ExAttackType6ExtraSizeIs5C[
    (sizeof(ExAttackType6Extra) == 0x5C) ? 1 : -1];

struct ExAttackType6Record
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
    ExAttackType6Extra *extra34;
    int parameter38;
    float rotation3C;
};

struct ExAttackType6InterruptView
{
    void SetInterrupt(short interrupt);
};

struct ExAttackType6PlayerView
{
    unsigned char unknown000[0xBC];
    AnmLoaded *anmFileBC;
    unsigned char unknown0C0[0x36C - 0xC0];
    PlayerCollisionQueryStateView collisionQuery36C;

    int CheckBulletCollision(
        PlayerPositionView *position,
        PlayerPositionView *collisionSize,
        Bullet *bullet);
};

struct ExAttackType6SideView
{
    unsigned char unknown00[0x04];
    ExAttackType6PlayerView *player04;
    EtamaController *etama08;
    unsigned char unknown0C[0x2C];
};

struct ExAttackType6GameManagerView
{
    ExAttackType6SideView sides[2];
};

struct ExAttackType6BulletDescriptorStorage
{
    unsigned char storage[0x214];
    ExAttackType6BulletDescriptorStorage();
};
typedef char ExAttackType6BulletDescriptorStorageSizeIs214[
    (sizeof(ExAttackType6BulletDescriptorStorage) == 0x214) ? 1 : -1];

extern ExAttackType6GameManagerView g_GameManager;
extern int g_PlayerRewardBaseValue;
float __stdcall AddNormalizeAngle(float angle, float delta);

int __fastcall ExAttackUpdateCallbackType6(ExAttackRecord *base)
{
    ExAttackType6Record *record =
        reinterpret_cast<ExAttackType6Record *>(base);
    ExAttackType6Extra *extra = record->extra34;
    Float3 collisionSize(24.0f, 24.0f, 0.0f);

    switch (extra->state00)
    {
    case 2:
        if (record->timer10 > 20)
            return 1;
        break;

    case 1:
        if (record->timer10 > 20)
        {
            if (record->timer10.HasTickedEvery(
                    20 - g_PlayerRewardBaseValue / 8))
            {
                ExAttackType6BulletDescriptorStorage storage;
                BulletSpawnDescriptor *descriptor =
                    reinterpret_cast<BulletSpawnDescriptor *>(&storage);

                descriptor->position = record->position20;
                descriptor->bulletType = 6;
                descriptor->color = 6;
                descriptor->aimMode = 5;
                descriptor->count1 = 2;
                descriptor->count2 = 1;
                descriptor->angle = extra->angle08;
                descriptor->speed1 = 0.0f;

                descriptor->InstallWaitTransform(0, 0, 80);
                descriptor->InstallVectorAccelerationTransform(
                    1, 0, 120,
                    g_PlayerRewardBaseValue * 0.00037878787f +
                        0.015833333f,
                    -999.0f);

                descriptor->angleStep = 0.0f;
                descriptor->speed2 = 0.0f;
                descriptor->unknown1FA = 0;
                descriptor->transformFlags = 0x00020014;

                g_GameManager.sides[record->opponentSide04]
                    .etama08->SpawnBulletPatternPrimary(descriptor);
            }

            if (g_GameManager.sides[record->opponentSide04]
                    .player04->CheckBulletCollision(
                    reinterpret_cast<PlayerPositionView *>(
                        &record->position20),
                    reinterpret_cast<PlayerPositionView *>(
                        &collisionSize),
                    0) == 2)
            {
                extra->state00 = 2;
                record->timer10 = 0;
                reinterpret_cast<ExAttackType6InterruptView *>(
                    record->dynamicData1C)->SetInterrupt(1);
                record->rotation3C = extra->angle08;
                return 0;
            }

            extra->motion14.FromAngleMagnitude(
                extra->angle08, 1.5f);
            extra->angle08 = AddNormalizeAngle(
                extra->angle08, extra->angularVelocity10);
            record->position20 += extra->motion14;

            if (record->position20.y >= 480.0f ||
                record->position20.y <= -32.0f ||
                record->position20.x >= 176.0f ||
                record->position20.x <= -176.0f)
            {
                return 1;
            }
        }

        record->rotation3C = extra->angle08;
        break;

    case 0:
        ExAttackInterpolate2D(
            &record->position20.x,
            &extra->point2C.x,
            &extra->point20.x,
            &extra->control44.x,
            &extra->control38.x,
            record->timer10,
            90.0f);

        if (record->timer10 > 90)
        {
            g_GameManager.sides[record->side08]
                .player04->anmFileBC->ExecuteAnmIdx(
                    reinterpret_cast<AnmVm *>(record->dynamicData1C),
                    9);
            ++extra->state00;
            record->position20 = extra->spawn50;
            record->unknown00 = record->opponentSide04;
            record->timer10 = 0;
        }
        break;
    }

    return 0;
}
