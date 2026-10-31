#include "Lzss.hpp"

#include <windows.h>

#define LZSS_OFFSET_BITS 13
#define LZSS_LENGTH_BITS 4
#define LZSS_DICTSIZE (1 << LZSS_OFFSET_BITS)
#define LZSS_BREAKEVEN 3
#define LZSS_LOOKAHEAD_MAX ((1 << LZSS_LENGTH_BITS) + LZSS_BREAKEVEN - 1)
#define LZSS_DICTSIZE_MASK (LZSS_DICTSIZE - 1)
#define LZSS_DICTPOS_MOD(pos, amount) (((pos) + (amount)) & LZSS_DICTSIZE_MASK)

struct LzssTreeNode
{
    int parent;
    int left;
    int right;
};

extern LzssTreeNode g_LzssTree[0x2001];
extern unsigned char g_LzssDictionary[0x2000];

typedef char LzssTreeNodeSizeIs0C[sizeof(LzssTreeNode) == 0x0C ? 1 : -1];

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

void Lzss::InitTree(int root)
{
    g_LzssTree[0x2000].right = root;
    g_LzssTree[root].parent = 0x2000;
    g_LzssTree[root].right = 0;
    g_LzssTree[root].left = 0;
}

void Lzss::InitEncoderState()
{
    int i;

    for (i = 0; i < 0x2000; i++)
        g_LzssDictionary[i] = 0;

    for (i = 0; i < 0x2001; i++)
    {
        g_LzssTree[i].parent = 0;
        g_LzssTree[i].left = 0;
        g_LzssTree[i].right = 0;
    }
}

void Lzss::ContractNode(int oldNode, int newNode)
{
    g_LzssTree[newNode].parent = g_LzssTree[oldNode].parent;

    if (g_LzssTree[g_LzssTree[oldNode].parent].right == oldNode)
        g_LzssTree[g_LzssTree[oldNode].parent].right = newNode;
    else
        g_LzssTree[g_LzssTree[oldNode].parent].left = newNode;

    g_LzssTree[oldNode].parent = 0;
}

void Lzss::ReplaceNode(int oldNode, int newNode)
{
    int parent = g_LzssTree[oldNode].parent;

    if (g_LzssTree[parent].left == oldNode)
        g_LzssTree[parent].left = newNode;
    else
        g_LzssTree[parent].right = newNode;

    g_LzssTree[newNode] = g_LzssTree[oldNode];
    g_LzssTree[g_LzssTree[newNode].left].parent = newNode;
    g_LzssTree[g_LzssTree[newNode].right].parent = newNode;
    g_LzssTree[oldNode].parent = 0;
}

int Lzss::FindNextNode(int node)
{
    int next = g_LzssTree[node].left;

    while (g_LzssTree[next].right != 0)
        next = g_LzssTree[next].right;

    return next;
}

int Lzss::AddString(int newNode, int *matchPosition)
{
    int i;
    int *child;
    int delta;

    if (newNode == 0)
        return 0;

    int testNode = g_LzssTree[LZSS_DICTSIZE].right;
    int matchLength = 0;

    for (;;)
    {
        for (i = 0; i < LZSS_LOOKAHEAD_MAX; i++)
        {
            delta = g_LzssDictionary[LZSS_DICTPOS_MOD(newNode, i)] -
                    g_LzssDictionary[LZSS_DICTPOS_MOD(testNode, i)];
            if (delta != 0)
                break;
        }

        if (i >= matchLength)
        {
            matchLength = i;
            *matchPosition = testNode;
            if (matchLength >= LZSS_LOOKAHEAD_MAX)
            {
                ReplaceNode(testNode, newNode);
                return matchLength;
            }
        }

        if (delta >= 0)
            child = &g_LzssTree[testNode].right;
        else
            child = &g_LzssTree[testNode].left;

        if (*child == 0)
        {
            *child = newNode;
            g_LzssTree[newNode].parent = testNode;
            g_LzssTree[newNode].right = 0;
            g_LzssTree[newNode].left = 0;
            return matchLength;
        }

        testNode = *child;
    }
}

void Lzss::DeleteString(int p)
{
    if (g_LzssTree[p].parent == 0)
        return;

    if (g_LzssTree[p].right == 0)
        ContractNode(p, g_LzssTree[p].left);
    else if (g_LzssTree[p].left == 0)
        ContractNode(p, g_LzssTree[p].right);
    else
    {
        int replacement = FindNextNode(p);
        DeleteString(replacement);
        ReplaceNode(p, replacement);
    }
}

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
