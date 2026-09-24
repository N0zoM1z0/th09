#include "TextHelper.hpp"

#include <string.h>

bool TextHelper::CopyTextToSurface(IDirect3DSurface8 *outSurface)
{
    D3DLOCKED_RECT lockedRect;
    unsigned char *srcBuf;
    D3DSURFACE_DESC outSurfaceDesc;
    size_t srcWidthBytes;
    int curHeight;
    RECT rectToLock;
    int dstWidthBytes;
    unsigned char *dstBuf;

    if (!IsAllocated())
        return false;
    outSurface->GetDesc(&outSurfaceDesc);
    rectToLock.left = 0;
    rectToLock.top = 0;
    rectToLock.right = GetWidth();
    rectToLock.bottom = GetHeight();
    if (outSurface->LockRect(&lockedRect, &rectToLock, 0))
        return false;
    dstWidthBytes = lockedRect.Pitch;
    srcWidthBytes = GetImageWidthInBytes();
    srcBuf = GetBuffer();
    dstBuf = (unsigned char *)lockedRect.pBits;
    if (outSurfaceDesc.Format == GetFormat())
    {
        for (curHeight = 0; curHeight < GetHeight(); ++curHeight)
        {
            memcpy(dstBuf, srcBuf, srcWidthBytes);
            srcBuf += srcWidthBytes;
            dstBuf += dstWidthBytes;
        }
    }
    outSurface->UnlockRect();
    return true;
}
