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

PlayerCollisionRegionCreateRegionView *
PlayerCollisionRegionCreateView::AllocateCollisionRegion(
    const PlayerRegionPointView *center, int lifetime, int delay)
{
    PlayerCollisionRegionCreateRegionView *region;
    if (releasedCountC114 <= 1)
    {
        region = &embeddedB0BC;
    }
    else
    {
        --releasedCountC114;
        region = releasedB908[releasedCountC114];
        releasedB908[releasedCountC114] = 0;
        activeB100[activeCountC110] = region;
        ++activeCountC110;
    }

    region->Reset();
    region->active3C = 1;
    region->centerX00 = center->x;
    region->centerY04 = center->y;
    region->lifetime24 = lifetime;
    region->delay40 = delay;
    return region;
}

PlayerCollisionRegionCreateRegionView *
PlayerCollisionRegionCreateView::CreateRectType1(
    const PlayerRegionPointView *center, float width, float height,
    int lifetime, int delay)
{
    PlayerCollisionRegionCreateRegionView *region =
        AllocateCollisionRegion(center, lifetime, delay);
    region->halfWidth10 = width * 0.5f;
    region->halfHeight14 = height * 0.5f;
    region->type38 = 1;
    return region;
}

PlayerCollisionRegionCreateRegionView *
PlayerCollisionRegionCreateView::CreateCircleType1(
    const PlayerRegionPointView *center, float radius, float radiusGrowth,
    int lifetime, int delay)
{
    PlayerCollisionRegionCreateRegionView *region =
        AllocateCollisionRegion(center, lifetime, delay);
    region->radius08 = radius;
    region->radiusGrowth0C = radiusGrowth;
    region->type38 = 1;
    return region;
}

PlayerCollisionRegionCreateRegionView *
PlayerCollisionRegionCreateView::CreateRectType0(
    const PlayerRegionPointView *center, float width, float height,
    int value, int lifetime, int delay)
{
    PlayerCollisionRegionCreateRegionView *region =
        AllocateCollisionRegion(center, lifetime, delay);
    region->value28 = value;
    region->type38 = 0;
    region->halfWidth10 = width * 0.5f;
    region->halfHeight14 = height * 0.5f;
    return region;
}

PlayerCollisionRegionCreateRegionView *
PlayerCollisionRegionCreateView::CreateCircleType0(
    const PlayerRegionPointView *center, float radius, float radiusGrowth,
    int value, int lifetime, int delay)
{
    PlayerCollisionRegionCreateRegionView *region =
        AllocateCollisionRegion(center, lifetime, delay);
    region->radius08 = radius;
    region->radiusGrowth0C = radiusGrowth;
    region->value28 = value;
    region->type38 = 0;
    return region;
}

PlayerCollisionRegionCreateRegionView *
PlayerCollisionRegionCreateView::CreateRectType4(
    const PlayerRegionPointView *center, float width, float height,
    int value, int lifetime, int delay)
{
    PlayerCollisionRegionCreateRegionView *region =
        AllocateCollisionRegion(center, lifetime, delay);
    region->value28 = value;
    region->type38 = 4;
    region->halfWidth10 = width * 0.5f;
    region->halfHeight14 = height * 0.5f;
    return region;
}

PlayerCollisionRegionCreateRegionView *
PlayerCollisionRegionCreateView::CreateCircleType4(
    const PlayerRegionPointView *center, float radius, float radiusGrowth,
    int value, int lifetime, int delay)
{
    PlayerCollisionRegionCreateRegionView *region =
        AllocateCollisionRegion(center, lifetime, delay);
    region->radius08 = radius;
    region->radiusGrowth0C = radiusGrowth;
    region->value28 = value;
    region->type38 = 4;
    return region;
}

PlayerCollisionRegionCreateRegionView *
PlayerCollisionRegionCreateView::CreateCircleType2(
    const PlayerRegionPointView *center, float radius, float radiusGrowth,
    int value, int lifetime, int delay)
{
    PlayerCollisionRegionCreateRegionView *region =
        AllocateCollisionRegion(center, lifetime, delay);
    region->radius08 = radius;
    region->radiusGrowth0C = radiusGrowth;
    region->value28 = value;
    region->type38 = 2;
    return region;
}
