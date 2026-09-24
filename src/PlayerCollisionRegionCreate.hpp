#pragma once

#include <stddef.h>

struct PlayerRegionPointView
{
    float x;
    float y;
};

struct PlayerCollisionRegionCreateRegionView
{
    float centerX00;
    float centerY04;
    float radius08;
    float radiusGrowth0C;
    float halfWidth10;
    float halfHeight14;
    float widthGrowth18;
    float heightGrowth1C;
    float angle20;
    int lifetime24;
    int value28;
    int hitCount2C;
    int hitCap30;
    int interval34;
    int type38;
    unsigned char active3C;
    unsigned char unknown3D[3];
    int delay40;

    int Reset();
};

typedef char PlayerCreateRegionSizeIs44[
    (sizeof(PlayerCollisionRegionCreateRegionView) == 0x44) ? 1 : -1];

struct PlayerCollisionRegionCreateView
{
    unsigned char unknown0000[0xB0BC];
    PlayerCollisionRegionCreateRegionView embeddedB0BC;
    PlayerCollisionRegionCreateRegionView *activeB100[514];
    PlayerCollisionRegionCreateRegionView *releasedB908[514];
    int activeCountC110;
    int releasedCountC114;

    PlayerCollisionRegionCreateRegionView *AllocateCollisionRegion(
        const PlayerRegionPointView *center, int lifetime, int delay);
    PlayerCollisionRegionCreateRegionView *CreateRectType1(
        const PlayerRegionPointView *center, float width, float height,
        int lifetime, int delay);
    PlayerCollisionRegionCreateRegionView *CreateCircleType1(
        const PlayerRegionPointView *center, float radius, float radiusGrowth,
        int lifetime, int delay);
    PlayerCollisionRegionCreateRegionView *CreateRectType0(
        const PlayerRegionPointView *center, float width, float height,
        int value, int lifetime, int delay);
    PlayerCollisionRegionCreateRegionView *CreateCircleType0(
        const PlayerRegionPointView *center, float radius, float radiusGrowth,
        int value, int lifetime, int delay);
    PlayerCollisionRegionCreateRegionView *CreateRectType4(
        const PlayerRegionPointView *center, float width, float height,
        int value, int lifetime, int delay);
    PlayerCollisionRegionCreateRegionView *CreateCircleType4(
        const PlayerRegionPointView *center, float radius, float radiusGrowth,
        int value, int lifetime, int delay);
    PlayerCollisionRegionCreateRegionView *CreateCircleType2(
        const PlayerRegionPointView *center, float radius, float radiusGrowth,
        int value, int lifetime, int delay);
};

typedef char PlayerCreateEmbeddedAtB0BC[
    (offsetof(PlayerCollisionRegionCreateView, embeddedB0BC) == 0xB0BC) ? 1 : -1];
typedef char PlayerCreateActiveAtB100[
    (offsetof(PlayerCollisionRegionCreateView, activeB100) == 0xB100) ? 1 : -1];
typedef char PlayerCreateReleasedAtB908[
    (offsetof(PlayerCollisionRegionCreateView, releasedB908) == 0xB908) ? 1 : -1];
typedef char PlayerCreateActiveCountAtC110[
    (offsetof(PlayerCollisionRegionCreateView, activeCountC110) == 0xC110) ? 1 : -1];
typedef char PlayerCreateReleasedCountAtC114[
    (offsetof(PlayerCollisionRegionCreateView, releasedCountC114) == 0xC114) ? 1 : -1];
