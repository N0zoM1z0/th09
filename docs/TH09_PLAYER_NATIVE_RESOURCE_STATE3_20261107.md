# Player native resource clamp and three complete State3 effect helpers

Target: verified Japanese TH09 v1.50a PE, SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
This is a natural C++ ownership/ABI checkpoint, not a statement of original
historic class naming, runtime equivalence or new exactness credit.

## Recover the real Player method receiver and charge call chain

The original Player `SubtractResourceClamped` at 0x0041F310 adjusts the
live `scalar30388` with a minimum of 1.0f. The three original-exact
State3-effect helpers at 0x00403C30, 0x00403C90 and 0x00403CF0 use the
same Player's +0x1B88 position, native region producer, State4 setter,
+0x303C8 timer, and actual side's EffectManager. They are invoked by the
complete but still-nonexact 1,210-byte Player ChargeAttack source.

The previously declared C++ member owners,
`PlayerResourceSubtractView : PlayerLifecycleView` and
`PlayerStateEffectView : PlayerResourceSubtractView`, add **zero physical
storage**, so their extra method identities are not supported by separate
original Player allocations. Both C++ declarations and true source
implementations now belong to `PlayerLifecycleView`; their prior derived
facades remain only as zero-storage source interfaces for the ChargeAttack
path. The native Player also directly owns the 190-byte SpawnShots method
from the previous checkpoint.

The source-level call chronology of ChargeAttack is unchanged: three
resource reductions, three State3 helpers, the real native shot loop, and
the original input/timer/attack transitions all stay in their current
positions. The three State3 functions reuse the genuine
`PlayerCollisionRegionCreateView` declaration, not the older duplicate
struct that caused a real VC7.1 C2011 error in the previous checkpoint.

## Complete exact-function and large-boss validation

Four independently pinned VC7.1 compile trials confirm that simply moving
the native method ownership preserves **every** original raw instruction
byte and all original relocations:

| Native original function | Physical bytes | Match |
| --- | ---: | --- |
| Player::SubtractResourceClamped | 51 | Complete exact |
| Player::ConfigureState3Effect13 | 91 | Complete exact |
| Player::ConfigureState3Effect14 | 91 | Complete exact |
| Player::ConfigureState3Effect15 | 100 | Complete exact |

The 1,210-byte original ChargeAttack source is still a 1,197-byte candidate
with **70 unchanged original-bound fields, 58 ordered calls, 1,160 unequal
overlap bytes and 13 absent target bytes**. Its four method symbols now
refer to the same pinned target addresses as above under the genuine
Player C++ receiver, instead of the former zero-storage facades.

The source-cold strict canonical replay covers the three real State3
functions, the resource clamp and 21 existing PlayerRuntime exact owners:
**25 complete exact functions / 5,724 physical bytes / 304 fields**.
An independent original charge diagnostic also passes. No compiler profile,
original PE, relocation addend, function size or literal table was altered
to force equality. Relevant bounded probe objects and comparison reports:
`.analysis/gpt-web-charge-native-effects-20261107/`. These comparisons
do not prove that the open ChargeAttack boss is exact, nor that all historic
method declarations have unique source spelling.

No whole-product cold replay, game link, semantic/portability acceptance,
Factory acceptance or remote push is claimed. Overall function status
remains **1,034 source-present / 1,021 exact / 13 nonexact**.
