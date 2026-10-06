# TH09 reconstruction handoff

This file is a **current-state restart note**, not an investigation journal.
Historical experiments, rejected hypotheses, packet-local byte counts, and old
candidate sizes belong in Git history and the historical sections of
`docs/KNOWLEDGE_BASE.md`.

If this file conflicts with `config/functions.csv`, `config/matches.csv`,
`config/match-units.toml`, or a fresh repository diagnostic, the ledgers and
fresh diagnostic win. Rerun the status commands before starting target-dependent
work; none of the numeric snapshots below are timeless facts.

## Active reconstruction goal

The operator-set milestone is **at least 95% of reviewed authored bytes exact**
(updated 2026-10-05). With the current 275,770-byte denominator, this requires
261,982 exact bytes; the current 216,128-byte ledger leaves 45,854 bytes.
All function sizes are eligible; prioritize medium and large owners, with
smaller closures or unblockers welcome (operator clarification 2026-10-05).
Recompute this gap after ledger changes. This milestone does not close the
whole-product, runtime, semantic or portability gates.

## Current ledger snapshot

Fresh `python3 scripts/report-reconstruction-status.py` at this checkpoint:

| Measure | Count |
| --- | ---: |
| Function candidates | 2,192 |
| Boundary/origin unreviewed | 0 |
| Reviewed but origin-unresolved | 35 |
| Confirmed authored | 980 |
| Classified exclusions | 1,177 |
| Source-present authored mappings | 980 |
| Canonical exact functions | 925 |
| Source-present non-exact functions | 55 |
| Source-present non-exact bytes | 59,642 |
| Authored without maintained source | 0 |
| Canonical exact authored bytes | 216,128 |

All 980 confirmed authored functions have maintained source. Faithful whole
Windows i386 product closure remains open; semantic reconstruction and
portability have not started. `docs/PROGRESS.md` is generated from the ledgers
and is the preferred compact status snapshot.

## Closed items that must not stay on the roadmap

The following formerly-large frontiers are already canonical exact and should
not be reopened merely because an older packet or roadmap says they are pending:

- `EtamaController::OnUpdate @ 0x004146F0` — 2,195 authored bytes.
- `TitleScreenView::TitleSetupThread @ 0x004249E1` — 534 bytes.
- `TitleScreenView::OnUpdateMusicRoom @ 0x00426E05` — 2,258 bytes.
- `EnemyManagerView::AddedCallback @ 0x00411DB0` — 372 bytes.
- `ExAttackUpdateCallbackType20 @ 0x0044ADE0` — 405 bytes.
- `TextHelper::RenderTextToTextureBold @ 0x004364C0` — 751 bytes.
- `TextHelper::RenderTextToTexture @ 0x004367B0` — 743 bytes.
- `ExAttackUpdateCallbackType8_9 @ 0x00446920` — 748 bytes.
- `ExAttackUpdateCallbackType11 @ 0x00446EE0` — 713 bytes.
- `ExAttackUpdateCallbackType12 @ 0x004471B0` — 710 bytes.
- `ExAttackUpdateCallbackType6 @ 0x00446060` — 698 bytes.
- `ExAttackUpdateCallbackType7 @ 0x004464A0` — 799 bytes.
- `ExAttackUpdateCallbackType14_22 @ 0x00443DF0` — 805 bytes.
- `AnmManager::Draw3D @ 0x0043B1A0` — 811 bytes.
- `Controller::GetInput @ 0x0042BE40` — 819 bytes.
- `PlayerDamagePlayerView::CalcDamageToEnemy @ 0x0041FCD0` — 996 bytes.

Their current exactness comes from `config/matches.csv` plus their configured
match units, not from historical prose.

## Latest exact Player lifecycle closure

AddedCallback at 0x0041EE50 now fully reproduces its 438-byte owner and all
20 independently bound fields with unchanged current PlayerRuntime source.
The older 302/358 ordinary-byte residual was stale after the existing exact
SHT-loader float-fastcall/source-lifetime correction. Two cold builds agree;
four call identities/ABIs, all five internal branches, cache/reload/failure
paths, data tables and callback roles are verified. InitializeAddedState remains
independently nonexact. Use player-added-callback; the October 6
player-collision-fresh packet has complete source/profile/field proof.
GameConfiguration.hpp is not in this TU's actual dependency closure.
Focused cold canonical replay passed and was accepted on 2774afac:
receipt 0eaa09cc9426a01714f87da3f9d77aa62f8ee8ee6fa5ffd787aeeaf06077ab1f.

## Latest exact Player damage closure

CalcDamageToEnemy now reproduces its full 996-byte owner with all 17 independently
bound fields. Ordinary in-class BottomY context recovers the final target x87
operand order without a helper call or spill. The real conversion-result pointer
is consumed as its existing Float3 object to write z, preserving the genuine call.
The same fresh object keeps PlayerBuildAabb exact at 55 bytes/four fields.
Use player-calc-damage-to-enemy; old two-byte residual claims are superseded.
The uncalled emitted accessor copy receives no independent target ownership.
Accepted Factory replay is required for this committed checkpoint; native product
and runtime gates remain open.

## Latest exact input closure

Controller::GetInput now recovers the actual Z/X/Shift masks in both keyboard
paths: shoot 0x1, bomb 0x2, focus 0x4. Ordinary OR-composed masks and paired
equivalent keys close all 819 bytes with nine bound fields. Use the focused
controller-get-input match unit; the older Packet 439 keyboard interpretation
and allocator-only conclusion are superseded. Native product/runtime gates
remain open.

## Priority frontier 1: EclManager::RunEcl

`EclManager::RunEcl @ 0x004086C0` remains source-present and **NON-EXACT**.

The current ECL carrier, rebuilt for the October 6 configuration-array integration
and proved byte/field-neutral across all 67 emitted owners, has RunEcl raw function
SHA-256
`93206b3996d97e62d6470b795e32c9cfdc9228a9bc8806efed073688b7420eef`.
`scripts/report-ecl-codegen.py` reports:

- 14,792 candidate logical code bytes, matching the target logical extent;
- 15,564 candidate physical code/table bytes, matching the target physical extent;
- stack frame `0x168`;
- 375 direct calls;
- 598 relocations;
- 193 compiler-table entries (6 easing + 187 opcode entries);
- final status: **NON-EXACT**.

Previously accepted unchanged siblings do not need another replay.
Structural size, frame, table, call-count, and relocation-count agreement are
diagnostics only and do not grant whole-owner exactness.

Fresh handler diagnostics narrow the current physical frontier further.
`scripts/report-ecl-handler-boundaries.py` compares the verified target opcode
table with the current COFF compiler table and reports 167/187 opcode entry
offsets exact. All 20 mismatching entries are exactly -1. Opcode 155 itself
starts at the correct +0x30B0, while opcode 156 starts at candidate +0x30DC
versus target +0x30DD, isolating one missing byte inside opcode 155.

The identity-audited `scripts/report-ecl-handler-shapes.py` now gives the more
useful instruction frontier. It first requires the complete CFG/relocation
identity audit to pass, then normalizes only independently paired call/data
fields and relative branch displacements. Scalar immediates remain significant,
so values such as the opcode-145 `0x00400000` flag mask cannot be mistaken for
an image pointer. On the current cold object it reports **142/148 unique handler
shapes matching**, with only six remaining shape mismatches:

- opcode 4 `JUMP`: 29/29 bytes, 6/8 instructions matching; argument/receiver
  scheduling differs;
- opcode 7 `SET_FLOAT`: 75/75 bytes, 15/23 instructions matching; the same
  value/lvalue operations use different volatile registers and scheduling;
- opcode 86 `SET_REMOTE_INT`: 86/86 bytes, 28/29 instructions matching; only
  the raw-value load versus remote-slot lookup schedule differs;
- opcode 155 `SET_TIMEOUT_SPELL`: 44 candidate bytes versus 45 target bytes;
- opcode 156 `SET_SPECIAL_INTERACTION`: 52/52 bytes, with register-color drift
  after the opcode-155 one-byte shift;
- opcode 157 `SET_TRAIL`: 160/160 bytes, with only its first byte-load/store
  register pair differing after the same shift.

This supersedes the older ignored `handler-shape.py` eight-handler report. That
older diagnostic produced false positives, including opcode 145, because it
zeroed any target immediate in the executable image range and therefore erased
the real scalar mask `0x00400000`. Do not route work from that result.

For opcode 155 specifically, target uses ECX for the shifted timeout-spell input
and EAX for the merged flag value; the candidate uses EAX/ECX respectively.
Target therefore emits the six-byte generic `AND ECX,0x01000000` while the
candidate emits the five-byte EAX-special encoding. The physical tail is
correspondingly shifted by one byte and the candidate ends with a compensating
alignment NOP. This remains an allocator/TU frontier, not a license to force
registers. Bounded natural probes of the opcode-155 expression (direct
bitfield, helper/mask forms, signed/unsigned locals, pointer/reference state,
function-scope scratch reuse, and the adjacent TH08-style spelling) all
reproduce the same 44-byte candidate handler. Reverting the recent
instruction-time context spelling and opcode-2 source shape also leaves opcode
155 unchanged. Do **not** repeat those controls without new cross-case/TU
evidence.

Useful restart commands:

~~~bash
# First verify the retained object's source/dependency manifest.
# Ordinary build/matching caches may predate the current Player header.
OBJ=build/gpt-dots-ecl-fresh-context2-20261006/array-header.obj
python3 scripts/report-ecl-codegen.py --json "$OBJ"
python3 scripts/report-ecl-handler-boundaries.py "$OBJ"
python3 scripts/audit-ecl-callsite-identities.py "$OBJ"
python3 scripts/report-ecl-handler-shapes.py "$OBJ"
~~~

The callsite and handler-shape reports are structural diagnostics and not
exactness Oracles.

The fresh whole-source/caller review found no new supported build hypothesis.
Real timer assignment/update visibility already has source-backed negative
controls. The October 6 plain Tick/post++/TickTimer visibility control is also
byte/field-neutral across all 65 previously emitted owners; do not repeat it.
The ordinary EnemyStateView timeout member was tested and rejected: it retains
a new non-target call (376 rather than 375). Do not force it inline or repeat it.
A borrowed two-int JUMP prefix is fully neutral across all 67 emitted owners,
28,838 bytes and 1,215 semantic fields; the same six-handler frontier remains.
The flag-subobject member also retains a non-target call: 376 calls, 599 fields
and 15,548 physical bytes; its 26-byte helper is not a binding candidate. The
instruction-owned RawInt/RawFloat/RawByte/RawShort context is fully neutral
across all 67 common owners, 28,838 bytes and 1,215 fields. Four uncalled 14-byte
accessor copies receive no target ownership. Neither context is a new exact
owner; do not repeat these precise arrangements unchanged.
Other untested in-class arrangements remain hypotheses, not evidence of a fix.
The new resolved-instruction member family is rejected because it adds 48
ReadInt and 18 ReadFloat runtime calls; RunEcl becomes 13,560 logical /14,332
physical bytes, 372 calls and 594 fields. A context-owned Jump also retains a
non-target call despite keeping 375 calls/598 fields overall, and emits
14,788 logical /15,560 physical bytes. All 66 other common owners remain
byte/field neutral in both trials. Do not force either new method inline or
repeat these precise source arrangements. Their complete compact proofs are
in the October 6 resolved-context and context-jump packets.
The later October 6 coherent seven-op primary state/late bitfield family is also
fully neutral: all 67 owners, 28,838 physical bytes and 1,215 semantic fields;
all 22 configured exact siblings independently target-match. It is distinct
from earlier single-handler and mask-template controls, and is now a recorded
negative. Its complete field-width, dependency and restoration proof is in the
primary-flag-family packet. Do not repeat this unchanged arrangement.

The isolated complete-type control is also fully neutral: global EnemyView now
experimentally holds the existing State layout and EnemyStateView aliases it,
with the same seven manager access sites and real method contracts. All 67
paired owners preserve 28,838 bytes/1,215 fields; the two renamed View helpers
are unchanged unreferenced identity leaves. All 22 exact siblings match. The
full 943-block identity audit preserves the same six-handler frontier. The
October 6 ecl-unified-owner packet records this distinct whole-object negative;
no canonical shared-layout promotion or repeated subset trial follows.

RunEcl is necessary for the 95% milestone: its 14,792 bytes exceed the current
13,788-byte non-exact allowance. This bounded plateau is not proof of impossibility.
The current 34-input canonical-path manifest is retained in the October 6
ecl-fresh-context2 packet;
the October 5 ECL packet preserves the preceding hypothesis review. The new
October 6 reference-returning WriteInt/WriteFloat wrapper control is also fully
neutral across RunEcl and all exact ECL siblings; do not repeat it.

## Latest large source-model controls

The October 6 opcode-local control-family lifetime model is fully neutral across
all 67 ECL executable sections, 28,838 bytes and 1,215 fields; the same six-handler
frontier remains. DrawReplayMenu's separate/common caption conversion owners
retain new runtime calls and are rejected. Type11 named/const-reference golden
results recover frame 0xDC but emit 775/732 bytes with wrong result-copy routing;
its exact helper and compiler iterator remain unchanged. These isolated controls
are not integrated and add no credit. See the latest Knowledge Base section and
the large-model-independent packet rather than repeating these arrangements.

## Next bounded investigations

Select from a fresh non-exact ledger before using historical residuals.
Type21 update now uses separate in-bounds magnitude/step array indices and a
real shared Float3 workspace in the existing ECL carrier. Its complete 1,074-byte
owner has 29 reviewed fields and 15 differences confined to two independent
ring-preheader setup orderings. Reproduce with inspect-exattack-type21.py; this
is NON-EXACT, with no partial credit. Type19 update now also indexes its separate arrays in bounds, in its original
standalone source. It remains NON-EXACT at 821 bytes/34 fields with 206 full
differences, the same 30-block graph and 11 ordered calls. Its helper-visible
819/817-byte probes are different contexts; no helper was copied into production.
Type19/21 initializers now use in-bounds arrays and real vector/vertex objects;
their complete target-sized 763/727-byte owners have six/nine preheader
differences. Use inspect-exattack-trail-init.py; both remain NON-EXACT.

PauseMenu's opaque helper/deferred-footer context remains non-exact: 1,772
physical bytes versus 1,776, with 816 differing overlapping positions. The
October 6 state-9 if/else form is fully byte/field-neutral. Inspect retained
controls before compiling another hypothesis.

DrawReplayMenu at 0x004234F6 now uses real Float3 objects. The isolated correction
is fully byte/field neutral: 1,244 candidate bytes versus 1,280 target bytes,
54 fields, 1,180 differing overlapping positions and 36 absent bytes. It remains
NON-EXACT; the old 36-byte size shorthand understated the complete residual.
No original expression grouping or broader class ownership is inferred.

Enemy OnUpdate's existing playfield conversion result now uses its genuine Float3
object and named x/y members. A fresh 12-input-bound baseline and candidate
preserve all five bodies, 4,060 bytes and 99 fields. The 3,900-byte physical owner
still has 39 complete differences with all 97 fields target-correct; no new
exact credit. This is separate from the parked new-accessor branch.

Later source-context controls are also recorded negatives: Enemy draw's full
second-angle predicate is byte/field neutral; DrawResult's two-output selection
member and FrontCalc's completion member retain non-target runtime calls.
Projection's scoped scratch lifetime and freshly rebound timer-header context
are neutral. The genuine PlayerShot Type1 VM/color subobject still gives the
190/204-byte tail-duplication frontier. Type14/22 draw's in-class vertex projection
operation is neutral at 974/959 bytes. Complete proofs and exact restoration
recipes are retained in the corresponding October 6 packets; none earns credit.
OnUpdateResult's new selected-character lookup member also remains neutral at
483/485 bytes and 34 fields after a current baseline binding rebuild; its
byte-load/widen scheduling frontier is unchanged. Its complete linked comparison
has 224 overlapping differences plus two absent bytes; the extent gap alone
is not the residual. All 48 existing code sections and 37 exact siblings remain
byte/binding neutral.

## Fresh bounded controls and configuration array

GameplaySetupThread and Player charge remain nonexact after new constructor,
member and branch-organization controls. GameConfiguration now uses the genuine
two-byte sideFocusMode[2] array at +B4 instead of indexing across separate chars.
All four direct TUs and actual copy/lifecycle/serialization consumers preserve
complete owner bytes and semantic fields; layout remains 0xCC, Supervisor +388.
The 93-TU/335-unit include inventory was classified, not cold-replayed. Bounded
compiler-identity closure additionally covers private labels and complete EH
graphs. Exactly 115 symbol spellings in 12 configured units are refreshed from
actual COFF destinations; 23 MIDI names were stale before this array change.
No destination, addend, extent or compiler profile changes. Current proof is in
the October 6 config-array-closure, config-direct, config-impact and
ECL fresh-context2 packets. All 14 focused committed-source receipts passed
and were accepted on 2774afac: the two changed exact bodies and 12
manifest-changed units. No array-related new exact credit.
Do not import the unrelated phase-sentinel overflow correction into
CleanupGameplayState's Front transition/death counter.

Effect count-member and network-parser terminal-scanner controls are negative.
Supervisor OnUpdate's new structured-cold-reregister candidate has the target
27 calls/92 fields and one title call, but is still 960/996 physical bytes with
803 overlap differences and 36 absent bytes. Canonical source is unchanged;
2,736 matching static traces are not runtime proof. Preserve this reproducible
source rather than repeating the deleted historical Packet 64 experiment.

The Front message wire API trial is fully neutral across 671 common-owner
bytes and 32 fields. Complete explicit-profile comparisons give Load 356
linked overlap differences plus two absent bytes and Release 135 plus three.
Their old two-/three-byte shorthand was only an extent gap. Release's graph
count difference is unreachable alignment; Load also changes cache/loop-test
placement while preserving bounds and all three returns. Audit the retained
invert-loop/invert-errorpath-loop controls and their provenance before a new
source trial. The October 6 front-message-wire packet has the complete graph.

## Current replay-save rendering frontier

Canonical DrawReplaySave source remains unchanged and NON-EXACT. The new isolated
inplace-target-depth candidate is 794/800 bytes with 41 fields. Indexed replay
accessors and genuine row expressions recover list/keyboard structure; the only
remaining instruction-structure seam is the first Z multiply at 0x423B0E, where
target uses four x87 instructions and the candidate one FMULP. Full entry-aligned
comparison remains 493 overlap differences plus six absent bytes. Independent
review checks all other operands, 19 branch destinations and four calls.
Use the October 6 replay-renderer-residuals and independent packets. Scalar-depth,
actual inline-vector, scalar-left and named common-frame-weight controls are
recorded negatives. The full genuine Float3 stage graph emits 852 bytes/40 fields;
the genuine in-place compound graph emits 794 bytes/41 fields but changes frame
and stack homes, with 496 full overlap differences plus six absent bytes. Both
keep the first Z seam and are rejected. No new canonical owner or receipt
follows. This is distinct from parked file saving.

## Current Charge input owner correction

Charge now calls the existing out-of-line WasPressed predicate on its actual
ReplayInputState object. The inconsistent private view definitions are removed;
0x8E layout, historyPressed +0x32 and native call ABI are unchanged. The exact
18-byte leaf has its new C++ symbol under replay-input-was-pressed. Charge stays
1,197/1,210 bytes with 1,160 overlap differences and 13 absent bytes. Canonical
and isolated carriers preserve all 1,217 bytes and 70 fields. All 23 configured
exact units across the five-TU/seven-profile header closure target-match;
complete non-debug code/EH graphs are neutral. The changed predicate passed its focused cold replay and was accepted on
ba203251: receipt 8b1c9216795bb80cf14cee588591049cee6a6646b11df4c14eb741872321e21a.
There is no new exact-byte credit.

## Current controller source-fidelity maintenance

The existing exact 47-byte SetButtonFromJoystickButtons helper now spells its
32-bit mask as `1U << buttonIndex`. Independent review and the canonical build
verify all 874 emitted bytes and 29 fields across the three-owner carrier are
unchanged. Negative-index handling and the signed 16-bit index API remain.
Shifts 0..31 now directly express unsigned mask arithmetic; no claim is made
that index 31 is reached. Counts >=32 remain undefined in C++, despite the
observed x86 count masking, so portable full-domain behavior is not claimed.
An explicit count-mask control adds three bytes and is rejected. Focused replay
is for this changed helper only; no new exact byte credit or poller closure.

The separate ordinary axis-range snapshot control remains 796/808 bytes with
32 fields and 102 overlap differences; early Y-bound scheduling is still wrong.
It is not integrated. The canonical poller remains 790 bytes and 29 fields.

## Current Type3 collision-vector correction

ExAttackUpdateCallbackType3 now consumes the real conversion-result pointer as
its existing PlayerPositionView object and writes x/y/z through named members.
The conversion call, local lifetime, 0x64 frame and every effect remain.
Independent review and the canonical build preserve all 800 emitted bytes and
38 fields across Type3 and its unchanged timer helper. The complete 780-byte
Type3 owner remains NON-EXACT with 15 linked-byte differences, all in the
middle.z versus tail.x argument-preparation window. No new coverage or replay
receipt follows. Current proof is in the October 6 frontier-fresh and
type3-independent packets; native product/runtime gates remain open.

## Current Etama draw vector-object correction

OnDraw now consumes its three genuine Float3 conversion results as the existing
objects and names x/y/z, avoiding indexing across distinct scalar members.
The old-header trial, current-array combined trial and actual canonical build
preserve all five emitted bodies: 1,672 bytes and 73 semantic fields. The two
uncalled seven-byte file-local helpers receive no target ownership. Exact
DrawSingleBullet and AddedCallback siblings preserve their complete 232/832
physical extents. OnDraw remains NON-EXACT: 594/600 bytes, 465 linked overlap
differences plus six absent bytes, with all 13 calls retained. No new receipt
or exact credit is needed for this changed nonexact body. Current carrier:
build/gpt-dots-etama-draw-canonical-20261006/canonical.obj. The fresh, combined
and canonical Etama packets retain full field/target/source proofs.

## Current Player added-state vector correction

InitializeAddedState is in separate PlayerAddedState.cpp, not the now-exact
PlayerRuntime AddedCallback carrier. A fresh baseline verifies its current
PlayerLifecycleView header context. The real position conversion result is now
consumed as its existing PlayerPositionView, with x/y/z writes. All four emitted
owners preserve 665 bytes and 30 semantic fields; all three reset siblings
still fully target-match. InitializeAddedState stays NON-EXACT at 572 physical
bytes (552 code plus a 20-byte switch table), 25 fields and 65 code differences;
the complete table is exact. Five windows remain: position/preheader22,
history8, flags20, side-table4 and mode-4 ordering11. Do not repeat this precise
typed-result control. Current source-bound carrier and full comparison are in
the October 6 player-added-canonical packet. No additional stale exact owner
was found in the current PlayerRuntime carrier.

## Current gameplay threshold correction

GameManagerSetup OnUpdate now performs its two score-threshold products in
unsigned 32-bit arithmetic, matching the target at the reachable phase 99999 sentinel.
The previous casts occurred after signed overflow. One focused build preserves
all 12 emitted owners byte-for-byte and field-for-field; OnUpdate remains 1230
bytes/87 fields with 105 full differences. Current proof is retained in the
October 6 setup-threshold and options-setup-audit packets. No exact credit.

The in-class ECL assignment-wrapper control is rejected: raw bytes match but 22
calls across seven owners bypass assignment and target SetCurrent instead.
RunEcl accounts for 16 changes. Do not treat raw neutrality as identity proof or
repeat this exact arrangement.

## Current Front continuation

FrontMessageRandom SetupRandomForSide now routes every fallback success through
the target final220/count guard. It remains682 versus693 bytes, with15 reviewed
fields and no exact credit. FrontAdded now uses two bounded five-vertex arrays;
the typed loop is byte-neutral. Branch-local loader errors recover its target
19-block graph and reset-through-return tail, but all706 bytes retain236 full
differences. Its position-store/early receiver scheduling remains open.
Current source-bound diagnostics and manifests are retained in the October6
front-random-guard and front-added-objects analysis packets.

Title DrawResult now uses bounded ranking-array and row cursors. Its current
canonical build preserves the 1,939-byte owner and all 82 fields, with the same
22 prefix differences. The October 6 title-ranking-bounds packet contains
current input hashes and the complete diagnostic. Enemy draw's newly tested
strict conjunction is neutral at the same four differences; do not repeat it.

## Parked specific actions

The following specific actions remain parked: ApplyNetworkInput ABI canonical
promotion, Type18/24 helper visibility, EnemyManager OnUpdate accessor work,
and the October 6 SaveReplay stream-row probe after its risk block.
A general continuation instruction does not reopen these actions. Work on other
authorized source-backed owners instead.

## Type18/24 reference frontier (helper-visibility action parked)

`ExAttackUpdateCallbackType18_24 @ 0x004491E0` remains **NON-EXACT**.

The maintained target-correct source is 1,442 bytes against the 1,436-byte
target. Fresh target review proved that the older 1,435-byte candidate was
misleadingly close because it tail-merged a state-2 completion path that the
target keeps as a distinct early return-one epilogue. Keep the corrected case
ordering and direct `extra->angle4C` use unless new target evidence contradicts
them.

The current hard seam is local lifetime/allocation: target collision size is at
`[ebp-0x24]`, three distinct Float3 history-delta slots are
`-0x30/-0x3C/-0x48`, and collision point is `-0x18`. Same-TU Float3
visibility can move some slots but coalesces the three deltas, so it is
diagnostic only.

## Other large owners

Select the next owner from the live ledgers
rather than an old roadmap. At this checkpoint the largest source-present
non-exact owners include:

- `EnemyManagerView::OnUpdate` — 3,883 bytes;
- `TitleScreenView::OnUpdateOptions` — 2,045 bytes;
- `TitleScreenView::DrawResult` — 1,939 bytes;
- `PlayerLifecycleView::UpdateMovementAndOptions` — 1,835 bytes; now retains the reviewed
  cold144 source: 1,900 physical bytes, all 66 field identities and both tables
  proved, but 144 full-byte differences remain. The shared-header impact is
  bounded by unchanged layout and 42 private-label updates in six exact units;
  no broad consumer replay was performed. The October 6 typed conversion-result
  cursor keeps the real call and all 1900 physical bytes/66 destinations neutral;
  two private table-label renamings are independently resolved. Current object is
  build/gpt-dots-config-direct-20261006/movement-title/movement-array.obj.
- `EnemyManagerDrawImpl` — 1,758 bytes;
- `PauseMenu::OnUpdate` — 1,734 bytes;
- `GameplaySetupThread` — 1,689 bytes;
- `SupervisorServiceUpdate` — 1,633 bytes;
- `FrontCalcCallback` — 1,491 bytes;
- `ReplayManagerView::SaveReplay` — 1,238 bytes; the scoped write-count lifetime
  is recovered, while packing-loop scheduling/spills remain non-exact.
- `GameManagerSetupLayout::OnUpdate` — 1,230 bytes.

This list is routing information only. Recompute it from `config/functions.csv`
and `config/matches.csv` before treating it as exhaustive.

For short functions, use `docs/SMALL_FUNCTION_FRONTIER.md`; its current filter
contains 11 non-exact authored/source-present functions at or below 256 bytes.
Large owners and leaf functions should both continue to receive serious work.

## Session restart and checkpoint rules

Follow `AGENTS.md` first. The minimum repository-side preflight is:

~~~bash
git status --short --branch
git diff
python3 scripts/verify-target.py
python3 scripts/validate-tracking.py --require-target
python3 scripts/report-reconstruction-status.py
~~~

From GPT-web, attest the registered `th09-ida` provider separately as described
in `AGENTS.md`; IDA remains provisional semantic evidence, not an exactness
Oracle.

After a coherent source/ledger batch:

~~~bash
python3 scripts/validate-tracking.py --require-target
python3 scripts/progress.py
python3 scripts/ci.py
git diff --check
git status --short --branch
~~~

Run focused canonical replays only for changed functions. Reuse hash-matched
unchanged-function evidence instead of replaying the whole historical cohort or
repeating cold builds after every edit (operator instruction, 2026-10-05).
Exactness credit is accepted only by the target-bound replay/acceptance path; a local commit is a
checkpoint, not proof.

Use commit messages of the form `gpt-dots: ...`. Do not push.

## Documentation hygiene

- Replace this handoff in place; do not append packet journals to it.
- Current exact/non-exact state comes from the ledgers, not packet prose.
- `docs/KNOWLEDGE_BASE.md` is durable evidence/history. Its recorded state
  labels and packet-local "current/remains" wording are historical unless a
  live ledger independently agrees.
- Prefer commands and durable structural facts over transient whole-owner byte
  counts in long-lived documentation.
- When a function is promoted to exact, remove it from active frontier lists in
  the same checkpoint.
- Keep `.analysis/` only for unresolved evidence that is still useful and not
  reproducible from tracked source/config; stale status snapshots should be
  regenerated or removed.
