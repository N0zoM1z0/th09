#include "PlayerCollisionRegionCreate.hpp"

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
