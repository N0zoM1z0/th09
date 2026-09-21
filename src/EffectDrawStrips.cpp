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

struct EffectStripView
{
    EffectManager *manager00;
    EffectVmView *vms04;
    int vmCount08;
    EffectFloat3 position0C;
    unsigned char unknown018[0x7C - 0x18];
    float radius7C;
    unsigned char unknown080[0xC9 - 0x80];
    unsigned char verticesDirtyC9;
    unsigned char unknown0CA[2];
    VertexDiffuseXyzrhw *verticesCC;
};

typedef char EffectStripVmsAt04[
    (offsetof(EffectStripView, vms04) == 0x04) ? 1 : -1];
typedef char EffectStripRadiusAt7C[
    (offsetof(EffectStripView, radius7C) == 0x7C) ? 1 : -1];
typedef char EffectStripDirtyAtC9[
    (offsetof(EffectStripView, verticesDirtyC9) == 0xC9) ? 1 : -1];
typedef char EffectStripVerticesAtCC[
    (offsetof(EffectStripView, verticesCC) == 0xCC) ? 1 : -1];

int __fastcall EffectDrawCallbackType27(Effect *effect)
{
    EffectStripView *view = reinterpret_cast<EffectStripView *>(effect);
    if (view->verticesDirtyC9 != 0)
    {
        VertexDiffuseXyzrhw *vertex = view->verticesCC;
        const Float3 *position =
            reinterpret_cast<const Float3 *>(&view->position0C);

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

        vertex->pos = *position;
        vertex->pos.x =
            g_GameManager.TransformPopupX(vertex->pos.x + view->radius7C);
        vertex->pos.y = g_GameManager.TransformPopupY(0.0f);
        vertex->rhw = 1.0f;
        vertex->diffuse = 0x70600040;
        ++vertex;

        vertex->pos = *position;
        vertex->pos.x =
            g_GameManager.TransformPopupX(vertex->pos.x + view->radius7C);
        vertex->pos.y = g_GameManager.TransformPopupY(448.0f);
        vertex->rhw = 1.0f;
        vertex->diffuse = 0x70600040;
    }

    g_AnmManager->DrawTriangleStrip(
        reinterpret_cast<AnmVm *>(view->vms04),
        view->verticesCC,
        6);
    return 1;
}

int __fastcall EffectDrawCallbackType36(Effect *effect)
{
    EffectStripView *view = reinterpret_cast<EffectStripView *>(effect);
    if (view->verticesDirtyC9 != 0)
    {
        VertexDiffuseXyzrhw *vertex = view->verticesCC;
        const Float3 *position =
            reinterpret_cast<const Float3 *>(&view->position0C);

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

        vertex->pos = *position;
        vertex->pos.y =
            g_GameManager.TransformPopupY(vertex->pos.y + view->radius7C);
        vertex->pos.x = g_GameManager.TransformPopupX(-144.0f);
        vertex->rhw = 1.0f;
        vertex->diffuse = 0x70600040;
        ++vertex;

        vertex->pos = *position;
        vertex->pos.y =
            g_GameManager.TransformPopupY(vertex->pos.y + view->radius7C);
        vertex->pos.x = g_GameManager.TransformPopupX(144.0f);
        vertex->rhw = 1.0f;
        vertex->diffuse = 0x70600040;
    }

    g_AnmManager->DrawTriangleStrip(
        reinterpret_cast<AnmVm *>(view->vms04),
        view->verticesCC,
        6);
    return 1;
}
