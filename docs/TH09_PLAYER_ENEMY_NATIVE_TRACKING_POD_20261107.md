# Native Player tracking POD shared by Enemy 3,900-byte boss and shot callbacks

Target: Japanese TH09 v1.50a original executable with verified SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
This is a physical C++ storage and source-ownership reconstruction checkpoint,
not a claim to uniquely establish historic nominal class or TU names.

## One actual Player +0x30364..+0x30383 storage window

The real Player allocation contains a 0x20-byte nearest-enemy tracking
record, independently observed in the full Enemy 3,900-byte update,
Enemy cleanup, Player UpdateStageD, and three separate native shot
callbacks. Its original physical coordinates are:

| Player offset | Native storage and role |
| --- | --- |
| +0x30364, +0x30368, +0x3036C | Three nearest-enemy position floats |
| +0x30370..+0x3037B | Second 12-byte reset window, still opaque |
| +0x3037C | `EnemyView *` homing target |
| +0x30380 | Last native status/reset word |

The previous main `PlayerLifecycleView` declared only an opaque
`unknown30364[0x20]` buffer. Consumers repeatedly cast that byte array
to a *separately reconstructed* `PlayerEnemyTrackingStateView` and manually
recreated the original pointer offsets. The actual Player now owns one
`PlayerEnemyTrackingNativeView tracking30364` containing only POD scalar,
byte-array and pointer members, with compile-time checks for the complete
0x20 extent and all known member offsets. No constructed `Float3` member
is inserted: this matters because the Player constructor's existing
constructed-subobject chronology is independently exact. Vector operators
borrow `Float3` only at the actual arithmetic and assignment call sites.
The separate older Enemy-facing projection is not promoted to a second
real Player allocation or an invented C++ subclass.

`EnemyManagerView::OnUpdate` now reads/writes the Player's real tracking
position and homing pointer in its damage/target-selection path. The
`EnemyView::CleanupAfterDeactivation` leaf clears that same typed pointer,
while Player stage D writes its two Float3 reset values and trailing word
at the same native addresses. Three shot callbacks use the typed position
scalars or their native address at the actual angle-computation boundary.
Original-order reads, assignments, score handling, enemy lifetimes,
control-flow branches, callback arguments and side selection are unchanged.

## Complete immutable original PE/COFF replay

The pinned VC7.1 source-cold checks cover **108 complete exact functions /
36,289 physical bytes / 1,712 relocation fields** across the changed
EclManager/Enemy, PlayerRuntime/constructor, Enemy cleanup and three
independent shot callback carriers. Explicit original-bound checks include
243-byte Player constructor, 522-byte Player OnUpdate, 184-byte Enemy
cleanup, 139-byte shot spawn-7 and 471/248-byte shot update-1/update-3.
The **3,900-byte Enemy OnUpdate** separately retains its exact target
size, **97 original-bound relocations, 69 ordered calls, original graph,
original switch destinations and the same 13 unlike ordinary bytes**.
Its 82-byte collision wrapper stays exact. This is a substantive source
and layout change but it grants **zero newly exact authored functions**.

The header/source change renumbered exactly **two ECL and 17 PlayerRuntime
compiler-private `$L` names** in the complete exact carriers. These were
not renamed speculatively: their actual same-section COFF definition
locations, owner-relative offsets, immutable original PE field operands and
destinations were individually reviewed; only the already-proven *names*
changed in the manifest. Neither original destination, field addend,
compiler profile, sibling byte comparison nor source function size was
waived. Private bounded proof packets:
`.analysis/gpt-web-player-tracking-pod-20261107/ecl-label-proof.json`,
`player-label-proof.json`, `scoped-verified.json`, and the full Enemy boss
report. Source backups and source-diff recipes remain ignored and local.

No all-function cold replay, Windows executable link, gameplay/runtime
semantics acceptance, Factory receipt or remote Git publication is claimed.
Global status remains **1,034 source-present / 1,021 exact / 13 nonexact**.
