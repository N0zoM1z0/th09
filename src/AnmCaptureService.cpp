#include "AnmManager.hpp"

#include <stddef.h>

namespace
{
struct AnmCaptureServiceView
{
    unsigned char unknown000000[0x08];
    int pendingSurfaceCapture08;
    unsigned char unknown00000C[0x2B28F0 - 0x0C];

    int pendingTextureCapture2B28F0;
    int textureSrcX2B28F4;
    int textureSrcY2B28F8;
    int textureSrcW2B28FC;
    int textureSrcH2B2900;
    int textureDstX2B2904;
    int textureDstY2B2908;
    int textureDstW2B290C;
    int textureDstH2B2910;

    int surfaceSrcX2B2914;
    int surfaceSrcY2B2918;
    int surfaceSrcW2B291C;
    int surfaceSrcH2B2920;
    int surfaceDstX2B2924;
    int surfaceDstY2B2928;
    int surfaceDstW2B292C;
    int surfaceDstH2B2930;
};

typedef char AnmCaptureServiceSurfacePendingAt08[
    (offsetof(AnmCaptureServiceView, pendingSurfaceCapture08) == 0x08) ? 1 : -1];
typedef char AnmCaptureServiceTexturePendingAt2B28F0[
    (offsetof(AnmCaptureServiceView, pendingTextureCapture2B28F0) == 0x2B28F0) ? 1 : -1];
typedef char AnmCaptureServiceTextureEndAt2B2910[
    (offsetof(AnmCaptureServiceView, textureDstH2B2910) == 0x2B2910) ? 1 : -1];
typedef char AnmCaptureServiceSurfaceEndAt2B2930[
    (offsetof(AnmCaptureServiceView, surfaceDstH2B2930) == 0x2B2930) ? 1 : -1];
}

void AnmManager::ServiceCaptureRequests()
{
    AnmCaptureServiceView *view =
        reinterpret_cast<AnmCaptureServiceView *>(this);

    if (view->pendingTextureCapture2B28F0 >= 0)
    {
        this->CaptureToTexture(
            view->pendingTextureCapture2B28F0,
            view->textureSrcX2B28F4,
            view->textureSrcY2B28F8,
            view->textureSrcW2B28FC,
            view->textureSrcH2B2900,
            view->textureDstX2B2904,
            view->textureDstY2B2908,
            view->textureDstW2B290C,
            view->textureDstH2B2910);
        view->pendingTextureCapture2B28F0 = -1;
    }

    if (view->pendingSurfaceCapture08 >= 0)
    {
        this->CaptureToSurface(
            view->pendingSurfaceCapture08,
            view->surfaceSrcX2B2914,
            view->surfaceSrcY2B2918,
            view->surfaceSrcW2B291C,
            view->surfaceSrcH2B2920,
            view->surfaceDstX2B2924,
            view->surfaceDstY2B2928,
            view->surfaceDstW2B292C,
            view->surfaceDstH2B2930);
        view->pendingSurfaceCapture08 = -1;
    }
}
