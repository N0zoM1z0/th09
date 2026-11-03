#pragma once
// Shared view of the original Enemy DWORD at +0x337C, used by both ECL
// movement and state handlers. Names describe independently reviewed TH09
// bit positions; unknown ranges do not imply an exhaustive Enemy layout.
struct Th09EclEnemyPrimaryFlagBits
{
    unsigned int unknown00 : 1;
    unsigned int boss01 : 1;
    unsigned int collision02 : 1;
    unsigned int damageable03 : 1;
    unsigned int noSprite04 : 1;
    unsigned int unknown05 : 1;
    unsigned int acceptsDamage06 : 1;
    unsigned int specialInteraction07 : 1;
    unsigned int unknown08_14 : 7;
    unsigned int mirrorMovementX15 : 1;
    unsigned int clampPosition16 : 1;
    unsigned int deathMode17 : 3;
    unsigned int unknown20_21 : 2;
    unsigned int anmRotation22 : 1;
    unsigned int callStackDisabled23 : 1;
    unsigned int timeoutSpell24 : 1;
    unsigned int allowOffscreen25 : 1;
    unsigned int unknown26_31 : 6;
};
// The same native Enemy +0x3380 DWORD controls no-death behavior in ECL
// movement. Other bits remain unknown rather than importing another game.
struct Th09EclEnemySecondaryFlagBits
{
    unsigned int unknown00_02 : 3;
    unsigned int noDeath03 : 1;
    unsigned int unknown04_05 : 2;
    // The exact Effect trail callback writes its 3-bit category to +0x3380
    // bits 6..8 of the same native Enemy secondary flag word.
    unsigned int effectCategory06_08 : 3;
    unsigned int unknown09_31 : 23;
};
typedef char Th09EclEnemySecondaryFlagsSizeIs4[
    sizeof(Th09EclEnemySecondaryFlagBits) == 4 ? 1 : -1];
typedef char Th09EclEnemyPrimaryFlagsSizeIs4[
    sizeof(Th09EclEnemyPrimaryFlagBits) == 4 ? 1 : -1];
