#include "FileSystem.hpp"
#include "ZunMemory.hpp"

#include <string.h>

namespace FileSystem
{
unsigned char *Decrypt(unsigned char *inData, int size, unsigned char xorValue,
                       unsigned char xorValueInc, int chunkSize, int maxBytes)
{
    int i;
    unsigned char *outCursorBackup;
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

        outCursorBackup = outCursor;
        outCursor = &outCursor[chunkSize - 1];

        for (i = (chunkSize + 1) / 2; i > 0; i--, inCursor++)
        {
            *outCursor = *inCursor ^ xorValue;
            outCursor -= 2;
            xorValue += xorValueInc;
        }

        outCursor = &outCursorBackup[chunkSize - 2];
        for (i = chunkSize / 2; i > 0; i--, inCursor++)
        {
            *outCursor = *inCursor ^ xorValue;
            outCursor -= 2;
            xorValue += xorValueInc;
        }

        size -= chunkSize;
        outCursor = &outCursorBackup[chunkSize];
        maxBytes -= chunkSize;
    }

    size += numUnencrypted;
    if (size > 0)
        memcpy(outCursor, inCursor, size);

    return out;
}
} // namespace FileSystem
