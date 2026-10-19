#include "EffectRingRuntimeView.hpp"
#include "AnmManager.hpp"
#include "AsciiGameManagerView.hpp"
#include "PlayerLifecycleView.hpp"
#include "ZunMemory.hpp"

struct EffectType16Vertex
{
    Float3 pos;
    float rhw;
    unsigned long diffuse;
};
typedef char EffectType16VertexSizeIs14[(sizeof(EffectType16Vertex) == 0x14) ? 1 : -1];
struct EffectType16ScaleView
{
    unsigned char unknown00[0x84];
    float horizontalScale84;
};
typedef char EffectType16ScaleAt84[(offsetof(EffectType16ScaleView, horizontalScale84) == 0x84) ? 1 : -1];


int __fastcall EffectInitializeCallbackType16(Effect *effect)
{
    effect->vertices = g_ZunMemory.Alloc(sizeof(EffectType16Vertex) * 33, "Effect");
    if (effect->vertices == 0)
        return -1;
    EffectRingRuntimeView *state = reinterpret_cast<EffectRingRuntimeView *>(effect);
    state->origin48 = *reinterpret_cast<const Float3 *>(&effect->position);
    state->axis54.z = 1.0f;
    state->angle80 = effect->velocity.x;
    state->axis54.x = 0.0f;
    state->axis54.y = 0.0f;
    state->axis60.x = 0.0f;
    state->axis60.y = -1.0f;
    state->axis60.z = 0.0f;
    state->thickness88 = effect->velocity.z;
    state->ellipseY94 = 0.0f;
    state->rotation98 = 0.0f;
    state->segments8C = 32;
    state->radius7C = 16.0f;
    state->frequency9C = 1.0f;
    effect->unknown0C9 = 1;
    reinterpret_cast<EffectType16ScaleView *>(effect)->horizontalScale84 = 0.01f;
    return 0;
}

int __fastcall EffectUpdateCallbackType16(Effect *effect)
{
    EffectType16ScaleView *state = reinterpret_cast<EffectType16ScaleView *>(effect);
    effect->unknown0C9 = 1;
    if (effect->timer >= 30 && state->horizontalScale84 < 0.08f)
        state->horizontalScale84 += 0.001f;
    effect->position = *reinterpret_cast<const EffectFloat3 *>(
        &g_GameManager.sides[effect->manager->sideIndex].player04->position1B88);
    return 1;
}

int __fastcall EffectDrawCallbackType16(Effect *effect)
{
    EffectRingRuntimeView *state = reinterpret_cast<EffectRingRuntimeView *>(effect);
    if (effect->unknown0C9 != 0)
    {
        float angleStep = 3.1415927f / static_cast<float>(state->segments8C - 1);
        float angle = 0.0f;
        EffectType16Vertex *vertex = static_cast<EffectType16Vertex *>(effect->vertices);
        const Float3 &origin = *reinterpret_cast<const Float3 *>(&effect->position);
        vertex->pos = origin;
        float centerY = vertex->pos.y - 432.0f;
        vertex->pos.y = centerY;
        vertex->pos.x = g_GameManager.TransformPopupX(vertex->pos.x);
        vertex->pos.y = g_GameManager.TransformPopupY(centerY);
        vertex->rhw = 1.0f;
        vertex->diffuse = 0x30600040;
        ++vertex;
        for (int i = state->segments8C; i > 0; --i)
        {
            vertex->pos.z = 0.0f;
            vertex->pos.FromAngleMagnitude(angle, 448.0f);
            const float &horizontalScale = *reinterpret_cast<const float *>(effect->unknown084);
            vertex->pos.x = horizontalScale * vertex->pos.x;
            vertex->pos += origin;
            float vertexY = vertex->pos.y - 432.0f;
            vertex->pos.y = vertexY;
            vertex->pos.x = g_GameManager.TransformPopupX(vertex->pos.x);
            vertex->pos.y = g_GameManager.TransformPopupY(vertexY);
            angle += angleStep;
            vertex->rhw = 1.0f;
            vertex->diffuse = 0x50600040;
            ++vertex;
        }
    }
    g_AnmManager->DrawTriangleFan(reinterpret_cast<AnmVm *>(effect->vms),
        static_cast<VertexDiffuseXyzrhw *>(effect->vertices), state->segments8C + 1);
    return 1;
}
