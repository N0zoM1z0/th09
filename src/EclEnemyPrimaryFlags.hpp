#pragma once
// Shared view of the original Enemy DWORD at +0x337C, used by both ECL
// movement and state handlers. Names describe independently reviewed TH09
// bit positions; unknown ranges do not imply an exhaustive Enemy layout.
struct Th09EclEnemyPrimaryFlagBits
{
    unsigned int active00 : 1;
    unsigned int boss01 : 1;
    unsigned int collision02 : 1;
    unsigned int damageable03 : 1;
    unsigned int noSprite04 : 1;
    unsigned int hidePrimary05 : 1;
    unsigned int acceptsDamage06 : 1;
    unsigned int specialInteraction07 : 1;
    unsigned int forceDeath08 : 1;
    unsigned int movementMode09_10 : 2;
    unsigned int movementEasing11_13 : 3;
    unsigned int unknown14 : 1;
    unsigned int mirrorMovementX15 : 1;
    unsigned int clampPosition16 : 1;
    unsigned int deathMode17 : 3;
    unsigned int persistAfterDeath20 : 1;
    unsigned int hasBeenInBounds21 : 1;
    unsigned int anmRotation22 : 1;
    unsigned int callStackDisabled23 : 1;
    unsigned int timeoutSpell24 : 1;
    unsigned int allowOffscreen25 : 1;
    unsigned int skipMovement26 : 1;
    unsigned int pauseWithPlayer27 : 1;
    unsigned int unknown28 : 1;
    unsigned int unknown29_30 : 2;
    unsigned int alternateAnmBank31 : 1;
};
// The same native Enemy +0x3380 DWORD controls no-death behavior in ECL
// movement. Other bits remain unknown rather than importing another game.
union Th09EclEnemySecondaryFlagBits
{
    // ECL movement interprets bits 9..10 as a movement mode.
    struct Movement
    {
        unsigned int deathFlag00 : 1;
        unsigned int bossMarkerState01_02 : 2;
        unsigned int noDeath03 : 1;
        unsigned int bossTimerPause04 : 1;
        unsigned int unknown05 : 1;
        // Effect trails and post-ECL interaction share the category at +0x3380.
        unsigned int effectCategory06_08 : 3;
        unsigned int movementMode09_10 : 2;
        unsigned int unknown11 : 1;
        unsigned int interactionTriggered12 : 1;
        unsigned int cardAttackIndicator13 : 1;
        unsigned int secondaryFlag4000_14 : 1;
        unsigned int unknown15_31 : 17;
    } movement;

    // ECL late side-category opcodes overlay bits 10..11 of that same word.
    struct Category
    {
        unsigned int deathFlag00 : 1;
        unsigned int bossMarkerState01_02 : 2;
        unsigned int noDeath03 : 1;
        unsigned int bossTimerPause04 : 1;
        unsigned int unknown05 : 1;
        unsigned int effectCategory06_08 : 3;
        unsigned int unknown09 : 1;
        unsigned int sideCategory10_11 : 2;
        unsigned int interactionTriggered12 : 1;
        unsigned int cardAttackIndicator13 : 1;
        unsigned int secondaryFlag4000_14 : 1;
        unsigned int unknown15_31 : 17;
    } category;
};
typedef char Th09EclEnemySecondaryFlagsSizeIs4[
    sizeof(Th09EclEnemySecondaryFlagBits) == 4 ? 1 : -1];
typedef char Th09EclEnemyPrimaryFlagsSizeIs4[
    sizeof(Th09EclEnemyPrimaryFlagBits) == 4 ? 1 : -1];
