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
| Function candidates | 2,192 |
| Boundary/origin unreviewed | 0 |
| Reviewed but origin-unresolved | 35 |
| Confirmed authored | 980 |
| Classified exclusions | 1,177 |
| Source-present authored mappings | 980 |
| Canonical exact functions | 897 |
| Source-present non-exact functions | 83 |
| Source-present non-exact bytes | 91,484 |
| Authored without maintained source | 0 |
| Canonical exact authored bytes | 184,286 |

The source-presence frontier is closed. Exact reconstruction is not complete.
The faithful Windows i386 product graph remains open. Semantic reconstruction
and portability have not started.

### Type-8/9 collision-exit return-contract control

Packet 746 freshly rechecks the shared `ExAttackUpdateCallbackType8_9`
collision exit at target `0x00446B02`: target loads `extra->angle08` through
EAX, while the maintained 748-byte candidate uses ECX at its only two
ordinary-byte differences. In the actual EclManager translation unit,
temporarily declaring `AnmVm::SetInterrupt` as returning the incoming short
leaves every candidate code byte unchanged (raw SHA-256
`63bcdf1bdf3ad5b2c4fedeb9fba713ba38317b26e926d39ea8665eb1335ffc8e`),
but changes its call relocation's symbol. The declaration was reverted.
Neither this result nor the callee's incidental AX residue establishes a
return contract or exactness. Do not repeat this ABI hypothesis; a new
target-backed source/data dependency is required to reopen this two-byte
register frontier. Coverage and phase gates are unchanged.

### Latest EffectManager bounded control recheck

Packet 743 freshly attests `EffectManager::OnUpdate @0x0040CDD0` against the
original target: maintained no-EH `/O2 /Ob1` is 475/475 bytes, with all 11
relocation positions and 427/431 ordinary bytes agreeing. Target loads primary
count `+0x34` before secondary `+0x38` at both the initial and loop-tail sums;
candidate reverses those two loads. A real staged addition and a per-iteration
pair of count snapshots both compile byte-identically to baseline, without
closing the four offsets. The same-TU `AddedCallback @0x0040D1C0` remains
51/51 with 28/35 ordinary comparable bytes; spelling the two genuine field
assignments separately lets VC7.1 merge the same common store but leaves its
ESI/EDI allocation unchanged. These are narrow negative controls, not a
compiler impossibility proof, source change, or partial exactness credit.
Do not repeat these forms without new target/source-family evidence; rotate
to a different large owner such as the current DrawResult frontier.

### Latest Type7 angular ExAttack complete-replay frontier

Packet 739 adds a complete, independently target-bound diagnostic for
ExAttackUpdateCallbackType7 @0x004464A0..0x004467BE. Target:799 bytes,
242 instructions,21 immediate calls,18 direct blocks,frame0x224 and four RETs.
Two cold maintained-source builds agree on all783 raw bytes and all35 complete
relocation records; raw SHA256 is
3f540f006091d0d52b15a28425adcf14e23344fa38ed9305910af02b880b2d0e.
All21 calls agree, but candidate has241 instructions and19 direct blocks.
Target's four bounds exits branch to its early state2 return-one block; the
candidate uses a later local return-one block and routes normal state1 exit
through a different return-zero block. Normalized alignment215/242 is routing
only. Size mismatch and graph mismatch grant no exactness or partial credit.

Actual maintained FromAngleMagnitude body visibility produces the target's
immediate ECX forwarding but a780-byte/240-instruction candidate, still with
18 versus19 blocks. Combining it with separate bounds or the previously
rejected shared-return label does not close the owner; the latter moves the
state2 branch in the wrong direction. Direct state1 exit, explicit VM/player
lifetimes and spawn-source lifetime are baseline-neutral. Do not repeat these
controls without new evidence. Reproduce with
`bash scripts/compile-probe.sh src/ExAttackUpdateType7.cpp build/exattack-type7.obj /MT /EHsc /Gs /DNDEBUG /Zi /Gy /GF /Oi /Gr /O2 /Ob1 /Oy- /I src`
and `python3 -B scripts/inspect-exattack-type7.py build/exattack-type7.obj`.
Next rotate to a genuinely different large owner, for example RunEcl, after
re-reading its live row and packet history. Packet739 changes no source or
matches; coverage remains897 exact /184286 of275770 (66.83%). The >95% and
native-product/later gates remain open.

### Earlier angular ExAttack complete-replay frontier

Packet 738 freshly reviews Type6 @0x00446060..0x00446319: 698 code/physical
bytes, frame0x224, 214 instructions, 19 direct calls, 18 direct blocks and
four RETs. Two maintained-source cold builds agree on all bytes and all30
records; raw SHA256 is
475594f58a36d6a2b3acd8fac32a283b570641110775fe1413139f7aaafdcf08.
Complete independently bound replay still has43 differences (535/578 ordinary
bytes); all calls/graphs agreeing does not grant exactness. Motion/rotation
allocation and state0 argument/animation/spawn-copy scheduling remain open.

Reproduce with bash scripts/compile-probe.sh src/ExAttackUpdateType6.cpp
build/exattack-type6.obj /MT /EHsc /Gs /DNDEBUG /Zi /Gy /GF /Oi /Gr /O2 /Ob1 /Oy- /I src
then python3 -B scripts/inspect-exattack-type6.py build/exattack-type6.obj.
The read-only script reuses the complete Type01 diagnostic with separately
reviewed Type6 bindings; no acceptance or partial credit. Seven natural controls
are raw/complete-record neutral: typed descriptor, direct state1 exit, state0 VM
snapshot, actual angle/descriptor-ctor visibility, used angle result and memcpy.
Branch-local position references worsen to56 differences. Do not repeat these
or Packet420's FromAngle-only visibility without new evidence. No source or
shared-header edit, match row or whole-repository replay is made.

Coverage rotation independently rechecks Type7 @0x004464A0..0x004467BE:
799 target bytes/242 instructions/21 calls; fresh separate-TU candidate is
783 bytes/241 instructions/35 fields, frame0x224, raw SHA256
3f540f006091d0d52b15a28425adcf14e23344fa38ed9305910af02b880b2d0e.
Its old live false-return diagnosis is stale: public FromAngleMagnitude already
returns void after Packet659. Target forwards preserved ECX; separate-TU source
reloads the motion address. Target callee does load EAX=this, so incidental EAX
is not proof of a source return promise. Bounds branches still target the early
state2 return-one block, unlike the candidate's later local block.
Packet739 supersedes that Type7 routing with complete independently bound
diagnostics; actual body visibility alone was already780/799 in Packet659,
and Packet432's shared label was rejected. Packet738 scratch is audited and
removed. Live897 exact,
184286/275770 bytes (66.83%),83 nonexact/91484 bytes remain unchanged; >95%,
native product/runtime and later gates stay active-incomplete.

### Earlier shared ExAttack type0/type1 update exact closure

Packet 737 closes ExAttackUpdateCallbackType01 @0x00441100..0x004412DA:
475 code/physical bytes, frame0x10, 166 instructions, 13 immediate calls,
20 direct blocks and four RETs without stack arguments. Template update slots
0x004A0F7C/0x004A0F8C both select it. Ten preceding and five following CC
bytes remain unowned; no boundary or denominator changes.

Removing the reconstruction-only single-use side-player cache and calling
CheckBulletCollision through the direct side-player expression naturally closes
all 43 prior byte differences, including state0 allocation/scheduling. Packet
376's claim that further work must require register steering is disproven.
The real void ExAttackInterpolation.hpp declaration and Bullet* collision
contract replace false local declarations. Both contract-only controls are
raw-byte-neutral but change actual callee symbols, not complete-record-neutral.
No helper body, visibility workaround, register forcing or profile change is
retained. All three-state motion/collision/Hermite behavior remains unchanged.

Two independent maintained-source cold builds and the fresh canonical carrier
agree on all 475 raw bytes and all 23 complete relocation records (13 REL32,
10 DIR32). Three GameManager fields retain addend4; targets are reviewed from
TH09 operands before acceptance. Raw SHA256 is
716cdce184f35319e11566ee8a7d706ff40de2002317baaeb61709534313a911.
Reproduce with python3 scripts/build-match-unit.py --unit exattack-type01-update
and python3 scripts/compare-coff-function.py --unit exattack-type01-update --json.
scripts/inspect-exattack-type01.py is complete-owner routing only, not acceptance.
Only this affected TU is compiled; no other TU or repository-wide cold cohort.
IDA prototypes/comments are corrected and read back; no bytes are patched.

Live coverage is 897 exact /184286 of275770 authored bytes (66.83%), with
83 nonexact /91484 bytes. Native product/storage/runtime, original TU/data
ownership, dual-Oracle semantics and portability remain open; >95% stays active.
The adjacent Type6 lead is independently rechecked in Packet738 above; its
already-direct collision receiver does not admit the Type01 cache control.
Packet 737 scratch is audited and removed; canonical/private/legacy
state is preserved.

### Earlier uncensused Player reward slot-zero closure

Packet 736 independently reviews the previously untracked callback-table slot0
at 0x004412E0..0x00441344 and adds it as authored/canonical exact. The complete
101-byte body has 36 instructions, two internal branches, one Spawn call and
five relocation fields. It receives owner-state in ECX and position in EDX,
loops on state +0x34 at 5*(28-base), spawns type0 for owner side, adds
5*base-140, and returns int zero. Type0 denotes table index only.

Predecessor 0x00441100 ends at RET 0x004412DA; five following CC bytes separate
this independent entry. Eleven CC bytes after its RET separate 0x00441350.
No tracked extent overlaps. AddedCallback loads the table at 0x0041EF91 and
publishes it at Player+0x30450; exact ApplyReward dispatches at 0x0041D389.
Direct IDA still lacks a function entry here, but its mapped bytes and table
agree with the verified PE. IDA omission does not establish physical ownership.

Two independent maintained-TU cold builds and the canonical carrier agree on
all raw bytes and complete relocation records; SHA256 is
5c19e8d7a6ab4d10b5261afc211578bdaa89f669c2576c50eee47ae357ed2ae2.
Build/replay player-owner-reward-callback-type0. The separate read-only
scripts/review-player-owner-reward-slot0.py reproduces bounded boundary/table
checks, not acceptance or whole-census completeness. All fifteen existing
same-TU owners retain raw bytes and records; all sixteen units replay exact.
No other TU or full-repository cold cohort is rebuilt.

At the Packet 736 checkpoint this added 101 bytes to both numerator and denominator:
896 exact /183811 of275770 bytes (66.65%), 84 nonexact /91959 bytes.
The frozen 35 unknown rows remain unchanged. Original TU/data/native/runtime
ownership and the >95% objective remain active-incomplete. Its adjacent Type01
routing lead is now independently closed by Packet 737 above.

### Earlier Player owner reward exact closure

Packet 735 supersedes Packet 222's frozen popup/register-allocation plateau.
PlayerOwnerStateView::ApplyReward at 0x0041D150..0x0041D73E is canonical exact:
1519 code bytes, 454 decoded instructions, frame 0x18 and all 91 relocation
fields (32 REL32 /59 DIR32). Complete replay covers 1540 physical bytes,
including alignment at 0x0041D73F and the five-entry table at 0x0041D740.
Only the 1519 code bytes receive new authored coverage.

Natural three popup cadence arms, unsigned packed-color predicates, shared
real position/velocity workspaces, direct value0 updates and a while loop
close the owner without register/volatile steering, padding or profile search.
All actual callers ignore EAX. The old returned-score contract was false on
the target's new-maximum path; both maintained ApplyReward and its already
exact state-advance wrapper are now void. The query at 0x0040F7D0 returns an
Enemy pointer, not a guessed int CheckMode; callback40 returns int with its
result ignored. These are target-bound call views, not unique original owners.

Two independent maintained-source cold objects and two fresh canonical builds
agree on every raw byte and complete relocation record. Raw SHA256 is
74008ecf88c704db0064e0559db8fed3b404b20a57c51693551f31bf43c4e0a9.
Reproduce with python3 scripts/build-match-unit.py --unit player-owner-apply-reward
and python3 scripts/compare-coff-function.py --unit player-owner-apply-reward --json.
The separate read-only inspect-player-owner-reward.py remains diagnostic only.
All 28 prior exact units across the three actual caller/wrapper TUs replay,
using nine profile/object carriers. Eleven UpdateBeforeState private labels
are refreshed after unchanged offset/type/addend and owner-relative target proof.
No full-repository cold cohort runs; layouts and inline bodies are unchanged.

Packet 735 totals were 895 exact functions /183710 of275669 authored bytes (66.64%),
84 nonexact functions /91959 bytes. Native link/storage/runtime, semantic and
port gates remain open. Packet-owned scratch is audited and removed; private,
legacy and canonical cache state is preserved. Packet 735's routing-only
callback-table slot0 observation is independently reviewed and closed by
Packet 736 above; it was not credited in Packet 735. Neither investigation
establishes whole-census completeness.
The >95% objective stays active-incomplete.

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
live ledger currently reports 897 exact and 83 non-exact. Recheck every listed
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
cohort was run. Current exact coverage is 184,286 /275,770 bytes (66.83%); the
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
units. No full-repository cold cohort runs. Packet 724 totals were 892 exact /178979 of
275669 authored bytes (64.93%), with 87 nonexact /96690 bytes. The >95% goal
and native/runtime/semantic/port gates remain open. Packet 725 closes replay below;
do not repeat Packet 724's
manual-byte-cursor or equivalent pointer-spelling probes without new evidence.

### Latest replay-menu complete exact closure

`TitleScreenView::OnUpdateReplayMenu @ 0x0042689A..0x00426E04` is canonical
exact: 1387 contiguous code/physical bytes, 404 instructions, frame 0x598 and
76 fields (31 REL32 /45 DIR32). Packet 725 removes the old stage-cancel break:
retail continues to the independent confirm check, including when both inputs
are present. State-1 cancel still returns immediately. Natural byte/word input
tests and ordered value1E5 launch if/else arms close all codegen differences;
unknown mode bytes still perform no launch-mode writes. Initialization source
and its enumeration quirks are unchanged. TH08 remains hypothesis-only.

Final probe, two independent maintained-source cold objects and canonical
carrier agree on every raw byte and relocation record; raw SHA-256 is
57936626034fcd4a1207c4a58461b9fff5458ed69903a320305e6718d2c8ea72.
Reproduce with `python3 scripts/build-match-unit.py --unit title-screen-replay-menu`
and `python3 scripts/compare-coff-function.py --unit title-screen-replay-menu --json`.
The read-only diagnostic supports `OBJECT --replay-menu` without granting credit.
All 34 prior O1 owners retain raw bytes and relocation geometry; nine StartMenu
private label spellings alone are refreshed after section/relative-offset and
unchanged-target proof. Fresh focused carriers replay 35 O1 units plus separate
O2 score-record-insert, all 36 exact. Ordinary selection still has two SIB
differences. No full-repository cold cohort runs. Packet 725 totals were 893 exact
/180366 of275669 authored bytes (65.43%), with 86 nonexact /95303 bytes.
The >95% objective and native/runtime/semantic/port gates remain open. Packet
726 repairs the adjacent result owner below without granting exactness.
Owned replay-menu scratch is removed after durable
recording; maintained source and the unit reproduce closure.

### Latest result-browser fidelity repair and non-exact frontier

`TitleScreenView::OnUpdateResult @0x004266B5..0x00426899` remains NON-EXACT.
Packet 726 repairs two old source errors: cancel jumps directly to return 1,
skipping all three timer increments, and both character consumers use result
order table 0x004A1DAC, not ordinary table 0x004A1D7C. The PE contents of these
separate tables agree; their physical identities do not. The new neutral
extern view creates no data definition or unique original ownership claim.

Two independent cold objects and canonical carrier reproduce 483 bytes /146
instructions /34 fields (9 REL32 /25 DIR32), versus target 485 bytes /147
instructions. Raw SHA-256 is
d7f2520f6089890a0455fac61c5a49bb158f3e5a6106cbfdb5b064855c820300.
The remaining selected-character load/widen/push schedule differs. Shared real
integer lifetimes with/without a char are byte-neutral and discarded; read
Packets 643/726 before repeating staging, scope, pointer, width or profile
controls. No new exact credit or full-function CFG agreement is claimed.

Reproduce with `python3 scripts/build-match-unit.py --unit title-screen-replay-menu`
for the shared O1 carrier, then
`python3 scripts/inspect-title-character-selection.py build/matching/TitleScreen.obj --result`.
The diagnostic reports actual paired jump destinations separately: baseline
cancel conflict is gone after repair, but one destination is unpaired. Four
new target-independent tests guard that diagnostic, not target exactness.
All 35 O1 accepted neighbors plus separate O2 score-record-insert replay exact
after proved spelling-only refresh of nine private StartMenu labels. No
full-repository cold cohort runs. Ordinary selection keeps two SIB differences.
Owned result probes/objects/PDBs are removed after durable recording. Totals
remain 893 exact /180366 of275669 authored bytes (65.43%), with 86 nonexact
/95303 bytes. The >95% goal and native/runtime/semantic/port gates remain open.
Packet 727 closes the replay-save owner below; the result frontier remains open.

### Latest replay-save complete exact closure

`TitleScreenView::UpdateReplaySave @0x00429662..0x00429D82` is canonical
exact: 1825 contiguous code/physical bytes, 521 instructions, frame 0x154 and
100 fields (59 REL32 /41 DIR32). Packet 727 replaces both unresolved
MoveTwoChoiceCursor calls with the already maintained MoveCursorHorizontal
@0x00424601 and removes that unused declaration. A genuine cancel-to-overwrite
label recovers target input precedence and shared interrupt tail; a shared
function-scope replay index and natural three-stream for loop recover all
target stack homes. No byte masks, padding, ABI/layout changes or profile
search are retained. Network saturation still writes file 1000; slot loading
clears 50 records but loads 25. Stage-4 type/cancel checks remain independent.

Final probe, two independent maintained-source cold objects and canonical
carrier agree on every raw byte and relocation record. Raw SHA-256 is
c40d0e6855c4293bc12aa8e7f45526202f1f1b02bc8f5cb9c000443a8f8bbb73.
Reproduce with `python3 scripts/build-match-unit.py --unit title-screen-replay-save`
and `python3 scripts/compare-coff-function.py --unit title-screen-replay-save --json`.
The read-only diagnostic supports `OBJECT --replay-save` without granting credit.
Replay name binds 0x004A8174; name-table field view binds 0x004A8398;
alphabet is loaded through the pointer at 0x004A1CF4. These are operand/view
facts, not new global data definitions or unique original ownership claims.

All 35 prior O1 owners retain raw bytes and relocation geometry; nine private
StartMenu labels are refreshed only after same-owner section/relative-offset
and unchanged-destination proof. Fresh O1/O2 carriers replay all 37 affected
exact units. No full-repository cold cohort runs. Ordinary selection's two
SIB differences and result browser's selected-char frontier remain unchanged.
Owned replay-save scratch is audited and removed after durable recording.
Packet 727 totals were 894 exact /182191 of275669 authored bytes (66.09%), with
85 nonexact /93478 bytes. The >95% objective and native/runtime/semantic/port
gates remain open. Packet 728 subsequently reviews MusicRoom below without
new exact credit. Packet 729 reviews OnUpdateOptions below without new exact
credit. Packet 730 substantially narrows DrawResult below without new exact
credit; Packet 731 recovers the complete keyboard loop and leaves only the
initial bank/currentScreen schedule, still without exact credit.

### Current DrawResult handoff

`TitleScreenView::DrawResult @0x00423D16..0x004244A8` remains non-exact.
Packet 731 supersedes Packet 730's keyboard frontier. The complete target
remains 1939 bytes /577 instructions /16 calls /frame 0x44. Do not resume
from the old size-only or broad allocation diagnosis.

The retained correction moves the real Float3 characterPosition from
function scope into each keyboard column iteration. Together with Packet
730's separate ranking row/active lifetimes, this recovers the complete
keyboard loop without register hints, padding or manufactured operations.
Two independent affected-TU cold objects agree on every raw byte and
relocation record: 1939 bytes /577 instructions /82 fields (16 REL32 /66
DIR32), SHA-256 2ceff58b20078ad74007269588a377a0b69c99527fc3345978dc1c88e3eb172b.
All 92 physical-order direct blocks agree in edges, per-block calls and
return cleanup. Normalized alignment is 574/577, with one unpaired
correspondence destination. Complete replay still differs in 22 bytes.
No match-unit or exact credit is added.

The only remaining byte frontier is the initial bank/currentScreen
schedule: target stores the group at EBP-8, then loads currentScreen into
EAX; compiler hoists the load into ECX before bank multiplication and
compares ECX against 13/14. Last difference is 0x00423D4F. The complete
1881-byte suffix 0x00423D50..0x004244A8 agrees after independent relocation
binding, but this is diagnostic only, not partial match credit.

Packet 731 records controls, including actual Ascii helper visibility,
screen snapshots, phase-local glyph lifetime and direct fade/color reuse.
Snapshots undo the recovered keyboard. Direct fade/color reuse fixes the
prefix but introduces memory shifts and a six-byte excess; neither signed
nor unsigned color closes it. Do not repeat these blindly or infer original
TU/header ownership from helper adjacency. Keep genuine value/lifetime
hypotheses distinct from compiler observations and original-source unknowns.
Packet 744 freshly confirms the complete baseline and target prefix. Treating
nameBankIndex as unsigned leaves function bytes and relocation records
unchanged; a genuine 13/14 `switch` emits 1937 bytes and the wrong initial
branch graph. Neither addresses the target's post-bank EAX screen load. The
old 1941-byte IDA comment is corrected and read back. Reopen only with new
TH09-local value/TU evidence, not those two source forms.

Reproduce only this TU with
`bash scripts/compile-probe.sh src/TitleScreenResultDraw.cpp build/result-draw.obj /MT /EHsc /Gs /DNDEBUG /Zi /Gy /GF /Gr /O1 /Ob1 /Oy- /I src`,
then `python3 scripts/inspect-title-result-draw.py build/result-draw.obj`.
The read-only diagnostic binds all 22 reviewed destinations and checks
complete decoding/CFG without canonical acceptance. Generic comparison
correctly reports mismatch (1593/1611 non-relocation bytes), not exact.
Owned probes/objects/PDBs are audited and removed after durable recording.
No other TU or repository-wide cold cohort is rebuilt. Coverage remains
894 exact /182191 of275669 authored bytes (66.09%), 85 nonexact /93478 bytes.
Packet 732's genuine raw/packed-alpha, shared integer and 32-bit type controls
are byte/record-neutral; narrower alpha and earlier default regress. Reopen
this initial frontier only with new target/TU/value evidence, not repeated
controls. Next rotate to DrawReplaySave at 0x004239F6..0x00423D15 (800 bytes)
with fresh complete target/current baseline review before applying the shared
keyboard/position hypothesis. Packet 733 below completes that fresh review.
The >95% and later gates remain open.

### Current separate frontier: DrawReplaySave value lifetimes

Packet 733 supersedes this owner's old 783-byte candidate, not its NON-EXACT
classification. The complete target at 0x004239F6..0x00423D15 is 800 bytes,
245 instructions, frame 0x34 and 28 direct blocks. Maintained source now uses
the real Float3, direct AsciiManager access, separate X/Y offsets, a genuine
reused interpolation workspace and phase-reused row index. The actual text
buffer stays two bytes; no private position reinterpret cast or inflated
buffer is retained.

Two independent affected-TU cold builds agree on all 793 raw bytes and all
41 relocation records (4 REL32/37 DIR32), SHA-256
a4f0be0320e7b60e07fb2f192a2b2bec9cb21d70de7a8114613fee7851f7e979.
Frame 0x34 and EDI replay/ESI manager/EAX list increment are recovered, but
main XYZ is EBP-1C/-18/-14 rather than target -18/-14/-10. Text/scalar homes,
list valid-arm physical placement, first weighted-Z FPU operation and glyph
scheduling remain different. Complete graph agreement is false at 28/28
blocks; normalized alignment 164/245 and all four call destinations agreeing
are diagnostic only. Size delta -7 is not a seven-byte remaining frontier.
No exact or partial credit is added; coverage remains 66.09%.

An enlarged 16-byte text probe recovered more homes, but target observations
prove only two accessed bytes, not capacity. It is rejected as unsupported
stack compensation; do not resume from its apparently narrow differences.
Other negative controls and original-source unknowns are in Packet 733.
No helper call is spuriously bound or forced inline.

Reproduce only this TU with
`bash scripts/compile-probe.sh src/TitleScreenReplaySaveDraw.cpp build/replay-save-draw.obj /MT /EHsc /Gs /DNDEBUG /Zi /Gy /GF /Gr /O1 /Ob1 /Oy- /I src`,
then `python3 scripts/inspect-title-replay-save-draw.py build/replay-save-draw.obj`.
The read-only complete diagnostic independently binds 23 reviewed destinations;
runtime replay-time view 0x004AC879 is not a PE literal or proven data owner.
All 35 source probes/84 objects and PDBs are audited and removed after durable
recording. Legacy ignored artifacts/private inputs remain untouched. Continue
from the actual maintained two-byte source, not size/alignment metrics alone.

### Latest gameplay-worker caller-contract repair

Packet 734 re-reviews all 1689 target bytes of GameplaySetupThread at
0x0041AF2D..0x0041B5C5. The initial-load release call reaches the full
ReplayManager release 0x00420AD0, not the alternate ReleaseStageObject
0x00420C10. Maintained source now uses a bounded member-call view; the
DeletedCallback's real alternate-release call remains untouched. Viewport
initialization uses actual Supervisor::InitializeViewports, and Player ANM
preload uses its actual int contract with observed ignored result. Native
symbol/layout ownership and runtime validation remain open.

Two independent /O2 /Os /Ob1 cold builds agree on 1689 bytes /416 instructions
and all 171 relocation records (33 REL32/138 DIR32), raw hash
35d7f3adf469e584e8adf9f1714914db0d4c2d7d7c8dba19bbb21d6aabcc70a1.
The target is 413 instructions. All 35 ordered calls, including two separately
reviewed IAT cells, now agree. Complete replay still has 921 differences;
physical-order graphs disagree at 78/77 blocks. Generic 402/1005 ordinary-byte
agreement is unchanged and previously masked the wrong release contract.
No new exact or partial credit is added; coverage remains 66.09%.

Reproduce only this TU with
`bash scripts/compile-probe.sh src/GameManagerSetup.cpp build/gameplay-setup.obj /MT /EHsc /Gs /DNDEBUG /Zi /Gy /GF /Oi /Gr /O2 /Os /Ob1 /Oy- /I src`,
then `python3 scripts/inspect-gameplay-setup.py build/gameplay-setup.obj`.
The diagnostic deliberately binds old ReleaseStageObject to its real alternate
entry and rejects unreviewed calls/fields. IAT cells are not assumed runtime
callees. Five new strict call-identity tests bring CI to 30 tests. Eight /Os
GameManager neighbors plus the non-/Os loading/capture neighbor replay exact.
Two new phase-index/rate-tail probes do not close the base-cache, flags-cursor
or cap-tail differences; see Packet 734 before repeating them. Packet-owned
scratch is audited and removed, preserving legacy/private state. Rotate
coverage or reopen only with new target/value/TU/topology evidence.

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
Packet 742 additionally tests a genuine shared-body label and short-circuit
`mode0 || mode1` source against fresh target CFG evidence. Both compile to the
target's 1491-byte extent and retain independently exact prefix/tail regions,
but put the mode1 test *before* the shared body instead of target's late
backward branch. Complete comparison fails (1011/1063 and 1001/1063 ordinary
bytes respectively); equal extent is not an exact owner. The diagnostic replay
script now supports both the maintained 1510-byte and these 1491-byte shapes.

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

Packet 740 replays the Packet705 world-result lifetime hypothesis against the
current Packet707 maintained source, not the stale historical probe. Baseline
remains14788/14792 logical bytes; a genuinely used by-value world result plus
pre-copy secondary-timer pointer acquisition emits14792/14792,frame0x168,
375 direct calls,598 relocations and all193 table entries. Two cold positive
builds have identical raw bytes and complete records (raw SHA256
81eec740e1db84c8edac46b5239e6363194b7a57e30bb8033677b3b3789f5030).
Per-handler normalized alignment rises from3881 to4144/4260 target
instructions, and 21/23/24 lengths close; only opcode155 retains44/45
physical bytes. Whole-owner normalized alignment rises from3936 to4253,
but the probe's first Float3 result home is EBP-0x10C versus target EBP-0x168,
with a different returned-value copy. It is NOT an exact or accepted source
change. Const-reference/direct-initialization and combined TH08-style bitfield
controls are raw-byte neutral. The current positive source and one object/PDB
pair are retained under `.analysis/ecl-core-740/` and
`build/verify-ecl-core-740/`; see Packet740 for reproduction and limits. The
stale IDA entry comment is corrected/read back. Next find a target-backed
lifetime that preserves the first return object while improving dispatch
handlers; do not infer exactness from equal length or normalized alignment.
Packet 745 closes a narrower scope question: ending the by-value result's
lexical scope immediately after the world-field copy, while retaining the
timer pointer across dispatch, compiles byte-for-byte and record-for-record
identically to Packet 740's positive probe. It still uses EBP-0x10C and stack
loads, not target EBP-0x168 and returned EAX. The tracked RunEcl diagnostic
now prints the first result home/copy source and full physical-byte SHA256;
four target-independent tests guard its fail-closed pattern recognition.
Do not repeat brace-only scope changes. A different real value/alias lifetime
must explain the target first copy and downstream handlers together.

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
Packet 729 freshly confirms 2045 contiguous code/physical bytes, 551
instructions, no frame locals/tables/padding, and the immediate KeyConfig seam
at 0x00427EE8. Maintained C++ is unchanged: unsigned help index, explicit option
wraps, four volume-key switches, 6/7/8 confirm switch and inclusive timed-sound
range stay intact. The target really omits SFX ones-digit VM32 SetSprite.

Maintained cold baseline and fresh canonical carrier agree on complete 2048
raw bytes, 554 instructions and all 135 relocation records (51 REL32 /84 DIR32),
hash a521dbdff6c30e098a38efca6a06b54d96aa3708f5750ace4df7dd979b483115.
Reproduce with `python3 scripts/build-match-unit.py --unit title-screen-play-menu-sound`
then `python3 scripts/inspect-title-options.py build/matching/TitleScreenOptions.obj`.
The diagnostic independently binds all 30 actual symbols and decodes the whole
owner. Similarity gives 540/551 with five unmapped destinations; separately,
all 121 direct basic blocks agree in physical-order edges, per-block call
sequence and return cleanup. This graph covers jumps without guessing missing
similarity correspondences; it proves neither predicates/data flow nor bytes.

Residual register lifetime still begins at right-scroll return +0x540: target
zeros EBX before testing AX, while candidate zeros it after the branch and later
uses EDI for zero /EBX for selector6. The first actual byte difference is already
at +0x15, an end-tail forward-jump displacement; there is no exact-prefix claim.
Packet 662's if-chain/normalized-selector controls remain rejected. Packet 729's
in-case exit label, real PlayMenuSound-before-caller and actual selection/helper
wrapper visibility are raw-byte/record neutral. Input/config-owner views only
change field bindings, not complete relocated code. Exposing the actual exact
42-byte Input scrolling body emits 2046 bytes /50 calls /134 fields /120 blocks,
coalescing a target-distinct selection call; it is rejected despite size proximity.
Do not repeat these context/alias controls without new target evidence.

PlayMenuSound remains 34/34 exact with all three fields. Eight target-independent
tests guard the added direct-CFG utility, alongside the four old correspondence
tests. All owned disposable probes are removed after durable recording. No
source edit, new match entry or exact coverage gain is retained; 894 exact
/182191 bytes (66.09%) and the >95% objective remain active-incomplete.

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
with 2,254/2,258 bytes under pinned `/O1 /Ob1 /Oy- /Gr`. Packet 728's fresh
complete target review fixes two actual old fidelity errors: both selection
calls reach canonical `SetRangeSelectionInterrupts @ 0x0042510E`, not
SetMenuSelectionSprites; the locked format at 0x0048F77C is `%5s ` followed
by eight CP932 fullwidth question marks, not the old shortened `%5s `.
Maintained ASCII escape literals compile to the exact 21-byte string including
NUL. The helper sets range pending interrupts 8/7, not sprites.

The target-backed list recovery keeps the song/script index absolute at
159+ in EBX while a separate zero-based VM cursor advances by `0x2A4`.
Maintained source now models that second induction explicitly, so VC7.1 emits
the target `vms + cursor + 159*0x2A4` address family, adjusted unlock-table
indexing, and the target stack-held track/Y cursors. An explicit flags-byte
pointer restores the target `LEA flags` followed by `OR byte ptr [ptr+1],18h`.
In both ready/init visibility refreshes, assigning `i = musicListingOffset`
before computing the visible bound restores the target cursor/bound register
roles.

Explicit assignment then separate cursor/index advances recover all three
parser-copy schedules. Pause uses the target physical Pause-first arms; the
song loop has genuine script-index, descriptor and VM-index streams in its for
header. Branch-local parser index remains unchanged. Shared parser index,
shared visibility bound, using that bound as the hidden counter, shared VM
index and a for-header unlocked pointer are byte/record-neutral controls;
do not repeat these declaration/scope variants without new evidence.

Two independent cold objects and the canonical carrier reproduce every raw
byte and every offset/type/symbol/addend record: 611 instructions, 74 fields
(25 REL32 /49 DIR32), raw SHA256
31af7d69f5acf79e25bdd9f6186932e62abef8bb8ac573197e7180b4b193d605.
Reproduce with `python3 scripts/build-match-unit.py --unit title-screen-draw-music-room`
then `python3 scripts/inspect-title-music-room.py build/matching/TitleScreenMusicRoom.obj`.
This diagnostic binds all independently reviewed destinations, fails closed on
unknown fields and decodes the full owner. All 25 calls agree in order;
normalized alignment is 600/613, paired CFG conflicts and unpaired destinations
are zero. Normalization is routing evidence, not exact acceptance.

Remaining differences are the two final hidden-loop register copies/cursor
roles, song-list initialization schedule, unlock-lookup SIB order and VM-index
increment schedule. The four-byte net gap is not a four-byte exactness claim.
`DrawMusicRoom` in the same TU still cold-replays 209/209 exact with all six
fields; no match entry is added. Owned disposable probes are removed after
durable recording. Coverage remains 894 exact /182191 bytes (66.09%), with
85 nonexact functions. The >95% objective and all later phase gates stay open.

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

Packet 741 adds a complete, independently TH09-bound physical diagnostic at
`scripts/inspect-etama-update.py`. Two cold maintained-source builds agree on
all2,216 raw bytes and88 full relocation records; raw SHA256 is
0f218aafc048890160aeef8ee02468ded5db7a5bbc83ac5802de942334572e88.
The 2,195-byte body has553 decoded instructions,62 direct calls,60 conditional
and18 unconditional jumps in both target and candidate. All33 direct callee
identities, global/float operands and six compiler-private destinations are
target-bound. After complete relocation, exactly43 bytes differ at relative
`+0x27..+0x52`; the entire `+0x53` physical suffix, including alignment and
five table entries, agrees. This is diagnostic only: no prefix/suffix or
partial exactness credit. The live row's older 19-versus-18 JMP statement is
corrected. IDA's stale 2,256-byte Packet181 entry comment is corrected and
read back.

Source controls with a function-scope bullet index, chained counter zeros,
broader real bullet/laser pointer or collision-result lifetimes and actual
SelectSide body visibility are byte-neutral. Sharing the counter-zero value
with the bullet loop index moves the EBX save but also moves zeroing before
the target's flag test, or creates an extra index store before the counter
stores; it is not a closing form. Do not repeat these variants without new
source/TU evidence. Reproduce with
`bash scripts/compile-probe.sh src/BulletManager.cpp build/etama-update.obj /MT /EHsc /Gs /DNDEBUG /Zi /Gy /GF /Oi /Gr /O2 /Ob1 /Oy- /I src`
and `python3 -B scripts/inspect-etama-update.py build/etama-update.obj`.
Focus next on a natural reason VC7.1 saves EBX before SelectSide and uses it
for exactly three post-call zero stores, without shifting the already-exact
physical suffix. No maintained source or canonical match changes.

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

All 2,192 tracked candidates have boundary/origin review. Current dispositions
are 980 authored, 1,177 excluded, and 35 deliberately unresolved. This is a
tracked-candidate statement, not a proof of a complete executable census. The
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
