# Shared native gameplay side record, 2026-11-06

Player ChargeAttack now reads its real GameManager side allocation directly.
The separate GameManager, Player and FrontSide 0x38-byte side records share one
declaration in `src/GameManagerSideState.hpp`. Player registration and FrontSide
creation assign the actual side pointers without reinterpret casts. ChargeAttack
uses the native FrontSide, EnemyManager and CardAttack slots directly.

This is source reconstruction progress in a maintained nonexact owner. It adds
no exact function credit and changes no compiler profile or game behavior.
The partial HUD player/runtime pointee views remain distinct; sharing the side
allocation does not establish that those partial views are complete classes.
Legacy aliases identify storage slots, not recovered original C++ field names.
FrontSide's separate outer GameManager view also remains unresolved.

## Fresh target and source investigation

Direct target-bound Ghidra rechecks ChargeAttack41F810, PlayerRegisterChain41F170
and FrontSide::Create41A5D0. Disk and Program SHA-256, x86 little-endian32,
base400000, entry47D45F and all685056 mapped file bytes are attested. Actual
factory assignments and their side-index arithmetic identify the same 0x38-byte
allocation and offsets. Ghidra provides semantic evidence; the compiler Oracle
separately verifies code, physical tails/tables and relocation destinations.

The original ChargeAttack keeps the initial side in EDI and computes input from
side*0x8E+4ACE18. After its first WasPressed and IsHeld calls it reloads config,
reads sideFocusMode+0xB4 into AL and saves that byte at EBP-1. Later original
instructions read that saved byte. The maintained compiler instead keeps mode
in BL and specializes the held-input branch. It emits1197 bytes/70 fields
against the original1210 bytes, with1160 unlike overlapping bytes and13 missing.

Current-native input-pointer, pressed-result snapshot and combined models do
not recover the saved-mode flow: pointer variants produce1195/70/1153 plus15
missing; pressed snapshots reproduce the maintained owner. A shared-side alias
model preserves1197/70 exactly. An explicit-owner static fastcall model also
reproduces all1197 bytes/70 fields; its physical ABI is one ECX pointer, EAX
result and plain RET, but its different nominal symbol is not adopted. Original
C++ member/static ownership remains unknown. Same-carrier Timer definitions
remain exact20/0. No register/order hints, assembly, barriers, padding or new
volatile qualifications are introduced. Later original sideIndex reloads remain
in source; the initial snapshot does not justify caching every later read.

## PlayerDeath RNG binding correction

Affected replay exposed an existing manifest error, introduced by the earlier
RNG name migration: PlayerDeath's two entries named a nonexistent
PlayerDeathRngRuntimeView while source still declared PlayerDeathReplayRngView.
Under the actual legacy names, the complete753-byte/38-field function already
matches the original. This is preserved separately as pre-repair evidence.

Fresh Ghidra rechecks EnterDeathState41E420 and the actual406200 callee. The
latter calls signed-float RNG42AEA0 and multiplies by its parameter. It is the
existing RngRuntimeView::GetRandomF32SignedInRange, not the unsigned404910
range method suggested by the old facade's name. The source now calls
`g_ReplayRng.GetRandomF32SignedInRange(3.1415927f)` through the real RNG header.
The two manifest names now identify that genuine global/method; offsets234/239,
kinds and target addresses4ACE0C/406200 are unchanged. Complete753/38 replay
passes. Existing same-profile signed-range and signed-float units are included
in the affected RNG group. No new RNG body or ABI is fabricated.

## Affected validation scope

The shared headers affect162 fixed source/profile groups:160 groups containing
448 existing exact units, plus the separate nonexact Movement and ChargeAttack
carriers. Compilation is serial, lowered priority and bounded. It is an affected
closure replay, not a whole-game cold replay. Title replay menu, title result
and network service closures are unaffected by this header batch.

Thirteen pre-change objects cover the affected nonexact carriers; eleven reuse
verified unchanged source/profile/object proofs and two are freshly built.
All413 regular function/profile bodies, four EH graphs, auxiliary generated
code and private cleanup definition hashes are preserved in their entirety.
The two creator objects separately preserve20 regular bodies and two EH graphs,
including actual private referent definitions. These conservation checks do not
claim those compiler-generated helpers match original runtime code.

The canonical manifest refreshes only compiler-private owner-local label names
whose actual definitions retain the original owner-relative positions. It does
not change offsets, kinds, addends, destinations or comparison extents. The
strict public comparison uses actual current objects via a private full manifest.
All448 comparisons pass:139770 physical bytes and7206 fields. The refresh
changes488 actual owner-local names across22 units. Public CI passes1377 tests
with29 skips; target-required tracking, documentation and progress also pass.
Nonexact ledgers and exact totals remain1034 source-present/1021 exact/13
nonexact. Product build, runtime, semantics and portability gates stay open.

Ignored evidence packets are `.analysis/charge-mode-snapshot-20261106/` and
`.analysis/native-side-record-20261106/`. Interrupted first-pass objects and the
pre-repair PlayerDeath object are retained with their own source epochs. The
incorrect first death entry request41D510 failed and is preserved separately;
only the successful reviewed41E420 export supports the claim above.

## Source binding and retained evidence

Source commit `ef5a1072ed1ca7cad8e6b6d875ea9fe0905e980b` binds284 current
tracked inputs/Git blobs,162 affected objects and13 selected compiler/SDK files.
The source/control proof SHA-256 is:

`4e738cb13954b32bfa7a2d514de8ced339078e8db3b9b112f7dda6f88d1c032c`

`python3 -B .analysis/native-side-record-20261106/verify.py` passes an
independent read-only replay of all448 exact comparisons, all thirteen
nonexact-containing payloads, both complete creator payloads, the pre-repair
PlayerDeath753/38 comparison, six Charge controls and three full mapped-byte
Ghidra attestations. It also verifies the recipe migration permits only the
reported actual private names and two genuine RNG bindings. Source/profile,
comparison extents and every original destination remain independently pinned.

The cleanup removes178 SHA-identical per-case source copies and176 disposable
PDBs,354 files/16892462 bytes in total. All current, baseline, interrupted and
experimental objects remain. Removed inputs resolve to independently retained
frozen copies through explicit path/hash receipts; changed models stay intact.
The first-pass and pre-native-Death epochs remain separate from current source.
This local evidence does not establish Factory acceptance or remote publication.
