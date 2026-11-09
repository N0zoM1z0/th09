# Complete native Player collision-region creation family

All eight TH09 v1.50a collision-region methods at 0x41CF30–0x41D110 now
belong to the actual `PlayerLifecycleView` allocation. The real 110-byte
allocator owns the Player's embedded 0x44-byte region, active/released
514-pointer pools and both counters. All seven rectangle/circle type
creators call that one member and use the same true region record.
The old `PlayerCollisionRegionCreateView` remains only as a bounded
storage-layout comparison, no longer as a separate method owner.

The native C++ conversion includes all observed external method users:
Player Shot Type2 (643 bytes), Shot Type4 (249), PlayerDamage (996),
Enemy reward handling (1,142), three Player State3 effect callbacks,
and FrontCalc (1,491). Five changed original PE REL32 operands were
checked individually against the unchanged targets 0x41CFA0, 0x41D010,
0x41D050, 0x41D090 and 0x41D110 before source symbol rebinding.

Scoped pinned VC7.1 recompilation proves **17 complete exact functions,
5,335 bytes, 221 relocated fields**, plus a separate 522-byte exact Player
OnUpdate. The still-nonexact 3,900-byte Enemy OnUpdate preserves all 97
relocations and its existing 13 unlike bytes. Focused Player native,
shot, collision and Enemy tests pass. No new canonical exact credit or
full-project cold replay is claimed.
