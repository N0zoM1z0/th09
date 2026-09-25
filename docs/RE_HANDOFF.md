# TH09 reconstruction handoff

This is the live restart document. It intentionally contains current state and
current priorities only. Historical packet-by-packet investigation belongs in
Git history and docs/KNOWLEDGE_BASE.md.

## Current state

Supported target: original Japanese TH09 v1.50a.
SHA-256: 10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822.

| Measure | Current value |
| --- | ---: |
| Function candidates | 2,191 |
| Boundary/origin unreviewed | 0 |
| Reviewed but origin-unresolved | 35 |
| Confirmed authored | 979 |
| Classified exclusions | 1,177 |
| Source-present authored mappings | 979 |
| Canonical exact functions | 865 |
| Source-present non-exact functions | 114 |
| Source-present non-exact bytes | 125,913 |
| Authored without maintained source | 0 |
| Canonical exact authored bytes | 149,756 |

The source-presence frontier is closed. Exact reconstruction is not complete.
The faithful Windows i386 product graph remains open. Semantic reconstruction
and portability have not started.

## Restart checklist

From the repository root:

    git status --short --branch
    git diff --check
    python3 scripts/verify-target.py
    python3 scripts/validate-tracking.py --require-target
    python3 scripts/report-reconstruction-status.py

Attest the active IDA database independently before using semantic evidence.
Require the same target hash, image base 0x00400000, entry 0x0047D45F and image
size 0x000E7000. IDA names and types are provisional evidence, not exactness
credit.

Before editing, inspect all tracked and untracked work. Private target binaries,
IDA databases, toolchains, compiler outputs and analysis artifacts must not be
committed.

## Boundary and origin closure

The complete 2,191-candidate inventory has boundary/origin review. Current
dispositions are 979 authored, 1,177 excluded and 35 intentionally unresolved.
The unresolved set is frozen by SHA-256
126885e1a6a78ac42b0d81852253714cc1c9eb99141066d495b0029a16ca5695.

Do not revisit the 35 unknown entries without genuinely new evidence that can
separate explicit source, implicit compiler-generated special members or
folded/shared ownership. The read-only closure scripts remain:

    python3 scripts/review-transition-data.py
    python3 scripts/review-runtime-residuals.py
    python3 scripts/apply-game-origin-review.py --group compiler
    python3 scripts/apply-game-origin-review.py --group authored
    python3 scripts/apply-game-origin-review.py --group ambiguous

Without --apply they must report the selected dispositions as already applied.

## Exact reconstruction state

The authoritative live totals come from report-reconstruction-status.py and the
tracking ledgers. At this checkpoint there are 979 source-present authored
functions: 859 canonical exact and 120 honest non-exact.

Canonical exactness requires a target-bound match unit and relocation-aware
replay. Maintained source, exact size, adjacent-game similarity, IDA naming or
successful compilation do not by themselves justify a match.

The authored/no-source roadmap is obsolete and must not be restarted. All
confirmed authored functions now have maintained source.

The numbered checkpoint notes below are historical summaries, not current
totals or priorities. Use the live counts above and the current frontier
sections later in this document; full packet evidence is in
`docs/KNOWLEDGE_BASE.md`.

Packet 528 makes `Th09EclRunControl::ApplyInterpolationOperation @ 0x00407560`
canonical exact at 157/157 bytes with five solved REL32 relocations. Its sole
RunEcl caller supplies Enemy in EDI and the instruction in ESI; an internal
`static` helper in the same translation unit recovers that private compiler
transport naturally. Two cold match-unit replays pass, and the 12 other exact
EclManager-TU units plus `InterpolateLinear` remain structural-exact. The
43-entry short-function frontier was updated in `docs/SMALL_FUNCTION_FRONTIER.md`.

Packet 529 closes `Th09EclRunControl::InstallInterpolationSlot @ 0x00407600`
at 255/255 bytes in two cold pinned-VC7.1 replays, with nine relocations
resolved to the target timer setter, operand resolvers, callback table, and
float resolver. Its sole RunEcl caller supplies Enemy in EBX and instruction
in ESI; the same-TU static source recovers that private transport. The
callback table's source/data owner remains unresolved, so this function-level
match does not close product linkage. The short frontier is now 42 functions.

Packet 530 closes `Th09EclRunControl::CallSubroutine @ 0x00407A80` at
149/149 bytes in two cold pinned-VC7.1 replays, with its sole relocation
resolved to `InitializeSubroutine @ 0x00406850`. Both target xrefs are inside
RunEcl: the remote-call path and the ordinary CALL opcode path. The helper's
same-TU static source preserves the target's context rereads around
initialization and shared-state copying; the exact function does not close the
larger RunEcl owner or product graph. The short frontier is now 41 functions.

Packet 541 closes `Th09EclRunControl::CompareOperands @ 0x00407700` at
842/842 logical bytes. Two cold pinned-VC7.1 `/O2 /Ob1` replays also match its
full 892-byte physical code/alignment/switch-table extent and resolve all 37
relocations. The sole RunEcl caller and same-TU `static` source reproduce the
target-private EDI=Enemy / ESI=instruction entry transport; four following CC
bytes remain unassigned. All 16 configured `EclManager.cpp` match units replay
exact. This helper is larger than the <=256-byte short-function filter, so the
40-entry short frontier is unchanged; RunEcl and Windows product closure remain
open.

Packet 542 closes `FrontSide::SetTimingDigits @ 0x00418950` at 319/319 bytes
with five target-resolved `AnmLoaded::SetSprite @ 0x00436AC0` relocations.
Target control flow has the zero-hundreds VM clear as the fallthrough and the
nonzero sprite setter on the taken branch; reversing the natural source branch
layout corrected the prior 261/299 ordinary-byte candidate without ABI or
register steering. The two `PlayerUpdateSelectorState` callers and the
`SetPatternTiming` formatter path remain independently owned. All 13 configured
`FrontSide.cpp` exact units replayed exact across two cold TU builds. The
function is larger than the <=256-byte frontier; `OnUpdate`, `AddedCallback`,
the larger FrontSide owners, and Windows product closure remain open.

Packet 544 closes `ScreenEffect::CalcShake @ 0x00422660` at 316/316 bytes with
all 16 target-bound relocation destinations resolved. The natural source now
retains the target's one left-associated x87 amplitude expression, snapshots
duration after incrementing the timer, and keeps the two freeze gates as
separate early returns. Two cold canonical unit replays and the connected
ScreenEffect exact-unit regression pass. This is outside the <=256-byte short
frontier; Windows product closure remains open.

Packet 545 advances `CardAttack::OnDraw @ 0x00404410` from a 324-byte candidate
to a stable 334-byte source candidate by walking the five secondary VMs with a
natural `AnmVm *` cursor. Two cold builds match 194/266 ordinary bytes and
resolve all 17 relocations, but instruction-scheduling and register-assignment
residuals remain; it is still non-exact and has no canonical match entry.

Packet 546 corrects the maintained return behavior of
`SupervisorFrameQueueView::InsertReceivedFrame @ 0x0042E9E0`: the target leaves
`this + 12 * (side * 10 + insertionIndex)` in EAX, although its current callers
discard that value. The source now returns the same cursor and the exact
`ResetTitleMode4Supervisor` caller still replays 177/177 with 12 relocations.
The helper itself remains non-exact: the 211-byte source differs from the
213-byte target from offset `+0x1C`, so its two-byte size gap is misleading;
see Packet 546. The short frontier remains 40.

Packet 547's 251-byte explicit-placement `/Ob1` result is historical and was
superseded by Packet 565. The target-backed Chain `/Ob0` lowering plus the
non-throwing `ChainElem` constructor makes ordinary `new ChainElem()`
reproduce the exact allocation and scalar-delete behavior.

Packet 548 advances `Supervisor::Supervisor @ 0x00431500` from a 61-byte
compound-OR source candidate to a cold-stable 62-byte `/O1 /Ob1` candidate by
expressing the same final flags value directly after the target-proven full
object clear. All three relocation destinations resolve and 41/50 ordinary
bytes match; nine bytes still differ from `+0x2F`, so the function remains
non-exact and the frontier remains 40.

Packet 550 finishes the bounded `EnemyView::HandleDeathRewards @ 0x004102B0`
owner/ABI cleanup. Its source now calls the canonical SoundPlayer, RNG,
GameManager popup-transform, Player collision-region, and Supervisor APIs;
shared `SoundPlayer.hpp` and `PlayerCollisionRegionCreate.hpp` declarations
remove the prior TU-local symbol shims. A fresh pinned VC7.1 object emits the
reviewed decorated owners and a 1,130-byte natural candidate against the
1,142-byte target. It remains NON-EXACT with no match credit; see Packet 550 in
the knowledge base. This is the stopping checkpoint for the current function
batch; choose future work from the live frontier only after recomputation.

Packet 551 closes `ZunTimer::ZunTimer @ 0x00403390` at 12/12 bytes by correcting
translation-unit ownership rather than steering registers. With the constructor
separated from the `Initialize` definition, pinned VC7.1 naturally preserves
`this` in ESI across the call and exactly reproduces the target; the sole REL32
resolves to exact `ZunTimer::Initialize @ 0x004014A0`. This moves the live totals
to 811 exact / 168 non-exact and reduces the <=256-byte frontier to 39 functions,
12 of them at most 128 bytes. See Packet 551 in the knowledge base.


Packet 552 closes `InputView::IsPressedScrolling @ 0x00423158` at 42/42
bytes. Target callers use `test ax, ax`, corroborating the retained 16-bit
return, while the callee masks both +0x06 and +0x04 fields with the 32-bit
argument. A single reused `u16` temporary in the short-circuit OR naturally
recovers the target's two load/mask/test stages and shared 0/1 result blocks.
The live totals are now 812 exact / 167 non-exact; the <=256-byte frontier is
38 functions, 11 at most 128 bytes. See Packet 552 in the knowledge base.


Packet 553 closes `AsciiManager::OnUpdate @ 0x00435B00` at 253/253 bytes.
The prior signed-side-index probe was aimed at the wrong value: the target's
sole `JL`/`JB` residual compares the absolute sideFlags address in EBX
against the fixed end-field address. Expressing that address comparison as
signed 32-bit changes only `+0xA2` and preserves all fourteen relocations.
The live totals are 813 exact / 166 non-exact; the <=256-byte frontier is 37
functions, 11 at most 128 bytes. See Packet 553 in the knowledge base.


Packet 554 closes `TextHelper::CopyTextToSurface @ 0x004361C0` at 242/242
bytes by correcting TU ownership, with no function-body rewrite. When the method
shared `TextHelper.cpp` with trivial getter definitions, VC7.1 kept the source
buffer pointer live in EDX across calls and emitted 224 bytes with a 0x48 frame.
An independent TU restores the target 0x4C frame, pointer spills/reloads, loop
layout, and all eight relocation destinations. Live totals are now 814 exact /
165 non-exact; the <=256-byte frontier is 36 functions. See Packet 554.


Packet 555 closes `InterpolateHermite @ 0x004074A0` at 188/188 bytes.
Fresh probing disproves the earlier fixed-FPU-plateau conclusion: explicit
Hermite basis grouping plus explicit left association of the four weighted
terms makes VC7.1 reproduce the target parameter/local stack slots and complete
x87 evaluation order, with all nine relocations solved. Live totals are now
815 exact / 164 non-exact / 140,683 non-exact bytes; the <=256-byte frontier is
35 functions, 11 at most 128 bytes. See Packet 555.


Packet 556 closes `ScoreFileView::LoadLastName @ 0x00421AA0` at 215/215
bytes. Reordering only semantically independent default-record initialization
makes VC7.1 use the target EBX/ESI nine-byte name-copy schedule; assigning
`chapterSize` before `chapterSizeCopy` also aligns the two 16-bit COFF
addends with the real `g_LastNameRecord` base. All 23 DIR32 relocations
replay exactly. Live totals are now 816 exact / 163 non-exact / 140,468
non-exact bytes; the <=256-byte frontier is 34 functions. See Packet 556.


Packet 557 closes `PlayerLifecycleView::CalcCircleCollision @ 0x0041BE70`
at 106/106 bytes. The target's `TEST AH,41h / JP true` proves a different
x87 unordered edge case from the old direct less-than spelling. Expressing the
observed control flow as a >= rejection followed by `return 1` reproduces
the full arithmetic, status-word test, branch layout and sole Float3-subtraction
relocation. Live totals are now 817 exact / 162 non-exact / 140,362 non-exact
bytes; the <=256-byte frontier is 33 functions, 10 at most 128 bytes.


Packet 558 closes `GameManagerPlayfieldView::IsWithinPlayfield @ 0x0041A6EB`
at 116/116 bytes. TH09's right/bottom edge tests use constant-left comparison
forms, and neighboring canonical GameManager units establish the missing
`/O2 /Os /Ob1` profile evidence. Together those two corrections recover all
x87 branch/cleanup details and six constant relocations. Live totals are now
818 exact / 161 non-exact / 140,246 non-exact bytes; the <=256-byte frontier is
32 functions, 9 at most 128 bytes.


Packet 559 closes `SupervisorFrameQueueView::PopFrame @ 0x0042EB80` at
119/119 bytes. Removing the explicit `sideBase` local and retaining repeated
receiver-relative queue expressions lets VC7.1 CSE the adjusted side base into
ECX exactly as the target does; EDX then naturally carries predicted data and
the shift count. The sole replay-input relocation also matches. Live totals are
now 819 exact / 160 non-exact / 140,127 non-exact bytes; the <=256-byte frontier
is 31 functions, 8 at most 128 bytes.


Packet 560 closes `ExAttackAllocateRecordView::AllocateDynamicData @ 0x00440D90`
at 81/81 bytes. Repeated natural `vmCount * 0x2A4` expressions expose the
scaled VM byte count as a common value that must survive allocation/memset;
VC7.1 therefore keeps it in EBX from entry and removes the old two-byte
EAX-to-EBX transfer. All three relocations replay exactly. Live totals are now
820 exact / 159 non-exact / 140,046 non-exact bytes; the <=256-byte frontier is
30 functions, 7 at most 128 bytes.


Packet 561 closes `EtamaController::SelectBulletSprite @ 0x004124E0` at
161/161 bytes. The prior `int` source return was inferred only from
incidental EAX residue; all three TH09 callers discard EAX and the TH08 family
uses `void`. Restoring a void branch-local `SetSprite(...); return;`
shape naturally produces all three target `bulletAnm` reloads and seven
relocations. Live totals are now 821 exact / 158 non-exact / 139,885 non-exact
bytes; the <=256-byte frontier is 29 functions, 7 at most 128 bytes.


Packet 562 closes `SupervisorFrameQueueView::InsertPredictedFrame @ 0x0042EAC0`
at 191/191 bytes. Keeping targetFrame live across the shift, advancing the
frame-field scan cursor itself, preserving target loop-increment order, and
using an object-relative final entry cursor recover the target side/base/pointer
lifetimes without register forcing. Live totals are now 822 exact / 157
non-exact / 139,694 non-exact bytes; the <=256-byte frontier is 28 functions,
7 at most 128 bytes.


Packet 563 closes `EnemyView::CleanupAfterDeactivation @ 0x00410110` at
184/184 bytes. A timer-threshold pointer alias after the four life-threshold
stores gives VC7.1 the target lifetime split: four stores, fastcall ECX setup,
timer store, then exact child-ECL cleanup. Both helper relocations replay exactly.
Live totals are now 823 exact / 156 non-exact / 139,510 non-exact bytes; the
<=256-byte frontier is 27 functions, with 7 at most 128 bytes.


Packet 564 closes `SupervisorNetworkState::IsServiceProviderAvailable @
0x00432770` at 199/199 bytes. Restoring the DirectPlay SDK-family initialized
locals and cleanup lifetime recovers the target EDI/ESI/EBX split and shared
zero-register lowering, with all twelve relocations exact. Live totals are now
824 exact / 155 non-exact / 139,311 non-exact bytes; the <=256-byte frontier is
26 functions, 7 at most 128 bytes.


Packet 565 closes `ChainReleaseView::ReleaseSingleChain @ 0x0042CAE0` at
241/241 bytes. The decisive combination is ordinary `new ChainElem()` with
the already target-backed Chain `/O2 /Ob0` profile: the non-throwing
constructor suppresses ctor-failure cleanup, while `/Ob0` preserves the
compiler-generated scalar deleting-destructor call required by the target.
All 18 relocations replay exactly, and the exact `ChainReleaseView::Release`
wrapper remains exact independently. Live totals are now 825 exact / 154
non-exact / 139,070 non-exact bytes; the <=256-byte frontier is 25 functions,
7 at most 128 bytes.


Packet 566 closes `PlayerShotUpdateCallbackType4 @ 0x00443930` at 249/249
bytes. Restricting the `position` alias to the aggregate-copy/center work and
spelling the initial Y integration directly as `shot->position2A4.y` makes
VC7.1 keep shot in ESI and the position pointer in EDI exactly as the target
does. All nine relocations replay exactly. Live totals are now 826 exact / 153
non-exact / 138,821 non-exact bytes; the <=256-byte frontier is 24 functions,
7 at most 128 bytes.


Packet 567 closes `LoadPlayerShtFile @ 0x0041BBE0` at 172/172 bytes.
Correcting physical `0x00401070` from the old double-returning alias to the
repository-proven float `sinf` contract restores the target x87 stack reuse.
Keeping `shtFile` short-lived for the first derived-size store while rereading
`*header` for the second store and callback-table loop also restores the
target-private ESI out-pointer / EDI file-pointer lifetime. All six relocations
replay exactly. Live totals are now 827 exact / 152 non-exact / 138,649
non-exact bytes; the <=256-byte frontier is 23 functions, 7 at most 128 bytes.


Packet 568 closes `PlayerCollisionQueryStateView::FindCollisionAtPlayer @
0x0041DA70` at 279/279 bytes. Reloading `player00` between the two
collision-bound assignments reproduces the target EBX lifetime exactly: EBX is
consumed as the first destination base, reloaded as Player, then consumed again
as the second destination base. All nine relocations replay exactly. Live totals
are now 828 exact / 151 non-exact / 138,370 non-exact bytes. The <=256-byte
routing frontier remains 23 functions because this helper is 279 bytes.


Packet 569 is a historical checkpoint **superseded by Packet 571**. It refined
`UpdatePlayerCollisionRegions @ 0x0041C8E0` from a 312-byte candidate to the
exact 273-byte target extent, but still had ten ordinary-byte differences in
pointer compaction. Do not resume from that NON-EXACT state; Packet 571 closes
the function completely.


Packet 570 closes `ExAttackInitializeCallbackType3 @ 0x00442580` at
454/454 bytes. The old one-byte-gap diagnosis was wrong: target and candidate
already shared the 0x1C frame. The real divergence was manual x/y/z copying
from POD `spawnPosition` into a `Float3` record field. Restoring one
same-layout aggregate assignment reproduces the target copy schedule and all 28
relocations exactly. Live totals are now 829 exact / 150 non-exact / 137,916
non-exact bytes. The <=256-byte routing frontier remains 23 functions because
this helper is 454 bytes.


Packet 571 closes `UpdatePlayerCollisionRegions @ 0x0041C8E0` at
273/273 bytes. Packet 569 had already recovered the target no-frame/index/slot
lifetime. Replacing the over-modeled three-variable compaction with
`while ((*current = current[1]) != NULL) ++current;` makes VC7.1 emit the
target ECX/EDX/EAX pipeline and closes the final ten ordinary bytes. Live totals
are now 830 exact / 149 non-exact / 137,643 non-exact bytes. The <=256-byte
routing frontier remains 23 functions because this helper is 273 bytes.


Packet 572 closes `CardAttack::OnDraw @ 0x00404410` at 334/334 bytes.
Removing the long-lived alias for the second main VM restores the target owner
lifetime naturally: ESI=CardAttack and EDI=VM0, while VM1 position/color/flags
and its draw argument are addressed directly through the CardAttack owner. All
17 relocations replay exactly. Live totals are now 831 exact / 148 non-exact /
137,309 non-exact bytes. The <=256-byte routing frontier is unchanged because
this helper is 334 bytes.


Packet 573 refines `EffectManager::OnUpdate @ 0x0040CDD0` from 482 bytes
to the exact 475-byte target extent. Initializing the effect-pool cursor before
the early guards restores the target entry-time ESI lifetime and leaves only
four ordinary-byte differences: VC7.1 commutes `primaryCount + secondaryCount`
at the initial and loop-tail dynamic bounds. All eleven relocations solve and
427/431 ordinary comparable bytes match. This remains NON-EXACT, so live totals
stay 831 exact / 148 non-exact / 137,309 non-exact bytes.


Packet 574 closes `CardAttack::OnUpdate @ 0x004041F0` at 536/536 bytes.
Removing the long-lived VM1 alias from the presentation helper restores direct
owner-relative scale writes, while reusing the already-exact
`transitionBits134 : 2` GameManager view reproduces the target transition-bit
RMW. The combined source also restores the target one-byte loop-alignment NOP;
all twenty-two relocations replay exactly. Live totals are now 832 exact / 147
non-exact / 136,773 non-exact bytes.


Packet 575 closes `UpdatePlayerBombState @ 0x0041CA00` at 335/335
bytes. TH09 proves the three SHT mode paths are embedded 0x40-byte buffers at
+0x2C/+0x6C/+0xAC rather than pointer fields. Removing long-lived target/owner
aliases and spelling the 400-point branch as mode-2 fallthrough / mode-1 taken
then reproduces the target lifetime and CFG exactly. All sixteen relocations
replay exactly. Live totals are now 833 exact / 146 non-exact / 136,438
non-exact bytes. The <=256-byte routing frontier is unchanged because this
helper is 335 bytes.


Packet 576 closes `TextHelper::InvertAlpha @ 0x00435FE0` at 472/472
bytes. Moving the method out of the TU that defines trivial `GetBuffer`
restores the target call-clobber lifetime, and spelling the equivalent channel
comparison as `red >= blue` restores the target fallthrough/taken branch
order. The sole GetBuffer relocation resolves exactly. Live totals are now
834 exact / 145 non-exact / 135,966 non-exact bytes. The <=256-byte routing
frontier is unchanged because this helper is 472 bytes.


Packet 577 closes `PlayerLifecycleView::EnterDeathState @ 0x0041E420`
at 753/753 bytes. TH09 retains a second explicit >=200 guard inside the
shotType-8 resource branch and returns on its false edge, producing the target
near jump to the epilogue. The 400-point mode-1 path also writes 100.0f rather
than the previous 400.0f reconstruction. All thirty-eight relocations replay
exactly. Live totals are now 835 exact / 144 non-exact / 135,213 non-exact
bytes.

Packet 531 closes `Background::Background @ 0x00403A40` at 160/160 bytes in
two cold pinned-VC7.1 `/O2 /Ob0` replays, with thirteen relocation destinations
resolved. The target's 32-element special-effect point array uses the generic
vector-constructor helper and a callback to shared/folded physical body
`0x004343D0`; its unique owner remains unknown. A dedicated 0x0C point view
keeps this target-observed construction separate from POD camera/stage points.
The exact `Background::RenderObjects` unit remains 3274/3274, and `Create`
remains exact independently under its `/Ob1` profile. The short frontier is
now 40 functions; this function-level `/Ob0` result does not establish the
whole Background translation-unit or Windows product profile.

Packet 532 closes `EffectManager::InitializeEffect @ 0x0040CA60` at 407/407
bytes in two independent cold pinned-VC7.1 `/O2 /Ob1` build/compare replays,
with all 17 relocations resolved. Target
control flow rereads the template VM count for allocation and loop bounds and
calls `SetAndExecuteScriptIdx @ 0x00403E00` through the manager's `+0x2D4`
ANM object; the maintained source now expresses those target observations.
This moves one function from non-exact to canonical exact, while the <=256-byte
short-function routing frontier remains 40 because this body is 407 bytes.
`OnUpdate`, `AddedCallback`, and faithful Windows product closure remain open.

Packet 533 records target-backed corrections to the still non-exact
`EffectManager::OnUpdate`: TH09 directly calls `Supervisor::SelectSide @
0x00401440` and rereads both pool counts at the outer-loop bound. The previous
source used a nonexistent `ApplyCameraMode` proxy and cached the count. The
natural `/O2 /Ob1` candidate is now 482/475; no exactness credit is claimed.

Packet 539 removes another misleading proxy layer: the TitleScreen replay menu,
replay-save flow, and music-room loader now call the real `FileSystem::OpenFile`
and `CheckIfFileAlreadyExists` namespace symbols from `FileSystem.hpp`, matching
their target callees at `0x0042C970` and `0x0042C480`. Focused exact controls still
pass (`OnUpdateResultNameEntry` 966/966; `DrawMusicRoom` 209/209). The replay and
music-room large owners remain non-exact; at the Packet 539 checkpoint this
did not change the then-current 807/172 exact/non-exact totals. Those historical
totals are superseded by the live status table above; the Windows product gate
remains open.

Packet 540 removes the legacy TitleScreen `g_OptionPointers` and
`TitleNetworkStateView` proxies for the network object. `TitleScreen.cpp` now
uses the declared `SupervisorNetworkState` global and its proven `active +0xA8`
field. Focused exact replays retain `OnUpdateModeSelect` (506/506), the three
character-cursor helpers (89/89 each), and `ActualAddedCallback` (335/335) with
relocations bound to the true global symbol. `OnUpdateDifficultySelect`
remains non-exact at its prior 884/886 candidate; no exactness credit or count
change is claimed.

The latest registration batch makes `Background::Create @ 0x00403AE0`
canonical exact at 307/307 bytes and 29 solved relocations. TH09 target has
no null guard after `AddToRegistry`; removing the source-only guard recovers
the direct chain-registration path. The final CI gate replays the accepted
`Background.cpp` units after the constructor profile is added.

The preceding bounded transform batch makes `UpdateBulletBoundaryBounce @
0x004138C0` canonical exact at 295/295 bytes. A reconstruction-only sprite
lookup call was absent from the target; direct access to the Bullet VM's
loaded-sprite field reproduces the complete body and all thirteen relocations.
All fourteen accepted `BulletManager.cpp` units replay in two cold rounds;
Packet 518 records the proof. The short frontier was 44 units at that
checkpoint; Packets 528-531 have since reduced the current filter to 40.

The preceding short-function batch makes `EffectManager::ResetPool @ 0x0040C990`
canonical exact at 194/194 bytes. Its former `int` return was unsupported by
the two target callers, and the natural void source reproduces all nine
relocations; Packet 517 in the knowledge base records the proof. All thirteen
accepted `EffectManager.cpp` units replay in two cold rounds. At that earlier
checkpoint, `Background::Background` remained non-exact despite a 160-byte
`/O1` candidate; Packet 531 supersedes that function status with an exact
function-focused `/O2 /Ob0` match. The short-function frontier then stood at
44 source-present non-exact units at at most 256 bytes.

The preceding short-function batch makes `TitleCharacterConfigView::GetOptionState
@ 0x004234A7` canonical exact at 54/54 bytes. Packet 516 also corrects the
target-disproved fixed-input read in `MoveCharacterCursorHorizontal`: both
horizontal cursor helpers are now target-sized at 167 bytes but remain
non-exact. All 24 configured `TitleScreen.cpp` units, across two profiles,
replay exact in two cold rounds.

The preceding small-function batch makes the four remaining short Bullet transform
updates canonical exact: `UpdateBulletVectorAcceleration @ 0x004134D0`
(179 bytes), relative direction change at `0x00413630` (202), absolute
direction change at `0x00413700` (199), and aimed direction change at
`0x004137D0` (236). Packet 514 records the shared math-call correction and
target-backed interval snapshot/owner accesses. All 13 configured
`src/BulletManager.cpp` units replay exact in two cold rounds. Other Bullet
functions remain independent non-exact candidates.

The preceding small-function batch makes `ScreenEffect::CalcFadeHold @ 0x004224F0`
(186 bytes), `UpdateBulletDeceleration @ 0x00413460` (110), and
`UpdateBulletPolarAcceleration @ 0x00413590` (158) canonical exact. Packet 513
records a real FadeHold release-decay correction (`255/8`, not the stale
`128/8`), the target's folded deceleration coefficient, and target-backed
Bullet-relative delta accesses. All 15 ScreenEffect and nine BulletManager
configured units replay exact in two cold same-TU rounds. Other members of
either family remain independent non-exact candidates.

Packet 512's preceding short batch closed `ScreenEffect::CalcFadeIn`,
`CalcFadeOut`, and `CalcArcadePulse` (389 authored bytes) and corrected the old
fade/x87 residual diagnosis. Five earlier ScreenEffect-related non-exact
functions remain outside `config/matches.csv`.

`Ending::RunEndingScript @ 0x0040E8A0` is the preceding large exact unit: 1,499
logical bytes, 1,727 code-plus-compiler-table owned bytes, and 88 target-solved
relocations. The earlier 1,695-versus-1,499 size claim compared an entire COFF
section with only target logical code and is withdrawn. The independent
`Ending::Ending @ 0x0040EF60` starts after one unowned `CC` byte at
`0x0040EF5F`. Packet 511 records the source and boundary evidence. The original
translation-unit partition remains unknown; this is a function-level exactness
claim, not Windows i386 product closure.

## Short-function routing frontier

The bounded snapshot in `docs/SMALL_FUNCTION_FRONTIER.md` lists all 23
current source-present non-exact authored functions whose target logical bodies
are at most 256 bytes; 7 are at most 128 bytes. This is a size filter, not a
verified leaf set or difficulty ranking. The live `config/functions.csv`
row and a fresh compiler/target check always override the snapshot.

Current routing facts:

- Former short ECL interpolation helpers are closed: `ApplyInterpolationOperation`,
  `InstallInterpolationSlot`, `CallSubroutine`, and
  `InterpolateHermite` are canonical exact. Packet 555 supersedes the old
  Packet-527 182/188 InterpolateHermite plateau.
- In the short Supervisor frame-queue family, `PopFrame` and
  `InsertPredictedFrame` are exact; only `InsertReceivedFrame` remains
  non-exact at 213 target bytes.
- The five TitleScreen cursor helpers and the two Bullet pattern wrappers remain
  bounded non-exact families. Their current candidate sizes and rejected natural
  variants are recorded in their live ledger rows and
  `docs/SMALL_FUNCTION_FRONTIER.md`; do not revive register forcing,
  var_order, padding, or loop-layout steering.
- `FileSystem::TryDecryptFromTable` is now 220/220 target-sized after
  restoring the original compare-before-bound scan order proven independently
  by TH09 bytes and the committed TH08 source family. The index-8 probe reads
  adjacent `g_CryptSignature[0]` and is then rejected by `index < 8`; this is
  retained as target behavior. Exactness is still open because ordinary scan
  codegen differs.
- Several target-sized short candidates remain genuine codegen plateaus:
  `EffectManager::AddedCallback`, `EnemyView::IntegrateMotion`, and
  `ParseNetworkConfigValue`. `FrontMessageOwnerView::InitializeMessageRuntime`
  is no longer in this set: Packet 591 closes it by restoring direct per-branch
  `Setup` calls whose common call tail VC7.1 merges naturally.

Packet 591 promotes a <=256-byte function again: `FrontMessageOwnerView::InitializeMessageRuntime @ 0x004181E0` is now exact. Use the live small-function frontier rather than older routing counts.

## Active large frontier: EclManager::RunEcl

RunEcl at 0x004086C0 remains the main actively investigated large owner.
Target logical size is 14,792 bytes, followed by 772 bytes of compiler-owned
tables: 6 easing entries and 187 opcode entries.

A fresh pinned VC7.1 build at this checkpoint reports:

| RunEcl measure | Target | Current candidate |
| --- | ---: | ---: |
| Logical bytes | 14,792 | 14,892 |
| Candidate-target delta | 0 | +100 |
| Stack frame | 0x168 | 0x154 |
| Immediate direct calls | 375 | 375 |
| Indirect calls | 4 | 4 |
| Relocations | target-owned | 598 |
| Integer resolver calls | 131 | 131 |
| Float resolver calls | 100 | 100 |
| Integer-lvalue resolver calls | 17 | 17 |
| Float-lvalue resolver calls | 24 | 24 |
| Compiler table entries | 193 | 193 |

This current-HEAD baseline was freshly rebuilt after the three Sep 24 ECL
helper-closure commits moved exact helpers into the `EclManager.cpp` TU. The
ignored `build/matching/EclManager.obj` found on entry predated those commits
(mtime Sep 23) and its 14,872-byte report is stale. Reproduce the current
candidate with `scripts/compile-probe.sh src/EclManager.cpp
build/matching/EclManager.obj /MT /EHsc /Gs /DNDEBUG /Zi /Gy /GF /Oi /Gr
/O2 /Ob1 /Oy- /I src`, then run `python3 scripts/report-ecl-codegen.py
build/matching/EclManager.obj --json`. The generated object is disposable and
was removed after this checkpoint.

Important current facts:

- Call structure is closed. Candidate and target both have 379 calls: 375
  immediate direct plus four indirect. Do not resurrect the older 376+3 count.
- Direct EnemyView::ResolveFloat is target-correct. The older wrapper-based
  14,696-byte near plateau was false because it created target-absent temporary
  homes and a 0x2C0 frame.
- Recent target-backed corrections include child-context scan lifetime, the
  fixed 0x20 laser-slot clear loop, coalesced interaction flags, repeated
  laser-slot relookup, child-ECL slot relookup, polar operand reevaluation,
  ordinary timer assignment, direct destination staging removal, late laser
  angle relookup and repeated side-owner lookup.
- Th09EclRunMovement::SetExtraAnmScript @ 0x00407B20 is now canonical
  exact at 261/261 bytes. Moving it from the artificial external helper TU
  into the RunEcl TU as a natural static function reproduces the target's
  private EDI/ESI entry transport and both opcode 57/61 callsites without
  register forcing.
- Th09EclRunMovement::SetPrimaryAnmScripts @ 0x00406740 is also
  canonical exact at 80/80 bytes after moving it into the RunEcl TU as a
  natural static helper. VC7.1 eliminates the unused instruction transport
  and selects the target-private EAX Enemy receiver.
- Th09EclRunMovement::ConfigurePolarMotion @ 0x004070A0 is now
  canonical exact at 346/346 bytes. Its sole RunEcl caller establishes
  compiler-private EDI=Enemy / ESI=instruction transport. The target also
  reevaluates speed and duration for the x and y products and reads duration
  again for the timer write; natural cos/sin * speed * duration source gives
  VC7.1 the target evaluation order.
- Th09EclRunMovement::ConfigureRelativeMotion @ 0x00407320 has been
  corrected from stale source assumptions and moved into the RunEcl TU. TH09
  fixes target - worldPosition, origin from position, and zeroing velocity
  +0x2D8C. The natural same-TU candidate is 295/296 bytes with the target
  0x18 frame; the remaining byte is a final flag-register scheduling plateau.
  TH08/TH095 bitfield source shapes shrink TH09 to 289 and are rejected.
- StartTimedPolarDisplacement @ 0x00407200 and
  Th09EclRunMovement::BeginBoundaryAwareMove @ 0x00407C30 are now canonical
  exact at 288/288 and 510/510 bytes. StartTimed requires repeated
  speed/duration operand evaluation; BeginBoundary requires the target
  Float3 pointer-identity call @ 0x004343D0 before reusing one pointer for all
  four boundary tests. Their private same-TU transports arise naturally.
- Th09EclRunLate::MoveRandomBiased @ 0x00407E30 is now reconstructed to a
  natural 517/516-byte near match, up from 411 bytes. TH09 target evidence
  requires repeated manager/side/player X lookups, branch-specific angle
  normalization and the Float3 pointer-identity call before the Y-bound tests.
  The remaining one-byte size residual is localized to Y-pointer lowering:
  target uses lea ecx,[eax+4] then two [ecx] comparisons; the best natural
  source keeps the returned base pointer and emits two [ecx+4] comparisons.
  Natural alias variants expand to 518 via mov/add, so the helper remains
  honestly NON-EXACT rather than steering an encoding.
- EnemyPostEclUpdateMovement @ 0x00408180 is now canonical exact over its
  complete physical owner: 967 logical bytes, one alignment NOP and the
  six-entry easing table, 992/992 total. The target-proven flags snapshot and
  same-TU visibility of the out-of-line void Float3::FromAngleMagnitude
  definition reproduce the private ESI transport naturally. The helper itself
  is now canonical exact; moving its definition into the shared header is
  target-disproved because exact type-8 initialization grows by one byte.
- EnemyPostEclUpdateShotAndAnm @ 0x00408560 is canonical exact at 349/349.
  RunEcl naturally supplies Enemy in EDI; its nested shot call uses the
  TU-private EBX=Enemy / ESI=instruction transport. Starting direction=0
  before the mirror test closes the remaining scheduling bytes.
- Th09EclRunBullet::DispatchShotInstruction @ 0x00408040 is now canonical
  exact at 315/315. Same-TU private visibility supplies EBX=Enemy /
  ESI=instruction naturally; target-backed int lifetimes for bulletType and
  color hoist each short sign-extension before its parameter-mask branch,
  closing the former 321-byte plateau without register or encoding steering.
- Earlier TU/owner fixes moved RunEcl from the superseded 14,244-byte
  candidate to a size-near 14,820/14,792 (+28) checkpoint. That checkpoint is
  no longer the structural baseline: its opcode switch was physically about
  0x140 bytes too late even though the aggregate size looked attractive.
- Fresh SET_BOSS review removes a stale cached stateIndex shape. TH09
  reevaluates the ECL integer twice on the positive path, reloads bossSlot336B
  at each UI/slot use, and lays out state >= 0 as fall-through with teardown on
  the taken negative branch. VC7.1 then naturally keeps zero in ESI and reuses
  the Float3 constructor return for the hidden (-999,-999,0) marker. This cuts
  another 12 logical bytes without changing any accepted EclManager exact unit.
- Main-loop CFG is now reconstructed around an inner dispatch loop instead
  of two long gotos. TH09 requires a stable pointer to secondaryTime094,
  secondary/time mismatch to break into the frame tail, activeContext loading
  only after life>0, and the first interpolation-slot address to stay live
  across the per-frame callback. With those natural lifetimes the first opcode
  entry is exactly relative +0x1F2, versus about +0x332 in the old committed
  shape. The current whole owner is 14,892/14,792 (+100) with frame 0x154;
  the 14,872 report was from the stale pre-current-HEAD object above.
  This larger aggregate is accepted as a more truthful reconstruction because
  block placement and the hot CFG are materially closer to TH09.
- The next RunEcl work is local layout, not blind size trimming. SET_FLOAT is
  already semantically shaped correctly but target places its float temporary
  at [ebp-0xA4] while the current candidate uses a disp8-reachable slot; three
  such accesses account for nine bytes of downstream case drift. A natural
  case-scoped `float result = ReadFloat(...); WriteFloat(...) = result` probe
  has zero codegen effect across two cold builds; skip that spelling. Direct
  activeContext rewrites for SET_SECONDARY_TIME were also tested and rejected
  because VC7.1 tail-merged the two Timer::operator= calls, dropping the
  target-correct direct-call multiplicity from 375 to 374.
- The latest ANM ownership review proves 0x00439CF0 and 0x00439DC0 are
  AnmManager member helpers. RunEcl opcode 157 uses trail render vertices at
  Enemy +0x3E68, not the trail sample buffer at +0x33E8, and prepares
  g_AnmManager in ECX at the callsite.
- Aggregate function size is diagnostic only. Prefer a locally target-correct
  handler even when it makes the whole function temporarily shorter. Do not
  reintroduce disproven staging temporaries just to approach 14,792.
- No register forcing, var_order, volatile steering, padding, assembly or
  profile roulette is allowed.

Use docs/KNOWLEDGE_BASE.md Packets 466 through 490 only as chronological
investigation history. The current functions.csv row plus a fresh
report-ecl-codegen.py run are the live baseline.

## Secondary large frontier: EnemyManagerView::OnUpdate

`EnemyManagerView::OnUpdate @ 0x00410730` is an active large-function frontier,
not a frozen size plateau. Fresh TH09 call-graph review invalidated several
stale maintained-source owners rather than merely changing compiler spelling:

- Target logical size is 3,883 bytes with frame `0x2A8` and 69 direct call
  sites. The old natural candidate was 3,156 bytes, frame `0x25C`, with only
  59 direct calls.
- The maintained source now uses the target-proven `Float3` by-value
  constructor/operators, the full 0x214 bullet spawn descriptor and side
  EtamaController path, four static playfield-test sites, three explicit death
  effect spawns, and `AnmProjectionAbs`.
- Target trail-history shifting is member-wise rather than one aggregate
  assignment. Spelling the position/velocity/angle copies separately makes
  VC7.1 naturally emit the target-shaped four-sample unrolled loop plus scalar
  remainder instead of `rep movsd`.
- Target-local owner review then corrected five stale free-call aliases:
  `Supervisor::SelectSide`, `EnemyEclManagerView::GetSubroutine`, all four
  `GameManagerPlayfieldView::IsWithinPlayfield` calls, both
  `EnemyAppendCollisionView::AppendPlayerCollisionBox` calls, and
  `SoundPlayer::PlaySoundPositionedByIdx`. The private ANM VM layout is also
  corrected to color1/color2/flags at `+0x1F0/+0x1F4/+0x1F8`.
- Those locally correct ABIs make the current pinned VC7.1 `/O2 /Ob1` build
  3,824/3,883 bytes with the target-exact frame `0x2A8` and 67/69
  direct calls after also restoring canonical RunEcl / EnemyView member
  receivers. The remaining logical gap is 59 bytes. Do not restore
  the disproven free aliases just to regain aggregate bytes. One target
  normal-path `Float3::operator+` call and one target-distinct
  `CleanupAfterDeactivation` site are currently compiler-merged.
- Target code repeatedly reloads `manager->sideState320` and its player/effect
  members at distant blocks. Keeping `side` and `player` as function-wide
  locals was the main remaining frame/lifetime error; removing them closes
  136 bytes and exactly closes the eight-byte frame gap.
- RunEcl now uses the current EnemyManager / offset-zero primary ECL receiver;
  Clamp/Integrate/Reset/Cleanup use their canonical EnemyView member owners.
  This locally correct owner cleanup shortens the aggregate by four bytes and
  is retained rather than reverting target-disproved aliases.
- `EnemyCoreUpdateAttachedEffects @ 0x0040F4C0`, from the same source file,
  still replays exact at 151/151 in two cold passes after these changes.
- The remaining 59-byte frontier now has bounded negative coverage: labels,
  branch aliases, explicit trail cursors, function-scope integer declarations,
  manager-prefix views and natural damage-pointer spellings do not recover the
  two merged target callsites. A named branch-local movement Float3 gives a
  tempting 3,864-byte candidate but has the wrong post-call copy shape and is
  rejected. Do not resume declaration/register roulette without new target
  evidence.

- Continue this owner by target-backed CFG/type/lifetime recovery. Do not add
  filler, force registers, use volatile steering/assembly, or chase alternate
  profiles for aggregate size.

The durable evidence is in Packets 491 through 498.

## Secondary large frontier: PlayerUpdateSelectorState

`PlayerUpdateSelectorState @ 0x004049A0` is an active Player large-owner frontier.
Fresh TH09 target/cold-compiler review now gives:

- target 3,238 bytes, frame `0x5C`, 33 physical direct callsites;
- current pinned VC7.1 `/O2 /Ob1` candidate 3,237/3,238 bytes, target-exact frame `0x5C`;
- target-backed timer fixes restore three folded current reads, three postfix
  increments, `HasTickedEvery`, and timer `operator==`;
- target-left and target-right pattern branches now keep their distinct
  `ResolvePatternOffset` callsites, closing the physical call count at 33/33;
- the entry construction still differs: target uses
  `??_H @ 0x00401470` over three 0x0C elements while the candidate emits an
  explicit constructor loop;
- same-TU `ResolvePatternOffset @ 0x00404710` still cold-replays exact at
  346/346 bytes and 412/412 owned compare extent.

The selector has target-exact frame `0x5C` and full 33/33 call surface.
Failure-tail Player caching/reload and target-backed 16-bit protocol storage at
`+0x32/+0x34` close the natural body to 3,237/3,238 bytes. This remains
NON-EXACT: target entry construction is `??_H` while the candidate has an
explicit ctor loop, and local branch layout still differs. EH-disabled lowering
does not change this. Do not chase the final aggregate byte by jump/register/
padding/profile tricks; resume only with new TH09-local source evidence.

The durable evidence is in Packets 499 through 505.

## Secondary large frontier: EtamaController::OnUpdate

`EtamaController::OnUpdate @ 0x004146F0` remains an active BulletManager
large-owner frontier. Fresh target-backed corrections give:

- target 2,195 bytes, frame `0x40`, 62 immediate direct calls;
- current pinned VC7.1 `/O2 /Ob1` candidate 2,188 bytes, frame `0x50`,
  with 62 external DISP32 relocations;
- three target-absent sprite helper calls are gone; VM loadedSprite storage is
  read directly;
- bullet cancel records now use canonical `AppendBoxRecord`;
- laser timing now uses the target GetCurrent / integer comparison operators,
  reducing `__ftol2` from eight calls to the target-observed two;
- the target-proven shared spawn completion tail, cached WAIT-flags flow and
  side-freeze queue path are now restored; the latter fixes a real source bug
  where frozen bullets incorrectly ticked two timers;
- the target-disproved 0.7 laser-size branch is removed, and FIRED-before-
  DESPAWNING switch ordering now puts the cold despawn block after laser
  AddNormalizeAngle/SetZRotation as in the target;
- laser state 0/2 now share a natural currentWidth local: state 0 publishes
  it to the Laser member, while state 2 keeps the shrinking value local as the
  target does;
- raw-section Capstone confirms 62 actual candidate calls, resolving the
  previous COFF objdump relocation-boundary ambiguity;
- all seven canonical units from the same TU remain exact in two cold passes.

The honest current frame is still 0x10 too large. Bounded natural lifetime
probes reproduce the target laser scalar homes but cannot make VC7.1 reuse the
four early Float3 return buffers naturally; declaration/type/named-temp variants
are exhausted. Do not force registers, use var_order, or chase aggregate size
alone.

The durable evidence is in Packets 506 through 509.

## Secondary large frontier: EtamaController::SpawnSingleBullet

`EtamaController::SpawnSingleBullet @ 0x00412960` has moved from a large
structural gap to a bounded five-byte frontier:

- target 1,893 bytes, frame `0x14`, 22 direct calls;
- current pinned VC7.1 `/O2 /Ob1` candidate 1,888 bytes, frame `0x14`;
- separate primary/secondary searches naturally reproduce the target 5-way /
  6-way allocator unrolling;
- sentinel resets use embedded pool bases, not cached cursor fields;
- the folded position conversion call and inline despawn SetSprite path are
  restored; loaded-sprite geometry is fixed to target field `+0x30`;
- call offsets are aligned through the second direct SetSprite and the entire
  transform-copy / AdvanceTransformProgram / return tail is structurally closed;
- the remaining five-byte displacement is isolated to VC7.1 transformFlags CSE:
  target memory-tests FAST then reloads the dword for NORMAL/SLOW, while the
  natural candidate keeps one dword value live. Clean TH08 corroborates the
  ordinary 32-bit / if-else-if source family, so do not fabricate a byte alias.

All seven canonical units from the same BulletManager TU remain exact in two
cold passes. Resume only with new TH09-local evidence that explains the flags
lifetime naturally.

The durable evidence is in Packet 510.

## Secondary large frontier: SupervisorServiceUpdate

`SupervisorServiceUpdate @ 0x00430AA0` is a non-boss 1,633-byte netplay owner.
Fresh target receiver review establishes that the callback's `Supervisor`
owns the two frame queues while `g_SupervisorNetworkState` owns DirectPlay
session policy. Maintained source now routes all four queue helpers through the
callback receiver. The current natural candidate is 1,627 bytes, with frame
`0x0C` versus target `0x14`; two cold builds replay the same disassembly. A
`u16 inputs[2]` local models the two popped per-side inputs and improves the
previous 1,625-byte candidate by two bytes, but the optimizer still keeps those
values in registers instead of reproducing the target's adjacent stack-word
stores/reloads. Keep this function NON-EXACT; no match unit or exactness credit
is added. Resume only with a new semantic/lifetime fact, not artificial
storage, register forcing, or padding. Packet 534 records the evidence.

## Other durable non-exact plateaus

Do not churn these without new evidence:

- ReplayManagerView::CaptureFrameSyncState 0x00420190: exact-sized 257 bytes,
  one commutative SIB byte remains.
- AsciiManager::OnUpdate 0x00435B00: exact-sized 253 bytes, one signedness
  branch byte remains; explicit signed source is worse.
- PlayerLifecycleView::CheckBulletCollision 0x0041DFF0: best natural 849
  versus target 862; remaining frontier is preserved-register allocation.
- Float3::FromAngleMagnitude 0x00441890 is no longer a plateau. Fresh TH09
  caller evidence fixes the void contract; committed TH08/TH095 corroborate the
  narrow x87 FSINCOS source family, and the EclManager-TU out-of-line definition
  now replays 32/32 with all 30 accepted callers preserved.
- TitleScreenView::OnUpdateKeyConfig 0x00427EE8: exact-sized 2803/2803 with
  eight ordinary comparable bytes of scheduler/SIB residual.
- FrontSide::OnDraw 0x004193E0: 3106/3094; remaining 12 bytes are transition
  overlay constructor/branch scheduling after bounded probes.
- FrontSide::OnUpdate 0x00418A90: 2458/2381, target frame 0x04 versus
  candidate 0x08. Target reloads side-state/player pointers by phase, but a
  natural scope-local pointer variant was two-cold-build stable at 2505 bytes
  and still had the 0x08 frame; it was reverted. Packet 535 records the exact
  bounded negative result. Do not repeat the same scope split without new
  TH09 mutation/lifetime evidence.
- TitleScreenView::UpdateScreen8Mode0 0x004289DB: target 2241, current natural
  `/O1 /Ob1` candidate 2232 with 54 direct calls on each side. A target-shaped
  per-visible-VM cursor accounts for a stable 12-byte improvement over the
  prior source; target frame allocation is 0x08 versus candidate 0x0C. Keep it
  NON-EXACT; Packet 536 records the comparison.
- TitleScreenView::UpdateScreen8Mode123 0x00425ACB: target 2153, current natural
  `/O1 /Ob1` candidate 2102. The target's per-unlocked-row `0x2A4` VM cursor
  with separate visible-row count is now reflected in source, but this made
  the aggregate size gap larger (39 to 51 bytes). Two cold builds agree at
  function-byte SHA-256 `223cf3ae76eac39efd2642defe6b2ee4531e749225eab248abdceb2552a641b1`;
  keep NON-EXACT and avoid further size-only cursor aliases. Packet 537 records
  the bounded result.
- PlayerLifecycleView::UpdateMovementAndOptions 0x0041C170: 1864/1835 with
  complete external call surface and bounded lifetime/type probes.
- EnemyManagerView::SpawnEnemy 0x0040F340: cold-stable 361/373; merge and
  duplicated-failure-tail alternatives are both worse.

These are not forbidden forever; they require evidence that invalidates a
previous negative result.

## Non-exact family snapshot

Current source-present/non-exact bytes are concentrated in these modules. This
is a snapshot, not a permanent ranking; recompute before choosing work.

| Module | Functions | Non-exact bytes |
| --- | ---: | ---: |
| TitleScreen | 26 | 27,050 |
| EclManager | 13 | 18,153 |
| Player | 26 | 17,687 |
| ExAttackController | 21 | 14,283 |
| EnemyManager | 11 | 9,902 |
| Front | 10 | 8,138 |
| BulletManager | 12 | 6,816 |
| FrontSide | 3 | 5,935 |
| Background | 8 | 5,306 |
| GameManager | 5 | 4,259 |
| ReplayManager | 5 | 3,949 |
| AnmManager | 7 | 3,375 |

When RunEcl reaches an honest local plateau, prefer target-proven helper,
ownership, ABI and layout leverage in these families before broad profile or
register experiments.

## Product gate

config/build.toml deliberately remains an open native-product graph. Exact
functions do not establish all translation units, data owners, libraries,
resources, static initializers, linker order or exercised Windows i386 runtime
paths.

Do not begin semantic reconstruction or portability until exact/source work has
reached an honest plateau and the native product gate has the required
translation-unit and runtime evidence.

## Workspace hygiene

.analysis/ is disposable working state, not a journal. At this checkpoint
`build/`, `.analysis/`, `.tools/`, and Python/cache directories are
absent; rebuildable probe objects, PDBs, copied probe sources, disassembly dumps,
and cache state were cleared after their durable facts were moved into tracked
source, ledgers, and documentation. The hash-attested canonical target
`resources/th09.exe` is intentionally retained because validation depends
on it. No current claim depends on the cleared workspace artifacts.

Use live source, ledgers, this handoff and the knowledge base to resume work.

After each checkpoint:

1. Move durable facts into tracked source/config/docs.
2. Keep only compact unresolved evidence when it is the sole record.
3. Delete rebuildable OBJ/PDB/probe/source copies, duplicate disassembly dumps,
   stale inventories and superseded comparison logs.
4. Never use .analysis/ as long-term memory.

If recovering a dirty session, explain an artifact before deleting it. For a
clean checkpoint with no tracked references to its receipts, prefer deletion.

## Durable evidence map

- config/functions.csv and config/function-origins.csv: candidate and origin
  ledger.
- config/implemented.csv: source-presence mapping.
- config/matches.csv and config/match-units.toml: canonical exact units.
- docs/KNOWLEDGE_BASE.md: durable TH09 facts and negative results.
- docs/SMALL_FUNCTION_FRONTIER.md: bounded short-function routing snapshot;
  re-filter the live ledger before selecting a new unit.
- docs/PROGRESS.md: generated totals.
- docs/RE_WORKFLOW.md and docs/ORACLES.md: phase and acceptance rules.
- scripts/report-reconstruction-status.py: authoritative live counts.
- scripts/report-ecl-codegen.py: RunEcl compiler-structure diagnostic.

The handoff is intentionally not a second knowledge base. Historical details
belong in Git and the knowledge base. Use `git log -12 --oneline --decorate`
for recent checkpoints.
