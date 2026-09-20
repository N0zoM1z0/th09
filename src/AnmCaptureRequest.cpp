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
