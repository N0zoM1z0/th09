#include "AnmManager.hpp"
#include "BulletManager.hpp"
#include "ExAttackController.hpp"
#include "ExAttackInterpolation.hpp"

#include <stddef.h>

struct ExAttackType21UpdateVertex
{
    Float3 pos;
    float rhw;
    unsigned long diffuse;
    float u;
    float v;
};

typedef char ExAttackType21UpdateVertexSizeIs1C[
    (sizeof(ExAttackType21UpdateVertex) == 0x1C) ? 1 : -1];

struct ExAttackType21UpdateExtra
{
    int state00;
    Float3 motion04;
    Float3 point10;
    Float3 point1C;
    Float3 control28;
    Float3 control34;
    Float3 spawn40;
    ExAttackType21UpdateVertex vertices4C[33];
    Float3 history3E8[32];
    unsigned char unknown568[0x578 - 0x568];
    float magnitudes578[32];
    float unknown5F8;
    float angularSteps5FC[32];
    float drift67C;
    float drift680;
    int drawPending684;
    float angle688;
};

typedef char ExAttackType21UpdateExtraSizeIs68C[
    (sizeof(ExAttackType21UpdateExtra) == 0x68C) ? 1 : -1];

struct ExAttackType21UpdateRecord
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
    ExAttackType21UpdateExtra *extra34;
};

struct ExAttackType21UpdateVmView
{
    unsigned char unknown000[0x1F8];
    unsigned int flags1F8;
};

struct ExAttackType21UpdatePlayerView
{
    unsigned char unknown000[0xBC];
    AnmLoaded *anmFileBC;
};

struct ExAttackType21UpdateSideView
{
    unsigned char unknown00[0x04];
    ExAttackType21UpdatePlayerView *player04;
    EtamaController *etama08;
    unsigned char unknown0C[0x2C];
};

struct ExAttackType21UpdateGameManagerView
{
    ExAttackType21UpdateSideView sides[2];
};

struct ExAttackType21BulletDescriptorStorage
{
    unsigned char storage[0x214];
    ExAttackType21BulletDescriptorStorage();
};

typedef char ExAttackType21BulletDescriptorStorageSizeIs214[
    (sizeof(ExAttackType21BulletDescriptorStorage) == 0x214) ? 1 : -1];

extern ExAttackType21UpdateGameManagerView g_GameManager;
float __stdcall AddNormalizeAngle(float angle, float delta);

int __fastcall ExAttackUpdateCallbackType21(ExAttackRecord *base)
{
    ExAttackType21UpdateRecord *record =
        reinterpret_cast<ExAttackType21UpdateRecord *>(base);
    ExAttackType21UpdateExtra *extra = record->extra34;

    switch (extra->state00)
    {
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
            ++extra->state00;
            record->position20 = extra->spawn40;
            record->unknown00 = record->opponentSide04;
            record->timer10 = 0;

            g_GameManager.sides[record->side08]
                .player04->anmFileBC->ExecuteAnmIdx(
                    reinterpret_cast<AnmVm *>(record->dynamicData1C),
                    11);

            reinterpret_cast<ExAttackType21UpdateVmView *>(
                record->dynamicData1C)->flags1F8 &= ~2u;
        }
        break;

    case 1:
    {
        float uvAngle = extra->angle688;
        ExAttackType21UpdateVertex *vertex = &extra->vertices4C[1];
        Float3 *history = &extra->history3E8[1];
        float *magnitude = &extra->magnitudes578[0];
        float historyAngle = -3.1415927f;
        int remaining = 32;

        do
        {
            float uvStorage[3];
            Float3 *uv = reinterpret_cast<Float3 *>(uvStorage);
            uv->FromAngleMagnitude(uvAngle, 0.5f);
            vertex->u = uv->x + 0.5f;
            vertex->v = uv->y + 0.5f;
            uvAngle = AddNormalizeAngle(uvAngle, 0.2026834f);

            *magnitude += magnitude[33];
            history->FromAngleMagnitude(historyAngle, *magnitude);
            *history += extra->spawn40;

            ++vertex;
            ++history;
            ++magnitude;
            historyAngle += 0.2026834f;
        }
        while (--remaining != 0);

        extra->angle688 =
            AddNormalizeAngle(extra->angle688, 0.052359879f);

        Bullet *bullet =
            &g_GameManager.sides[record->opponentSide04]
                .etama08->primaryBullets[0];
        for (int i = 0; i < 175; ++i, ++bullet)
        {
            if (bullet->state != 0 &&
                bullet->unknown10BE == 0 &&
                bullet->sprites.unknownD44 != 330)
            {
                float deltaStorage[3];
                Float3 *delta = reinterpret_cast<Float3 *>(deltaStorage);
                *delta = bullet->position - record->position20;
                if (delta->x * delta->x + delta->y * delta->y <= 1024.0f)
                {
                    bullet->unknown10BE = 1;

                    ExAttackType21BulletDescriptorStorage storage;
                    BulletSpawnDescriptor *descriptor =
                        reinterpret_cast<BulletSpawnDescriptor *>(&storage);
                    descriptor->position = bullet->position;
                    descriptor->bulletType = 22;
                    descriptor->color = 0;
                    descriptor->aimMode = 1;
                    descriptor->count1 = 1;
                    descriptor->count2 = 1;
                    descriptor->angle = bullet->angle;
                    descriptor->angleStep = 0.0f;
                    descriptor->speed1 = bullet->speed + 1.0f;
                    descriptor->speed2 = 0.0f;
                    descriptor->unknown1FA = 0;
                    descriptor->transformFlags = 0;

                    Bullet *spawned =
                        g_GameManager.sides[record->opponentSide04]
                            .etama08->SpawnBulletPatternPrimary(descriptor);
                    spawned->unknown10BE = 1;
                }
            }
        }

        bullet =
            &g_GameManager.sides[record->opponentSide04]
                .etama08->secondaryBullets[0];
        for (int i = 0; i < 360; ++i, ++bullet)
        {
            if (bullet->state != 0 &&
                bullet->unknown10BE == 0 &&
                bullet->sprites.unknownD44 != 330)
            {
                float deltaStorage[3];
                Float3 *delta = reinterpret_cast<Float3 *>(deltaStorage);
                *delta = bullet->position - record->position20;
                if (delta->x * delta->x + delta->y * delta->y <= 1024.0f)
                {
                    bullet->unknown10BE = 1;

                    ExAttackType21BulletDescriptorStorage storage;
                    BulletSpawnDescriptor *descriptor =
                        reinterpret_cast<BulletSpawnDescriptor *>(&storage);
                    descriptor->position = bullet->position;
                    descriptor->bulletType = 22;
                    descriptor->color = 0;
                    descriptor->aimMode = 1;
                    descriptor->count1 = 1;
                    descriptor->count2 = 1;
                    descriptor->angle = bullet->angle;
                    descriptor->angleStep = 0.0f;
                    descriptor->speed1 = bullet->speed + 0.69999999f;
                    descriptor->speed2 = 0.0f;
                    descriptor->unknown1FA = 0;
                    descriptor->transformFlags = 0;

                    Bullet *spawned =
                        g_GameManager.sides[record->opponentSide04]
                            .etama08->SpawnBulletPatternSecondary(descriptor);
                    spawned->unknown10BE = 1;
                }
            }
        }

        if ((reinterpret_cast<ExAttackType21UpdateVmView *>(
                 record->dynamicData1C)->flags1F8 & 1u) == 0)
            return 1;
        break;
    }

    case 2:
        if (record->timer10 > 20)
            return 1;
        break;

    default:
        break;
    }

    return 0;
}
