#pragma once

// PE-attested i386 0x1EC replay layout shared by replay input and both
// Title renderers. Retail C++ naming and TU partition remain unproven.
#include <stddef.h>
#include "GameConfiguration.hpp"

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef int i32;

struct ReplayFrameDataStartView
{
    u32 value00;
    u16 stageSeed;
    u8 value06;
    u8 value07;
    u8 value08;
    u8 value09;
    u16 value0A;
    u8 value0C;
    u8 unknown0D[3];
    u32 value10;
    u8 unknown14[0x0C];
};

typedef char ReplayFrameDataStartSizeIs20[(sizeof(ReplayFrameDataStartView) == 0x20) ? 1 : -1];

// These two subobjects model the observed constructor calls and wire ranges.
// Their inheritance is a reconstruction view, not an original C++ class claim.
struct ReplayHeaderView
{
    u32 magic;
    u16 formatVersion;
    u8 value06;
    u8 value07;
    u8 unknown008[4];
    i32 fileSize;
    u32 checksum;
    u8 unknown14;
    u8 obfuscationKey;
    u8 unknown16[2];
    i32 compressedSize;
    i32 decompressedSize;
    ReplayFrameDataStartView *frameStart[3][10];
    u8 *fpsStart[10];
};

struct ReplayPayloadView
{
    ReplayPayloadView();

    u8 randomPayloadByte;
    u8 value0C1;
    u16 value0C2;
    char playTime[10];
    char playerName[8];
    u8 unknown0D6;
    u8 value0D7;
    u8 unknown0D8[4];
    GameConfiguration configSnapshot;
    u8 unknown1A8[0x28];
    u32 value1D0;
    u32 value1D4;
    u32 value1D8;
    char exeVersion[6];
    u8 unknown1E2[2];
    u8 value1E4;
    u8 value1E5;
    u8 value1E6;
    u8 value1E7;
    u8 value1E8;
    u8 value1E9;
    u8 unknown1EA[2];
};

struct ReplayDataView : ReplayHeaderView, ReplayPayloadView
{
    ReplayDataView();
};

typedef char ReplayHeaderSizeIsC0[(sizeof(ReplayHeaderView) == 0xC0) ? 1 : -1];
typedef char ReplayPayloadSizeIs12C[(sizeof(ReplayPayloadView) == 0x12C) ? 1 : -1];
typedef char ReplayPayloadConfigAt1C[(offsetof(ReplayPayloadView, configSnapshot) == 0x1C) ? 1 : -1];
typedef char ReplayDataPayloadAtC0[(offsetof(ReplayDataView, randomPayloadByte) == 0xC0) ? 1 : -1];
typedef char ReplayDataSizeIs1EC[(sizeof(ReplayDataView) == 0x1EC) ? 1 : -1];
typedef char ReplayDataFrame0At20[(offsetof(ReplayDataView, frameStart) == 0x20) ? 1 : -1];
typedef char ReplayDataFrame1At48[(offsetof(ReplayDataView, frameStart) + sizeof(((ReplayDataView *)0)->frameStart[0]) == 0x48) ? 1 : -1];
typedef char ReplayDataFrame2At70[(offsetof(ReplayDataView, frameStart) + 2 * sizeof(((ReplayDataView *)0)->frameStart[0]) == 0x70) ? 1 : -1];
typedef char ReplayDataFpsAt98[(offsetof(ReplayDataView, fpsStart) == 0x98) ? 1 : -1];
typedef char ReplayDataConfigAtDC[(offsetof(ReplayDataView, configSnapshot) == 0xDC) ? 1 : -1];
typedef char ReplayDataVersionAt1DC[(offsetof(ReplayDataView, exeVersion) == 0x1DC) ? 1 : -1];

// Both Title replay renderers refer to this exact physical record, not
// independent synthetic ReplaySave/ReplayMenu replay structures.
typedef char ReplayDataMagicAt00[(offsetof(ReplayDataView, magic) == 0) ? 1 : -1];
typedef char ReplayDataFrameCharacterAt06[
    (offsetof(ReplayFrameDataStartView, value06) == 6) ? 1 : -1];
typedef char ReplayDataPlayTimeAtC4[
    (offsetof(ReplayDataView, playTime) == 0xC4) ? 1 : -1];
typedef char ReplayDataPlayerNameAtCE[
    (offsetof(ReplayDataView, playerName) == 0xCE) ? 1 : -1];
typedef char ReplayDataDifficultyAtD7[
    (offsetof(ReplayDataView, value0D7) == 0xD7) ? 1 : -1];
typedef char ReplayDataReplayModeAt1E4[
    (offsetof(ReplayDataView, value1E4) == 0x1E4) ? 1 : -1];
typedef char ReplayDataVersusModeAt1E5[
    (offsetof(ReplayDataView, value1E5) == 0x1E5) ? 1 : -1];
