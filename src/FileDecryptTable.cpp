#include "FileSystem.hpp"
#include "ZunMemory.hpp"

#include <stddef.h>

namespace FileSystem
{
struct DecryptParams
{
    unsigned char key;
    unsigned char xorValue;
    unsigned char xorValueInc;
    unsigned char unused;
    int chunkSize;
    int maxBytesToDecrypt;
};
// Original TH09 wire table contains eight 12-byte decryption parameter rows.
// Keep the field offsets tied to the independently reviewed target layout.
typedef char DecryptParamsSizeIs0C[(sizeof(DecryptParams) == 0x0C) ? 1 : -1];
typedef char DecryptKeyAt00[(offsetof(DecryptParams, key) == 0x00) ? 1 : -1];
typedef char DecryptXorAt01[(offsetof(DecryptParams, xorValue) == 0x01) ? 1 : -1];
typedef char DecryptIncrementAt02[(offsetof(DecryptParams, xorValueInc) == 0x02) ? 1 : -1];
typedef char DecryptChunkAt04[(offsetof(DecryptParams, chunkSize) == 0x04) ? 1 : -1];
typedef char DecryptMaxBytesAt08[(offsetof(DecryptParams, maxBytesToDecrypt) == 0x08) ? 1 : -1];

extern DecryptParams g_DecryptParams[8];
typedef char DecryptParamRowsTotal0x60[
    (sizeof(g_DecryptParams) == 0x60) ? 1 : -1];
extern unsigned char g_CryptSignature[3];

unsigned char *TryDecryptFromTable(
    unsigned char *inData, int *unusedFileSize, int size)
{
    unsigned char *rawData = inData;
    unsigned char *decryptedData;

    if (rawData[0] == g_CryptSignature[0] - 0x20 &&
        rawData[1] == g_CryptSignature[1] - 0x40 &&
        rawData[2] == g_CryptSignature[2] - 0x60)
    {
        unsigned int i = 0;
        unsigned int targetKey = rawData[3];

        if (targetKey != g_DecryptParams[0].key - 0x10)
        {
            unsigned int rowOffset = 0;
            unsigned int keyBias = 0;

            for (;;)
            {
                if (rowOffset >= sizeof(g_DecryptParams))
                    return rawData;

                unsigned int key =
                    *reinterpret_cast<unsigned char *>(
                        reinterpret_cast<unsigned char *>(g_DecryptParams) +
                        rowOffset + sizeof(DecryptParams));
                rowOffset += sizeof(DecryptParams);
                keyBias += 0x10;
                key -= keyBias;
                key -= 0x10;
                ++i;

                if (targetKey != key)
                    continue;
                if (i >= 8)
                    return rawData;
                break;
            }
        }

        decryptedData = Decrypt(
            rawData + 4, size - 4,
            g_DecryptParams[i].xorValue,
            g_DecryptParams[i].xorValueInc,
            g_DecryptParams[i].chunkSize,
            g_DecryptParams[i].maxBytesToDecrypt);
        g_ZunMemory.Free(inData);
        return decryptedData;
    }

    return rawData;
}
} // namespace FileSystem
