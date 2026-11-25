#include "GameManagerRuntimeView.hpp"
#include "PlayStats.hpp"
#include "RngRuntimeLeaves.hpp"
#include "SupervisorNetworkState.hpp"

#include "GameManagerOpponentSelectionView.hpp"

extern RngRuntimeView g_Rng;
extern unsigned char g_TitleCharacterUnlocked[16];
extern int g_FrontRuntimeValues90[4];
OpponentSelectionEntry g_OpponentEntries[17] = {
    {   9,   0,   0, 200,   0,   0,   2,  86 },
    {   9,   0,   1, 200,   0,   0,   3,  86 },
    {   9,   1,   2, 200,   0,   0,   4,  86 },
    {   9,   2,   3, 200,   0,   0,   5,  86 },
    {   9,   3,   4, 200,   0,   0,   6,  86 },
    {   9,   4,   5, 200,   0,   0,   7,  86 },
    {   9,   5,   6, 200,   0,   0,   8,  86 },
    {   9,   6,   7, 200,   0,   0,   9,  86 },
    {   9,   0,   8, 200,   0,   0,  10,  86 },
    {   9,  13,   9, 200,   0,   0,  11,  86 },
    {   9,   9,  10, 200,   0,   0,  12,  86 },
    {   9,  14,  11, 200,   0,   0,  13,  86 },
    {   9,  11,  12, 200,   0,   0,  14,  86 },
    {   9,  15,  13, 200,   0,   0,  15,  86 },
    {   9,   5,  14, 200,   0,   0,   8,  86 },
    {   9,   5,  15, 200,   0,   0,   8,  86 },
    {  -1,  -1,  -1,  -1,   0,   0,   0,   0 },
};

static OpponentSelectionEntry s_OpponentList00[21] = {
    {   0,   4,   5,  50,  51,  52,   7, 128 },
    {   0,   6,   7,  70,  71,  72,   9,  96 },
    {   1,   4,   5,  50,  51,  52,   7,  96 },
    {   1,   6,   7,  70,  71,  72,   9, 128 },
    {   1,   5,   6,  60,  61,  62,   8,  32 },
    {   2,   5,   6,  60,  61,  62,   8, 128 },
    {   2,   7,   8,  80,  81,  82,  10,  96 },
    {   3,   5,   6,  60,  61,  62,   8, 128 },
    {   3,   7,   8,  80,  81,  82,  10,  96 },
    {   3,   3,   4,  40,  41,  42,   6,  32 },
    {   4,   3,   4,  40,  41,  42,   6, 128 },
    {   4,   2,   3,  30,  31,  32,   5, 128 },
    {   4,   1,   2,  20,  21,  22,   4, 128 },
    {   4,   0,   1,  10,  11,  12,   3, 128 },
    {   5,   0,   1,  13,  14,  15,   3, 128 },
    {   5,   1,   2,  23,  24,  25,   4, 128 },
    {   6,   9,  10, 100, 101, 102,  12, 256 },
    {   7,  11,  12, 120, 121, 122,  14, 256 },
    {   8,  12,  13, 130, 131, 132,  15, 256 },
    {   9,   0,  -1, 200,   0,   0,   2, 256 },
    {  -1,  -1,  -1,  -1,   0,   0,   0,   0 },
};

static OpponentSelectionEntry s_OpponentList01[24] = {
    {   0,   4,   5,  50,  51,  52,   7, 128 },
    {   0,   6,   7,  70,  71,  72,   9,  96 },
    {   1,   4,   5,  50,  51,  52,   7,  96 },
    {   1,   6,   7,  70,  71,  72,   9, 128 },
    {   1,   5,   6,  60,  61,  62,   8,  32 },
    {   2,   5,   6,  60,  61,  62,   8, 128 },
    {   2,   7,   8,  80,  81,  82,  10,  96 },
    {   2,   3,   4,  40,  41,  42,   6,  32 },
    {   2,   2,   3,  30,  31,  32,   5,  32 },
    {   3,   5,   6,  60,  61,  62,   8, 128 },
    {   3,   7,   8,  80,  81,  82,  10,  96 },
    {   3,   3,   4,  40,  41,  42,   6,  32 },
    {   3,   2,   3,  30,  31,  32,   5,  32 },
    {   4,   3,   4,  40,  41,  42,   6, 128 },
    {   4,   2,   3,  30,  31,  32,   5, 128 },
    {   4,   1,   2,  20,  21,  22,   4, 128 },
    {   4,   0,   0,   0,   1,   2,   2, 128 },
    {   5,   1,   2,  23,  24,  25,   4, 128 },
    {   5,   0,   0,   3,   4,   5,   2, 128 },
    {   6,   9,  10, 100, 101, 102,  12, 256 },
    {   7,  11,  12, 120, 121, 122,  14, 256 },
    {   8,  12,  13, 130, 131, 132,  15, 256 },
    {   9,   0,  -1, 200,   0,   0,   2, 256 },
    {  -1,  -1,  -1,  -1,   0,   0,   0,   0 },
};

static OpponentSelectionEntry s_OpponentList02[23] = {
    {   0,   4,   5,  50,  51,  52,   7, 128 },
    {   0,   6,   7,  70,  71,  72,   9,  96 },
    {   1,   4,   5,  50,  51,  52,   7,  96 },
    {   1,   6,   7,  70,  71,  72,   9, 128 },
    {   1,   5,   6,  60,  61,  62,   8,  32 },
    {   2,   5,   6,  60,  61,  62,   8, 128 },
    {   2,   3,   4,  40,  41,  42,   6,  32 },
    {   2,   2,   3,  30,  31,  32,   5, 128 },
    {   3,   5,   6,  60,  61,  62,   8, 128 },
    {   3,   3,   4,  40,  41,  42,   6, 128 },
    {   3,   2,   3,  30,  31,  32,   5, 128 },
    {   3,   0,   0,   0,   1,   2,   2, 128 },
    {   3,   0,   1,  10,  11,  12,   3, 128 },
    {   4,   3,   4,  40,  41,  42,   6, 128 },
    {   4,   2,   3,  30,  31,  32,   5, 128 },
    {   4,   0,   0,   0,   1,   2,   2, 128 },
    {   4,   0,   1,  10,  11,  12,   3, 128 },
    {   5,   7,   8,  80,  81,  82,  10, 256 },
    {   6,  10,  11, 110, 111, 112,  13, 256 },
    {   7,  11,  12, 120, 121, 122,  14, 256 },
    {   8,  12,  13, 130, 131, 132,  15, 256 },
    {   9,   0,  -1, 200,   0,   0,   2, 256 },
    {  -1,  -1,  -1,  -1,   0,   0,   0,   0 },
};

static OpponentSelectionEntry s_OpponentList03[24] = {
    {   0,   4,   5,  50,  51,  52,   7, 128 },
    {   0,   6,   7,  70,  71,  72,   9,  96 },
    {   1,   4,   5,  50,  51,  52,   7,  96 },
    {   1,   6,   7,  70,  71,  72,   9, 128 },
    {   1,   5,   6,  60,  61,  62,   8,  32 },
    {   2,   5,   6,  60,  61,  62,   8, 128 },
    {   2,   6,   7,  70,  71,  72,   9, 128 },
    {   2,   7,   8,  80,  81,  82,  10,  96 },
    {   2,   0,   0,   0,   1,   2,   2,  48 },
    {   2,   0,   1,  10,  11,  12,   3,  48 },
    {   3,   5,   6,  60,  61,  62,   8, 128 },
    {   3,   3,   4,  40,  41,  42,   6,  96 },
    {   3,   7,   8,  80,  81,  82,  10,  96 },
    {   3,   0,   0,   0,   1,   2,   2,  48 },
    {   3,   0,   1,  10,  11,  12,   3,  48 },
    {   4,   3,   4,  40,  41,  42,   6, 128 },
    {   4,   0,   0,   0,   1,   2,   2,  24 },
    {   4,   0,   1,  10,  11,  12,   3,  24 },
    {   5,   1,   2,  20,  21,  22,   4, 128 },
    {   6,   9,  10, 100, 101, 102,  12, 256 },
    {   7,  11,  12, 120, 121, 122,  14, 256 },
    {   8,  12,  13, 130, 131, 132,  15, 256 },
    {   9,   0,  -1, 200,   0,   0,   2, 256 },
    {  -1,  -1,  -1,  -1,   0,   0,   0,   0 },
};

static OpponentSelectionEntry s_OpponentList04[21] = {
    {   0,   4,   5,  50,  51,  52,   7, 128 },
    {   0,   6,   7,  70,  71,  72,   9,  96 },
    {   1,   4,   5,  50,  51,  52,   7,  96 },
    {   1,   6,   7,  70,  71,  72,   9, 128 },
    {   1,   5,   6,  60,  61,  62,   8,  32 },
    {   2,   5,   6,  60,  61,  62,   8, 128 },
    {   2,   7,   8,  80,  81,  82,  10,  96 },
    {   3,   5,   6,  60,  61,  62,   8, 128 },
    {   3,   7,   8,  80,  81,  82,  10,  96 },
    {   3,   2,   3,  30,  31,  32,   5, 128 },
    {   3,   0,   0,   0,   1,   2,   2, 128 },
    {   3,   0,   1,  10,  11,  12,   3, 128 },
    {   4,   2,   3,  30,  31,  32,   5, 128 },
    {   4,   0,   0,   0,   1,   2,   2, 128 },
    {   4,   0,   1,  10,  11,  12,   3, 128 },
    {   5,   1,   2,  20,  21,  22,   4, 128 },
    {   6,  10,  11, 110, 111, 112,  13, 256 },
    {   7,  11,  12, 120, 121, 122,  14, 256 },
    {   8,  12,  13, 130, 131, 132,  15, 256 },
    {   9,   0,  -1, 200,   0,   0,   2, 256 },
    {  -1,  -1,  -1,  -1,   0,   0,   0,   0 },
};

static OpponentSelectionEntry s_OpponentList05[25] = {
    {   0,   6,   7,  70,  71,  72,   9,  96 },
    {   1,   5,   6,  60,  61,  62,   8, 128 },
    {   2,   1,   2,  20,  21,  22,   4,  64 },
    {   2,   0,   1,  10,  11,  12,   3,  64 },
    {   2,   5,   6,  60,  61,  62,   8, 128 },
    {   2,   7,   8,  80,  81,  82,  10,  96 },
    {   2,   1,   2,  20,  21,  22,   4,  64 },
    {   2,   0,   1,  10,  11,  12,   3,  64 },
    {   3,   5,   6,  60,  61,  62,   8, 128 },
    {   3,   7,   8,  80,  81,  82,  10,  96 },
    {   3,   4,   3,  30,  31,  32,   5,  64 },
    {   3,   1,   2,  20,  21,  22,   4,  64 },
    {   3,   0,   1,  10,  11,  12,   3,  64 },
    {   3,   0,   0,   0,   1,   2,   2,  64 },
    {   4,   7,   8,  80,  81,  82,  10,  96 },
    {   4,   4,   3,  30,  31,  32,   5, 128 },
    {   4,   1,   2,  20,  21,  22,   4,  64 },
    {   4,   0,   1,  10,  11,  12,   3,  64 },
    {   4,   0,   0,   0,   1,   2,   2,  64 },
    {   5,   4,   4,  40,  41,  42,   6, 256 },
    {   6,   9,  10, 100, 101, 102,  12, 256 },
    {   7,   8,   9,  90,  91,  92,  11, 256 },
    {   8,  12,  13, 130, 131, 132,  15, 256 },
    {   9,   0,  -1, 200,   0,   0,   2, 256 },
    {  -1,  -1,  -1,  -1,   0,   0,   0,   0 },
};

static OpponentSelectionEntry s_OpponentList06[20] = {
    {   0,   4,   5,  50,  51,  52,   7, 128 },
    {   1,   6,   7,  70,  71,  72,   9, 128 },
    {   1,   7,   8,  80,  81,  82,  10,  64 },
    {   2,   6,   7,  70,  71,  72,   9, 128 },
    {   2,   7,   8,  80,  81,  82,  10,  96 },
    {   2,   5,   4,  40,  41,  42,   6, 128 },
    {   3,   5,   4,  40,  41,  42,   6, 128 },
    {   3,   7,   8,  80,  81,  82,  10,  96 },
    {   3,   1,   2,  20,  21,  22,   4, 128 },
    {   3,   0,   1,  10,  11,  12,   3, 128 },
    {   3,   0,   0,   0,   1,   2,   2, 128 },
    {   4,   1,   2,  20,  21,  22,   4, 128 },
    {   4,   0,   1,  10,  11,  12,   3, 128 },
    {   4,   0,   0,   0,   1,   2,   2, 128 },
    {   5,   5,   3,  30,  31,  32,   5, 128 },
    {   6,   9,  10, 100, 101, 102,  12, 256 },
    {   7,   8,   9,  90,  91,  92,  11, 256 },
    {   8,  12,  13, 130, 131, 132,  15, 256 },
    {   9,   0,  -1, 200,   0,   0,   2, 256 },
    {  -1,  -1,  -1,  -1,   0,   0,   0,   0 },
};

static OpponentSelectionEntry s_OpponentList07[21] = {
    {   0,   4,   5,  50,  51,  52,   7, 128 },
    {   1,   5,   6,  60,  61,  62,   8,  64 },
    {   1,   7,   8,  80,  81,  82,  10,  64 },
    {   2,   5,   6,  60,  61,  62,   8,  64 },
    {   2,   7,   8,  80,  81,  82,  10,  64 },
    {   2,   6,   4,  40,  41,  42,   6,  64 },
    {   2,   6,   3,  30,  31,  32,   5,  64 },
    {   3,   6,   3,  30,  31,  32,   5,  64 },
    {   3,   6,   4,  40,  41,  42,   6,  64 },
    {   3,   5,   6,  60,  61,  62,   8,  64 },
    {   3,   7,   8,  80,  81,  82,  10,  64 },
    {   3,   6,   3,  30,  31,  32,   5,  64 },
    {   3,   6,   4,  40,  41,  42,   6,  64 },
    {   4,   1,   2,  20,  21,  22,   4, 128 },
    {   4,   0,   1,  10,  11,  12,   3, 128 },
    {   5,   0,   0,   3,   4,   5,   2, 128 },
    {   6,  10,  11, 110, 111, 112,  13, 256 },
    {   7,   8,   9,  90,  91,  92,  11, 256 },
    {   8,  12,  13, 130, 131, 132,  15, 256 },
    {   9,   0,  -1, 200,   0,   0,   2, 256 },
    {  -1,  -1,  -1,  -1,   0,   0,   0,   0 },
};

static OpponentSelectionEntry s_OpponentList08[19] = {
    {   0,   4,   5,  50,  51,  52,   7, 128 },
    {   0,   6,   7,  70,  71,  72,   9, 128 },
    {   1,   4,   5,  50,  51,  52,   7, 128 },
    {   1,   6,   7,  70,  71,  72,   9, 128 },
    {   2,   5,   6,  60,  61,  62,   8, 128 },
    {   2,   1,   2,  20,  21,  22,   4,  48 },
    {   3,   2,   3,  30,  31,  32,   5,  96 },
    {   3,   5,   6,  60,  61,  62,   8, 128 },
    {   3,   0,   1,  10,  11,  12,   3,  48 },
    {   4,   2,   3,  30,  31,  32,   5, 128 },
    {   4,   1,   2,  20,  21,  22,   4,  64 },
    {   4,   0,   1,  10,  11,  12,   3,  64 },
    {   4,   0,   0,   0,   1,   2,   2,  64 },
    {   5,   7,   4,  40,  41,  42,   6, 128 },
    {   6,  10,  11, 110, 111, 112,  13, 256 },
    {   7,  11,  12, 120, 121, 122,  14, 256 },
    {   8,  12,  13, 130, 131, 132,  15, 256 },
    {   9,   0,  -1, 200,   0,   0,   2, 256 },
    {  -1,  -1,  -1,  -1,   0,   0,   0,   0 },
};

static OpponentSelectionEntry s_OpponentList09[24] = {
    {   0,   4,   5,  50,  51,  52,   7, 128 },
    {   0,   6,   7,  70,  71,  72,   9, 128 },
    {   1,   4,   5,  50,  51,  52,   7, 128 },
    {   1,   6,   7,  70,  71,  72,   9, 128 },
    {   1,   5,   6,  60,  61,  62,   8, 128 },
    {   2,   5,   6,  60,  61,  62,   8, 128 },
    {   2,   1,   2,  20,  21,  22,   4,  48 },
    {   2,   7,   8,  80,  81,  82,  10,  64 },
    {   2,   0,   1,  10,  11,  12,   3,  48 },
    {   3,   2,   3,  30,  31,  32,   5,  96 },
    {   3,   1,   2,  20,  21,  22,   4,  64 },
    {   3,   0,   1,  10,  11,  12,   3,  48 },
    {   3,   1,   4,  40,  41,  42,   6,  64 },
    {   3,   7,   8,  80,  81,  82,  10,  64 },
    {   4,   2,   3,  30,  31,  32,   5, 128 },
    {   4,   1,   2,  20,  21,  22,   4,  64 },
    {   4,   0,   1,  10,  11,  12,   3,  64 },
    {   4,   1,   4,  40,  41,  42,   6,  64 },
    {   5,   0,   0,   0,   1,   2,   2, 128 },
    {   6,   9,  10, 100, 101, 102,  12, 256 },
    {   7,  11,  12, 120, 121, 122,  14, 256 },
    {   8,  12,  13, 130, 131, 132,  15, 256 },
    {   9,   0,  -1, 200,   0,   0,   2, 256 },
    {  -1,  -1,  -1,  -1,   0,   0,   0,   0 },
};

static OpponentSelectionEntry s_OpponentList10[24] = {
    {   0,   4,   5,  50,  51,  52,   7, 128 },
    {   0,   6,   7,  70,  71,  72,   9, 128 },
    {   1,   4,   5,  50,  51,  52,   7, 128 },
    {   1,   6,   7,  70,  71,  72,   9, 128 },
    {   1,   5,   6,  60,  61,  62,   8, 128 },
    {   2,   5,   6,  60,  61,  62,   8, 128 },
    {   2,   1,   2,  20,  21,  22,   4,  96 },
    {   2,   0,   0,   0,   1,   2,   2,  64 },
    {   2,   7,   8,  80,  81,  82,  10,  64 },
    {   3,   2,   3,  30,  31,  32,   5,  96 },
    {   3,   1,   2,  20,  21,  22,   4,  64 },
    {   3,   0,   0,   0,   1,   2,   2,  64 },
    {   3,   3,   4,  40,  41,  42,   6,  64 },
    {   3,   7,   8,  80,  81,  82,  10,  64 },
    {   4,   2,   3,  30,  31,  32,   5, 128 },
    {   4,   1,   2,  20,  21,  22,   4, 128 },
    {   4,   0,   0,   0,   1,   2,   2, 128 },
    {   4,   3,   4,  40,  41,  42,   6, 128 },
    {   5,   0,   1,  10,  11,  12,   3, 128 },
    {   6,  10,  11, 110, 111, 112,  13, 256 },
    {   7,  11,  12, 120, 121, 122,  14, 256 },
    {   8,  12,  13, 130, 131, 132,  15, 256 },
    {   9,   0,  -1, 200,   0,   0,   2, 256 },
    {  -1,  -1,  -1,  -1,   0,   0,   0,   0 },
};

static OpponentSelectionEntry s_OpponentList11[20] = {
    {   0,  14,   7,  70,  71,  72,   2,  96 },
    {   0,  14,   6,  60,  61,  62,   2,  64 },
    {   1,  14,   7,  70,  71,  72,   2, 128 },
    {   1,  14,   6,  60,  61,  62,   2,  64 },
    {   2,  14,   7,  70,  71,  72,   2, 128 },
    {   2,  14,   6,  60,  61,  62,   2, 128 },
    {   2,  14,   8,  80,  81,  82,   2, 128 },
    {   3,  14,   4,  40,  41,  42,   2, 128 },
    {   3,  14,   0,   0,   1,   2,   2, 128 },
    {   3,  14,   1,  10,  11,  12,   2, 128 },
    {   3,  14,   8,  80,  81,  82,   2,  96 },
    {   4,  14,   4,  40,  41,  42,   2, 128 },
    {   4,  14,   0,   0,   1,   2,   2, 128 },
    {   4,  14,   1,  10,  11,  12,   2, 128 },
    {   5,  14,   2,  20,  21,  22,   2, 128 },
    {   6,   8,   9,  90,  91,  92,   2, 256 },
    {   7,  11,  12, 120, 121, 122,   2, 256 },
    {   8,  12,  13, 130, 131, 132,   2, 256 },
    {   9,   0,  -1, 200,   0,   0,   2, 256 },
    {  -1,  -1,  -1,  -1,   0,   0,   0,   0 },
};

static OpponentSelectionEntry s_OpponentList12[20] = {
    {   0,   4,   5,  50,  51,  52,   7, 128 },
    {   1,   3,   4,  40,  41,  42,   6,  64 },
    {   1,   7,   8,  80,  81,  82,  10,  64 },
    {   1,   3,   4,  40,  41,  42,   6,  64 },
    {   1,   7,   8,  80,  81,  82,  10,  64 },
    {   2,   1,   2,  20,  21,  22,   4,  96 },
    {   3,   2,   3,  30,  31,  32,   5,  96 },
    {   3,   1,   2,  20,  21,  22,   4,  64 },
    {   3,   3,   4,  40,  41,  42,   6,  64 },
    {   3,   7,   8,  80,  81,  82,  10,  64 },
    {   4,   2,   3,  30,  31,  32,   5,  96 },
    {   4,   1,   2,  20,  21,  22,   4,  64 },
    {   4,   3,   4,  40,  41,  42,   6,  64 },
    {   4,   7,   8,  80,  81,  82,  10,  64 },
    {   5,   0,   1,  10,  11,  12,   3, 128 },
    {   6,   8,   9,  90,  91,  92,  11, 256 },
    {   7,  11,   0,   0,   1,   2,   2, 256 },
    {   8,  12,  13, 130, 131, 132,  15, 256 },
    {   9,   0,  -1, 200,   0,   0,   2, 256 },
    {  -1,  -1,  -1,  -1,   0,   0,   0,   0 },
};

static OpponentSelectionEntry s_OpponentList13[21] = {
    {   0,   4,   5,  50,  51,  52,   7, 128 },
    {   0,   6,   7,  70,  71,  72,   9, 128 },
    {   1,   4,   5,  50,  51,  52,   7, 128 },
    {   1,   6,   7,  70,  71,  72,   9, 128 },
    {   1,   5,   6,  60,  61,  62,   8, 128 },
    {   2,   5,   6,  60,  61,  62,   8, 128 },
    {   2,   7,   8,  80,  81,  82,  10,  64 },
    {   2,   3,   4,  40,  41,  42,   6,  64 },
    {   2,   2,   3,  30,  31,  32,   5,  96 },
    {   3,   3,   4,  40,  41,  42,   6,  64 },
    {   3,   2,   3,  30,  31,  32,   5,  96 },
    {   3,   7,   8,  80,  81,  82,  10,  64 },
    {   4,   1,   2,  20,  21,  22,   4, 128 },
    {   4,   3,   4,  40,  41,  42,   6,  64 },
    {   4,   2,   3,  30,  31,  32,   5,  96 },
    {   5,   0,   1,  10,  11,  12,   3, 128 },
    {   6,   9,  10, 100, 101, 102,  12, 256 },
    {   7,  11,  12, 120, 121, 122,  14, 256 },
    {   8,  12,   0,   0,   1,   2,   2, 256 },
    {   9,   0,  -1, 200,   0,   0,   2, 256 },
    {  -1,  -1,  -1,  -1,   0,   0,   0,   0 },
};

OpponentSelectionEntry *g_OpponentLists[14] = {
    s_OpponentList00,
    s_OpponentList01,
    s_OpponentList02,
    s_OpponentList03,
    s_OpponentList04,
    s_OpponentList05,
    s_OpponentList06,
    s_OpponentList07,
    s_OpponentList08,
    s_OpponentList09,
    s_OpponentList10,
    s_OpponentList11,
    s_OpponentList12,
    s_OpponentList13,
};

int g_OpponentParameterTable[225] = {
     5,  4,  3,  2,  1,
     5,  4,  3,  2,  1,
     5,  4,  3,  2,  1,
     8,  8,  4,  2,  1,
    10, 10,  5,  3,  1,
    22, 12,  6,  3,  1,
    26, 14,  7,  4,  1,
    40, 16,  8,  4,  1,
    55, 30,  9,  5,  1,
     5,  4,  3,  2,  1,
     5,  4,  3,  2,  1,
     5,  4,  3,  2,  1,
    10, 10, 12,  9,  7,
    12, 18, 15, 11,  9,
    38, 20, 18, 15, 10,
    45, 32, 25, 20, 15,
    50, 32, 25, 20, 15,
    60, 59, 45, 40, 30,
     8,  5,  3,  2,  1,
     8,  5,  3,  2,  1,
     8,  5,  3,  2,  1,
    10, 10, 12,  9,  7,
    12, 18, 15, 11,  9,
    45, 20, 18, 15, 10,
    50, 32, 25, 20, 15,
    50, 32, 25, 20, 15,
    60, 59, 45, 40, 30,
    12,  8,  3,  2,  1,
    12,  8,  3,  2,  1,
    12,  8,  3,  2,  1,
    15, 10, 12,  9,  7,
    18, 18, 15, 11,  9,
    45, 20, 18, 15, 10,
    50, 32, 25, 20, 15,
    50, 32, 25, 20, 15,
    60, 59, 45, 40, 30,
    61, 61, 61, 61, 61,
    62, 62, 61, 61, 61,
    63, 63, 61, 61, 61,
    64, 64, 61, 61, 61,
    65, 65, 61, 61, 61,
    66, 65, 61, 61, 61,
    67, 65, 64, 61, 61,
    68, 66, 64, 61, 61,
    70, 69, 68, 67, 65,
};
int g_OpponentModeValues[80] = {
    10, 30, 50, 60, 0,
    0x00E8F0FF, 0x00F0E8FF, 0x00FFE8F0, 0x00FFE8F0,
    0x00E8F0FF, 0x00F0E8FF, 0x00FFE8F0, 0x00FFE8F0,
    0x00E8F0FF, 0x00F0E8FF, 0x00FFE8F0, 0x00FFE8F0,
    0x00E8F0FF, 0x00F0E8FF, 0x00FFE8F0, 0x00FFE8F0,
    0x00E8F0FF, 0x00F0E8FF, 0x00FFE8F0, 0x00FFE8F0,
    0x00E8F0FF, 0x00F0E8FF, 0x00FFE8F0, 0x00FFE8F0,
    0x00E8F0FF, 0x00F0E8FF, 0x00FFE8F0, 0x00FFE8F0,
    0x00E8F0FF, 0x00F0E8FF, 0x00FFE8F0, 0x00FFE8F0,
    0x00E8F0FF, 0x00F0E8FF, 0x00FFE8F0, 0x00FFE8F0,
    0x00E8F0FF, 0x00F0E8FF, 0x00FFE8F0, 0x00FFE8F0,
    0x00E8F0FF, 0x00F0E8FF, 0x00FFE8F0, 0x00FFE8F0,
    0x00E8F0FF, 0x00F0E8FF, 0x00FFE8F0, 0x00FFE8F0,
    1, 2, 0,
    3, 4, 0,
    5, 6, 0,
    7, 8, 0,
    7, 9, 0,
    10, 11, 0,
    12, 13, 15,
    12, 14, 15,
    16, 17, 0,
};

// Opponent selection uses the real list, weight, mode and unlock arrays,
// together with the two native gameplay-side records. Original signed
// indices and live evaluation order remain unchanged, without new guards.

void AsciiGameManagerView::SelectOpponentConfiguration()
{
    if (IsGameMode0() || IsGameMode1())
    {
        OpponentSelectionEntry *entry =
            (g_OpponentLists[sides[0].value20]);
        OpponentSelectionEntry *candidates[16];
        int candidateCount = 0;
        int totalWeight = 0;

        // Keep the live candidate list and side index across RNG/stat calls.
        unsigned char *usedCharacters = usedCharactersD8;
        short key = entry->key00;
        if (key >= 0)
        {
            int targetKey = counterFC;
            do
            {
                if (key == targetKey)
                {
                    totalWeight += entry->weight0E;
                    candidates[candidateCount++] = entry;
                }

                key = entry[1].key00;
                ++entry;
            }
            while (key >= 0);
        }

        // Every subsequent decision observes the signed low word. Unsigned
        // storage preserves the target subtraction's modulo-65536 behavior.
        unsigned short roll =
            g_Rng.GetRandomU32InRange(totalWeight);

        int selectedIndex = 0;
        if (candidateCount > 0)
        {
            for (;;)
            {
                OpponentSelectionEntry *candidate =
                    candidates[selectedIndex];
                unsigned short weight =
                    static_cast<unsigned short>(candidate->weight0E);

                if (static_cast<short>(roll) <
                        static_cast<short>(weight) &&
                    usedCharacters[candidate->character04] == 0)
                {
                    break;
                }

                roll -= weight;
                if (++selectedIndex >= candidateCount)
                    break;
            }
        }

        if (selectedIndex >= candidateCount)
        {
            selectedIndex = 0;
            if ((usedCharacters[candidates[0]->character04]) != 0)
            {
                do
                {
                    ++selectedIndex;
                }
                while ((usedCharacters[candidates[selectedIndex]->character04]) != 0);
            }
        }

        OpponentSelectionEntry *selected =
            candidates[selectedIndex];

        selectedValueF4 = selected->valueF4_02;
        sides[1].value20 = selected->character04;
        g_PlayStatsRecord.IncrementCharacterStat5(
            static_cast<char>(selected->character04));
        (usedCharacters[sides[1].value20]) = 1;
        selectedEntryF0 = selected;

        AsciiGameSideRuntimeView *state = sides[0].state;
        // The target also takes this arm for an unordered float comparison.
        if (state->value14 != 0 || !(state->value00 > 0.0f))
        {
            int variant = g_FrontRuntimeValues90[0];
            if (variant > 3)
                variant = 3;
            int index = counterFC + 9 * setupMode11C;
            sides[1].selectedParameter2C =
                g_OpponentParameterTable[5 * index + 1 + variant];
        }
        else
        {
            int index = counterFC + 9 * setupMode11C;
            sides[1].selectedParameter2C =
                g_OpponentParameterTable[5 * index];
        }

        return;
    }

    int selector = selector138;
    counterFC = 9;

    OpponentSelectionEntry *selected = 0;

    if (selector == -1)
    {
        int randomSide = g_Rng.GetRandomU32InRange(2);
        selected = g_OpponentEntries;
        if (selected->key00 >= 0)
        {
            int wantedCharacter = sides[randomSide].value20;
            do
            {
                if (selected->character04 == wantedCharacter)
                    break;
                ++selected;
            }
            while (selected->key00 >= 0);
        }
    }
    else if (selector == -2)
    {
        int availableCount = 0;
        for (int countCharacter = 0;
             countCharacter <= 13;
             countCharacter += 2)
        {
            if (g_TitleCharacterUnlocked[countCharacter] ||
                g_SupervisorNetworkState->active)
            {
                ++availableCount;
            }

            if (g_TitleCharacterUnlocked[countCharacter + 1] ||
                g_SupervisorNetworkState->active)
            {
                ++availableCount;
            }
        }

        unsigned int selectedVisible =
            g_Rng.GetRandomU32InRange(availableCount);
        int visibleIndex = 0;
        int character = 0;

        for (;;)
        {
            if (g_TitleCharacterUnlocked[character] ||
                g_SupervisorNetworkState->active)
            {
                if (visibleIndex == static_cast<int>(selectedVisible))
                    break;
                ++visibleIndex;
            }

            if (g_TitleCharacterUnlocked[character + 1] ||
                g_SupervisorNetworkState->active)
            {
                if (visibleIndex == static_cast<int>(selectedVisible))
                {
                    character += 1;
                    break;
                }
                ++visibleIndex;
            }

            if (g_TitleCharacterUnlocked[character + 2] ||
                g_SupervisorNetworkState->active)
            {
                if (visibleIndex == static_cast<int>(selectedVisible))
                {
                    character += 2;
                    break;
                }
                ++visibleIndex;
            }

            if (g_TitleCharacterUnlocked[character + 3] ||
                g_SupervisorNetworkState->active)
            {
                if (visibleIndex == static_cast<int>(selectedVisible))
                {
                    character += 3;
                    break;
                }
                ++visibleIndex;
            }

            character += 4;
            if (character >= 16)
                break;
        }

        selected = g_OpponentEntries;
        while (selected->key00 >= 0)
        {
            if (selected->character04 == character)
                break;
            ++selected;
        }
    }
    else
    {
        selected = g_OpponentEntries;
        while (selected->key00 >= 0)
        {
            if (selected->character04 == selector)
                break;
            ++selected;
        }
    }
    if (selected->key00 < 0)
        selected = g_OpponentEntries;

    // Keep the mode snapshot, but reread the table after the first side store.
    int mode = setupMode11C;
    sides[0].selectedParameter2C = g_OpponentModeValues[mode];
    sides[1].selectedParameter2C = g_OpponentModeValues[mode];
    selectedValueF4 = selected->valueF4_02;
    selectedEntryF0 = selected;
}
