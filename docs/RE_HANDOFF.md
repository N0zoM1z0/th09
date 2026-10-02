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
| Canonical exact functions | 892 |
| Source-present non-exact functions | 87 |
| Source-present non-exact bytes | 96,690 |
| Authored without maintained source | 0 |
| Canonical exact authored bytes | 178,979 |

The source-presence frontier is closed. Exact reconstruction is not complete.
The faithful Windows i386 product graph remains open. Semantic reconstruction
and portability have not started.

### Earlier six-function focus

The earlier bounded focus contains six source-present, non-exact functions
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

Work has since expanded to the broader backlog in `/tmp/vc_sth.txt`; these six
remain open but are no longer the exclusive focus. That temporary list's
progress snapshot is stale: it reports 881 exact and 98 non-exact, while the
live ledger currently reports 892 exact and 87 non-exact. Recheck every listed
candidate against `config/functions.csv` and the match-unit manifest before
resuming it; use the temporary file only as a historical routing aid.

### Latest screen-8 exact closures

`TitleScreenView::UpdateScreen8Mode123 @ 0x00425ACB` is canonical exact at
2,153 bytes /564 instructions /89 relocation fields. Two cold builds reproduce
the complete owner, frame 0x0C, all raw bytes and every relocation record.
Packet 721 supersedes Packet 537's moving-VM cache: retail reloads vms after
SetSprite. Initialization really uses banks 93/61 for both selections, even
though later side0 cursor calls use 92/60. Preserve that asymmetry. Real input
snapshots, independent phase guards, equality-to-one reset, two-character array
and direct cancel-branch returns close the maintained source without ABI,
layout, visibility-body, pragma or compiler-profile changes.

`UpdateScreen8Mode0 @ 0x004289DB` is now canonical exact at 2,241 bytes
/627 instructions /116 fields (54 REL32 plus 62 DIR32). Packet 722 closes all
27 former SIB differences by assigning each selected character to the existing
real i32 value before the six RGB writes in each confirm/reset phase. Separate
branch-local integers close only nine differences; actual shared value identity
matters to VC7.1. The char pair, frame8, target stack homes, first cancel DWORD
read and later WORD check remain intact. The partial read-width view does not
introduce another data owner. Two independent maintained-source cold objects
and canonical carrier agree on every raw byte and relocation record; raw hash
is 1baacb2027e93dab4f176c7e981305867cbce7ec6a4fbb11a6e9cab44d2f48f9.
Reproduce with `python3 scripts/build-match-unit.py --unit title-screen-character-select-mode0`
and `python3 scripts/compare-coff-function.py --unit title-screen-character-select-mode0 --json`.

All 32 prior O1 exact units retain complete raw bytes and relocation records,
including StartMenu's private labels. Focused fresh carriers replay 33 O1 units
plus the separate O2 score-record-insert unit, all exact. No full-repository cold
cohort was run. Current exact coverage is 178,979 /275,669 bytes (64.93%); the
>95% objective and native/runtime gates remain open. Ordinary character
selection remains independently nonexact; Packet 724 closes screen16 below.

### Latest ordinary character-selection repair and two-byte frontier

`TitleScreenView::OnUpdateCharacterSelect @ 0x004254A7` remains NON-EXACT.
Packet 723 supersedes the stale 1577-byte/0x0C-frame candidate: two cold builds
and the canonical carrier now reproduce 1572 bytes, 421 instructions, frame8,
all 29 ordered direct calls and 85 fields (29 REL32 /56 DIR32). Full independent
replay differs only at +0x3DC/+0x3EF, the SIB bytes of the two visible-VM init
LEAs: target EAX-base/EDI-index versus equivalent candidate EDI-base/EAX-index.
No new match unit or partial credit is registered. Raw hash is
43a2b1ee0ddc2babde41d3905443ed6f6592a4c13d0f0580fb2b31ed2ddd4bbb.
Reproduce with pinned O1/Ob1 TitleScreen compilation and
`python3 scripts/inspect-title-character-selection.py OBJECT --ordinary`.

Retained source fixes two genuine old errors: difficulty 4 reads config slot 4
(base +0x8C +24*character), not slot 0; launch first clears stage 0x004A7E8C,
then sets the existing global mode 0x004B3690 to 2. It no longer uses the
provisional duplicate launch-state view in this owner. Exact GetOptionState's
0x7C table layout is unchanged. Char pair/shared integer, descending RGB stores,
short-circuit unlock checks, physical phase order and direct calls recover all
target bytes except the two recorded SIB bytes. Pointer/bank/index-spelling controls are neutral;
splitting integer scope swaps real homes, and absolute VM indices change cursor
induction. Read Packet 723 before repeating these controls. All 34 existing
TitleScreen units replay exact after one verified private StartMenu table-label
spelling refresh; no target address changes. That checkpoint's coverage was
64.60%; the subsequent screen16 closure raises live coverage to 64.93%.

### Latest screen16 final-selection exact closure

`TitleScreenView::UpdateScreen16 @ 0x00426334..0x004266B4` is now canonical
exact: 897 contiguous code/physical bytes, 246 instructions, frame8 and 62
fields (14 REL32 /48 DIR32). Packet 724 corrects two old behavior errors:
launch clears stage 0x004A7E8C, not global mode; cancel calls mode0 only for
mode 0, mode123 only for modes 1/2/3, and neither for mode 4. Init reloads
the order table after SetSprite and snapshots the selected char before resets.
Natural four launch-mode if/else arms, separate cancel switch-call arms and
an index-first hidden-entry pointer loop reproduce the target shared tails
and byte-offset induction. No explicit byte cursor or goto probe is retained.

Two independent maintained-source cold builds, final probe and canonical
carrier agree on every raw byte and full relocation record. Raw SHA-256 is
662a50707d3d30a3091593a7399a9f7bd3a79b9074f150ad3f082d5f90b86a4f.
Reproduce with `python3 scripts/build-match-unit.py --unit title-screen-update-screen16`
and `python3 scripts/compare-coff-function.py --unit title-screen-update-screen16 --json`.
The tracked diagnostic also supports `OBJECT --screen16` without granting credit.
All 33 prior O1 owners retain raw bytes; nine StartMenu private labels change
spelling only after section/owner-relative offsets and unchanged destinations
are verified. Fresh focused O1 plus separate O2 carriers replay all 35 exact
units. No full-repository cold cohort runs. Totals are 892 exact /178979 of
275669 authored bytes (64.93%), with 87 nonexact /96690 bytes. The >95% goal
and native/runtime/semantic/port gates remain open. Next rotate to the adjacent
result/replay owner with fresh target review; do not repeat Packet 724's
manual-byte-cursor or equivalent pointer-spelling probes without new evidence.

### Earlier non-exact investigation: Player movement and KeyConfig

`PlayerLifecycleView::UpdateMovementAndOptions @ 0x0041C170` remains
non-exact. Packet 720 corrects the measurement scope: target code is 1,835
bytes, but the physical owner is 1,900 bytes including one alignment byte and
two eight-entry switch tables. Old candidate measurements of 1,864 bytes
included tables/alignment, not 1,864 bytes of code. Maintained source now
reloads the side and ANM owners across actual calls, uses the target's counted
four-option traversal, and copies the first history vector as a real aggregate.
Two independent builds reproduce 1,787 code /1,852 physical bytes, all 66
relocation records and the complete 23-direct-call order. The target-bound
`scripts/inspect-player-movement.py` replays independently reviewed external
destinations and actual COFF internal labels; its normalized 433/489 instruction
alignment is diagnostic only and grants no coverage. Remaining branch selection,
SHT load/angle/FPU scheduling, register lifetimes and option-loop layout are open.

Packet 719 also records a negative KeyConfig real-callee-visibility probe:
the actual SoundPlayer definition leaves the entire candidate unchanged at
2,803 bytes /2,325 of 2,331 ordinary bytes. No sound/source/profile changes
are retained. Packet 720 ended at 889 functions /173,688 bytes (63.01%);
Packet 721 and the live table above supersede those totals.

### Latest large-owner closure: FrontSide::OnUpdate

`FrontSide::OnUpdate @ 0x00418A90` is canonical exact at 2,381 bytes.
Two independent pinned VC7.1 `/O2 /Ob1 /Oy- /Gr` builds reproduce the complete
owner, all 95 relocation fields, the 4-byte frame and all 62 direct calls.
All fourteen pre-existing exact units in `src/FrontSide.cpp` remain exact;
the focused fresh carrier now replays fifteen units without manifest repairs.

Target review recovers phase-local Player/runtime reloads, the folded timer
current-read call, counted strobe loops, packed immediate meter colors,
shared quotient/remainder snapshots and the re-evaluated rank bound without
the reconstruction-only seven-item clamp. The transition timer is driven by
`auxA678.unknown554` at owner +0xABCC, not the independent meter state +0xA65C.
Expression-only layout views remove frontend accessor temporaries; retaining
Player only within the pulse phase naturally closes the last address-generation
window. Packet 535's larger scoped-alias probe is not evidence that the old
broad cache was faithful. No extra callee bodies, profile changes or register
directives are retained. See Packet 718 for bounded controls and full replay.
Folded getter ownership and native product/runtime closure remain independent.

### Latest widened-backlog closure: Background stage script and camera interpolation

`BackgroundRunStageScriptPhase @ 0x004018F0` and its private
`InterpolateBackgroundCameraVector @ 0x004016E0` are canonical exact at
2,472 and 423 code bytes. Two independent pinned VC7.1 `/O2 /Ob0 /Oy- /Gr`
builds reproduce the complete 2,652- and 448-byte physical extents, including
all 133 and 23 relocation fields and every compiler switch-table entry.
Only 2,895 code bytes receive new coverage credit; table/alignment bytes do not.

Actual same-TU `ZunTimer::operator>=`, `operator<` and scalar Hermite
definitions recover compiler register knowledge without inlining or artificial
ABI steering. Their single maintained definitions now reside in
`src/BackgroundStageScript.cpp`; the existing three exact units remain exact.
This is a compile-carrier visibility result, not unique original TU ownership.
All five exact units in this carrier and all eighteen remaining exact units in
`src/ZunTimer.cpp` passed focused fresh-object replay.

TH08 supplied the case-grouping and instruction-reload-loop hypotheses; TH09
target review independently supports them. Target opcode 10's mode-before-timer
assignment, camera-motion guard/case order and direct single-use angle argument
close the remaining scheduling differences. Packets 415/454's register plateau
is superseded, not an established compiler limitation. See Packet 717 for
negative controls, full relocation review and reproduction commands. Native
product/runtime closure remains open.

### Latest widened-backlog closure: EnemyManager::SpawnEnemy

`EnemyManagerView::SpawnEnemy @ 0x0040F340` is now canonical exact. Two cold
pinned VC7.1 `/O2 /Ob1 /Oi /Oy- /Gr` builds reproduce all 373 bytes and all
four relocations; the complete unit replays at 373/373. The target's separate
primary/opposing ECL failure branches and shared successful record tail are
captured with an ordinary `for/continue/break` scan. TH08's corresponding
`SpawnEnemy2` supplied only that source-shape hypothesis; TH09 evidence fixes
the 128-slot scan, adjacent 129th-record sentinel, manager split, copied
0x78-byte context, record offsets, and ABI. See Packet 686.

### Latest widened-backlog closure: Background::OnDrawHighPrio

`Background::OnDrawHighPrio @ 0x004033E0` is now canonical exact at 671 bytes.
The target callback selects clear behavior from `clearColor` but takes the
opaque clear color from `skyFog.color`; it applies RGBA mix for that clear and
RGB-only mix for later fog state. The translucent branch draws the sky/fog
color square and clears only Z. It also gates the two stage VMs independently
by script index, transforms stageVm0 x from -144 and sets z to 0.99, renders
objects 0/1 while spell state is at most 1, and restores mix color unless tint
is retained. The canonical unit reproduces all 671 bytes and 46 relocation
destinations. All eight configured exact units in `src/Background.cpp`
replayed exactly after the change; see Packet 688.

### Latest widened-backlog closure: TitleScreenView::OnUpdateDifficultySelect

`TitleScreenView::OnUpdateDifficultySelect @ 0x0042A45B` is now canonical exact
at 886 bytes. Two cold pinned VC7.1 `/O1 /Ob1 /Oy- /Gr` builds reproduce all 45
relocation destinations. Spelling both mode switches with separate case arms
matches the target's decrement chains while preserving their distinct mode-4
behavior. Its adjacent `OnUpdateModeSelect` remains 506/506; the preceding
`OnUpdateStartMenu` remains 1560/1560 code bytes and 1592/1592 including its
switch table. See Packet 699.

### Latest large-owner progress: EnemyManager::OnUpdate

`EnemyManagerView::OnUpdate @ 0x00410730` remains non-exact but now cold-builds
the complete 3,883-byte body and 3,900-byte physical extent, with the target
`0x2A8` frame, all 69 direct calls in physical order, and matching scalar/Float3
stack homes. Packet 702 supersedes the old 3,824-byte / 67-call plateau. The
Front-owned script gate, conditional position conversion, sprite/Player reloads,
schedule reads, record induction, death-case order, and shared draw-list tail
are now target-backed. Packet 703 recovers actual shared-zero leaf visibility,
the single-precision homing comparison, typed death-descriptor copy, and
index-first loop induction. Complete ordinary comparison is 3,271/3,512; relocation
positions still differ, so no exact promotion is made. Focused same-TU helper
replay remains 151/151. Resume from Packet 703's localized special descriptor,
early draw-index, trail-copy scheduling, and homing register-role residuals.
Packet 708 rules out actual descriptor-constructor visibility and a named,
used homing world-position pointer: both leave owner bytes unchanged. The
equivalent arithmetic bullet-count expression loses a required Float3 call;
do not repeat these controls or Packet 702's position/type store permutation.
The 95% authored-byte objective remains active and incomplete.

### Current FrontSide draw handoff

`FrontSide::OnDraw @ 0x004193E0` remains non-exact. Packet 709 replaces
reconstruction-only layout-cast accessor functions with direct-expression
macros and restores all three point constructions within each horizontal
transition-mode arm. This reopens the old /Ob0 diagnostic: it now emits the
target generic vertex-array constructor and all 116 calls without artificial
accessor calls. Maintained /O2 /Ob1 is 3088/3094; /O2 /Ob0 is 3085/3094.
Both still have frame 0x68 instead of target 0xA4. The six target Float3
temporary homes and scalar/counter homes remain unresolved. Equal +0x582
transition offsets never established full HUD-prefix byte equality. Read
Packet 709 before repeating the failed point-reference or real-constructor
visibility controls. Neither profile is an accepted owner match.

### Latest large-owner closure: FrontMessageRuntimeView::Update

`FrontMessageRuntimeView::Update @ 0x00416590` is now canonical exact.
Packet 713 recovers timer advancement when the first message is not yet due,
natural while-loop and cold-return structure, actual branch-local portrait
lifetimes and direct VM member expressions instead of an inline accessor.
These changes retain Packet 712's target-backed payload/global reload fixes
and close its opcode 1/2/3 and run-script scheduling residuals. Two independent
cold pinned /O2 /Ob1 builds reproduce all 2852 code bytes, the complete
2968-byte extent including the 116-byte table, and all 167 relocations.
Decoder remains 39/39 exact. Credit covers code only; native product and
runtime gates remain open. Read Packets 712-713 before reusing stale lengths,
timer/payload behavior or shared-tail controls.

Packet 714 closes the cross-unit timer contract audit exposed by that replay.
AsciiManager::OnUpdate expires score popups with strict `timer > 60`, not
`>= 60`: target call 0x00435B77 reaches SETNLE at 0x00403DE0. The old source
and relocation symbol disagreed with that callee even though address replay
was exact. Source and symbol are now corrected; two independent builds and
all eleven configured AsciiManagerMenu.cpp replays are exact. Eight already
stale AsciiMenuState4 private label names are refreshed without byte/target
changes. This is a contract repair, not new exact coverage.

### Current Front calc handoff

`FrontCalcCallback @ 0x00417630` remains non-exact. Packet 715 supersedes the
old 1476-byte / cursor-CSE plateau: natural indexed 2x5 panel loops produce
all three target cursors, the outer count at EBP-4 and the target 0x10 frame.
The global-first dynamic-sprite comparison and direct signed-short message /
result-table expressions additionally recover target schedules. Two fresh
pinned /O2 /Ob1 builds in Packet 716 emit 1510/1491 bytes, 389 instructions,
42 target-ordered calls and 108 relocations. Repeated transition-side member
reads, rather than a broad local snapshot, now reproduce frame60's EDX
retention and failed-float-path second test. Run
`python3 scripts/replay-front-calc-regions.py OBJECT` to independently replay
the 395-byte prefix and 813-byte frame-dispatch-through-return tail; both
regions have zero differences, including 26/56 reviewed relocation fields.
This is diagnostic independent placement, not a whole-function replay or
partial coverage credit. Resume from mode0/1 completion CFG sharing only:
target shares the full side/stage body and physically puts mode1 after the
mode0 body; candidate repeats side/stage tests (+19 bytes). Ordinary OR,
predicate flags/switches, duplicate-body inversion, early return/goto,
GameManager field views and real mode-body visibility controls do not close
the owner. Neither extent proximity nor subrange equality permits promotion.

### Current Player selector handoff

`PlayerUpdateSelectorState @ 0x004049A0` remains non-exact. Packet 711
additionally recovers triangular grid starts 0/1/2, separate half-size/radius
cursors, call-relative Player/position snapshots and the 10x10 tracking box.
Tracking changes protocol direction, not the retained pattern/history; its
rightward collision probe deliberately receives the old pattern. Protocol
reloads, signed RNG-to-float interpretation and post-copy history table reads
now follow TH09. Maintained /O2 /Ob1 independently reproduces 3275/3238 bytes,
frame 0x5C, 33 calls and 135 relocations; normalized alignment 723/960 is only
diagnostic. Same-TU `ResolvePatternOffset` remains exact across 346 code / 412
physical bytes after two private table-label refreshes. Expanded real helper
operations plus one used offset workspace under /Ob0 now recover the generic
constructor and both selectedPattern/alternate homes (-0x18/-0x10), but the
current 3288-byte owner is still non-exact. The current ignored reproducer is
input-bound under `.analysis/player-selector-711-20261002`; older fixtures are
superseded and removed. Resume from the first timing/config-pointer lifetime,
scalar/vector scheduling and tracking-tail CFG, not arbitrary local order or
flag wrappers. Read Packets 710-711 before treating constructor/size similarity
as acceptance. The >95% goal remains active and incomplete.

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

Packet 704's complete direct-call-order review found and corrected opcode 169's
reversed physical branch order and unordered-float predicate. Its normalized
angle arm now precedes its subtract-only arm, and all 375 direct calls pair in
physical order with consistent destinations. The handler remains 186 bytes;
140/142 ordinary bytes match, with only the two correct shared-restart jumps
encoding the upstream four-byte displacement difference. This does not promote
the owner or imply that its many equal-length residuals are closed.

Packet 705 additionally recovers physical branch order for opcodes 62/113/137
and the world-position/callback-flag prefix schedule. The maintained owner stays
14,788/14,792. A retained ordinary world-result lifetime probe now closes the
21/23/24 handler lengths and much of opcodes 8-39, disproving a blanket backend
impossibility claim, but breaks the first return-temporary home/copy and other
vector homes. It is hypothesis evidence only, not the maintained source or an
exact unit; see Packet 705 before reopening those arithmetic handlers.

Packet 706 recovers the child-context selection tail by reading/writing the
newly installed active context, rather than accessing the embedded child fields
directly. Its 46-byte region has only one shared-restart displacement difference;
the maintained owner still has no new exact credit. Real ResolveFloatLValue body
visibility leaves both the maintained and retained lifetime-probe bytes unchanged.

Packet 707 replaces the separate time-scale alias with the already verified
GameManager speedEC field. Flattening the six fragment scopes, moving the world
assignment inside the loop, and exposing the actual vector/operand bodies do
not change RunEcl instruction bytes; these are now negative compiler evidence,
not reasons to repeat those probes. The Packet 705 lifetime hypothesis remains
unresolved, and the owner remains non-exact.

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

### Current EtamaController draw handoff

`EtamaController::OnDraw @ 0x00413BE0` remains source-present/NON-EXACT at the
maintained 568-byte candidate versus the 600-byte target. Fresh direct TH09
disassembly confirms the 48-record Laser loop carries two pointer streams: a
stack-held cursor starts at the first Laser-record base and is passed as the
body VM to `AnmManager::Draw2D`; ESI is separately biased by `+0x208` for
Laser fields and the start-cap VM. Both advance by `0x59C` per record. These
are target address-flow facts, not recovered C++ declarations. A simple
natural `Laser::bodyVm` alias probe did not explain the target and was
discarded. Exact `DrawSingleBullet` remains a separate helper result; see
Packets 700-701.

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
