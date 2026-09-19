#include "BulletManager.hpp"
#include "AnmManager.hpp"
#include "AsciiGameManagerView.hpp"
#include "Supervisor.hpp"

extern void __fastcall fsincos(float *sine, float *cosine, float angle);
extern float __stdcall AddNormalizeAngle(float angle, float delta);

struct BulletDrawEffectManagerView
{
    void DrawBulletLayerEffects();
};

struct BulletDrawSideStateView
{
    unsigned char unknown00[0x0C];
    BulletDrawEffectManagerView *effectManager0C;
};

static int DrawSingleBullet(Bullet *bullet)
{
    AnmVm *vm;
    switch (bullet->state)
    {
    case 2:
        vm = &bullet->sprites.spawnFastVm;
        break;
    case 3:
        vm = &bullet->sprites.spawnNormalVm;
        break;
    case 4:
        vm = &bullet->sprites.spawnSlowVm;
        break;
    case 5:
        vm = &bullet->sprites.despawnVm;
        break;
    default:
        vm = &bullet->sprites.bulletVm;
        break;
    }

    float *position = bullet->position.operator float *();
    float *vmPosition = vm->pos.operator float *();
    vmPosition[0] = g_GameManager.TransformPopupX(position[0]);
    vmPosition[1] = g_GameManager.TransformPopupY(position[1]);
    vmPosition[2] = 0.05f;
    vm->color1 |= 0x00FFFFFFu;

    if (*reinterpret_cast<short *>(vm->unknown_01FC) != 0)
        vm->SetZRotation(AddNormalizeAngle(1.5707964f + bullet->angle, 0.0f));

    return g_AnmManager->Draw2D(vm);
}

int EtamaController::OnDraw(EtamaController *controller)
{
    g_Supervisor.ConfigureGameplayViewport(controller->sideIndex);

    Laser *laser = controller->lasers;
    for (int i = 0; i < 48; ++i, ++laser)
    {
        if (laser->inUse == 0)
            continue;

        float sine;
        float cosine;
        fsincos(&sine, &cosine, laser->angle);

        float halfLength =
            (laser->endOffset - laser->startOffset) * 0.5f +
            laser->startOffset;

        float *position = laser->position.operator float *();
        float *bodyPosition = laser->bodyVm.pos.operator float *();
        bodyPosition[0] =
            g_GameManager.TransformPopupX(position[0] + cosine * halfLength);
        bodyPosition[1] =
            g_GameManager.TransformPopupY(position[1] + sine * halfLength);
        bodyPosition[2] = 0.07f;
        laser->color = -1;
        g_AnmManager->Draw2D(&laser->bodyVm);

        if (laser->startOffset < 16.0f || laser->speed == 0.0f)
        {
            if (!laser->hideCapDuringStartup || laser->state != 0)
            {
                float *capPosition =
                    laser->startCapVm.pos.operator float *();
                capPosition[0] = g_GameManager.TransformPopupX(
                    position[0] + cosine * laser->startOffset);
                capPosition[1] = g_GameManager.TransformPopupY(
                    position[1] + sine * laser->startOffset);
                capPosition[2] = 0.05f;

                float widthScale = laser->width / 10.0f;
                laser->startCapVm.color1 = laser->bodyVm.color1;
                laser->startCapVm.flagsWord |= 0x40;
                laser->startCapVm.color1 |= 0xFF000000u;
                laser->startCapVm.scale.x =
                    widthScale * ((16.0f - laser->startOffset) / 16.0f);
                laser->startCapVm.scale.y = laser->startCapVm.scale.x;
                if (laser->startCapVm.scale.y <= 0.0f)
                {
                    laser->startCapVm.scale.x = widthScale;
                    laser->startCapVm.scale.y =
                        laser->startCapVm.scale.x;
                }

                g_AnmManager->Draw2D(&laser->startCapVm);
            }
        }
    }

    for (int i = 0; i < 6; ++i)
    {
        Bullet *node = controller->drawBuckets[i];
        while (node != NULL)
        {
            DrawSingleBullet(node);
            node = node->nextInDrawBucket;
        }
    }

    reinterpret_cast<BulletDrawSideStateView *>(controller->sideState)
        ->effectManager0C->DrawBulletLayerEffects();
    return 1;
}
