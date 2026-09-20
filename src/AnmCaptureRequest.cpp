#include "AnmCaptureRequest.hpp"

int AnmCaptureRequestView::QueueCaptureRequest(
    int captureIndex,
    int srcX, int srcY, int srcW, int srcH,
    int dstX, int dstY, int dstW, int dstH)
{
    if (pendingCaptureIndex08 >= 0)
        return -1;

    pendingCaptureIndex08 = captureIndex;
    srcX2B2914 = srcX;
    srcY2B2918 = srcY;
    srcW2B291C = srcW;
    srcH2B2920 = srcH;
    dstX2B2924 = dstX;
    dstY2B2928 = dstY;
    dstW2B292C = dstW;
    dstH2B2930 = dstH;
    return 0;
}
