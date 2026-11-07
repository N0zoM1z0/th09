# TH09 Player collision-region fields are part of the real native Player allocation

Target: verified Japanese TH09 1.50a, SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
This checkpoint **changes the source of a nonexact 862-byte Player boss** and
protects both its exact leaves and the actual region producer. The original
historic C++ names and TU boundaries remain unproven.

## Reconcile producer with actual consumer storage

Previously `PlayerLifecycleView` declared its Player +0x28BC..+0xC11C range
as one opaque byte array. The exact native region producer's separate
`PlayerCollisionRegionCreateView` already proved the following storage
coordinates inside that **same Player allocation**:

| Native member | Physical Player offset | Source extent |
| --- | ---: | ---: |
| Embedded collision region | `+0xB0BC` | `0x44` bytes |
| Active region pointers | `+0xB100` | `514` four-byte pointers |
| Released region pointers | `+0xB908` | `514` four-byte pointers |
| Active pointer count | `+0xC110` | 4 bytes |
| Released pointer count | `+0xC114` | 4 bytes |

`PlayerLifecycleView.hpp` now has these *real typed members* at the
observed addresses; a compile-time `PlayerLifecycleNativeRegionAllocation`
compares the actual `offsetof` and array extents against the independently
original-bound producer. The remaining original +0xC118..+0xC11C gap stays
opaque, followed by the unchanged 128-shot array. The complete Player
allocation is still exactly `0x30F70` bytes. These are ordinary C++
fixed-layout declarations, not a synthetic second allocation.

`PlayerLifecycleView::CheckBulletCollision` now scans its native
`activeB100` member directly. The former
`reinterpret_cast<PlayerCollisionRegionCreateView *>(this)` overlay and
borrowed pool reference are removed. The live region, eligible type
(1/2/3), delay checks, intersection formulas, cancel rewards, and
non-returning loop/return chronology are otherwise preserved. It still
walks a null-terminated active array and does **not** switch to released
storage or add a new capacity clamp. The exact region allocator continues
to publish into this same physical active array from its separately exact
source owner.

## Pinned compiler / complete original PE comparison

A frozen-input natural C++ control first compiled the new Player native
member layout and direct scan in an isolated experiment and measured the
complete 862-byte boss, then restored every tracked file. The adopted
source was rebuilt and separately measured with the original-bound
`--owner player --skip-exact-siblings` inspector:

- Target original: **862 physical bytes**, 26 original call/data fields,
  33 CFG blocks.
- Current candidate: **849 physical bytes**, same 26 fields and 12 ordered
  calls, 32 blocks, **799 unequal overlaps and 13 missing target bytes**.
- **No additional mismatches** relative to the previous maintained source;
  this remains one *nonexact* function, never partial exactness credit.

Scoped source-cold replays passed **5 exact same-PlayerCollisionHelpers
functions / 1,220 bytes / 32 original fields** and **35 additional exact
PlayerAddedState, PlayerRuntime, PlayerCollisionRegionCreate and
PlayerCollisionQuery functions / 7,256 physical bytes / 341 fields**.
Those are **40 distinct exact native Player functions / 8,476 bytes /
373 fields**. One additional dependent source-cold replay of the separate
1,436-byte, 44-field ExAttack Type18/24 exact boss also passes with this
updated native Player header: **41 distinct exact functions / 9,912 bytes /
417 fields** across the selected affected carriers. No all-function cold
replay was run.
The separate exact region allocator is included; no new authored function
is promoted.

After the new include expanded the native header, VC7.1 changed six
compiler-local `$L` *names* in PlayerAddedState and eleven in PlayerRuntime,
without moving their original instruction/table coordinates. Every changed
field was verified against the actual same-section COFF definition and the
independently recorded original PE operand before updating only the 17
manifest names. All original targets, relocation kinds, addends and
function extents stay fixed. The original and current build recipes remain
unchanged and strict.

Independent private receipts in
`.analysis/gpt-web-player-direct-region-20261107/` include the frozen
experiment inputs and comparisons, `added-local-label-proof.json`,
`runtime-labels.json`, focused actual compiler audits,
`helpers-exact.json`, and `affected-exact-final.json`. Tests guard the
native Player array against shifted offsets, incorrect producer publishing,
wrong source Player, and mistakenly scanning the released pool. Previously
textual `reinterpret_cast` assumptions were replaced with actual member
and negative data-flow checks, not deleted.

This is concrete C++ owner/layout reconstruction and original-byte
regression evidence only. The remaining nonexact boss, product/runtime,
portability, remote publication and Factory acceptance gates remain open.
Global count stays **1,034 source-present / 1,021 exact / 13 nonexact**.
