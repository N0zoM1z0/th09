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
261,982 exact bytes; the current 218,501-byte ledger leaves 43,481 bytes.
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
| Canonical exact functions | 928 |
| Source-present non-exact functions | 52 |
| Source-present non-exact bytes | 57,269 |
| Authored without maintained source | 0 |
| Canonical exact authored bytes | 218,501 |

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

## Latest exact Front calc closure

FrontCalcCallback at 0x00417630 now reproduces its complete 1,491-byte owner,
384 instructions, 42 calls and all 107 independently established fields.
The source models count>=2 completion and frame dispatch as complementary
arms with one final frame increment/return. Within the duplicated mode0/mode1
completion bodies, the zero-side owner nests stage>=8 replay-neutral handling.
This ordinary whole-function continuation context recovers the target late
mode1 backward sharing; earlier isolated nesting and shared-label negatives
used a different outer CFG. No helper, profile, ABI, extent or denominator change.
Use front-calc-callback; the old 1,510-byte/partial-region frontier is superseded.
Independent full-owner proof is in the October 6 frontcalc-independent packet.
Focused forced-recompile replay passed and was accepted on da33a646:
receipt 3a36fbd779ea9cc2d0cdc474f704feed395fe4cfed728f10826aadba44387d7f.
Native product and runtime gates remain open.

## Latest exact Background added closure

Background::AddedCallback at 0x00403830 now reproduces its complete 475-byte
owner, 140 instructions, 16 branches, nine calls and all 19 independently bound
fields. Exclusive stage-culling arms share one real ClearSpellBackgroundState
and return continuation. This recovers the target stage-5 body followed by the
stage-9 backward branch; earlier label/guard negatives retained duplicated
success continuations. No ABI, profile, extent or denominator change.
Use background-added-callback. Both isolated and canonical builds match; all
seven same-profile exact siblings retain 2,193 bytes/139 fields. Complete
code/EH/data collateral is neutral, with canonical SafeSEH indices checked by
actual handler identity. Focused forced-recompile replay passed and was accepted
on 901484cd: receipt 5d7b0cec9bfbe7de05aaa158cc6dc35c5d6409a76c18a810da96a32da73231e4.
Native product and runtime gates remain open.

## Latest exact Type6 initializer closure

ExAttackInitializeCallbackType6 at 0x00445EC0 reproduces the complete 407-byte
owner, 119 instructions and all 38 independently bound fields. The two genuine
left/right branches each publish angle and angular velocity; spawn Y/Z and
opponent-space publication share their existing continuation. No RNG call,
store order, Float32 publication, helper, ABI, layout or profile changes.
Independent review and the canonical carrier agree; all 66 other code owners
and 35 non-code sections are neutral. All 22 previous exact siblings retain
9,126 bytes/401 fields. CompareOperands +0x14 changes only its actual local
symbol spelling from $L8517 to $L8516, with owner-relative identity verified.
Use exattack-type6-initialize and ecl-compare-operands for focused replay.
Focused forced-recompile replays passed and were accepted on cd0cee99:
Type6 e831c0d4b00f8c8dffb6be9ba2ca5557c02e9aea27d8316136f01286b98f0991;
CompareOperands 952f16b28c31c06644c58fcc50cac10d1f3e3baf61bb69142e822afa2e8f1a95.
Native product and runtime gates remain open.

## Latest received-frame storage context

InsertReceivedFrame now forms all byte-address roots through its existing complete
layout view. This preserves the target's read-before-index guard and side-1/index-10
write into adjacent lastReceived[0], while removing entries47C[20].frame04 from the
source. Isolated and canonical objects are fully neutral across both nondebug
sections: 211/213 owner bytes, 172 overlap differences plus two absent, no fields
or calls. Runtime layout/lifetime and valid-side assumptions remain open.
Use .analysis/gpt-dots-frame-insert-contexts-20261006/verify-current.py; richer
typed-record and ordinary record-member contexts regress and are not integrated.
No exact manifest changed and no replay is needed.

Etama OnDraw's new positive active-record arm and cap-rejection continuation are
fully neutral; indexed Laser selection regresses to 568/600 bytes. Both exact
siblings remain intact. The etama-loop-contexts packet retains complete proofs;
do not repeat these precise models unchanged.

## Priority frontier 1: EclManager::RunEcl

`EclManager::RunEcl @ 0x004086C0` remains source-present and **NON-EXACT**.

The current ECL carrier includes the October 6 Type6 initializer closure and
the byte-neutral random-biased movement typed-consumer correction.
Its 66 other owners are byte/field-neutral; RunEcl retains raw function
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
OBJ=build/gpt-dots-biased-movement-20261006/canonical.obj
python3 scripts/report-ecl-codegen.py --json "$OBJ"
python3 scripts/report-ecl-handler-boundaries.py "$OBJ"
python3 scripts/audit-ecl-callsite-identities.py "$OBJ"
python3 scripts/report-ecl-handler-shapes.py "$OBJ"
~~~

The callsite and handler-shape reports are structural diagnostics and not
exactness Oracles.

The new consumed timer/due-instruction guard is fully neutral on this carrier:
removing the one-pass dispatch loop preserves all 67 owners, 28,838 bytes and
1,215 fields, with the same frame and six-handler frontier. It is distinct from
the earlier inner-continue and outer-context loops. The October 6 enclosing-models
packet retains its complete proof; do not repeat this precise arrangement.

The later pending-subroutine while-loop context preserves reported size/frame/
call/field counts but shifts every opcode entry: 0/187 target offsets match.
Its strict complete CFG identity pairing fails, so no target binding or handler-
shape claim follows. All 66 other emitted owners are byte/field neutral.
The October 6 structured-traversals packet retains this isolated negative;
do not repeat the guarded pending-entry form unchanged.

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
The current source-bound carrier is
`build/gpt-dots-biased-movement-20261006/canonical.obj`; verify it without
compiling using the same packet
`.analysis/gpt-dots-biased-movement-20261006/verify-current.py`. The preceding
Type6 proof is retained in its packet;
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

## Latest whole-context controls

The latest structural-owner packet adds five distinct negatives. Fully indexed
trail-array traversal changes the Type19/21 initializer extents to 762/712 bytes
and gives Type21 update 50 full differences. DrawResult's full selection member
retains a non-target call, while its explicit final restoration continuation is
fully neutral across 1,942 bytes/82 fields. ECL's raw-path entry into the shared
integer store emits 15,560 physical bytes, 597 fields and 374 direct calls;
strict identity/shape/codegen checks reject that input, so no new handler claim
follows. Effect's complementary inactive/active loop arms emit 461/475 bytes;
all 14 same-profile exact siblings retain their complete targets. Canonical
source is unchanged. Use the October 6 structural-owner-contexts packet for
full source/include/field/target proofs; do not repeat these precise forms.

Four later phase/dispatch controls are now recorded negatives. Options' frozen
if/else entry changes the graph and emits 2,049/2,045 bytes; its dispatch-only
switch is neutral across both owners. ECL's complete post-dispatch scratch
lifetime family is neutral across all 67 owners/28,838 bytes/1,215 fields,
with the same six-handler frontier. ScreenEffect's in-class final-phase endpoint
operation is neutral across 22 common owners/2,997 bytes/190 fields; its uncalled
20-byte copy has no ownership. The October 6 phase-dispatch-contexts packet has
source/target/object proofs. Do not repeat these precise arrangements unchanged.

Current-carrier RunEcl six-fragment scope flattening is now tested and fully
neutral across all 67 owners/28,838 bytes/1,215 fields. Earlier scope results
used an older carrier, but this current context is no longer an open route.
Two genuine outer main/child completion-loop controls regress to frame 0x16C
and 14,808/14,820 logical bytes; both preserve all 66 other owners. The strict
identity gate rejects their extents. Keep the canonical six-handler frontier.

Enemy draw's collapse/render completion and later valid-prefix emission-loop
condition are both fully neutral, preserving the same four FSUBR/PUSH differences.
DrawResult's declaration-only zero-offset bank view is also neutral after its
single renamed external storage symbol is independently bound; it neither fixes
the 22 prefix differences nor establishes original global-object ownership.
FindCollision's common restoration gives 409/602 bytes and is rejected; fresh
baseline is 604
bytes. DrawResult's bank/screen constructor stays as a non-target call, while
trivial aggregate input gives 1,941 bytes with wrong stack homes. Enemy
OnUpdate's common world-position completion gives 3,840/3,900 physical bytes
and loses one target call context. All are isolated negatives, distinct from
parked accessor work. The October 6 completion-contexts packet retains source
deltas, current bindings and precise comparison limits. No new credit.

## Latest operand-expression and trail-subobject controls

The October 6 operand macro hypothesis has now been tested. Replacing all 314
resolved-access wrapper calls preserves 63 retained owners/28,679 bytes/1,211
fields, including the same six-handler RunEcl frontier. The four removed
helper copies are unreferenced and have no target ownership. Expanding raw
lvalue access separately shrinks opcode 157 to 158/160 bytes and is rejected;
the physical owner remains 15,564 bytes with 598 independently paired fields.
No macro candidate is integrated.

Type19/21 Float2 UV field grouping is neutral across all 67 ECL owners. A
consumed UV-reference form gives 757/728 bytes against 763/727; separating center
initialization from rim traversal gives 763/713 bytes. Complete target
comparisons and unchanged collateral are independently verified in the
ecl-operand-macros and trail-uv-owner packets. All 23 exact ECL siblings and 35
non-code sections remain intact. Do not repeat these five unchanged contexts;
there is no new exact credit or canonical source change.

## Latest selection and completion controls

Three new isolated controls are terminal negatives; canonical source is unchanged.
DrawResult's complementary initial screen-selection ownership emits 1,939 bytes
and 82 fields but regresses from 22 to 60 complete linked differences. Its
other emitted body and all 16 nondebug noncode sections remain neutral.

RunEcl's positive active-interpolation body plus absent-child continue traversal
is fully neutral across all 67 owners, 28,838 bytes/1,215 semantic fields and
35 noncode sections. The 943-block identity audit and six-handler 142/148
shape frontier remain. This is distinct from the earlier instruction-walk,
difficulty-mask and outer-context loops.

The smaller context-owned SetInstructionTime operation also remains an actual
non-target call: helper 22 bytes/one field, RunEcl 15,560 physical bytes with
598 fields/375 calls. It covers the JUMP/JUMP_DEC shared tail while leaving
signed32 displacement evaluation after assignment in the caller. Strict
identity/shape gates reject it. Do not force this method inline or repeat it.
Both ECL candidates preserve all 23 exact siblings, 9,533 physical bytes/439
fields; the other 66 common owners and 35 noncode sections are unchanged.

The independent semantic-context-audit packet records full source/include,
owner/field/target and collateral proof. Historical compiler/system-header
provenance remains limited; audit-time hashes are not retrospective attestation.
The three precise source models are not open hypotheses anymore. Prior Enemy
argument-owner and ECL instruction-latch controls remain recorded negatives in
the Knowledge Base and their retained packets; they should not be repeated alone.

## Latest fan, successor and pool-count controls

Four isolated controls add no source change or exact credit. Direct access to the
shared trail-array subobject is neutral across all 67 ECL owners; the previous
consumed-reference regression is not a general statement about that layout.
A separate center/rim fan layout regresses Type19/21 initializers to 69/315 full
overlap differences, with Type21 713/727 bytes. Type21 update stays unchanged.

The ordinary instruction-owned signed-16 Next method is neutral across all 67
common ECL owners and preserves the six-handler RunEcl frontier. Its uncalled
seven-byte copy has no target ownership. Effect's grouped PoolCounts layout is
neutral across all 16 owners, with the same four OnUpdate count-read differences.

All 23 ECL and 14 Effect exact siblings, actual input bindings and noncode
sections pass independent review. Full proof and provenance limits are retained
in the October 6 semantic-contexts packet. Do not repeat these precise models
unchanged. The earlier region, heading, nested-reference and planar-dot controls
remain recorded negatives in the Knowledge Base and their packets.

## Current random-biased movement consumer

MoveRandomBiased now consumes the actual conversion-result address as the
existing EnemyFloat3 view and reads its named Y member. The genuine Float3
conversion call and both sequential reflections remain; inherited object-view
and lifetime assumptions are not closed. Isolated and canonical carriers
preserve all 67 owners, 28,838 bytes/1,215 fields and all 102 nondebug sections.
All 23 exact siblings fully target-match. Twenty compiler-private symbol names
in ecl-post-update-movement and ecl-compare-operands are refreshed from actual
same-owner destinations; no target/address/addend/profile change.

The complete helper remains 517/516 bytes with 28 fields, 145 instructions,
14 ordered calls and 148 linked overlap differences plus one excess. Its old
one-byte shorthand described only extent. Typed Float3/EnemyFloat3 consumers
are neutral; positive-duration early completion gives 520 bytes and is rejected.
These three precise source arrangements are measured; no new exact credit.
The October 6 biased-movement packet retains independent and canonical proof.

## Latest pool, vector and strip-state controls

Three October 7 isolated source controls are terminal negatives. Complete
collision-pool subobject grouping is neutral across all eight AddedState
nondebug sections. Using the existing Float3 type for its actual position,
history and conversion result changes only the genuine conversion symbol at
+0x24, still bound to 0x4343D0; full linked bytes stay neutral. Both retain
572 physical bytes, 25 fields and the same 22 preheader differences.

Enemy draw's strip working-state aggregate regresses to 1,755/1,758 bytes,
53 fields and 463 overlap differences plus three absent. Its 28 calls and 42
branches survive, but raw block counts are 67/68; only explicitly verified
identity-padding collapse gives 66 matching structural blocks. No byte is
masked or credited. All six exact siblings and all noncode collateral remain
intact. Canonical source and manifests are unchanged; no replay is needed.

Full source/field/target and input proof is independently reviewed. Recheck with
python3 -B .analysis/gpt-dots-pool-strip-audit-20261007/verify-current.py.
The historical Enemy baseline include closure is manifest-bound without
showIncludes; new controls have actual include logs. See the latest Knowledge
Base section for limits. Do not repeat these three precise source models.

## Latest broadcast and traversal controls

Four October 7 isolated source models are now measured negatives. GameManager
input broadcasting emits 1,202/1,230 bytes; Type19/21 complete ring working-state
aggregates emit 755/763 and 726/727. RunEcl context-traversal grouping regresses
to 15,572 physical bytes and frame 0x170; its strict identity gate rejects the
input, so no new handler claim follows. Front's ten-entry weighted fallback
loop emits 686/693 bytes while retaining the corrected common final guard.

Canonical source remains unchanged. All unrelated owners, eight GameManager
and 23 ECL exact siblings, and all noncode collateral are preserved. Complete
source reversal, byte/field comparisons and input/provenance limits are in the
broadcast-ring-contexts packet and latest Knowledge Base section. Recheck with
python3 -B .analysis/gpt-dots-broadcast-ring-contexts-20261007/verify.py.
Do not repeat these four precise models unchanged; no new exact credit.

## Latest query, cursor and dispatch-continuation controls

Five October 7 isolated source models are independently reviewed negatives.
Type3's named collision-query aggregate retains 780 bytes/38 fields/24 calls,
but gives 34 complete linked differences versus 15; the ordinary four-sample
VM/history loop emits 697/780 bytes with 35 fields. Its exact timer and all
noncode collateral are unchanged.

Front's carried instruction cursors retain publication points and post-music
reloads, but Load gives 401/405 bytes (338 differences plus four absent) and
Release gives 254/271 (226 plus 17). Their 18/14 fields, eight/six ordered calls
and all noncode are independently checked. The explicit probe profile is not
proof of the complete historical production invocation.

RunEcl's shared integer-result publication outside the switch changes inlining:
15,512 physical bytes, 596 fields, 373 direct calls and frame 0x168. Strict
identity rejects the changed shape; no new handler claim follows. All 66 other
owners, 23 exact siblings (9,533 bytes/439 fields) and 35 noncode sections stay
intact. Canonical source and its existing six-handler frontier are unchanged.

Recheck the three retained packets' verify.py without compilation:
.analysis/gpt-dots-type3-query-contexts-20261007
.analysis/gpt-dots-front-carried-cursor-20261007
.analysis/gpt-dots-ecl-result-continuation-20261007
The last packet retains the independent review. No replay or credit applies.
Do not repeat these precise models. The preceding collision-shape and Options
volume-continuation negatives remain recorded in the Knowledge Base and their
collision-shape-contexts packet.

## Latest recovery and operand-value controls

Six recovered/new isolated controls are terminal negatives. Trail vertex-owned UV
leaves both initializers unchanged and worsens Type21 update to 61 differences;
captured interpolation-array traversal fails RunEcl extent identity. Byte/short
value-return readers are neutral across 65 common owners. Player movement's
two-float working aggregate worsens full linked differences 144 to 178.

ECL const value-return scalar readers grow frame 0x168 to 0x2C0 and break seven
exact siblings. Borrowed const-reference readers preserve all 65 common owners
and 23 exact siblings, with the same six-handler frontier. No canonical source
or exact credit changes. Full independent proof and no-compile verifier:
.analysis/gpt-dots-recovery-reader-audit-20261007/verify.py
Do not repeat these precise source/value-category models unchanged.

The subsequent complete decrypt-table/signature storage view is also rejected:
218/220 bytes, 11 fields, 212 overlap differences plus two absent. It widens
the first signature read to a dword through +0x63; original storage/padding
ownership remains unknown. The fresh baseline retains 38 full differences.
Use the decrypt-storage-view packet verify.py; no canonical change or credit.

## Latest whole-owner storage controls

Four October 7 isolated source models are measured negatives. Supervisor service's
named packet union is neutral at 1,632/1,633 bytes and 108 fields; complete-object
memcpy plus actual word members also stays 1,632 bytes, with an uncalled four-byte
getter copy. ApplyNetworkInput's API remains untouched. Charge input snapshot
grouping is neutral at 1,197/1,210 bytes, 70 fields and 58 ordered calls.

ECL shared scalar-storage grouping is neutral across all 67 common owners and
23 exact siblings; its five-byte RawStorage copy is unreferenced. RunEcl retains
the same six-handler frontier. Wire lifetime/alignment/aliasing remain unknown.
Canonical source and exact manifests are unchanged; no exact replay applies.
Use .analysis/gpt-dots-service-packet-owner-20261007/verify.py for complete
retained-object/source/include/field and target proof. Do not repeat these four
precise models unchanged; the Knowledge Base records their limits.

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

Type19/21 initializer counted-for loops and reusable UV scratch objects are
now separately tested and fully neutral across all 67 ECL carrier owners.
Consuming the current vertex into a reference at iteration entry instead emits
765/720 bytes versus target 763/727 and is rejected; the other 65 owners stay
neutral. Their six/nine-byte canonical preheader residuals remain. Full source,
COFF, field and target comparison proofs are in the structured-traversals packet.

PauseMenu's new result/normal completion decision is fully neutral; a duplicated
completed-closing animation continuation emits 1,836/1,776 physical bytes and
is rejected. Both preserve canonical source. The October 6 pause-outer-continuation
packet holds complete proofs. Its opaque helper/deferred-footer context remains non-exact: 1,772
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
member and branch-organization controls. Gameplay's later ordered registration/null-test chain is fully byte/field-neutral;
its partial-construction loop exit emits 1,669/1,689 bytes and is rejected.
Current full comparisons and the bound reused carrier are retained in the
October 6 gameplay-registration-chain packet. GameConfiguration now uses the genuine
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

## Current DirectPlay message-handler controls

HandleMessage remains 768 authored /808 physical bytes, 46 fields and 125
complete linked differences on a fresh current-input baseline. Per-case lock
completion recovers exactly seven tail bytes, leaving 118 differences; it is
not integrated. Receive-kind switch and non-frame hierarchy controls regress;
frame-packet forwarding retains a non-target call. The existing Supervisor
clock-field view is fully linked-neutral. These five models are now measured,
not open hypotheses. The October 6 network-dispatch packet retains complete
comparisons, an independent audit of the first four, and a no-compile verifier.

The three scalar aliases bind Supervisor frameCounter +0x458, frameStartTime
+0x464 and waitTime +0x46C. Do not infer distinct globals or original class
ownership. Canonical source and all exact owners are unchanged; no replay or
new credit follows. Further work needs a genuinely different source context.

## Current playback-stage frontier

BeginPlaybackStage now forms the existing target-observed stage-table address
through the complete ReplayDataView byte representation. This avoids stepping
outside a single pointer-array row for the ten-slot stage domain, while retaining
the unusual address null test. No serialization or ABI change. The isolated and
canonical objects preserve all 30 nondebug sections, including 14 normal owners,
both auxiliary-less EH companions, all ten same-profile exact siblings and
complete SafeSEH/EH identities. Source-domain/lifetime assumptions remain.

The fresh full owner is 538/540 bytes with 32 fields, 49 overlap differences and
two absent bytes. The old seed-only shorthand omitted early pointer-load and
tail scheduling seams. Full valid-stage continuation and selected-stage aggregate
models regress; scoped restoration and the frame seed getter are neutral.
None is integrated. Do not repeat those four precise arrangements unchanged.
The October 6 playback-context packet retains source/input/target and independent
all-section proof; current carrier is
build/gpt-dots-playback-context-20261006/canonical.obj. No new exact credit.

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

The later Type3 whole-state if/else model regresses to 781/780 bytes with
703 linked overlap differences plus one excess. Complementary lazy collision
continuations and three separate aggregate sample initializers are neutral
across all 800 emitted code bytes/38 fields and all six nondebug sections.
The 20-byte timer sibling remains exact. No source is integrated or credited.
The October 6 type3-state-contexts packet retains complete independent proof;
do not repeat these three precise arrangements unchanged.

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

## Current Player added-state fields and mode ownership

InitializeAddedState's complete private layout now declares the observed float
at +0x7C, two-state int array at +0xA0 and separate side state at +0xA8.
Its 0x30F70 size, all existing offsets, signed upper-only clamp, per-mode pair
publication, default non-publication and mode-4 side/capacity overrides remain.
No shared header, ABI, compiler profile, ownership extent or ledger changes.

The whole owner remains 572 physical bytes (552 authored plus the 20-byte table),
25 fields, 11 calls, 10 direct branches and five switch entries. The field-owned
context closes all eleven mode-4 add/store differences, narrowing the full
comparison from 33 to 22. Only position/pool-preheader +0x3A..+0x56 remains.
The earlier per-mode publication improvement from 65 to 33 is retained.
These are complete-owner diagnostics, never partial exact credit.

Independent isolated and actual canonical reviews agree across all eight
nondebug sections. Three exact reset siblings retain 93 bytes/five fields;
the four noncode sections are unchanged. Current carrier:
build/gpt-dots-added-state-field-owner-20261006/canonical.obj.
Use .analysis/gpt-dots-added-state-field-owner-20261006/verify-current.py.
The mode-owners and owner-controls-review current verifiers are now historical
for AddedState; they bind its previous source. Native/runtime and inherited
object-view/lifetime assumptions remain open. No exact manifest changed or
replay is needed.

Two follow-ons preserve the same 22 differences and are not integrated.
An ordinary position-initialization method adds an unreferenced 40-byte copy.
Exposing the real out-of-line conversion adds a three-byte definition referenced
by the existing genuine call, with no new target ownership. Earlier chained-pair,
position aggregate and pool-cursor arrangements remain recorded negatives.
Do not repeat these precise models unchanged.

Controller's single in-class axis-mask operation remains rejected at 797/808
bytes, 117 overlap differences plus 11 absent; canonical is 790/808 with 102
plus 18. Its exact button helpers stay intact and its uncalled 74-byte copy
has no ownership. The controller-axis-operation packet retains complete proof.

FrontAdded's successful-resource arm and closed-panel helper remain negative.
Its new ordinary per-vertex SetPosition model fully inlines all ten calls and
preserves all 30 XYZ final stores, but emits 693/706 bytes with 37 fields,
385 overlap differences plus 13 absent. All 21 ordered calls and the 19-block
graph remain; its emitted 24-byte method has no nondebug references.
Canonical Front source is unchanged. The front-vertex-review packet retains
full source/field/store/collateral proof; do not repeat this unchanged model.

## Current gameplay threshold correction

GameManagerSetup OnUpdate now performs its two score-threshold products in
unsigned 32-bit arithmetic, matching the target at the reachable phase 99999 sentinel.
The previous casts occurred after signed overflow. One focused build preserves
all 12 emitted owners byte-for-byte and field-for-field; OnUpdate remains 1230
bytes/87 fields with 105 full differences. Current proof is retained in the
October 6 setup-threshold and options-setup-audit packets. No exact credit.

Four later setup callback models are now negative. Snapshot construction and
aggregate-only publication both retain non-target helper calls. Late-score
equality continue is fully neutral at 1,230 bytes/87 fields/105 differences;
a whole ready/else callback emits 1,231 bytes and regresses its complete linked
comparison. Current-input and independent full-owner proofs are in the October 6
setup-continuations packet. All 11 other common owners, eight same-profile exact
siblings and seven noncode sections are unchanged. Canonical source is untouched;
do not repeat these precise models or force their helpers inline.

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
  two private table-label renamings are independently resolved. The movement
  consumer now uses the existing canonical ReplayInputState declaration and
  opaque IsHeld ABI; full bytes/fields and all six emitted helper copies are
  neutral. Current source-bound carrier is
  build/gpt-dots-movement-owner-context-20261006/input-owner.obj.
  Production input-array ownership remains open. In-class SHT speed getters,
  a shared two-mode directional operation, and a grouped runtime subobject are
  newly measured neutral contexts. Grouped position/bounds work retains a
  non-target call and is rejected. These exact source arrangements are now
  recorded negatives; no new exact credit or unchanged-leaf replay follows.
  A nonzero-state active arm with one final return is rejected: it adds a
  separate seven-byte idle epilogue and shifts both tables by eight. All
  original active operations and frame 0x18 remain unchanged. The complete
  1,908-byte physical candidate has 176 overlap differences plus eight excess;
  no new active-path lifetime evidence follows. See the October 6 enclosing-models
  packet instead of repeating this source form.
- `EnemyManagerDrawImpl` — 1,758 bytes;
- `PauseMenu::OnUpdate` — 1,734 bytes;
- `GameplaySetupThread` — 1,689 bytes;
- `SupervisorServiceUpdate` — 1,633 bytes;
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
