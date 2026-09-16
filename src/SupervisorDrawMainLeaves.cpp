#include <stddef.h>

struct SupervisorDrawMainAnmManagerView
{
    void ResetDrawState();
    void *GetSurface(int surfaceIndex);
};

struct SupervisorDrawMainAsciiManagerView
{
    void SetAlpha(int alpha);
};

namespace
{
struct SupervisorDrawMainAnmStateView
{
    unsigned char unknown000000[0x123FC];
    void *surfaces[32];
    unsigned char unknown1247C[0x128E4 - 0x1247C];
    int spritesToDraw;
    unsigned char vertexStorage[0x2B28E8 - 0x128E8];
    void *vertexBufferEndPtr;
    void *vertexBufferStartPtr;
};

struct SupervisorDrawMainAsciiLayout
{
    unsigned char unknown0000[0x826B];
    unsigned char colorAlpha;
};

typedef char SupervisorDrawMainSurfaceAt123FC[
    (offsetof(SupervisorDrawMainAnmStateView, surfaces) == 0x123FC) ? 1 : -1];
typedef char SupervisorDrawMainSpritesAt128E4[
    (offsetof(SupervisorDrawMainAnmStateView, spritesToDraw) == 0x128E4) ? 1 : -1];
typedef char SupervisorDrawMainVertexStorageAt128E8[
    (offsetof(SupervisorDrawMainAnmStateView, vertexStorage) == 0x128E8) ? 1 : -1];
typedef char SupervisorDrawMainVertexEndAt2B28E8[
    (offsetof(SupervisorDrawMainAnmStateView, vertexBufferEndPtr) == 0x2B28E8) ? 1 : -1];
typedef char SupervisorDrawMainVertexStartAt2B28EC[
    (offsetof(SupervisorDrawMainAnmStateView, vertexBufferStartPtr) == 0x2B28EC) ? 1 : -1];
typedef char SupervisorDrawMainAsciiAlphaAt826B[
    (offsetof(SupervisorDrawMainAsciiLayout, colorAlpha) == 0x826B) ? 1 : -1];
}

void SupervisorDrawMainAnmManagerView::ResetDrawState()
{
    SupervisorDrawMainAnmStateView *anm =
        reinterpret_cast<SupervisorDrawMainAnmStateView *>(this);
    void *vertexStart = &anm->vertexStorage[0];
    anm->spritesToDraw = 0;
    anm->vertexBufferEndPtr = vertexStart;
    anm->vertexBufferStartPtr = vertexStart;
}

void *SupervisorDrawMainAnmManagerView::GetSurface(int surfaceIndex)
{
    return reinterpret_cast<SupervisorDrawMainAnmStateView *>(this)
        ->surfaces[surfaceIndex];
}

void SupervisorDrawMainAsciiManagerView::SetAlpha(int alpha)
{
    reinterpret_cast<SupervisorDrawMainAsciiLayout *>(this)->colorAlpha =
        static_cast<unsigned char>(alpha);
}
