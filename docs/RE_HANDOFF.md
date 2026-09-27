# TH09 reconstruction handoff

This is the live restart document. It contains current state and operating rules
only. Historical investigation belongs in docs/KNOWLEDGE_BASE.md and Git
history.

## Authority order

When sources disagree, use this order:

1. config/functions.csv for live authored/excluded/source-present status.
2. config/matches.csv plus config/match-units.toml for canonical exactness.
3. scripts/report-reconstruction-status.py for live totals.
4. This handoff and docs/SMALL_FUNCTION_FRONTIER.md for routing.
5. docs/KNOWLEDGE_BASE.md packets for historical evidence and negative experiments.

Historical packet words such as current, now, remains, candidate sizes,
frontier counts, and exact/non-exact status describe their checkpoint unless a
live ledger row independently confirms them.

## Current state

Supported target: original Japanese TH09 v1.50a.

SHA-256:
10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822

| Measure | Current value |
| --- | ---: |
| Function candidates | 2,191 |
| Boundary/origin unreviewed | 0 |
| Reviewed but origin-unresolved | 35 |
| Confirmed authored | 979 |
| Classified exclusions | 1,177 |
| Source-present authored mappings | 979 |
| Canonical exact functions | 881 |
| Source-present non-exact functions | 98 |
| Source-present non-exact bytes | 112,682 |
| Authored without maintained source | 0 |
| Canonical exact authored bytes | 162,987 |

The source-presence frontier is closed. Exact reconstruction is not complete.
The faithful Windows i386 product graph remains open. Semantic reconstruction
and portability have not started.

### Current six-function exact frontier

The current bounded focus contains six source-present, non-exact functions
(2,046 target bytes): `ScoreFileView::LoadScoreRecords` (437/441 candidate
bytes), `ScoreFileView::OpenScore` (523/523), `EffectManager::OnUpdate`
(475/475), `EffectManager::AddedCallback` (51/51),
`ScreenEffect::CalcShakeEnvelope` (334/336), and
`FileSystem::TryDecryptFromTable` (220/220). Candidate length is not an
exactness measure; the live rows in `config/functions.csv` retain the focused
byte evidence and semantic notes. After the latest shake-envelope source
correction, all 18 configured canonical exact units in `src/ScreenEffect.cpp`
passed a same-TU cohort replay. The shake-envelope function itself remains
non-exact.

Fresh full target disassembly rechecked the `OpenScore` chapter loop. Its one-time stack load into ESI is at relative +0x171; the backedge at +0x194 targets +0x174 and skips that load. On a TH9K match ESI receives the current chapter pointer, which remains live through later nonmatching chapters; the found flag gates the final version check. The maintained `th9kChapter` source matches this semantic flow. The target still differs in code generation and remains non-exact at 277/395 ordinary bytes. See Packet 685.

## Restart checklist

From the repository root:

    git status --short --branch
    git diff --check
    python3 scripts/verify-target.py
    python3 scripts/validate-tracking.py --require-target
    python3 scripts/validate-docs.py
    python3 scripts/progress.py --check
    python3 scripts/report-reconstruction-status.py

If any count in this file differs from the report, the report and ledgers win
and this file must be refreshed before more reconstruction work.

Attest the active semantic-analysis database independently before trusting it.
The expected image base is 0x00400000, entry point 0x0047D45F, image size
0x000E7000, and target hash is the one above. IDA names/types are provisional
semantic evidence; they never grant exactness credit.

## Exactness rules

A function is canonical exact only when it has a target-bound match unit and a
relocation-aware replay accepted by the repository workflow. Source presence,
exact logical size, adjacent-game similarity, IDA naming/decompilation, a
successful compile, or a Git commit do not grant exactness by themselves.

Prefer ordinary source explanations for target code shape. Do not keep register
forcing, volatile used only for codegen steering, artificial padding,
target-byte embedding, arbitrary var_order, fake returns, or profile roulette.
Narrow source-family-backed exceptions already documented and replayed
canonically are separate from this rule.

After a substantive exact promotion, update the ledgers and docs, run the
validators, replay the affected units, then checkpoint with a commit message of
the form:

    gpt-web: short description

## Work routing

The current <=256 authored/source-present/non-exact routing set is maintained in
docs/SMALL_FUNCTION_FRONTIER.md. Start there, but re-read the live
config/functions.csv row before editing any candidate. Size is a routing
heuristic, not a difficulty score or a verified leaf classification.

Do not resume from old roadmap numbers copied into chat or historical packets.
Recompute from the live ledger. Many former plateaus were later closed by
correcting source shape, translation-unit ownership, return contracts, local
lifetimes, or function-local compiler profiles.

Do not turn target size into a global priority rule. Interleave large owners
with smaller leaves: a large owner can unlock ABI, TU, layout, and stack-lifetime
evidence that closes multiple downstream functions, while short functions remain
useful for focused compiler experiments. In particular, do not defer
EclManager::RunEcl merely because it is large.

For a candidate with extensive negative probes, read its live ledger notes and
the latest relevant knowledge packet before trying another spelling. New target
or source-family evidence is a reason to reopen a plateau; repeating previously
rejected codegen steering is not.

### Current RunEcl handoff

At this checkpoint EclManager::RunEcl is complete maintained source but remains
NON-EXACT. A fresh clean-HEAD pinned VC7.1 build is 14,788/14,792 logical bytes
with an exact 0x168 stack frame, 375 immediate direct calls, four indirect
calls, 598 relocations, resolver counts 131/100/17/24, and all 193 compiler-table
entries present. Only four physical handler-length mismatches remain:
opcode 21 INT_SUBTRACT (-3), opcode 23 INT_DIVIDE (+3), opcode 24 INT_MODULO
(-3), and opcode 155 SET_TIMEOUT_SPELL (-1). All other RunEcl physical handler
lengths are target-length. This is a handler-length census only: it does not
mean RunEcl is four bytes or four local edits away from exact. Equal-length
handlers can still contain ordinary byte, register-allocation, or scheduling
differences, so choose the next hypothesis from a fresh complete comparison.

The 21/23/24 direct arithmetic spelling is supported by the exact adjacent TH08
source family. TH09-local probes using explicit locals, ternaries, out-helpers,
switch-wide macros, typed operand overlays, and declaration reordering do not
close the residuals. Opcode 155 is semantic/CFG-correct; natural bitfield and
shifted-value spellings retain the same one-byte allocator difference. Reopen
these only with new allocator/TU/lifetime evidence; do not add register forcing,
volatile steering, padding, assembly, or target-byte encodings.

### Current Anm loader handoff

`AnmManager::ReadAnmEntries @ 0x0043C610` has a complete 390-byte, 17-relocation
focused replay unit. The repository-local VC7.1 comparison is exact. Its
Factory receipt `receipt:5ae8d87bfdfd06c78ede36a51c033facea334af19152f97d71b9942b14b3c1ac`
returned `pass / accepted`; see Packet 665. Older failed jobs stopped before
comparison because of unrelated global locks.

### Current TitleSetupThread handoff

`TitleScreenView::TitleSetupThread @ 0x004249E1` is now canonical exact.
Under the established pinned VC7.1 `/O1 /Ob1 /Oy- /Gr` TitleScreen profile,
the maintained source reproduces all 534 target bytes and all 41 reviewed
relocations with the target `0x1C` frame.

The closing source-shape evidence is three-part. TH09 itself fixes Supervisor
`totalPlayTime` at `+0x798`, the complete setup-worker target body, and every
callee/field used here. Clean committed TH08 contributes only the source-family
hypothesis: indexed Title VM access with direct `Float3(...)` construction,
branch-local `PreloadSurface` failure handling, and a redundant
`totalPlayTime` if/else whose two arms call the same fade registration.
Replaying those shapes against TH09 makes VC7.1 emit the target VM induction,
`TEST EAX,EAX` surface check, and fastcall argument-materialization order
without register forcing or padding.

All 30 pre-existing configured `TitleScreen.cpp` units still replay exact
after the change (29 O1 TitleScreen units plus the separate O2 score-record
unit). `title-screen-start-menu` required only compiler-private `$L...` /
EH-label name refreshes; relocation offsets, types, solved target destinations
and function bytes are unchanged. The new setup-worker unit brings the source
file to 31 configured replay units.

### Current OnUpdateOptions handoff

`TitleScreenView::OnUpdateOptions @ 0x004276EB` remains source-present/non-exact.
The corrected nine-entry options source has unsigned help-text indexing,
explicit left/right option wrap branches, four volume-key switches, a 6/7/8
confirm-key switch, and an inclusive 3..4 timed-sound range. A focused pinned
`/O1 /Ob1 /Oy- /Gr` build emits 2048/2045 bytes, 135 relocations, and the
target's 51 calls, 26 unconditional jumps, and 60 conditional jumps. The
remaining register-lifetime mismatch begins after the right-scroll call at
+0x540: target clears EBX before testing AX; the candidate clears it after the
pressed branch and later uses EDI for zero and EBX for selector 6. The
exact-sized alternative with a 6/7/8 `if` chain does not reproduce the target
switch; the normalized-switch alternative inserts redundant instructions.
See Packet 662. Do not promote this owner on length or census alone.

### Current Title start-menu handoff

`TitleScreenView::OnUpdateStartMenu @ 0x00429E23` now has a focused exact
replay unit. Its 1,560 code bytes, adjacent 32-byte compiler switch table, and
all 107 relocations reproduce the target under pinned VC7.1 O1/Ob1. The
case-1 unlocked branch and immediate returns for confirmed menu choices were
the source corrections; see Packet 664 and accepted Factory receipt
`receipt:7b1487ee3c96cd6807f2fa4cefc9ecf96c75f8ccb2104c1048da6d6fedd06945`.
All 29 configured TitleScreen O1 units
and the separate O2 score-record unit replay exact after this correction.

### Current ExAttack type-4 update handoff

ExAttackUpdateCallbackType4 at 0x00442BD0 is now canonical exact under the
pinned VC7.1 /O2 /Ob1 /Oi /Oy- /Gr profile. Two cold repository-defined
replays reproduce all 739 bytes and 28 relocation destinations. The closing
one-byte residual was not register steering: target calls the shared/folded
identity body at 0x004343D0 and retains returned EAX for the collision-size
stores. Modeling that call as the already target-bound
PlayerPositionView::operator float*() instead of a guessed unique collision
constructor naturally emits the target sequence. Physical 0x004343D0
ownership remains unresolved/shared; exactness is local to this callback and
does not assign a unique source owner to that folded body. See Packet 675.

### Current ExAttack type-8/9 handoff

`ExAttackUpdateCallbackType8_9 @ 0x00446920` now lives in the EclManager
translation unit, where the maintained void `Float3::FromAngleMagnitude`
definition is visible. A cold pinned build reproduces the 748-byte target
extent and all 32 relocation destinations; 618/620 ordinary comparable bytes
match. The two residuals at +0x1E3/+0x1E7 are the collision-exit angle load and
store using ECX instead of target EAX. Keep it source-present/non-exact. All 18
pre-existing exact EclManager-TU units replay after compiler-private label
refresh only, and RunEcl remains 14,788/14,792 bytes.

### Current ExAttack type-3 handoff

ExAttackUpdateCallbackType3 @ 0x00442750 has been re-opened after the type-4
update closure invalidated the old 793-byte / 0x6C-frame plateau. The maintained
source now emits the exact 780-byte extent and target 0x64 frame. Target-backed
source-shape fixes are the indexed 31-sample history shift, direct history31
bounds, real PlayerPositionView collision/sample locals, the exact
operator float*() collision-size conversion, direct current-Y load from the
record, and three direct side/player collision receiver expressions. A cold
pinned probe resolves all 38 relocations and matches 616/628 ordinary bytes.

Keep it source-present/non-exact. The only remaining ordinary mismatch is a
12-byte scheduling window at +0x196..+0x1A7: target commits the middle sample's
z=0 store before preparing the tail-x inverse-popup call, while stock VC7.1
moves the identical store into that call-prep window. Bounded aggregate,
implicit-zero, nested-scope, declaration-order, alias, and dependency probes did
not improve this without regressing size or broader codegen.

### Current ExAttack type-11/12 handoff

`ExAttackUpdateCallbackType11 @ 0x00446EE0` and
`ExAttackUpdateCallbackType12 @ 0x004471B0` now share the EclManager
translation unit and its maintained `Float3::FromAngleMagnitude` definition.
The focused pinned build emits their target lengths, 713 and 710 bytes, with
all 30 relocation destinations solved for each. Ordinary comparable bytes are
553/593 and 550/590 respectively. The identical 40-byte residual pattern
is localized to collision-exit angle register choice and state-0 animation /
spawn-copy scheduling. Both remain source-present/non-exact. At the end of
this source batch, all 18 configured EclManager exact units cold-replayed
byte- and relocation-exact after refreshing two compiler-private jump-table
label names. Focused checks suffice between later source edits.

### Current MusicRoom update handoff

`TitleScreenView::OnUpdateMusicRoom @ 0x00426E05` remains source-present/non-exact,
but a fresh target/object replay now emits 2,254/2,258 bytes under the pinned
`/O1 /Ob1 /Oy- /Gr` profile. The candidate has the target `0x1C` frame, 25
calls, 27 unconditional jumps, 80 conditional jumps, and all 74 relocations.
This supersedes the older 2,225-byte list-loop frontier.

The target-backed list recovery keeps the song/script index absolute at
159+ in EBX while a separate zero-based VM cursor advances by `0x2A4`.
Maintained source now models that second induction explicitly, so VC7.1 emits
the target `vms + cursor + 159*0x2A4` address family, adjusted unlock-table
indexing, and the target stack-held track/Y cursors. An explicit flags-byte
pointer restores the target `LEA flags` followed by `OR byte ptr [ptr+1],18h`.
In both ready/init visibility refreshes, assigning `i = musicListingOffset`
before computing the visible bound restores the target cursor/bound register
roles.

The remaining net size deficit is four bytes: in each final hidden-row loop the
target scales the bound register and then copies it to the byte-offset cursor,
while the candidate coalesces those registers and omits the 2-byte copy.
Broader register and branch scheduling differences remain elsewhere, so equal
censuses and the four-byte size gap do not imply near-byte exactness.
`DrawMusicRoom` in the same TU still cold-replays 209/209 exact.

### Current EnemyManager draw handoff

`EnemyManagerDrawImpl @ 0x00411670` now cold-builds as 1,743/1,758 bytes
with the target 0x88 frame and matching 28-call/42-branch census. Packet 668
records the target-backed shared secondary-VM cursor, cached strip taper bit,
float-width absolute-value comparisons, sprite-field reloads, and removal of
an unused previous-angle initialization. The remaining strip loop uses EBX for
the sample pointer and a stack index in the target, while the candidate keeps
the index in EBX. It is source-present/non-exact. Both high/low draw wrappers
remain exact at 35/35 and 38/38 bytes from the same TU.

### SpawnSingleBullet exact closure

`EtamaController::SpawnSingleBullet @ 0x00412960` now replays all 1,893
logical code bytes and the complete 1,932-byte code/alignment/switch-table
extent with 47 reviewed relocations under pinned VC7.1. Packet 670 records
the target-backed zone cooldown store, low-byte FAST flag test, and direct
template-sprite read that closed the last ordinary byte differences. The
four CC bytes before exact `SpawnBulletPatternPrimary` remain unowned. All
17 other configured `BulletManager.cpp` exact units still replay exactly;
Factory receipt `receipt:1ca12fe1f0b8e40a387314c0c350dcf59f7e4079414b939594438acfeab9fb3e`
returned `pass / accepted` on code-changing commit `510d71c`. This codegen result
does not close the native product or runtime gates.

### Etama OnUpdate large-function frontier

`EtamaController::OnUpdate @ 0x004146F0` remains source-present/non-exact, but
a fresh target/object review supersedes the old 2,208-versus-2,195 aggregate
size note. The target body ends at relative `+0x893` (the `ret` is at
`+0x892`), and the pinned `/O2 /Ob1` candidate now has the same body end.
Its 2,216-byte COFF symbol extent additionally owns one post-`ret` alignment
byte and the 20-byte compiler switch table, so that auxiliary extent is not a
function-body size mismatch.

The target-backed source-shape corrections are now: bullet-loop advancement is
`++bullet, ++i`; the three spawning cases keep their completion logic separate
in source and converge through `activateBullet`, which makes VC7.1 naturally
emit direct `ESI+0x2A8`, `ESI+0x54C`, and `ESI+0x7F0` VM arguments plus
the direct `ESI+0xDC4` cancelled flag. The body still has the exact `0x40`
stack frame, 62 calls, 60 conditional jumps, 553 instructions, and 18
unconditional jumps. The draw-bucket paths tail-merge exactly and the cached
laser despawnDuration store-before-test shape remains target-backed.

Fresh current-worktree A/B comparison now matches 1,832/1,864 ordinary
comparable bytes versus 1,820/1,864 at prior baseline `b381c02`. Directly spelling all three
side-player collision/query receivers closes the former graze/bullet/box
register-scheduling window, and grouping the laser center half-length sum before
adding `position.x` closes the former x87 add-order window. The only remaining
ordinary differences are the entry `+0x27..+0x52` EBX lifetime: target saves
EBX before SelectSide, zeros EBX after the call, and uses it for all three
counter stores, while the candidate uses EDX and saves EBX at the loop
preheader. Natural zero-local and declaration-hoisting probes compile back to
the same candidate shape, so do not repeat those spellings without new
allocator/lifetime evidence. All 18 configured same-TU exact units replay
exactly after refreshing SpawnSingleBullet's compiler-private switch-table
label from `$L3092` to `$L3091`; its attested destination remains
`0x004130C8` and the full 1,932-byte compare remains exact.

### Current large ExAttack callback handoff

`ExAttackUpdateCallbackType18_24 @ 0x004491E0` remains 1435/1436 bytes.
The target has a 0x48 frame with collision size at `[ebp-0x24]`, three
distinct history-delta slots at `-0x30/-0x3C/-0x48`, and collision point at
`-0x18`. The maintained separate-TU candidate allocates those objects in a
different order. Same-TU visibility of the exact `Float3` constructor and
subtraction definitions moves collision size to the target slot but merges the
three history deltas into one slot; it does not prove original TU ownership.
Simple source declaration and condition inversions do not close the callback.
`ExAttackUpdateCallbackType21 @ 0x0044B500` remains 1074/1074 but retains
an ESI=record / EDI=extra allocation where the target uses EDI=record /
ESI=extra. See Packet 663 before repeating compiler-context probes. No
source or exactness state changed.

## Boundary and origin closure

All 2,191 tracked candidates have boundary/origin review. Current dispositions
are 979 authored, 1,177 excluded, and 35 deliberately unresolved. The
unresolved set is frozen by SHA-256:

126885e1a6a78ac42b0d81852253714cc1c9eb99141066d495b0029a16ca5695

Do not revisit the 35 unknown entries without genuinely new evidence that can
separate explicit authored source, implicit compiler-generated special members,
or folded/shared ownership.

The review scripts remain:

    python3 scripts/review-transition-data.py
    python3 scripts/review-runtime-residuals.py
    python3 scripts/apply-game-origin-review.py --group compiler
    python3 scripts/apply-game-origin-review.py --group authored
    python3 scripts/apply-game-origin-review.py --group ambiguous

Without --apply, they should report the selected dispositions as already
applied.

## Repository and artifact hygiene

Treat dirty work as current repository state to understand, not as a reason to
reset or avoid the worktree. Inspect tracked, untracked, and ignored artifacts
before changing them; discard a change only after establishing that it is
superseded or reproducible.

Keep compiler outputs/probes under build/, analysis scratch under .analysis/,
Python bytecode, temporary logs, local toolchains, Wine state, and private
analysis artifacts out of Git. Neither build/ nor .analysis/ is an exactness
authority.

resources/th09.exe is intentionally local/ignored and is the canonical target
input used by verification/replay. The pinned local compiler/toolchain input
under .tools/ is likewise infrastructure, not disposable build output. Do not
delete either during ordinary cleanup.

At the 2026-09-26 handoff cleanup, the remaining .analysis/ tree consisted of
ignored probe sources, compiler outputs, comparison/report files, and temporary
scripts. The audit found no tracked reference to a specific scratch file; live
status and current unresolved work remain represented by the tracked ledgers,
this handoff, and the historical knowledge file. The ignored .analysis/ tree,
the reproducible build/ tree, and repository Python __pycache__ directories
were therefore cleared. Historical packets may name scratch files that no
longer exist; those paths are not durable evidence. Recreate any needed probe
from tracked source/config plus the verified target and pinned toolchain, and
revalidate its inputs before relying on it.

A clean scratch tree is not evidence of codegen exactness. Canonical exactness
still comes only from the tracked match-unit/ledger state and accepted
target-bound replay.

## Documentation discipline

docs/KNOWLEDGE_BASE.md is an investigation history, not a second live ledger.
Preserve useful negative experiments there, but treat later corrections as
superseding earlier conclusions. Do not copy old packet counts into current
routing docs.

docs/PROGRESS.md and resources/progress.svg are generated from the live tracking
state. Update them through repository scripts rather than manual editing.
