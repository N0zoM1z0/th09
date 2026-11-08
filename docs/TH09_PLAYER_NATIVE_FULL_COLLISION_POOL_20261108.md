# TH09 Player: all 512 native 0x44-byte collision records share the exact Reset

Original target: independently verified Japanese TH09 v1.50a executable,
SHA-256 `10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
This recovers native C++ storage and original method ownership, not a newly
byte-exact function or complete runtime/product acceptance.

## The entire +0x28BC to +0xB0BC physical region pool belongs to Player

The complete native 0x30F70-byte `PlayerLifecycleView` already contained
an exact `PlayerCollisionRegionCreateRegionView` embedded fallback record
at +0xB0BC, plus active/released pointer tables at +0xB100/+0xB908
(each **514 pointers**) and their counts at +0xC110/+0xC114. The
original **512-record pool** from Player +0x28BC through +0xB0BB
was still represented as one large opaque `unknown28BC` byte window.
A second, independent `PlayerAddedCollisionRegionView` class existed
solely for the 512-record reset loop inside exact
`PlayerLifecycleView::InitializeAddedState`.

The pool is now a typed `PlayerCollisionRegionCreateRegionView
collisionRegions28BC[512]` *inside the real Player allocation*. Each
record is the original **0x44-byte constructor-free POD** also used by
the independently exact `AllocateCollisionRegion` producer, the
embedded fallback record, the active/released lists, and the Player's
still-open bullet collision boss. The initializer now directly resets
the real Player array and fills its real released and active pointer
lists. Loop capacity is derived from `sizeof(collisionRegions28BC) /
sizeof(collisionRegions28BC[0])`, exactly **512**, not from a separate
hardcoded view. The active/released counts and original reset sequence
remain unchanged. No new region constructors or additional allocations
were introduced.

The original **23-byte `Reset @0x0041BDA0`** now belongs to the real
`PlayerCollisionRegionCreateRegionView`, not the fake AddedCollision
view. It performs `memset(this,0,sizeof(*this))`, then restores the
actual `interval34` word at +0x34 to one. Both independent original
callers—**110-byte `AllocateCollisionRegion` @0x0041CF30** at call
site +0x46 and **552-code/572-physical-byte `InitializeAddedState`
@0x0041EBC0** at call site +0x5A—now reference the same native Reset
symbol at the unchanged original target VA.

## Independent complete original PE/COFF verification

Three source-isolated natural fixed-VC7.1 experiments tested a native
region class within the existing initializer view, direct accesses to
Player's real pool and pointer tables, and a compile-time-derived 512
iteration bound. **Every experiment reproduced the complete original
23-byte Reset and all 572 physical initializer bytes**, including all
25 original relocation fields and post-return jump data. A separate
immutable original-PE proof checked the true 0x41BDA0 Reset field,
all six compiler-private initializer label definitions and the original
table/branch operands before updating only those six names. The
independent 110-byte original allocator stays exact as a separate source
owner, proving source/callee identity from both directions.

With the changed native Player header, scoped original-bound source-cold
replay passes **123 complete already-exact functions / 39,702 physical
bytes / 1,871 relocation fields** across AddedState,
PlayerCollisionRegionCreate, PlayerRuntime, PlayerOwnerStateReward,
FrontResult, ECL exact siblings and Player collision helpers. Two ECL,
eleven PlayerRuntime and six reward private-label name changes were
individually reviewed against immutable PE/COFF original destinations.
No original table address, field type, call target or compiler profile
was changed.

The separately **nonexact 862-byte Player CheckBulletCollision** keeps
849 candidate bytes, 26 bound fields, 12 ordered calls, 799 unlike
ordinary overlapping bytes and 13 missing original bytes. The
**3,900-byte Enemy OnUpdate** keeps 97 fields, 69 calls and 13 unlike
ordinary bytes; **15,564-byte RunEcl** remains 30 ordinary bytes unlike,
with all 598 original bound fields/tables untouched. Structural recovery
does not promote any of those functions to exactness.

Focused natural source trials, PE/COFF proofs and boss reports:
`.analysis/gpt-web-player-native-region-pool-20261108/`.
No full-project cold replay, linked Windows game executable, runtime
semantic acceptance, Factory receipt or remote Git push is claimed.
Global ledger is unchanged: **1,034 source-present / 1,021 exact /
13 nonexact**.
