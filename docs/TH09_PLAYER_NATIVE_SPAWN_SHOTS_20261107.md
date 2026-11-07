# Native Player SpawnShots owner and complete Charge/Effect caller chain

TH09 original: Japanese 1.50a PE, SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
This is a reviewed natural-C++ source ownership checkpoint; preserved exact
functions receive **no newly authored byte-exact credit**.

## One real Player shot method, not a second derived allocation

The full original **190-byte `SpawnShots` at 0x0041F4C0** iterates the
native Player's `shots[128]` at Player +0xC11C. Each native shot occupies
0x484 bytes; the original source-facing callback views of shot fields and
SHT descriptors keep their 0x484/0x38 layouts. The previously maintained
method was declared under a duplicate zero-storage
`PlayerSpawnShotsView : PlayerStateEffectView` façade. Source now declares
and implements the method on the actual `PlayerLifecycleView`, retaining
its `__fastcall` source ABI, method arguments, callback dispatch and
physical receiver pointer. The callback's first argument is likewise the
native Player pointer, not the empty façade type. Both source forms compile
to the same original bytes under the pinned VC7.1 profile.

The bound `128` in the exact producer now comes from
`sizeof(shots) / sizeof(shots[0])`; a separate native layout check binds
`sizeof(PlayerSpawnShotsShotView)` to the real `PlayerShotCtorView` stride.
This is the actual shared allocation also accessed by Player UpdateShots
and the complete Player Runtime. No new shot record or shot pointer is
allocated. The still-useful `PlayerSpawnShotsView` façade remains as a
zero-size addition for separate Player state-effect/charge methods; it no
longer declares or owns `SpawnShots`.

The **1,210-byte original Player charge-update boss**, implemented in
`PlayerChargeAttackUpdate.cpp`, retains both complete native shot call
locations at original 0x0041F4C0. Its full candidate remains **1,197
bytes, 70 original-bound fields, 58 ordered calls, 1,160 unequal overlap
bytes and 13 missing bytes**, without changing any other target-bound
field/call. The independent 38-byte exact Effect Type35 update also binds
the same native shot method at its real REL32 offset +0x1B; its 72-byte
initialization partner continues to consume the Player's genuine +0xBC
ANM resource. These are *real cross-owner calls*, not separate methods
at different original addresses.

## Original PE/COFF source-cold comparison

Two native C++ callback parameter models were compiled independently,
with a raw-byte comparison of the complete 190-byte method against the
prior exact source. Both preserved all 190 bytes. The adopted native
callback parameter additionally passes strict canonical original PE/COFF
comparison. Fresh source-cold replay covers **24 existing exact owners,
5,691 complete physical bytes and 292 relocation fields** across the
exact PlayerSpawnShots, Effect35, and PlayerRuntime source profiles. The
nonexact complete ChargeUpdate is separately guarded against every
previously matching original byte and its real ABI/fields/calls. Only
these affected carriers were compiled; no full cold replay was run.

Source-linked `SpawnShots` COFF symbol spellings in the exact function,
Effect35 original call field, charge original-backed diagnostic and
function inventory were updated together. No original address, profile,
function size, relocation type, relocation addend or literal definition
was edited to force a match. The native header's expanded compiler
visibility also renumbered 17 compiler-private `$L` symbols inside the
852-byte `PlayerLifecycleView::UpdateBeforeState` and the 572-byte
`InitializeAddedState` physical owners. An independent cold recompilation
checked every actual COFF field, same-section local label definition,
original PE address and complete owner physical bytes; 11+6 were verified
as name-only, with zero changed original-relative destinations. Only those
reviewed names were updated in the unchanged strict match manifest.
The ignored local receipt is
`.analysis/gpt-web-shots-native-20261107/player-label-proof.json`.
The old façade's C++ method name was not archival evidence of the historic
game class spelling.

Private reproducible experimental controls and targeted diagnostic:
`.analysis/gpt-web-shots-native-20261107/`. Whole Windows linking, runtime
behavior, Factory oracle acceptance and remote publication remain open.
Source-present/exact/nonexact totals remain 1,034 / 1,021 / 13.
