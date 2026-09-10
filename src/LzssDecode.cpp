#include "Lzss.hpp"

#include <windows.h>

#define LZSS_OFFSET_BITS 13
#define LZSS_LENGTH_BITS 4
#define LZSS_DICTSIZE (1 << LZSS_OFFSET_BITS)
#define LZSS_DICTSIZE_MASK (LZSS_DICTSIZE - 1)
#define LZSS_DICTPOS_MOD(pos, amount) (((pos) + (amount)) & LZSS_DICTSIZE_MASK)

extern unsigned char g_LzssDictionary[LZSS_DICTSIZE];

#define DECODE_ADVANCE_READ_HEAD \
    inBitMask >>= 1; \
    if (inBitMask == 0) \
        inBitMask = 0x80;

#define DECODE_WRITE_BYTE(data) \
    *outCursor++ = (unsigned char)(data); \
    g_LzssDictionary[dictHead] = (unsigned char)(data); \
    dictHead = LZSS_DICTPOS_MOD(dictHead, 1);

#define DECODE_HANDLE_FETCH \
    if (inBitMask == 0x80) \
    { \
        currByte = *inCursor; \
        if (inCursor - in >= size) \
            currByte = 0; \
        else \
            inCursor++; \
        checksum += currByte; \
    }

#define DECODE_UNPACK_BIT \
    DECODE_HANDLE_FETCH; \
    inBits = currByte & inBitMask; \
    DECODE_ADVANCE_READ_HEAD;

#define DECODE_UNPACK_BITS(bitCount) \
    outBitMask = 1 << ((bitCount) - 1); \
    inBits = 0; \
    while (outBitMask != 0) \
    { \
        DECODE_HANDLE_FETCH; \
        if ((currByte & inBitMask) != 0) \
            inBits |= outBitMask; \
        outBitMask >>= 1; \
        DECODE_ADVANCE_READ_HEAD; \
    }

unsigned char *Lzss::Decode(unsigned char *in, int inSize,
                            unsigned char *out, int outSize)
{
    unsigned char inBitMask;
    unsigned int currByte;
    unsigned int checksum;
    int size;
    unsigned char *inCursor, *outCursor;
    unsigned int dictHead;
    unsigned int inBits;
    int matchOffset;
    int matchLength;
    int i;
    unsigned int dictValue;
    unsigned int outBitMask;

    inBitMask = 0x80;
    currByte = 0;
    checksum = 0;
    size = inSize;

    if (out == NULL)
    {
        out = (unsigned char *)GlobalAlloc(0, outSize);
        if (out == NULL)
            return NULL;
    }

    inCursor = in;
    outCursor = out;
    dictHead = 1;

    for (;;)
    {
        DECODE_UNPACK_BIT;
        if (inBits != 0)
        {
            DECODE_UNPACK_BITS(8);
            DECODE_WRITE_BYTE(inBits);
        }
        else
        {
            DECODE_UNPACK_BITS(13);
            matchOffset = inBits;
            if (matchOffset == 0)
                break;

            DECODE_UNPACK_BITS(4);
            matchLength = inBits + 2;
            for (i = 0; i <= matchLength; i++)
            {
                dictValue = g_LzssDictionary[LZSS_DICTPOS_MOD(matchOffset, i)];
                DECODE_WRITE_BYTE(dictValue);
            }
        }
    }

    while (inBitMask != 0x80)
    {
        DECODE_UNPACK_BIT;
    }

    return out;
}
