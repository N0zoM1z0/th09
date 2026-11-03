#include "AnmManager.hpp"
#include "AsciiManager.hpp"
#include "EffectManager.hpp"

struct VertexDiffuseXyzrhw
{
    Float3 pos;
    float rhw;
    unsigned long diffuse;
};
typedef char EffectStripVertexSizeIs14[
    (sizeof(VertexDiffuseXyzrhw) == 0x14) ? 1 : -1];

struct EffectStripGameManagerView
{
    float TransformPopupX(float value);
    float TransformPopupY(float value);
};

extern EffectStripGameManagerView g_GameManager;
extern AnmManager *g_AnmManager;

// Type27 and Type36 draw the actual native 0xD8-byte Effect allocation.
// These are the same +0x04 ANM VM, +0x0C position, +0x7C radius, +0xC9
// dirty flag, and +0xCC vertex pointer used by other verified callbacks.
typedef char EffectStripNativeOwnerFields[
    (sizeof(Effect) == 0xD8 &&
     offsetof(Effect, vms) == 0x04 &&
     offsetof(Effect, position) == 0x0C &&
     offsetof(Effect, radius7C) == 0x7C &&
     offsetof(Effect, unknown0C9) == 0xC9 &&
     offsetof(Effect, vertices) == 0xCC) ? 1 : -1];

int __fastcall EffectDrawCallbackType27(Effect *effect)
{
    Effect *view = effect;
    if (view->unknown0C9 != 0)
    {
        VertexDiffuseXyzrhw *vertex = reinterpret_cast<VertexDiffuseXyzrhw *>(view->vertices);
        const Float3 *position =
            reinterpret_cast<const Float3 *>(&view->position);

        vertex->pos = *position;
        vertex->pos.x =
            g_GameManager.TransformPopupX(vertex->pos.x - view->radius7C);
        vertex->pos.y = g_GameManager.TransformPopupY(0.0f);
        vertex->rhw = 1.0f;
        vertex->diffuse = 0x70600040;
        ++vertex;

        vertex->pos = *position;
        vertex->pos.x =
            g_GameManager.TransformPopupX(vertex->pos.x - view->radius7C);
        vertex->pos.y = g_GameManager.TransformPopupY(448.0f);
        vertex->rhw = 1.0f;
        vertex->diffuse = 0x70600040;
        ++vertex;

        vertex->pos = *position;
        vertex->pos.x = g_GameManager.TransformPopupX(vertex->pos.x);
        vertex->pos.y = g_GameManager.TransformPopupY(0.0f);
        vertex->rhw = 1.0f;
        vertex->diffuse = 0x30600040;
        ++vertex;

        vertex->pos = *position;
        vertex->pos.x = g_GameManager.TransformPopupX(vertex->pos.x);
        vertex->pos.y = g_GameManager.TransformPopupY(448.0f);
        vertex->rhw = 1.0f;
        vertex->diffuse = 0x30600040;
        ++vertex;

        const float &positiveRadius = view->radius7C;
        vertex->pos = *position;
        vertex->pos.x =
            g_GameManager.TransformPopupX(positiveRadius + vertex->pos.x);
        vertex->pos.y = g_GameManager.TransformPopupY(0.0f);
        vertex->rhw = 1.0f;
        vertex->diffuse = 0x70600040;
        ++vertex;

        vertex->pos = *position;
        vertex->pos.x =
            g_GameManager.TransformPopupX(positiveRadius + vertex->pos.x);
        vertex->pos.y = g_GameManager.TransformPopupY(448.0f);
        vertex->rhw = 1.0f;
        vertex->diffuse = 0x70600040;
    }

    g_AnmManager->DrawTriangleStrip(
        reinterpret_cast<AnmVm *>(view->vms),
        reinterpret_cast<VertexDiffuseXyzrhw *>(view->vertices),
        6);
    return 1;
}

int __fastcall EffectDrawCallbackType36(Effect *effect)
{
    Effect *view = effect;
    if (view->unknown0C9 != 0)
    {
        VertexDiffuseXyzrhw *vertex = reinterpret_cast<VertexDiffuseXyzrhw *>(view->vertices);
        const Float3 *position =
            reinterpret_cast<const Float3 *>(&view->position);

        vertex->pos = *position;
        vertex->pos.y =
            g_GameManager.TransformPopupY(vertex->pos.y - view->radius7C);
        vertex->pos.x = g_GameManager.TransformPopupX(-144.0f);
        vertex->rhw = 1.0f;
        vertex->diffuse = 0x70600040;
        ++vertex;

        vertex->pos = *position;
        vertex->pos.y =
            g_GameManager.TransformPopupY(vertex->pos.y - view->radius7C);
        vertex->pos.x = g_GameManager.TransformPopupX(144.0f);
        vertex->rhw = 1.0f;
        vertex->diffuse = 0x70600040;
        ++vertex;

        vertex->pos = *position;
        vertex->pos.y = g_GameManager.TransformPopupY(vertex->pos.y);
        vertex->pos.x = g_GameManager.TransformPopupX(-144.0f);
        vertex->rhw = 1.0f;
        vertex->diffuse = 0x30600040;
        ++vertex;

        vertex->pos = *position;
        vertex->pos.y = g_GameManager.TransformPopupY(vertex->pos.y);
        vertex->pos.x = g_GameManager.TransformPopupX(144.0f);
        vertex->rhw = 1.0f;
        vertex->diffuse = 0x30600040;
        ++vertex;

        const float &positiveRadius = view->radius7C;
        vertex->pos = *position;
        vertex->pos.y =
            g_GameManager.TransformPopupY(positiveRadius + vertex->pos.y);
        vertex->pos.x = g_GameManager.TransformPopupX(-144.0f);
        vertex->rhw = 1.0f;
        vertex->diffuse = 0x70600040;
        ++vertex;

        vertex->pos = *position;
        vertex->pos.y =
            g_GameManager.TransformPopupY(positiveRadius + vertex->pos.y);
        vertex->pos.x = g_GameManager.TransformPopupX(144.0f);
        vertex->rhw = 1.0f;
        vertex->diffuse = 0x70600040;
    }

    g_AnmManager->DrawTriangleStrip(
        reinterpret_cast<AnmVm *>(view->vms),
        reinterpret_cast<VertexDiffuseXyzrhw *>(view->vertices),
        6);
    return 1;
}
