#include "AnmManager.hpp"
#include "AsciiManager.hpp"
#include "ExAttackController.hpp"
#include "PlayerLifecycleView.hpp"

#include <stddef.h>

struct ExAttackType01Extra
{
    int state00;
    Float3 motion04;
    Float3 point10;
    Float3 point1C;
    Float3 point28;
    Float3 point34;
    Float3 spawnPosition40;
};

struct ExAttackType01Record
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
    ExAttackType01Extra *extra34;
};

struct ExAttackUpdateAnmVmView
{
    void SetInterrupt(short interrupt);
};

struct ExAttackCollisionCircleView
{
    unsigned char storage[0x1808];
    void AppendCircleRecord(const PlayerPositionView *position, float radius, int unknown);
};

struct ExAttackUpdatePlayerView
{
    unsigned char unknown000[0xBC];
    AnmLoaded *anmFileBC;
    unsigned char unknown0C0[0x36C - 0xC0];
    ExAttackCollisionCircleView collisionQuery36C;

    int CheckBulletCollision(
        PlayerPositionView *position,
        PlayerPositionView *collisionSize,
        void *bullet);
};

struct ExAttackUpdateSideView
{
    unsigned char unknown00[0x04];
    ExAttackUpdatePlayerView *player04;
    unsigned char unknown08[0x30];
};

struct ExAttackUpdateGameManagerView
{
    ExAttackUpdateSideView sides[2];
};

extern ExAttackUpdateGameManagerView g_GameManager;

float *__fastcall ExAttackInterpolate2D(
    float *out,
    float *point1C,
    float *point10,
    float *point34,
    float *point28,
    float time,
    float duration);

int __fastcall ExAttackUpdateCallbackType01(ExAttackRecord *base)
{
    ExAttackType01Record *record =
        reinterpret_cast<ExAttackType01Record *>(base);
    ExAttackType01Extra *extra = record->extra34;
    Float3 collisionSize(24.0f, 24.0f, 0.0f);

    switch (extra->state00)
    {
    case 2:
        if (record->timer10 > 20)
            return 1;
        break;

    case 1:
    {
        if (record->timer10 > 20)
        {
            ExAttackUpdatePlayerView *player =
                g_GameManager.sides[record->opponentSide04].player04;

            if (player->CheckBulletCollision(
                    reinterpret_cast<PlayerPositionView *>(&record->position20),
                    reinterpret_cast<PlayerPositionView *>(&collisionSize),
                    0) == 2)
            {
                extra->state00 = 2;
                record->timer10 = 0;
                reinterpret_cast<ExAttackUpdateAnmVmView *>(
                    record->dynamicData1C)->SetInterrupt(1);
                return 0;
            }

            record->position20 += extra->motion04;

            if ((extra->motion04.x < 0.0f &&
                 record->position20.x < -144.0f) ||
                (extra->motion04.x > 0.0f &&
                 record->position20.x > 144.0f))
            {
                extra->motion04.x = -extra->motion04.x;
            }

            extra->motion04.y += 0.04f;
            if (extra->motion04.y > 3.0f)
                extra->motion04.y = 3.0f;

            g_GameManager.sides[record->opponentSide04]
                .player04->collisionQuery36C.AppendCircleRecord(
                    reinterpret_cast<PlayerPositionView *>(&record->position20),
                    18.0f,
                    0);

            if (record->position20.y >= 480.0f)
                return 1;
        }
        break;
    }

    case 0:
        ExAttackInterpolate2D(
            &record->position20.x,
            &extra->point1C.x,
            &extra->point10.x,
            &extra->point34.x,
            &extra->point28.x,
            record->timer10,
            90.0f);

        if (record->timer10 > 90)
        {
            g_GameManager.sides[record->side08]
                .player04->anmFileBC->ExecuteAnmIdx(
                    reinterpret_cast<AnmVm *>(record->dynamicData1C), 9);
            ++extra->state00;
            record->position20 = extra->spawnPosition40;
            record->unknown00 = record->opponentSide04;
            record->timer10 = 0;
        }
        break;
    }

    return 0;
}
