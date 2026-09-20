#pragma once

#include <stddef.h>

struct AnmCaptureRequestView
{
    unsigned char unknown000[0x08];
    int pendingCaptureIndex08;
    unsigned char unknown00C[0x2B2914 - 0x0C];
    int srcX2B2914;
    int srcY2B2918;
    int srcW2B291C;
    int srcH2B2920;
    int dstX2B2924;
    int dstY2B2928;
    int dstW2B292C;
    int dstH2B2930;

    int QueueCaptureRequest(
        int captureIndex,
        int srcX, int srcY, int srcW, int srcH,
        int dstX, int dstY, int dstW, int dstH);
};

typedef char AnmCaptureRequestPendingAt08[
    (offsetof(AnmCaptureRequestView, pendingCaptureIndex08) == 0x08) ? 1 : -1];
typedef char AnmCaptureRequestSrcAt2B2914[
    (offsetof(AnmCaptureRequestView, srcX2B2914) == 0x2B2914) ? 1 : -1];
typedef char AnmCaptureRequestDstEndAt2B2930[
    (offsetof(AnmCaptureRequestView, dstH2B2930) == 0x2B2930) ? 1 : -1];
