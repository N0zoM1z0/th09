// Effect template 34 callbacks; lexical member of the real ECL/math carrier.
// The initializer is exact. The complete 946-byte draw owner is still nonexact.
#include "EffectManager.hpp"
#include "EffectType34RuntimeView.hpp"
#include "AsciiGameManagerView.hpp"
#include "AnmManager.hpp"
#include "Supervisor.hpp"
#include "ZunMemory.hpp"
#include "RuntimeAngleMath.hpp"
#include <string.h>
#include <windows.h>
#include <d3d8.h>

struct EffectType34SetupView
{
    unsigned char unknown00[0x48];
    EffectFloat3 origin48;
    EffectFloat3 axis54;
    EffectFloat3 axis60;
    unsigned char unknown6C[0x7C - 0x6C];
    float radius7C;
    float parameter80;
    unsigned char unknown84[4];
    float parameter88;
};
struct SupervisorType34DeviceView
{
    unsigned char unknown00[8];
    IDirect3DDevice8 *device;
};
extern AnmManager *g_AnmManager;

int __fastcall EffectInitializeCallbackType34(Effect *effect)
{
    effect->vertices = g_ZunMemory.Alloc(sizeof(EffectType34VerticesView), "Effect");
    if (effect->vertices == 0)
        return -1;
    memset(effect->vertices, 0, sizeof(EffectType34VerticesView));
    EffectType34SetupView *state = reinterpret_cast<EffectType34SetupView *>(effect);
    state->origin48 = effect->position;
    state->parameter80 = effect->velocity.x;
    state->axis54.x = 0.0f;
    state->axis54.y = 0.0f;
    state->axis54.z = 1.0f;
    state->axis60.x = 0.0f;
    state->axis60.y = -1.0f;
    state->axis60.z = 0.0f;
    state->parameter88 = effect->velocity.z;
    effect->value94 = 0;
    effect->value98 = 0;
    effect->value9C = 1.0f;
    effect->value8C = 32;
    state->radius7C = 16.0f;
    effect->unknown0C9 = 1;
    EffectType34VerticesView *vertices = static_cast<EffectType34VerticesView *>(effect->vertices);
    vertices->width50 = 0.0f;
    vertices->state00 = 0;
    return 0;
}

int __fastcall EffectDrawCallbackType34(Effect *effect)
{
    EffectType34VerticesView *vertices = static_cast<EffectType34VerticesView *>(effect->vertices);
    float angle = -3.1415927f;
    AddNormalizeAngle(angle, -1.5707964f);
    unsigned char alpha;
    if (vertices->state00 == 0)
        alpha = 255;
    else
        alpha = static_cast<unsigned char>(255 - static_cast<int>(effect->timer) * 255 / 50);
    VertexDiffuseXyzrhw *vertex = vertices->fan54;
    const Float3 *position = reinterpret_cast<const Float3 *>(&effect->position);
    vertex->pos = *position;
    vertex->pos.x = g_GameManager.TransformPopupX(vertex->pos.x);
    vertex->pos.y = g_GameManager.TransformPopupY(vertex->pos.y);
    vertex->diffuse = (static_cast<unsigned long>(alpha) << 22) | 0x00FF0000;
    vertex->rhw = 1.0f;
    ++vertex;
    const unsigned long fanColor = (static_cast<unsigned long>(alpha) << 24) | 0x00FF0000;
    for (int i = 0; i < 32; ++i)
    {
        if (angle >= 3.1415927f)
            angle -= 6.2831855f;
        vertex->pos.z = 0.0f;
        vertex->pos.FromAngleMagnitude(angle, vertices->width50);
        vertex->pos += *position;
        vertex->pos.x = g_GameManager.TransformPopupX(vertex->pos.x);
        vertex->pos.y = g_GameManager.TransformPopupY(vertex->pos.y);
        vertex->diffuse = fanColor;
        vertex->rhw = 1.0f;
        angle += 0.2026834f;
        ++vertex;
    }
    g_AnmManager->DrawTriangleFan(reinterpret_cast<AnmVm *>(effect->vms), vertices->fan54, 33);
    g_AnmManager->FlushVertexBuffer();
    SupervisorType34DeviceView *supervisor = reinterpret_cast<SupervisorType34DeviceView *>(&g_Supervisor);
    supervisor->device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    supervisor->device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    supervisor->device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    supervisor->device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
    supervisor->device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
    supervisor->device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    const unsigned long lineColor = (static_cast<unsigned long>(alpha) << 24) | 0x00FFFFFF;
    int layer = 0;
    do
    {
        vertex = vertices->lines2E8[layer];
        float *offset = vertices->jitterCE8[layer];
        for (int i = 0; i < 32; ++i)
        {
            if (angle >= 3.1415927f)
                angle -= 6.2831855f;
            vertex->pos.z = 0.0f;
            vertex->pos.FromAngleMagnitude(angle, *offset + vertices->width50);
            vertex->pos += *position;
            vertex->pos.x = g_GameManager.TransformPopupX(vertex->pos.x);
            vertex->pos.y = g_GameManager.TransformPopupY(vertex->pos.y);
            vertex->diffuse = lineColor;
            vertex->rhw = 1.0f;
            angle += 0.2026834f;
            ++vertex;
            ++offset;
        }
        supervisor->device->SetVertexShader(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
        supervisor->device->DrawPrimitiveUP(D3DPT_LINESTRIP, 31, vertices->lines2E8[layer], sizeof(VertexDiffuseXyzrhw));
        if (vertices->state00 == 0)
            break;
        angle += 0.05067085f;
        ++layer;
    } while (layer < 4);
    g_AnmManager->ClearVertexShader();
    g_AnmManager->ClearSprite();
    g_AnmManager->ClearTexture();
    g_AnmManager->ClearColorOp();
    g_AnmManager->ClearBlendMode();
    g_AnmManager->ClearZWrite();
    supervisor->device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    supervisor->device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    supervisor->device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    supervisor->device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    vertices->unknown4C = 0;
    return 1;
}
