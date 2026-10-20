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
    unsigned int unknown08_15 : 8;
    unsigned int clampPosition16 : 1;
    unsigned int deathMode17 : 3;
    unsigned int unknown20_21 : 2;
    unsigned int anmRotation22 : 1;
    unsigned int callStackDisabled23 : 1;
    unsigned int timeoutSpell24 : 1;
    unsigned int allowOffscreen25 : 1;
    unsigned int unknown26_31 : 6;
};
typedef char Th09EclEnemyPrimaryFlagsSizeIs4[
    sizeof(Th09EclEnemyPrimaryFlagBits) == 4 ? 1 : -1];
