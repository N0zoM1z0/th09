#include "Lzss.hpp"

#include <windows.h>

#define LZSS_OFFSET_BITS 13
#define LZSS_LENGTH_BITS 4
#define LZSS_DICTSIZE (1 << LZSS_OFFSET_BITS)
#define LZSS_BREAKEVEN 3
#define LZSS_LOOKAHEAD_MAX ((1 << LZSS_LENGTH_BITS) + LZSS_BREAKEVEN - 1)
#define LZSS_DICTSIZE_MASK (LZSS_DICTSIZE - 1)
#define LZSS_DICTPOS_MOD(pos, amount) (((pos) + (amount)) & LZSS_DICTSIZE_MASK)

extern unsigned char g_LzssDictionary[LZSS_DICTSIZE];

#define ENCODE_ADVANCE_WRITE_HEAD \
    outBitMask >>= 1; \
    if (outBitMask == 0) \
    { \
        *outCursor++ = (unsigned char)outBits; \
        outBits = 0; \
        outBitMask = 0x80; \
    }

#define ENCODE_PACK_BIT(bit) \
    if (bit) \
        outBits |= outBitMask; \
    ENCODE_ADVANCE_WRITE_HEAD;

#define ENCODE_PACK_BITS(bitCount, writeOneIf) \
    bitfieldMask = 1 << ((bitCount) - 1); \
    while (bitfieldMask != 0) \
    { \
        if (writeOneIf) \
            outBits |= outBitMask; \
        ENCODE_ADVANCE_WRITE_HEAD; \
        bitfieldMask >>= 1; \
    }

unsigned char *Lzss::Encode(unsigned char *in, int inSize, int *outSize)
{
    unsigned char outBitMask;
    unsigned int outBits;
    unsigned char *out;
    unsigned char *inCursor;
    unsigned char *outCursor;
    unsigned int dictHead;
    int i;
    int maxMatchLength;
    int matchLength;
    int matchOffset;
    int bytesToCopyToDict;
    int dictValue;
    unsigned int bitfieldMask;

    outBitMask = 0x80;
    outBits = 0;

    out = (unsigned char *)GlobalAlloc(0, inSize * 2);
    if (out == NULL)
        return NULL;

    inCursor = in;
    outCursor = out;
    *outSize = 0;
    InitEncoderState();
    dictHead = 1;

    for (i = 0; i < LZSS_LOOKAHEAD_MAX; i++)
    {
        if (inCursor - in >= inSize)
            dictValue = -1;
        else
            dictValue = *inCursor++;

        if (dictValue == -1)
            break;

        g_LzssDictionary[dictHead + i] = (unsigned char)dictValue;
    }

    maxMatchLength = i;
    InitTree(dictHead);
    matchLength = 0;
    matchOffset = 0;

    while (maxMatchLength > 0)
    {
        if (matchLength > maxMatchLength)
            matchLength = maxMatchLength;

        if (matchLength <= LZSS_BREAKEVEN - 1)
        {
            bytesToCopyToDict = 1;
            ENCODE_PACK_BIT(1);
            ENCODE_PACK_BITS(8, (bitfieldMask & g_LzssDictionary[dictHead]) != 0);
        }
        else
        {
            ENCODE_PACK_BIT(0);
            ENCODE_PACK_BITS(LZSS_OFFSET_BITS, (bitfieldMask & matchOffset) != 0);
            ENCODE_PACK_BITS(LZSS_LENGTH_BITS,
                             (bitfieldMask & (matchLength - LZSS_BREAKEVEN)) != 0);
            bytesToCopyToDict = matchLength;
        }

        for (i = 0; i < bytesToCopyToDict; i++)
        {
            DeleteString(LZSS_DICTPOS_MOD(dictHead, LZSS_LOOKAHEAD_MAX));

            if (inCursor - in >= inSize)
                dictValue = -1;
            else
                dictValue = *inCursor++;

            if (dictValue == -1)
                maxMatchLength--;
            else
                g_LzssDictionary[LZSS_DICTPOS_MOD(dictHead, LZSS_LOOKAHEAD_MAX)] =
                    (unsigned char)dictValue;

            dictHead = LZSS_DICTPOS_MOD(dictHead, 1);
            if (maxMatchLength != 0)
                matchLength = AddString(dictHead, &matchOffset);
        }
    }

    ENCODE_PACK_BIT(0);
    ENCODE_PACK_BITS(LZSS_OFFSET_BITS, false);

    *outSize = outCursor - out;
    return out;
}
