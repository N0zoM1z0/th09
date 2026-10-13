#include "AnmManager.hpp"
#include "AsciiGameManagerView.hpp"
#include "AsciiManager.hpp"
#include "ExAttackController.hpp"
#include "ExAttackType19ExtraView.hpp"

#include <stddef.h>

// Both Type19 and Type21 draw routes use the target-proven common prefix
// of the Type19 initializer/update dynamic data record (through +0x684).
// Type21 also owns an extra angle at +0x688, unused by this draw callback.
typedef ExAttackType19Vertex ExAttackTrailVertex;
typedef ExAttackType19Extra ExAttackTrailExtra;

struct ExAttackTrailRecordView
{
    int unknown00;
    int opponentSide04;
    int side08;
    int active0C;
    ZunTimer timer10;
    AnmVm *vms1C;
    Float3 position20;
    int vmCount2C;
    void *next30;
    ExAttackTrailExtra *extra34;
};

int __fastcall ExAttackDrawCallbackType19_21(ExAttackRecord *base)
{
    ExAttackTrailRecordView *record =
        reinterpret_cast<ExAttackTrailRecordView *>(base);
    ExAttackTrailExtra *extra = record->extra34;

    if (extra->state00 >= 1)
    {
        unsigned char alpha =
            *(reinterpret_cast<unsigned char *>(record->vms1C) + 0x1F3);

        ExAttackTrailVertex *vertex = &extra->vertices4C[0];
        vertex->pos = extra->history3E8[0];
        float angle = -3.1415927f;
        vertex->pos.x = g_GameManager.TransformPopupX(vertex->pos.x);
        vertex->pos.y = g_GameManager.TransformPopupY(vertex->pos.y);
        vertex->diffuse =
            (static_cast<unsigned long>(alpha) << 24) | 0x00FFFFFFUL;

        ++vertex;
        Float3 *history = &extra->history3E8[1];
        int remaining = 31;
        do
        {
            if (angle >= 3.1415927f)
                angle -= 6.2831855f;

            vertex->pos.z = 0.0f;
            vertex->pos = *history;
            vertex->pos.x = g_GameManager.TransformPopupX(vertex->pos.x);
            vertex->pos.y = g_GameManager.TransformPopupY(vertex->pos.y);
            vertex->diffuse = 0x008080FFUL;

            ++vertex;
            ++history;
            angle += 0.2026834f;
        }
        while (--remaining != 0);

        *vertex = extra->vertices4C[1];

        g_AnmManager->DrawTexturedTriangleFan(
            record->vms1C,
            reinterpret_cast<VertexTex1DiffuseXyzrhw *>(
                extra->vertices4C),
            33);
    }

    extra->drawPending684 = 0;
    return 0;
}
