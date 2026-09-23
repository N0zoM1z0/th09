#include "FileSystem.hpp"
#include "ZunMemory.hpp"

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

extern DecryptParams g_DecryptParams[8];
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
        while (i < 8 &&
               rawData[3] !=
                   g_DecryptParams[i].key - (i << 4) - 0x10)
        {
            i++;
        }

        if (i >= 8)
            return rawData;

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
