#include "AnmManager.hpp"
#include "AsciiManager.hpp"
#include "ExAttackController.hpp"
#include "ExAttackInterpolation.hpp"
#include "PlayerLifecycleView.hpp"

struct ExAttackType5UpdateExtra
{
    int state00;
    Float3 vector04;
    Float3 vector10;
    Float3 point1C;
    Float3 point28;
    Float3 control34;
    Float3 control40;
    Float3 spawn4C;
};

struct ExAttackType5UpdateRecord
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
    ExAttackType5UpdateExtra *extra34;
};

struct ExAttackType5InterruptView
{
    void SetInterrupt(short interrupt);
};

struct ExAttackType5PlayerView
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

struct ExAttackType5SideView
{
    unsigned char unknown00[0x04];
    ExAttackType5PlayerView *player04;
    unsigned char unknown08[0x30];
};

struct ExAttackType5GameManagerView
{
    ExAttackType5SideView sides[2];
};

extern ExAttackType5GameManagerView g_GameManager;

int __fastcall ExAttackUpdateCallbackType5(ExAttackRecord *base)
{
    ExAttackType5UpdateRecord *record =
        reinterpret_cast<ExAttackType5UpdateRecord *>(base);
    ExAttackType5UpdateExtra *extra = record->extra34;
    Float3 collisionSize(6.0f, 32.0f, 0.0f);

    switch (extra->state00)
    {
    case 2:
        if (record->timer10 > 12)
            return 1;
        break;

    case 1:
        if (record->timer10 > 12)
        {
            if (g_GameManager.sides[record->opponentSide04]
                    .player04->CheckBulletCollision(
                    reinterpret_cast<PlayerPositionView *>(&record->position20),
                    reinterpret_cast<PlayerPositionView *>(&collisionSize),
                    0) == 2)
            {
                extra->state00 = 2;
                record->timer10 = 0;
                reinterpret_cast<ExAttackType5InterruptView *>(
                    record->dynamicData1C)->SetInterrupt(1);
                return 0;
            }

            record->position20 += extra->vector04;
            extra->vector04 += extra->vector10;

            if (extra->vector04.y > 4.5f)
                extra->vector04.y = 4.5f;

            g_GameManager.sides[record->opponentSide04]
                .player04->collisionQuery36C.AppendBoxRecord(
                    reinterpret_cast<PlayerPositionView *>(&record->position20),
                    reinterpret_cast<PlayerPositionView *>(&collisionSize),
                    0);

            if (record->position20.y >= 480.0f)
                return 1;
        }
        break;

    case 0:
        ExAttackInterpolate2D(
            &record->position20.x,
            &extra->point28.x,
            &extra->point1C.x,
            &extra->control40.x,
            &extra->control34.x,
            record->timer10,
            40.0f);

        if (record->timer10 > 40)
        {
            g_GameManager.sides[record->side08]
                .player04->anmFileBC->ExecuteAnmIdx(
                    reinterpret_cast<AnmVm *>(record->dynamicData1C), 9);
            ++extra->state00;
            record->position20 = extra->spawn4C;
            record->unknown00 = record->opponentSide04;
            record->timer10 = 0;
        }
        break;
    }

    return 0;
}
