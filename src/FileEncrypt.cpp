#include "FileSystem.hpp"
#include "ZunMemory.hpp"

#include <string.h>

namespace FileSystem
{
unsigned char *Encrypt(unsigned char *inData, int size, unsigned char xorValue,
                       unsigned char xorValueInc, int chunkSize, int maxBytes)
{
    int i;
    unsigned char *inCursorBackup;
    int numUnencrypted = (size % chunkSize < chunkSize / 4) ? size % chunkSize : 0;
    unsigned char *inCursor = inData;
    unsigned char *out =
        (unsigned char *)g_ZunMemory.Alloc(size, "./system\\global.h");
    unsigned char *outCursor = out;

    if (out == NULL)
        return inData;

    numUnencrypted += size & 1;
    size -= numUnencrypted;

    while (size > 0 && maxBytes > 0)
    {
        if (size < chunkSize)
            chunkSize = size;

        inCursorBackup = inCursor;
        inCursor = &inCursor[chunkSize - 1];

        for (i = (chunkSize + 1) / 2; i > 0; i--, outCursor++)
        {
            *outCursor = *inCursor ^ xorValue;
            inCursor -= 2;
            xorValue += xorValueInc;
        }

        inCursor = &inCursorBackup[chunkSize - 2];
        for (i = chunkSize / 2; i > 0; i--, outCursor++)
        {
            *outCursor = *inCursor ^ xorValue;
            inCursor -= 2;
            xorValue += xorValueInc;
        }

        size -= chunkSize;
        inCursor = &inCursorBackup[chunkSize];
        maxBytes -= chunkSize;
    }

    size += numUnencrypted;
    if (size > 0)
        memcpy(outCursor, inCursor, size);

    return out;
}
} // namespace FileSystem
