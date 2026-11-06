# TH09 reconstruction — current handoff

> **Current next-agent restart guide (2026-11-04):**
> [TH09_NEXT_AGENT_HANDOFF_20261104.md](TH09_NEXT_AGENT_HANDOFF_20261104.md)
> supersedes "current/latest" claims in this dated research history and
> the [2026-11-02 handoff](TH09_NEXT_AGENT_HANDOFF_20261102.md).
> In particular, Player Movement is **122** unlike bytes, not the historical
> **129** in the earlier 2026-11-02 section below. The live Git tree,
> ledgers, source/PE and scoped compiler replay remain authoritative.
> Sections below describe **their own historical source checkpoints**;
> do not interpret old names or private class layouts as current.


## 2026-11-06 continued main — Natural network checksum and wait-flow review

The service checksum now names the wire input word. The old high checksum word
contributes zero to the stored sum modulo65536; complete1627 raw bytes and106
referent identities remain unchanged. Original1633-byte comparison stays1381
unlike overlaps plus six missing bytes. See
[the source/flow review](TH09_NETWORK_CHECKSUM_AND_FLOW_20261106.md).

Fresh Ghidra attests service/receiver/all mapped bytes. The receiver copies12
wire bytes and queues input/seed without a checksum comparison in that branch.
Only physical call positions25..30 differ in the baseline order; branch-arm
swaps and two structured wait returns preserve the whole baseline object.
Complete-frame aggregates and earlier success blocks remain nonexact. Genuine
visible GetSeed4/range34 remain exact42/1 across three comparisons, but remove
one frame-field reload from the caller, yielding1622/105/1500 plus11 missing.
No ownership/qualifier inference or non-checksum source model is adopted.

Fourteen fresh build groups and four unchanged scoped proofs pass20 complete
exact comparisons3767/131, including all19 direct callees and receiver808/46.
57 focused tests plus tracking/progress/whitespace pass. Source200591bd binds
37 current inputs/Git blobs and13 provider files; source-proof SHA-256
`ae9d683f2d9a940090d9415071ed9e5ad9dfbe40437f1db55e2ea12606acde62`.
Control seal `c0d035244de5e27737d65c7c6780a9c010a4c153384a86c70ce944926ff2fa64`
passes independent read-only replay. A disposable seed/range-visible context
rebuild reproduces1622/105 and both genuine callees.146 hashed source copies
and25 PDBs2060288 bytes are removed; all24 new objects remain. Counts remain
1021/13 and the reconstruction goal stays active.

## 2026-11-06 continued main — Shake natural phase models remain unresolved

Three current-native models remove the old volatile reference construction.
Ordinary and typed-parameter phases both produce334/20 with249 unlike overlaps
and two missing bytes; unsigned final endpoint produces339/20 with249 unlike
and three extra. All20 referents,eight ordered calls and26 blocks agree, but
none preserves the maintained310 equal positions. No source or qualifier is
adopted; original C++ volatility remains unknown. The current owner stays
336/20/26. See [the complete review](TH09_SHAKE_NATURAL_PHASES_20261106.md).

Fresh Ghidra attests both reviewed owners and all mapped bytes.70 complete
comparisons7293/397 cover48 experimental siblings,18 unchanged canonical
owners and four actual Timer/RNG callees. Seal
`2bda1cc0aa0f4cca878046dfb8ee1c3eae5a6c80bdcd75461394be12a9a36165`
passes independent read-only replay; a disposable typed-parameter rebuild also
reproduces full raw bytes/COFF and16 siblings.45 hashed copies and four
PDBs524288 bytes are removed; all four new objects remain. Game source stays
7e293460;1021/13 and the active goal remain open.

## 2026-11-06 continued main — Natural RunEcl wire snapshot adopted

Opcode4 now snapshots the existing four-byte wire argument and assigns its
integer member to the real context Timer. Scalar memcpy transport and its
codegen commentary are removed. Complete canonical RunEcl stays15564/598/30
with no field/table differences; complete Enemy stays3900/97/13. Six affected
build groups pass91 exact comparisons36301/1783;29 unchanged source/profile/
object-bound callees2027/71 are reused. The alternative /Ob0 carrier loses one
noninlined RawInt call (11980/649); its six exact leaves remain exact. See
[the source/control review](TH09_RUNECL_WIRE_SNAPSHOT_20261106.md).

Eight natural controls preserve616 repeated exact siblings194944/8768. Real
co-visible Timer definitions remain exact43/2 across three comparisons, but
revealing the setter changes the full caller to15560/598 with6230 unlike
overlaps plus four missing bytes. Public shape guards continue to reject it;
no Timer-definition relocation is adopted. Current counts stay1021/13 and
the goal remains active. This is natural source progress without new byte
exactness or product/runtime credit. Source7e293460 binds90 current inputs,
six actual objects and91 comparisons; source-proof SHA-256
`3d2ab9c3c0bcf48e150af77c55d2c822d7be4a7a744703a0b45e840f2bd6f9fe`.
Control seal `a61bac1fe402aa495008476cd2ea93bf9bfc2e412fa790a3eb9491347cf7a57a`
replays91 current,29 reused,616 experimental sibling and three actual Timer
comparisons read-only.680 hashed duplicate sources and14 PDBs2891776 bytes
are removed; all14 new objects remain. Full public CI passes1377/29.

## 2026-11-06 continued main — Effect context and genuine callee visibility

Nine successful current-native contexts preserve135 exact Effect siblings
22770/900. Receiver/cast/early-return/typed-owner variants retain8 or11 unlike
bytes. Actual GetAnm definitions match20/0 while their callers shrink45/4 with
42 unlike overlaps plus six missing bytes. Actual PreloadAnm90/4 preserves
51/4/8. The first Preload setup misses windows.h; its complete-header model
recovers the same real body. No source or interface is adopted. Original
Create+0xC0 stores owner into ChainElem+1C and+0xC6 stores40D1C0 into+8;
actual callback pointer operand is+0xC9. See
[the visibility review](TH09_EFFECT_CONTEXT_VISIBILITY_20261106.md).
Fresh Ghidra attests callback/creator/all mapped bytes.36 unchanged comparisons
5543/206 and three actual producer comparisons130/4 remain separately scoped.
Seal `553a9bb5306dd3ae144f395890580f294618efead110d437279f843dd0f66f73`;
178 hashed source copies and ten PDBs are removed; all nine objects remain.
The earlier Setup packet additionally removes ten PDBs1818624 bytes and keeps
all ten objects. Neither batch adds exact credit:1021/13 and the goal stay open.

## 2026-11-06 continued main — Shared Setup rate context remains unresolved

Ten current-native contexts test early exits, shared publication, nested/ordered
dispatch and an isolated actual caller. None closes the full worker; isolated
compilation retains every1682/172/694-plus-seven-missing byte/field. The real
counter initializer emits five extra calls and fails original binding. Nine
whole-TU contexts preserve90 complete siblings24993/2034; unchanged current14
proofs3480/286 reuse verified source657e9699 inputs/objects. See
[the bounded contexts](TH09_SETUP_SHARED_RATE_CONTEXT_20261106.md).
Seal `566a400d36a806dda4ff94863329f6aac2e5deb8a66614c725659c0445465519`;
390 hashed copies are pruned; all ten objects remain. Fresh Ghidra attests all
mapped bytes. No source/profile/interface is adopted;1021/13 and the goal
remain open. Do not infer original switch/source spelling from the decompiler
or select a longer candidate solely for fewer overlapping differences.

## 2026-11-06 continued main — Native Setup source recovers all original field roles

The maintained worker now uses native side/runtime fields and structured rank
publication. Complete1682/172 compares694 unlike overlaps plus seven missing
original bytes; all172 referent multiplicities and35 calls agree. Both flows
have413 instructions/78 blocks, with412 instruction correspondences. The
remaining shared C8=22 block is original+431 versus candidate+522. This is
nonexact:988 physical positions match, versus785 before;249 prior positions
are lost and452 gained. The new guard accounts for every missing byte and
protects all988 equal positions. See
[the native rank checkpoint](TH09_SETUP_RANK_PUBLICATION_20261106.md).

The coherent actual int Player declaration/definition preserves328/29 and
original whole-ECX caller zeroing; unsigned-char also matched the producer,
so original formal width stays ambiguous. Three fresh affected profile builds
pass14 complete canonical comparisons3480/286. All25 unchanged regular
body/profiles6742/546 and the actual EH graph are preserved. Ten bounded
rank-slice traces retain every ordered rank/player write; this is not product
runtime validation. Exact totals stay1021/13 and the goal remains active.

Source `657e9699`; source-proof SHA-256
`8023ad887bc478521658b77fa3e516a401214626e6d228e3796a1016c2940f93`.
It binds60 tracked inputs/Git blobs,eight compiler/SDK files and all eight new
objects. Independent14-unit plus50 experimental sibling replay passes17365/
1416, with no new function credit. Full CI passes1376/29;26 focused guard and35
status tests pass.195 hashed source duplicates and eight PDBs are removed.
The complete no-/Os worker remains1828/189/1592 plus139 extra bytes, nonexact.

## 2026-11-06 continued main — Native Setup access isolates remaining rank differences

Ten current-native source/profile controls preserve all ten exact Setup
siblings each. Direct native field access restores the original root reloads
and side flag anchor; retaining the initial Setup sequence yields1697/174,
698 unlike overlaps plus eight extra bytes and two extra rank-limit stores.
It loses243 prior equal positions, so it is not adopted. The real byte/int
Player producer pair both matches328/29, including actual EH identity/data;
its wider caller still retains904. The byte interface remains maintained.
Fresh Ghidra attests worker/reset/Player and all mapped bytes. See
[the native access review](TH09_SETUP_NATIVE_ACCESS_20261106.md).
Sixteen actual models pass106 complete sibling comparisons28748/2346;
seal `0db217584dc2b1ab0b0ebf50b4f0def4abd316e33f2b3ca0cb7dd79f89cc2008`.
704 hashed copies and11 PDBs are removed; all16 objects remain. Code stays
398b69e4 and1021 exact/13 nonexact. Continue from native target/source
questions; reference code supplies natural source-shape ideas. The goal
remains active.

## 2026-11-06 continued main — Playback setter width is not a unique source type

A coherent unsigned-int SetSeed declaration/definition matches all14 original
bytes in both existing Replay profiles, as does the maintained unsigned-short
interface. The original word access does not uniquely determine formal width.
Both wider-setter callers and read-only frame-header callers retain every
538-byte raw instruction and32 identities:39 unlike overlaps plus two missing.
Six actual objects pass28 complete sibling comparisons8225/464, with no private
label changes. Fresh Ghidra covers caller/setter/creator and all mapped bytes;
original4216F5 independently records the installed420840 callback. No source or
interface is adopted. See [the width review](TH09_REPLAY_SEED_WIDTH_20261106.md).
Seal `04cd6f55ae6aafe0f9971de6f2e088d45774a79932791ae179ae829b015f19f2`;
23 hashed copies are pruned. Code remains398b69e4 and1021 exact/13 nonexact;
the goal stays active. 35 focused tracking/progress/origin/documentation tests
pass. Nine probe PDBs across the Replay and Title packets are removed by
path/SHA; all nine new objects remain.

## 2026-11-06 continued main — Title result tests actual caller/callee scope

The complete pinned reference is483/34/224 plus two missing bytes. Removing
the inactive/all cursor definitions from the current caller preserves every
484-byte raw instruction and all34 fields; the separate actual175-byte cursor
still matches. A signed-byte declaration/definition pair instead produces a
488-byte caller and176-byte cursor; the actual callee fails60 overlaps plus
one extra byte. No source or narrower interface is adopted. Fresh Ghidra
attests both entries/all mapped bytes;44 correctly source-scoped unchanged
comparisons19583/978 pass without compiler jobs. See
[the caller/callee review](TH09_TITLE_RESULT_NATIVE_ANM_20261106.md).
Seal `84e46bf0dc4986441b64a874bd3e0732d8112b35dccad8a059ba89e906c6d179`;
60 hashed duplicate copies are pruned. Source stays398b69e4 and1021 exact/13
nonexact; the goal remains active.

## 2026-11-06 continued main — Seed operations use the native RNG owner

Source `398b69e4`; immutable proof SHA-256
`5e15c80d2e1ce618e6916f01915bb2c834c3e3505282eb301a6cbd6b43e3b443`
binds196 tracked inputs/Git blobs, eight SDK files and60 actual objects.
Independent complete read-only replay passes; no compiler jobs are repeated.

Replay, Network and startup now use RngRuntimeView for seed get/set/count reset;
the duplicate Replay RNG layout and casts are removed. Fresh Ghidra attests the
4/14/8-byte leaves and all mapped file bytes. 58 affected source/profile builds
pass241 complete exact comparisons91667/4878; a separate canonical leaf build
passes four comparisons39/0. The eight affected nonexact owners retain every
current frontier. 533 private field spellings refresh only at independently
verified unchanged destinations. Full CI passes1376 tests/29 skips;21 focused
tests pass. All524 reviewed prior body/profile contributions are accounted for,
including current sealed Setup carriers and actual private EH definitions.
59 disposable PDBs are removed by path/SHA; all objects remain. See
[the native owner review](TH09_NATIVE_RNG_SEED_OWNER_20261106.md).
Authored totals remain1021 exact/13 nonexact; native product/runtime and the
goal stay open. This checkpoint's source hashes/objects live in
`.analysis/rng-native-seed-20261106/`.

## 2026-11-06 continued main — Complete Network reference and word-width countercheck

Complete pinned `app_network.cpp` yields 1610 bytes / 106 fields, all bound,
with 1478 unlike overlaps and 23 missing original bytes. Its three literal
payloads agree; its whole flow remains nonexact. The paired native word-width
producer again matches131/1, while its service caller is1629/106/1388 plus four
missing bytes. This confirms the earlier width ambiguity; no interface or
source is adopted. Fresh Ghidra covers service/input/queue with all mapped
bytes attested. Seal:
`adf4581b2d28eef2eb80643a7453e213019ef8b4c679952abcd28b880fe323b5`.
See [the updated frame review](TH09_NETWORK_FRAME_CONTEXT_20261106.md).
Authored totals remain1021 exact/13 nonexact; the goal stays active.

## 2026-11-06 continued main — Complete Enemy reference rejects direct substitution

Two complete pinned-reference Enemy builds, before/after correcting its speed
constant to the original 1.0f, retain 3872 physical bytes / 96 fields. All actual
fields and five literal definitions are checked; each differs at 3297 overlaps
with 28 original bytes missing. Its movement branches share one vector-add
call, producing 68 calls versus the original 69. No source is adopted.
Fresh Ghidra attests all 685056 mapped bytes; unchanged native source/SDK/object
proofs and strict Enemy3900/97/13 plus RunEcl15564/598/30 replay. 157 hashed
duplicates are pruned. Seal:
`b9f2730ab6f6f93bdbd6562c47451991d286da4fa5057e259bbf7ff220cb23cd`.
See [the complete reference review](TH09_ENEMY_COMPLETE_REFERENCE_20261106.md).
Authored totals remain 1021 exact / 13 nonexact; the goal stays active.

## 2026-11-06 continued main — Natural RunEcl reference blocks and whole contexts

The actual reference opcode4 block reproduces29 original bytes. Opcode155
produces the same45-byte instruction sequence, with40 equal bytes before its
address-dependent jump, but sits two bytes early. Complete reference binding
now covers all598 fields/six literals and still differs4149 physical bytes.
Seven reference refinements and a termination-order control retain that whole
candidate. Lexical and native-type contexts regress or fail the strict
shape/identity gate; no source is adopted. The maintained owner remains30.
Fifteen frozen contexts/sixteen objects and a restored complete carrier are
sealed by `3511c6bc2878f43b41f4786a404abc0c7a5c79a8f6e0d1908192ab63366c4452`.
112 unchanged code-unit proofs31883/1460 replay without compiler jobs;1277
hashed duplicate copies are pruned. See
[the complete-context review](TH09_RUNECL_NATURAL_REFERENCE_CONTEXT_20261106.md).
Current authored totals remain1021 exact/13 nonexact; the goal stays active.

## 2026-11-06 continued main — Angle byte proof retained, origin returned to review

The actual pinned SDK atan2f and maintained VectorAngle wrapper independently
produce the same15 original bytes. This prevents inferring game authorship
from the existing compiler comparison. Origin becomes unknown/review; source,
matching100 codegen status and the runtime-vector-angle recipe remain intact.
Only authored mapping/name/match credit is withdrawn. Two fresh native units
pass126 complete bytes/six fields, and fresh Ghidra attests the original leaf.
Current totals are1034 authored mappings,1021 authored exact functions and13
nonexact functions, with38 origin-unresolved candidates. The98% gate remains open. See
[the origin review](TH09_ANGLE_ORIGIN_REVIEW_20261106.md).
The independently replayed proof SHA-256 is
`9bc0390a0c531971bf7b78cdb4d7daf2f68bd233dbc3452e564c9802a616ddb5`;
35 focused tests and the unchanged full RunEcl guard pass.

## 2026-11-06 continued main — Effect pre-call dataflow rechecked

Original and current Effect AddedCallback both retain ESI=context and EDI=ANM;
the old external register-swap comment is not current evidence. All eight
differences precede the first GetAnm call; bounded static transfer agrees at
that call, and the complete37-byte suffix is equal. Native member/free helper
contexts preserve51/four/eight and all15 Effect siblings; neither is adopted.
Two native callee carriers plus the baseline pass36 strict complete comparisons
5543/206. Reviewed callback metadata is read back after mapped-byte pre/post
checks.19 source inputs/six objects are sealed by
`a81efd28bd9ce66f2061c039284d4010721ca4067292907e5aef8e38668366eb`;
54 duplicate copies are pruned and a restored full carrier reproduces its
raw/fields and15 siblings.14 focused tests pass. See
[the original-bound entry review](TH09_EFFECT_PRECALL_DATAFLOW_20261106.md).
Game source remains `1d3a23c1`;1022 exact/13 nonexact and the goal stay open.

## 2026-11-06 continued main — Setup uses its native configuration cell

Source `1d3a23c1`; sealed proof SHA-256
`1d58fdc50eaaa54b7949cf535f4061096aa18b6367148037250cc959951893ff`
binds59 tracked inputs/Git blobs, three objects/profiles and eight SDK files.
Independent source replay passes, as do five frozen controls and two restored
complete carriers.193 hashed duplicate copies are pruned; changed controls,
frozen inputs and actual objects remain.

Setup allocation/copy/reads and the deleted callback now share the typed
GameManager+E8 configuration pointer already consumed by Replay. Five actual
COFF alias changes retain the same4A7E78 destination and complete linked worker.
Three affected source/profile builds pass12 canonical comparisons3987/335,
including the Supervisor physical tail;40 private labels retain actual
same-owner definitions/original operands. Complete Setup remains1689/171/904,
with35 ordered calls and all785 previously equal positions protected. Five
complete controls retain this frontier;34 focused tests pass. See
[the source review](TH09_SETUP_NATIVE_CONFIGURATION_20261106.md).
No new exact credit;1022 exact/13 nonexact and the active goal remain open.

## 2026-11-06 continued main — Charge mode producer and caller audited

Original mode capture is configuration byte[side+B4] -> AL -> local[EBP-1],
with two later byte accesses. The extra-mode-argument hypothesis does not
explain that producer. Three current baseline/native-owner/const-side contexts
retain every1197 raw byte and70 COFF rows,1160 unlike overlaps and13 missing.
Each actual20-byte Timer >= body and five literals pass. No source is adopted.
The complete522/40 caller replays using17 unchanged source inputs and its
b9e1a95a object/profile/SDK proof. Its missing Ghidra function entry is created,
its physical ECX/EAX projection read back, and all685056 mapped bytes reattested.
See [the entry audit](TH09_CHARGE_ENTRY_MODE_AUDIT_20261106.md).
89 hashed duplicate copies are pruned; all changed/frozen inputs and actual
objects remain. Source stays36002b4b;1022 exact/13 nonexact and the goal remain open.

## 2026-11-06 continued main — Replay composition and Enemy controls

Three complete Replay member/base layouts retain1238/53/two, while every
actual18/15-byte constructor pair remains original-equal. Nine full comparisons
3813/165 do not identify original composition/inheritance or constructor origin.
Four Enemy descriptor/count contexts retain13 or regress214/2954 overlaps;
all complete RunEcl companions stay15564/598/30. No source model is adopted.
Both restored complete carriers reproduce all raw bytes/fields after hashed
copy pruning. See [the complete controls](TH09_COMPLETE_OWNER_LAYOUT_CONTROLS_20261106.md).
Canonical source and1022 exact/13 nonexact remain unchanged; the goal stays active.

## 2026-11-06 continued main — replay header names its serialized locations

Source `36002b4b`; sealed receipt SHA-256
`be937055150692469c349828bfa2a42738f8f4eaaeddf23a0baf6656dcf73584`
binds71 tracked inputs/Git blobs, ten objects/profiles and eight compiler/SDK
files. Independent source replay passes. Six frozen controls also pass;66
hashed duplicate copies are pruned, and a restored full control reproduces
all raw bytes/fields before its ten duplicates are pruned again.

SaveReplay writes32-bit offsets through shared frameOffset/fpsOffset storage;
the591-byte original-equal loader reads those words before rebasing the existing
pointer members. HeaderC0/record1EC and offset-table locations remain guarded.
Seven affected sources are covered by nine isolated canonical profile builds
plus the separate nonexact menu carrier.66 canonical comparisons26915/1472
and two origin-unknown constructors33/2 pass:68 total26948 bytes/1474 fields.
Nine private Title labels retain actual same-owner definitions/original operands.
Complete Save in both profiles remains1238/53/two; every affected nonexact
frontier retains its full prior raw body/effective fields.177 focused tests pass.
See [the source and original review](TH09_REPLAY_SERIALIZED_LOCATIONS_20261106.md).
No new authored exact credit; the active goal and13 remaining owners stay open.

## 2026-11-06 continued main — complete RunEcl residual sites re-investigated

Fresh target-attested Ghidra and nine fixed-profile complete source controls
retain the two-site frontier: opcode4 seven bytes, opcode155 twenty-three.
The six-entry interpolation table precedes the187-entry main table; handler
selection is now checked from actual same-owner COFF definitions. The two
copied-integer controls stay30; native byte-valued bitfields regress1938/202/26.
All nine complete Enemy owners remain3900/97/13. No source is adopted.
See [the full residual review](TH09_RUNECL_RESIDUAL_SITES_20261106.md).
The unchanged86-input ECL baseline/object proof is reused.763 hashed duplicate
copies are pruned, and a restored full carrier independently reproduces its
raw bytes and all fields. Effect callback metadata is also created and read
back after mapped-byte attestation; no target bytes change or exact credit follows.
The active goal and13 nonexact owners remain open.

## 2026-11-06 continued main — result update uses implemented ANM providers

Source `17a17736`; sealed local receipt SHA-256
`bf16e0b0de664f55f751772db031942fca3e6442d61dd52f2f4beaad86504c69`
binds40 inputs, four objects/profiles, eight compiler/SDK files and44 complete
comparisons19583 bytes/978 fields. Its independent replay passes. Six retained
full-source contexts independently replay their fields/literals/graphs.
116 SHA-identical staged copies are pruned; a restored native context retains
its entire raw body/fields, and its20 unchanged inputs are pruned again.

Result initialization now uses the real AnmManager surface/interrupt/script
providers and shared native VM interfaces.41 affected Title units plus three
actual providers pass44 complete comparisons19583 bytes/978 fields in four
existing source/profile builds. Nine private labels retain actual definitions
and original operands. Full result code stays484/485 bytes,34 fields,147
instructions,nine calls,40 blocks and218 unlike overlaps plus one missing.
Every raw byte/effective field and all266 equal positions remain protected.
Six source controls do not close the character-transport deficit; the175-byte
cursor helper keeps its full32-bit index contract. See
[the complete source review](TH09_TITLE_RESULT_NATIVE_ANM_20261106.md).
108 focused Title tests pass with eight skips. No new exact credit; totals
remain1022 exact/13 nonexact.
Full public CI passes1376 tests with29 skips; tracking/docs/progress/whitespace pass.

## 2026-11-06 continued main — complete Player speed contexts rechecked

Three SHT storage/read models retain the full movement raw body at1900/66/122.
The actual direction-offset consumer passes412 complete bytes/18 fields in
each context, including both switch tables and actual private definitions.
No model is adopted. Ten full-owner contexts across two preserved source epochs
and twelve callee comparisons1638 bytes/60 fields replay independently. See
[the bounded review](TH09_PLAYER_MOVEMENT_SPEED_CONTEXT_20261106.md).
149 hashed duplicate copies are pruned; a restored core context reproduces its
full caller, fields and three exact providers.

Source `b9e1a95a`; sealed local receipt SHA-256
`bc4d06b3b4f4940fb40095dc71378349ae7ed8ef9060741d41917f0fed4f9112`
binds104 inputs, four objects, eight compiler/SDK files and five canonical
comparisons674 bytes/42 fields. Its independent replay passes. Exact count
stays1022; the goal and13 nonexact owners remain open.

## 2026-11-06 continued main — Player movement uses implemented native math

Six bounds operations now resolve to the existing implemented Float3 addition/
subtraction owners, retiring declaration-only PlayerPositionView math seams
from this caller. Local storage projections preserve Player header/construction.
Complete movement stays1900 bytes/66 fields/23 ordered calls/122 differences,
with every raw byte, effective field and residual position unchanged. The
129-byte ledger prose is corrected. See
[the source and target review](TH09_PLAYER_MOVEMENT_NATIVE_MATH_20261106.md).
Two freshly rebuilt exact units and three unchanged source-bound math providers
pass five complete comparisons674 bytes/42 fields. All86 provider inputs and
the original object/profile/SDK proof are verified without rebuilding ECL.
Six native relocation-contract tests and50 focused movement tests pass
(two skips). No new exact credit; totals remain1022 exact/13 nonexact.
Full public CI passes1370 tests with29 skips; tracking/docs/progress/whitespace pass.

## 2026-11-06 continued main — complete network frame contexts rechecked

Five source contexts recheck the original20-byte local frame and swapped
seed/predicted homes. Adjacent arrays and actual queue definitions retain
1627/106/1381; phase union and successful-path placement remain nonexact.
Both co-visible contexts retain four exact queue owners, eight complete
comparisons1144 bytes/two fields. No model is adopted. See
[the bounded full-owner review](TH09_NETWORK_FRAME_CONTEXT_20261106.md).
Ninety hashed duplicate inputs are pruned; one restored co-visible context
rebuilds with identical raw body/fields and exact callees.

Source `f350f750`; sealed local receipt SHA-256
`dcb6b7b906a738f7444fbc7bd30d4a54cb7f00e0950fed079fad50a6e82b31da`
binds33 inputs, six objects, eight compiler/SDK files and seven canonical
comparisons3597 bytes/73 fields. Its independent replay passes; the service
remains1627/1633 bytes with1381 unequal overlaps and six missing. Exact count
stays1022; the goal and13 nonexact owners remain open.

## 2026-11-06 continued main — observed network input selectors restored

Fresh original-byte review corrects the earlier no-prepared-index claim:
430EC2 loads ECX from current network state+AC and431041 loads ECX=2 before
the two input calls. Declaration/definition and callers preserve those values;
the original-equal input body still ignores the selector. Original C++ arity
remains unproved. Seven affected complete units pass3597 bytes/73 fields under
their existing profiles. Full service remains nonexact at1627/1633 bytes,
106 fields,1381 unequal overlaps and six missing; all41 direct-callee
multiplicities agree. See [the corrected review](TH09_NETWORK_INPUT_SELECTORS_20261106.md).
Nine target-contract tests guard the observed producer/call edges. No new
exact credit; totals remain1022 exact/13 nonexact. Full public CI passes1364
tests with29 skips; target-required tracking/progress/docs/whitespace pass.

## 2026-11-06 continued main — complete SaveReplay offset context rechecked

Nine fixed-profile source controls recheck the two destination-address transport
bytes. Six retain1238/53/two; early offset publication, positive outer guarding
and deferred chain initialization fail the complete/protected-byte guards.
Header/payload members preserve both15/18-byte constructors. Twelve actual
co-visible memory/RNG bodies match336 bytes under the caller profile, without
changing the canonical RNG profile. No production model is adopted. See
[the complete evidence](TH09_REPLAY_OFFSET_RECHECK_20261106.md).
All112 current source/tool inputs,14 actual objects and eight provider
hashes remain bound to the prior source receipt; no canonical cold replay.
The goal and the13 nonexact functions remain open; exact count stays1022.

## 2026-11-06 continued main — complete Enemy source contexts rechecked

Fresh five-function Ghidra attestation and eleven fixed-profile source contexts
recheck Enemy's two draw-index and eleven special-branch transport bytes.
Seven models retain3900/97/13; three actual-periodic contexts retain45 unlike
bytes; one native byte-copy model fails the literal-position guard. The real
co-visible periodic33-byte and Effect184-byte callees remain original-equal.
No production model is adopted and no exact credit changes. See
[the bounded evidence](TH09_ENEMY_CONTEXT_RECHECK_20261106.md).
The prior83-unit ECL source proofs remain unchanged and are reused.

## 2026-11-05 continued main — complete Enemy native Player reads

Enemy update now shares the native Player's focus, position, state timer and
tracking storage with the four exact leaves. Both ECL profiles cold-build from86
quoted inputs;83 complete units pass29856 bytes/1389 fields. Two private label
spellings retain proven actual definitions and original operands. Full Enemy
and RunEcl raw bodies/actual referents are unchanged in both profiles; the main
original-linked frontiers remain3900/97/13 and15564/598/30. Two further RunEcl
timeout controls fail to improve that frontier and are not adopted. See
[the source review](TH09_NATIVE_PLAYER_TRACKING_20261105.md).
Source `bfa170c9`; sealed receipt SHA-256
`e52571a93f3b68573aee8cae83b021fe078549efa8133663b5801a93563e5ada`.
The independent verifier checks112 current inputs/14 objects and112 complete
comparisons, including29 unchanged leaf/callee/alias proofs from `435ffda0`.
Those combined comparisons pass31883 bytes/1460 fields. The two source
receipts and their original-bound reports are preserved separately.
No new exact/origin or product/runtime credit; totals remain1022 exact/13 nonexact.

## 2026-11-05 continued main — native Player tracking leaves

Three SHT homing callbacks retire private Player layouts in favor of the existing
native Player; Enemy cleanup shares that storage. The +30364 component/+3037C
homing pointer is guarded against the native opaque region, without adding
construction. Four complete leaves pass1042 bytes/35 fields;22 freshly rebuilt
callee/sibling units and three unchanged bound callees also pass. Enemy/Player
tests pass205 with five skips. See [the source review](TH09_NATIVE_PLAYER_TRACKING_20261105.md).
The complete Enemy source checkpoint is described above. No new exact/origin
credit; totals remain1022 exact/13 nonexact.

## 2026-11-05 continued main — native replay state and complete affected closure

Replay now retires 18 separate globals in favor of actual GameManager members,
including side +2C/+30, signed +138 and the +E8 configuration pointer. All 348
affected units pass 115410 complete compared bytes/6134 fields under 118 existing
profiles, plus two affected diagnostic builds. The source rebuild uncovers and
repairs seven older declaration conflicts in two independent source checkpoints.
All 466 private field-name refreshes preserve actual definitions and original
destinations. Eight nonexact dependent bodies retain their full raw hashes and
field destinations; full RunEcl remains 30 differences and Enemy remains 13.
SaveReplay stays two bytes unlike; playback stays 538/540 with39 unlike overlaps
and two missing. See [the source review](TH09_REPLAY_NATIVE_STATE_20261105.md).
Full public CI passes 1355 tests with 29 skips; target/tracking/docs checks pass.
Source `c8a0bc15`; sealed receipt SHA-256
`194c65dc2bb227ea5a360ca163dde3ad5796c783d55ce265d06ab56f58b8e540`.
It binds 276 inputs/132 actual objects and both independent repair receipts.
The verifier replays 348 affected units, six unchanged callees and ten
historical recovery comparisons without overwriting the immutable receipts.
No new exact/origin credit; totals remain 1022 exact/13 nonexact.

## 2026-11-05 continued main — six callback native declarations repaired

CardAttack's 24-byte query and ExAttack updates 2/15/20/25/26 now use the
shared native GameManager. All six old sources reproduce incompatible global
declarations with the old header; the fixed sources match 1968 complete bytes/
66 fields in both header contexts, with identical raw bodies and actual COFF.
Three Player ANM reads use the original +BC through header24.anmFile98. Ten
native GameManager symbol identities retain original destinations. See
[the callback review](TH09_CALLBACK_NATIVE_MANAGER_REBUILD_20261105.md).
No new exact credit. The broader replay/native-header work remains in progress.

## 2026-11-05 continued main — enemy lifecycle native declaration repaired

The old committed EnemyManagerLifecycle source independently reproduces C2040
with its old GameManager header: a byte-array declaration conflicts with the
native object already included. Native side records and valueF4 restore all
four configured owners, 1355 complete bytes/67 fields. Both original profiles
pass with the old header as well as the pending expanded header; all raw bodies
and actual COFF rows agree across contexts. Three manifest symbols preserve
the original roots/addends. See [the source review](TH09_ENEMY_LIFECYCLE_NATIVE_MANAGER_20261105.md).
No new exact/origin credit; the broader replay/native-header work is still in
progress and must finish its affected-owner comparisons before acceptance.

## 2026-11-05 continued main — original replay-name span recovered

Fresh target-attested Ghidra and a bounded trace of the original copy establish
the name span `+CE..+D6`: eight title-entry characters plus their zero terminator.
The shared record view now uses `playerName[9]`; all later offsets and the 1EC
extent are unchanged. All 68 affected units pass 26948 physical bytes/1474 fields
across nine existing profiles. Nine private Title labels were reviewed against
identical raw bytes, addends and owner-relative destinations before refreshing
their manifest spellings. All affected nonexact owners and the three menu
contexts retain their complete previous diagnostics; SaveReplay remains 2 bytes
different at +B7/+BB. See [the source investigation](TH09_REPLAY_NAME_STORAGE_20261105.md).
Source checkpoint `4adec272`; immutable receipt SHA-256
`4e4b1c873b127832aa0cf2d53d7c46636e4e0a340501e8d3cc04acbd02a90826`.
It binds 76 inputs/13 actual objects plus the historical cache receipt;
the separate verifier replays all 68 current and 11 historical comparisons.
Full public CI passes 1354 tests with 29 skips; target/tracking/docs checks pass.
Totals remain 1035 source-present/1022 exact/13 nonexact; no product/runtime claim.

## 2026-11-05 continued main — independent GameManager profile objects

The `/Os` primary and no-`/Os` loading-helper profiles now retain distinct
objects. The old helper build demonstrably replaced the1230-byte primary
with1397 bytes in the shared cache. Both actual profiles now remain available
and all11 units pass2991 physical bytes/243 fields simultaneously; the primary
setup worker retains its identical1689/171/904 diagnostic. Flags, source and
target bindings are unchanged. All27 GameManager tests pass. See
[the bounded cache review](TH09_GAME_MANAGER_PROFILE_OBJECTS_20261105.md).
The replay-name candidate is preserved separately; no new exact credit or
product/runtime claim follows. Totals remain1022 exact/13 nonexact.

## 2026-11-05 continued main — SaveReplay closes the checksum address byte

One shared index across the seven stage/byte loops restores+3F6 in both actual
ReplayInput profiles. The complete1238-byte owner now has onlytwo differences,
+B7/+BB; raw and resolved bodies otherwise remain unchanged from92ddf9b3.
All53 actual fields,380 instructions,23 calls and49 blocks survive. Stage-only,
byte-only and checksum-pair sharing leave the old three differences; sharing
the chain cursor or copy length adds no benefit. The guard protects1236 equal
bytes and explicitly rejects a checksum-byte regression. Both affected profiles
retain12 canonical units/4093 physical bytes/232 fields; seven unchanged callee
proofs are source/profile/object-bound reuses. See
[the bounded source review](TH09_REPLAY_SHARED_INDEX_20261105.md). All48 Replay
tests and full CI1354 tests/29 skips pass. SaveReplay remains nonexact; totals
remain1022 exact/13 nonexact.
Source checkpoint7d510243 and its immutable30-input local receipt are bound
in the review. Six disposable control recompiles verify the tightened guard;
only66 SHA-identical duplicate source copies are pruned from that packet.

## 2026-11-05 continued main — Replay uses native GameManager counters

Recording, playback and save metadata now share the actual GameManager
+378/+37C dwords, replacing four standalone global names. Fresh original
reset/capture/restore/save evidence establishes their common physical owner;
broader field names remain unknown. Both actual ReplayInput profiles preserve
12 canonical units/4093 physical bytes/232 fields; seven freshly rebuilt callee
bodies retain698 bytes/13 fields without constructor-origin promotion.
SaveReplay retains the identical1238/53/3 resolved frontier in both profiles;
playback remains538/540 with39 unlike overlaps andtwo missing. Reference chain
lifetime/checksum controls do not improve it. See
[the source review](TH09_REPLAY_NATIVE_COUNTERS_20261105.md).
All47 Replay tests and full CI1353 tests/29 skips pass. No new exact credit;
totals remain1022 exact/13 nonexact. Source checkpoint92ddf9b3 and its sealed
local receipt are recorded in the review, independently of Factory acceptance.

## 2026-11-05 continued main — shake residual operand classification

Fresh target-attested shake work retains336 bytes/20 fields/132 instructions/
eight ordered calls/26 agreeing blocks and26 unlike bytes. Strictly resolved
instruction comparison places every difference in a GP register selector;
all other operands, stack/frame registers and x87 operations agree. This is
residual classification, not semantic equivalence or exact credit. Original
volatile qualifiers/local declarations remain unknown. The fresh main object
passes16 canonical siblings/1555 physical bytes/77 fields, with a separate
disposable replay of complete bodies and actual COFF rows. See
[the source-bound review](TH09_SHAKE_REGISTER_RESIDUALS_20261105.md). No source,
ledger or acceptance change; totals remain1022 exact/13 nonexact.

## 2026-11-05 continued main — complete Enemy append/count counterevidence

Fresh target-attested Enemy update work retains3900 physical bytes/97 fields/
69 ordered calls/170 agreeing blocks and13 unlike bytes. Independent list
append controls expand to3912 and are rejected. Moving count initialization
to the reference position shrinks to3852 and also merges the later movement
branches, dropping one original vector-add call. The arithmetic count alone
reproduces the maintained raw body and all13 differences. Four fresh disposable
recompiles reproduce complete Enemy/RunEcl raw bodies and actual fields.
RunEcl's full30-byte frontier is unchanged;83 existing canonical ECL proofs
are reused after checking144 receipt inputs and both object hashes. See
[the bounded review](TH09_ENEMY_APPEND_COUNT_20261105.md). No game source,
ledger or exact-credit change; totals remain1022 exact/13 nonexact.

## 2026-11-05 continued main — result byte snapshot and complete callee context

Fresh Title result work retains484/485 bytes,34 fields/nine calls/147
instructions and40 agreeing ordinal blocks, with218 unlike overlaps plusone
missing. Seven scalar/lookup/native-reference forms collapse to the same483-byte
raw/COFF body and are not adopted. A representation copy adds a call/field and
is rejected; the existing score /O2 profile retains its exact216-byte score
owner but rejects533-byte result/185-byte cursor bodies. The actual original
cursor remains175/2 in every primary-profile trial. All40 current main-profile
Title witnesses replay18756 physical bytes/948 fields. See
[the current-source review](TH09_TITLE_RESULT_BYTE_SNAPSHOT_20261105.md).
No game source/ledger or exact-credit change; totals remain1022 exact/13 nonexact.

## 2026-11-05 continued main — charge input-mode lifetime counterevidence

Fresh target-attested current-source charge work reproduces1197/1210 bytes,
70 fields/58 calls and1160 unlike overlaps plus13 missing. The original mode
byte uses EBP-1; current code keeps it inBL and specializes a held-input edge.
Three natural lifetime/bitfield controls preserve every raw/linked byte and
actual COFF row; a native input reference regresses the extent to1195 and is
rejected. All actual Timer >= siblings retain20 original-equal bytes. See
[the bounded review](TH09_CHARGE_INPUT_LIFETIMES_20261105.md).
No game source/ledger change or new exact credit; totals remain1022 exact/13
nonexact. Input storage/source context remains open without register hints.

## 2026-11-05 continued main — complete Effect Type29 draw exact

Type29 draw448750 now matches all2220 bytes/129 fields/675 instructions/72
ordered calls/41 physical blocks in the existing EclManager Type11 /Ob0 carrier.
Independent per-fan angle lifetimes reduce92 differences to10; restoring the
real main-fan snapshot chronology closes the complete owner. No compiler hints,
new assembly, layout or profile changes are introduced. Three complete
same-object polar/SDK bodies retain32/26/36 original-equal bytes without alias
or origin credit. See [the exact review](TH09_EFFECT29_EXACT_20261105.md).
Both affected profiles pass83 canonical units/29856 physical bytes/1389 fields;
48 unchanged polar-cohort proofs are reused. RunEcl retains identical raw/linked
hashes and all30 differences; Enemy remains13. The guard protects every Type29
byte and its original field/call/graph/callee evidence.
Canonical totals:1035 source-present /1022 exact /13 nonexact;
262049 exact reviewed-authored bytes (89.7788%). Original TU ownership,
Factory acceptance and Windows product/runtime/port closure remain open.
Full CI passes1351 tests/29 skips; target/tracking, docs/progress and whitespace
checks pass. Source checkpoint `b6a9792c` and its immutable hashed local
proof are recorded in [source-bound receipts](RE_ACCEPTED_RECEIPTS.md), separately
from Factory acceptance.

## 2026-11-05 continued main — native polar API and complete Type29 extent

Type29 draw now has all 2220 bytes, 129 original-position fields, 675 instructions,
72 ordered calls and 41 identical physical blocks, with 92 ordinary differences.
The selected existing EclManager Type11 `/Ob0` carrier reproduces the original
six local-center receiver reloads. The sole existing 32-byte polar primitive is
reused through the physical explicit-vector API; original nominal ownership
remains unknown. Three complete same-object callee proofs are mandatory.
The reviewed transition restores1089 old differences and15 missing bytes while
changing11 old matches; the guard protects all2128 now-equal positions.
All130 affected canonical units pass47047 physical bytes/2371 fields across29
source/profile builds. RunEcl retains the identical30-byte difference list and
raw/linked hashes; Enemy remains13. All26 existing bounded drawing fixtures
agree, covering672 reachable original instructions, without a product/runtime
claim. See [the recovery review](TH09_EFFECT29_POLAR_RECOVERY_20261105.md).
Totals remain1021 exact/14 nonexact; Type29 has no new exactness credit.
Full CI passes1351 tests with29 skips; target/tracking, docs/progress and
whitespace checks pass.
Source checkpoint `4586b675` and its immutable hashed local proof are
recorded in [source-bound receipts](RE_ACCEPTED_RECEIPTS.md).

## 2026-11-05 continued main — current-header Type6 update recovered

The shared-polar caller audit finds that unchanged Type6 source no longer
compiles with current native headers. Removing its conflicting private roots
and using the real Player header/ANM/descriptor/angle interfaces restores all
698 bytes/30 fields. Eleven caller/callee/sibling units retain1359 physical
bytes/60 fields; seven private ANM table labels retain proven destinations.
See [the recovery review](TH09_EFFECT29_POLAR_RECOVERY_20261105.md).
This restores existing exact evidence; totals remain1021 exact/14 nonexact.
Full CI passes1347 tests with29 skips; target/tracking/docs/progress and
whitespace checks pass.
Type29 has a private2220/129/92 candidate with the original six local receiver
reloads. Its broader caller migration remains under review; the maintained
Type29 frontier remains2205/129/1170 plus15 missing until that cohort closes.

## 2026-11-05 continued main — timeout probes reach the complete Oracle

Two optional ECL operand recipes used a retired `state` assignment and failed
before compilation. Both now recognize the reviewed native Enemy assignment,
retain the single byte copy and reject mixed/duplicate/unknown storage roots.
Real source-frozen CLI replays reach15564 bytes/598 fields, then correctly
reject239 field-byte and26 table-byte differences. Native bitfield and complete
accessor-surface controls do not improve RunEcl and are not adopted. See
[the bounded review](TH09_ECL_TIMEOUT_PROBES_20261105.md).
Game/compiler/profile/object inputs are unchanged;82 canonical proofs are reused
with their source/object hashes. Full RunEcl retains the identical30 unlike
bytes and Enemy13; biased movement remains exact516/28. Totals stay1021 exact/
14 nonexact. Six focused tests and full CI1346 tests/29 skips pass, together
with target/tracking, docs/progress and whitespace. This is a diagnostic repair
and bounded negative evidence, with no new Factory or product claim.

## 2026-11-05 continued main — complete biased ECL movement exact

MoveRandomBiased407E30 now matches all516 bytes/28 fields/14 calls, including
its same-object288-byte polar callee. Real SDK indexing and native typed operand
reads preserve the actual compiler-private interfaces without register hints.
Both rebuilt canonical EclManager profiles retain all82 exact units,27636
physical bytes and1260 fields. Main RunEcl retains the identical30 unlike
bytes; Enemy update remains13. The /Ob1 copy of the leaf is uncredited.
See [the complete-source review](TH09_ECL_SDK_MOVEMENT_EXACT_20261105.md).
Canonical ledger: 1035 source-present / 1021 exact / 14 non-exact.
Exact reviewed-authored bytes increase to259829 (89.0182%). Original TU/profile
closure, product/runtime/port and Factory acceptance remain open. CI passes1343
tests with29 skips; target/tracking, docs/progress and whitespace checks pass.
Source checkpoint `33291074` and its immutable hashed local receipt are in
[source-bound receipts](RE_ACCEPTED_RECEIPTS.md); the recorded game/tool inputs
agree with that commit. This is local compiler evidence, not a Factory verdict.

## 2026-11-05 continued main — complete Etama renderer exact

OnDraw413BE0 now matches all600 bytes/27 fields/13 calls/157 instructions.
Real SDK inline conversion with one /O2 /Ob0 source profile also preserves the
232-byte helper and832-byte AddedCallback; the same-object SDK alias matches
three bytes without authored/origin credit. Current-header Bullet dependencies
retain their original bytes/tails after definition-backed private-name updates.
See [the exact-source review](TH09_ETAMA_SDK_EXACT_20261105.md).
Canonical ledger: 1035 source-present / 1020 exact / 15 non-exact.
Exact reviewed-authored bytes increase to259313 (88.8414%). Product/runtime/port
and Factory acceptance remain open.
Source checkpoint `9507e12a` and its hashed local receipt are recorded in
[source-bound receipts](RE_ACCEPTED_RECEIPTS.md); current game source inputs
agree with that immutable commit. CI passes1341 tests with29 skips.

## 2026-11-05 continued main — complete ECL probe include closure

The shared variant stager now copies/hashes every quoted project dependency,
fixing a real duplicate-type compile failure from mixed live/staged headers.
The raw runner binds the complete observed/staged input set. Its real typed
integer macro trial completes15564 bytes/598 fields/30 ordinary differences,
with no field/table differences. CI passes1339 tests with29 skips. Source
checkpoint `6b07a109` retains90 exact units; totals remain1019/16. See
[the bounded review](TH09_ECL_WIRE_ARGUMENTS_20261105.md).

## 2026-11-05 continued main — typed ECL wire operands

The instruction prefix names its four-byte trailing operands; RawInt/RawFloat
use typed members instead of byte arithmetic and scalar pointer casts. Seven
actual source/profile builds preserve90 exact units/33719 physical bytes/1631
fields. All311 renamed private fields retain definition/PE-verified targets.
The complete RunEcl difference list stays identical at15564/598/30, Enemy
stays3900/97/13 and biased movement stays517/516 with148 unlike overlaps.
Fresh native assignment/scope/owner/Y-alias controls remain nonexact. The raw
context probe supports the maintained representation and rejects changed
storage. See [the bounded review](TH09_ECL_WIRE_ARGUMENTS_20261105.md).
Totals stay1019 exact/16 nonexact; this is an operand-model checkpoint.

## 2026-11-05 continued main — minimal network input interface

**Historical source checkpoint; target interpretation corrected November 6.**
Both original callers prepare ECX explicitly. See
[the restored observed interface](TH09_NETWORK_INPUT_SELECTORS_20261106.md).

GetInput's unused source index is removed; both service calls now use its
minimal no-argument interface. Original body/known direct consumers do not
consume/prepare that index; original C++ arity remains unproven. All seven
affected exact units pass3597 bytes/73 fields, with13 focused tests passing.
Service remains nonexact at1616/1633 bytes,106 fields,1366 unlike overlaps and
17 missing. This is an input-contract correction, not an exact promotion.
See [the bounded review](TH09_NETWORK_INPUT_CONTRACT_20261105.md).
Totals stay1019 exact/16 nonexact; tracking/docs/progress/whitespace pass.
Hashed duplicate per-case headers are cleaned; frozen inputs, unique controls
and their reconstruction recipes remain in the service-flow packet.

## 2026-11-05 continued main — native Controller input owner

The819-byte GetInput uses shared Supervisor fields, removing a padded duplicate
and conflicting global declaration. All nine fields remain exact; its real
ControllerInput callee/siblings also replay exact.11 focused tests pass.
Service body visibility, shared-workspace and member controls remain nonexact;
input arity is under separate review. See
[the bounded review](TH09_CONTROLLER_INPUT_OWNER_20261105.md).
Totals stay1019 exact/16 nonexact; no new exact credit.

## 2026-11-05 continued main — direct reset fields

The100-byte ResetGameManager uses the setup side/rank/menu fields directly;
its duplicate private reset layout is removed. Stores+BC/+110 have typed,
offset-checked members with unknown semantic roles. All12 affected units
remain3987 bytes/335 fields exact across three actual source/profile builds;
40 Supervisor local fields retain verified destinations.24 focused tests and
tracking/docs/progress/whitespace pass. Worker stays1689/171/904. See
[the bounded review](TH09_SETUP_RESET_FIELDS_20261105.md).
Totals stay1019 exact/16 nonexact; no new exact-function credit.
The two proof packets now have separate object directories. Checkpoint
`ecd136eb` was cold-replayed from immutable inputs after a local directory
collision; the current reset objects retain verified SHA-256 in setup-reset.
All source/object hashes and both full12-unit receipts reconcile. Details:
[reset proof recovery](TH09_SETUP_RESET_FIELDS_20261105.md).
Only hashed duplicate per-case inputs in the setup-flow packet and disposable
reuse orchestration were cleaned; frozen inputs, changed controls, compiler
objects, receipts and cold-build verification recipes remain.

## 2026-11-05 continued main — third setup record and shared Effect slot

Setup models the three0x38 physical records observed by reset. Record2's
EffectManager slot+7C replaces the standalone pointer in creation, cleanup
and deletion. All12 affected units pass3987 bytes/335 fields across three
actual source/profile builds;40 Supervisor local fields retain verified
original destinations.20 focused tests pass. Complete worker remains1689/
171/904; fresh loop/context/reference controls do not provide a replacement.
See [the bounded review](TH09_SETUP_THIRD_RECORD_20261105.md).
Totals stay1019 exact/16 nonexact; no new exact credit.

## 2026-11-05 continued main — replay copy/checksum controls

Fresh SaveReplay still passes its1238/53/3 whole-owner guard. Three natural
copy/offset/dereference controls preserve every raw byte and actual COFF field;
a checksum cursor and combined control regress to1230 bytes/245 overlap
differences/8 missing and are rejected. Maintained source is retained. See
[the bounded review](TH09_REPLAY_COPY_CONTROLS_20261105.md).
Only hashed duplicate per-case inputs were cleaned in this and the preceding
Enemy packet; shared inputs, changed controls and recovery recipes remain.
Totals stay1019 exact/16 nonexact; no new exact credit.

## 2026-11-05 continued main — one native Enemy layout

Both post-ECL callbacks now use EnemyView; the duplicate0x5430 EnemyCoreView
layout is removed, with opaque helper ABI tags retained. All85 affected exact
units replay27847 bytes/1252 fields;20 private label names are definition/PE
verified in two functions. Full Enemy13, RunEcl30, Effect29 and biased-movement
frontiers are unchanged.82 focused tests and tracking/docs/progress/whitespace
pass. See [the bounded review](TH09_ENEMY_SINGLE_LAYOUT_20261105.md).
Totals stay1019 exact/16 nonexact; this is source consolidation, not new credit.

## 2026-11-05 continued main — readonly replay-menu record

ReplayMenu reads the selected record through const and supplies name/date
arguments directly, removing two inline wrappers. Fresh complete raw bytes
and all57 actual COFF records remain unchanged:1280 bytes/13 calls/72 blocks/
29 differences. Standalone, real declarations and full Title implementation
contexts all reproduce the same payload; all1251 equal positions remain.
Nine focused tests and tracking/docs/progress/whitespace pass. See
[the bounded review](TH09_REPLAY_MENU_READS_20261105.md).
Totals remain1019 exact/16 nonexact; no new exact credit.

## 2026-11-05 continued main — Etama private argument boundary

Fresh target/control evidence confirms DrawSingleBullet's physical Bullet
argument in EBX, distinct from a public ECX fastcall. A two-source external
split changes the helper to236 bytes and the parent to602; it is rejected.
Retained232/16 helper and832/30 AddedCallback still replay exact. The parent
remains nonexact; this is ABI evidence, not new function credit. See
[the bounded review](TH09_ETAMA_PRIVATE_ABI_REVIEW_20261105.md).
The reviewed38-row Effect prefix's55 non-null callback addresses all have
authored source mappings; whole-game inventory remains unproven.

## 2026-11-05 continued main — direct native movement access

Movement uses the vector's real float-pointer result and genuine main/option
AnmVm objects, removing the position reinterpretation and five VM casts.
Fresh complete raw bytes and all66 effective COFF fields remain unchanged:
1900 physical bytes/23 calls/122 differences. Two local label names are verified
at unchanged actual section offsets. The exact Player update caller and native
SetInterrupt callee pass focused cold replay;27 movement tests pass. The direct
input-pointer variant fails field coverage and is rejected. See
[the bounded review](TH09_PLAYER_MOVEMENT_REVIEW_20261105.md).
Totals stay1019 exact/16 nonexact; no new exact credit.

## 2026-11-05 continued main — native Front indicator and ECL reinvestigation

Front's enemy indicator uses the native AnmVm array; AddedCallback clears
the same two alpha fields directly. Both full owners remain213/10 and706/37
exact. Both ECL profiles rebuild:all81 exact units pass; complete Enemy13
and RunEcl30 frontiers remain. Two private label spellings change only after
actual same-section definitions prove the immutable original targets.
Fresh typed-wire/bitfield/timer/context and SDK controls do not improve the
large owners and are rejected. See
[the bounded review](TH09_ECL_FRONT_REVIEW_20261105.md).
Totals stay1019 exact/16 nonexact. No full cold replay or product/runtime claim.

## 2026-11-05 continued main — structured charge update

The complete independent reference charge body reproduces our linked candidate.
Charge now uses structured pressed/held/released branches and direct timers.
Fresh maintained-source before/adopted builds preserve every raw/linked byte
and all70 actual COFF identities:1197/1210 bytes,58 calls,1160 overlap
differences and13 missing. Its visible20-byte Timer >= sibling is exact.
Fresh Replay seed/config/native-header and Effect GetAnm controls do not improve
their frontiers and are rejected. See
[the bounded review](TH09_CHARGE_REPLAY_REVIEW_20261105.md).
Source checkpoint:`bccbc5e4`. The new `--owner charge` cold-compiles only this
owner, checks the actual Timer sibling and protects all37 already-equal bytes.
Full public CI passes1338 tests(29 skipped), tracking/docs/progress/whitespace.
Totals remain1019 exact/16 nonexact; product/runtime closure remains open.

## 2026-11-05 continued main — native result storage and complete-byte guard

Result update uses shared input/unlock fields with ten unchanged effective
operands. Its full 484/485-byte, 34-field, 218-difference frontier is preserved.
The Oracle now protects all 266 previously equal positions and verifies the
actual 19-byte PNG initializer. Both Title source profiles preserve 41 exact
units. Full local CI passes 1333 tests with 29 skips, plus tracking/docs/progress
and whitespace checks. Fresh character/loop and Rotate2D contexts regress;
fresh Etama parent matches still break its 232-byte exact sibling and are
rejected. See [the bounded review](TH09_EFFECT_PLAYER_REVIEW_20261105.md).
Totals stay at 1019 exact / 16 nonexact; no candidate gains partial credit.

## 2026-11-05 continued main — native screen device and renewed controls

Screen clear/rectangle rendering now use the shared Supervisor device field,
removing twelve casts and the duplicate device projection. All three actual
profiles rebuild: 18 canonical units, 2562 bytes and 164 fields remain exact.
Nine RegisterChain label names are verified against actual COFF definitions;
28 focused tests pass. The complete shake frontier stays at 336/20/26.
Fresh ordinary/unsigned envelope and reference-shaped player/SDK controls
regress and are rejected; current player source remains 849/862 bytes.
See [the bounded review](TH09_EFFECT_PLAYER_REVIEW_20261105.md).
Totals stay at 1019 exact and 16 nonexact.

## 2026-11-05 continued main — natural ReplayMenu expressions

ReplayMenu replaces manual component arithmetic and staged character arguments
with SDK vector expressions and direct format arguments, credited to Cirno-Labs
DrawState11. Fresh full raw bytes and actual COFF identities remain identical:
1280 bytes, 57 fields and 29 differences; all 1251 previously equal positions
survive. The three real-owner contexts agree. Full local CI passes 1330 tests
with 29 skips, plus tracking/docs/progress and whitespace checks.
New complete ECL bitfield/jump controls regress and are rejected, while fresh
Enemy count arithmetic stays at 13 differences. See
[the current-source review](TH09_NATIVE_SOURCE_REVIEW_20261105.md).
Tracking remains 1019 exact / 16 nonexact; no new exact credit.

## 2026-11-05 continued main — shared Enemy collision storage

The82-byte collision wrapper uses the established PlayerLifecycleView query
field instead of two padded private structures. Fresh target-attested Ghidra
covers the wrapper, Enemy update and RunEcl. Both ECL carrier profiles rebuild:
all81 canonical exact units pass; full Enemy3900/97/13 and RunEcl15564/598/30
frontiers remain unchanged. Only two compiler-private label spellings move,
with actual COFF definitions proving the same immutable destinations. See
[the current-source review](TH09_NATIVE_SOURCE_REVIEW_20261105.md).
No new exact count or product/runtime claim; tracking remains1019/16.

## 2026-11-05 continued main — native MusicRoom and current-source retries

See [TH09_NATIVE_SOURCE_REVIEW_20261105.md](TH09_NATIVE_SOURCE_REVIEW_20261105.md).
MusicRoom now uses shared AnmVm and ReplayInputState fields; both complete
owners remain2258/74 and209/6 exact from one fresh object. Four native input
relocations keep the original effective4ACF3A/4ACF3B addresses through the
verified4ACE18 root and actual122/123 addends. Full local CI1330/29 skips,
tracking/docs/progress and whitespace pass. Exact totals remain1019/16.
Current-source constructor/configuration/encoder retries retain SaveReplay's
1238/53/3. Fresh ReplayMenu SDK, buffer and expression controls retain1280/57/29;
one index-snapshot candidate regresses and is rejected. These are fresh bounded
observations, not a presumption that earlier negative notes are authoritative.

## 2026-11-05 merged main — native Options and complete carrier repair

PR [#1](https://github.com/N0zoM1z0/th09/pull/1) is merged at `1015aa9a` after
remote CI passed. New commits use `gpt-6.1-sol:`. See
[CIRNO_LABS_NATURAL_CPP_20261105.md](CIRNO_LABS_NATURAL_CPP_20261105.md)
for controlled compiler comparisons and the next source-model guidance.
Options now uses shared native AnmVm fields and direct SetSprite pointers;
its full 2045 bytes/135 fields remain exact. Whole-carrier review recovered a
34-byte PlayMenuSound definition omitted in the previous integration while
its unit remained registered. Both functions now replay in the same fresh
object; the new `--owner options` checks both and the real 42-byte scrolling
helper. This repairs prior source coverage; it is not new function credit.
Tracking remains 1019 exact /16 nonexact, 258713/291883 authored bytes.
The twelve reference-adapted source files pass135/135 canonical comparisons,
reusing unchanged source/profile objects and rebuilding only Options. Full
local public CI passes1330 tests with29 skips.
The frozen source controls reproduce Type3's 15-byte residual solely by
narrowing the three position objects' scope. Hiding the Options helper body
breaks exactness despite unchanged calls/graph. The SaveReplay cursor rewrite
worsens three differences to149 and is rejected; original source is retained.

## 2026-11-05 Cirno-Labs integration branch — current source checkpoint

Operator authorized integration, attribution, local commits and push on
`reconstruction/cirno-labs-integration`. See
[CIRNO_LABS_INTEGRATION_20261105.md](CIRNO_LABS_INTEGRATION_20261105.md).
The three native Title cursor owners and Supervisor received-frame insertion
are now canonical exact: 774 physical bytes and ten relocation fields. Live
tracking is **1019 exact / 16 nonexact**, 258713/291883 authored bytes.
Result/options drawing and update, controller input and the full Supervisor
update owner are now also canonical exact; the former result barrier is gone.
The third batch adds Front random setup/AddedCallback, replay-save drawing,
ExAttack Type3 update/Type14-22 draw, and the complete network-message owner.
Etama reference integration was rejected because its helper visibility changed
the existing exact bullet-draw sibling. Both original source and its 268-byte
frontier are retained. MoveRandomBiased remains 148 differences plus one extra
byte even with the reference-native source; neither candidate earns credit.
The older external-audit section below describes the pre-integration source.
GPL adaptations and original MIT grants are recorded in THIRD_PARTY_NOTICES.md.

## 2026-11-05 external reference audit — no canonical promotion

The operator requested the Cirno-Labs/th09 clone under `_reference/` and a
check with our Oracle. Read [the bounded audit](CIRNO_LABS_REFERENCE_AUDIT_20261105.md).
Pinned upstream `d7653152` yields eight complete external-source matches:
6562 physical bytes, 351 relocation fields, actual literal definitions checked,
zero unresolved references. These cover currently nonexact Title result,
options and three cursor owners, Supervisor update/frame insertion and
controller input. Our source/ledgers remain unchanged at 1005 exact functions;
this is diagnostic evidence, not canonical acceptance. Upstream's 98.65%
function count is distinct from its 88.11% game-byte figure.

The separate `--group ecl --build` audit includes the entire upstream
15564-byte/598-field interpreter, including the 772-byte physical tail. It
has **3695 ordinary non-relocation differences** and 12 deliberately unresolved
helper/literal fields under our pinned compiler: not exact. Upstream also
marks it DIFF. Its self-reported 96.97% whole-file equality is another measure,
not a game-authored-byte exactness percentage. The maintained RunEcl source
and its historical 30-byte frontier were not replayed or modified here.

Reproduce only this bounded sample with `python3 -B
scripts/audit-cirno-reference.py --group title --group system --build`.
Ignored checkout, CP932 source mirror, original-bound JSON/build receipts and
bounded Ghidra exports remain locally available as documented in the audit.
No full cold replay, source import, game run or push was performed.

## Operator status — reconstruction explicitly resumed

The operator explicitly resumed TH09 reconstruction and requires serious work
on both complete large bosses and leaf functions. Re-investigate source and
target evidence rather than trusting old notes or previous attempts. Commit
reviewable progress with commit subjects prefixed gpt-6.1-sol:; create multiple coherent
commits in a session when there are multiple substantive advances. Use only
focused complete-owner/carrier replays for changed units and dependencies; do
not run a full cold replay for routine iteration. Preserve incoming dirty work
and inspect it before changing or cleaning it.

The former October source-changing checkpoint was 80313a4f
(gpt-web: narrow Title DrawResult residual to three bytes). Newer November 2
source changes are described below and tracked by the live Git log. The most
recent gameplay exactness promotion remains the Effect34 draw at 2d6303de.
The October 31 maintenance-only state is historical, not the current operator
instruction. Local commits are checkpoints, not proof of remote publication or
Factory acceptance. The 98% reviewed-authored-byte goal remains open;
canonical codegen, whole-product linking, runtime semantics and ports remain
separate milestones.

## 2026-11-02 continuation — native AnmLoaded and ANM owners

The **live Git tree and original PE/COFF replays**, not prior handoff text,
govern current state. These are local source checkpoints, not a remote push or
accepted whole-game replay. No full all-owner cold replay was run.

- `182071b3` established one original-backed 0x1C `AnmLoaded` class, with
  resource record offsets +0x04, +0x08, +0x0C, +0x10, +0x14, +0x18,
  eliminating a private SetSprite resource overlay.
- `707e9616` eliminated the runtime-only 0x1C projection from the exact
  143-byte `AnmLoaded::SetAndExecuteScript`, keeping the original signed
  short VM file index and every original call/field.
- `1bcbf9d1` removed the full loader-only 0x1C projection, binding the real
  resource record into every native ANM load/release/texture owner, including
  `LoadTextureData` (558 bytes/16 fields), `ReadAnmEntries` (390/17), and
  `AnmManager` ctor (341/29). All 16 source-cold loader exact units survived.
- `4e9b0ca3` converts both Etama's exact
  832-byte `AddedCallback` and Effect's 51-byte nonexact `AddedCallback` to
  `AnmLoaded::rawData04`. Etama's four exact siblings, including 2216-byte
  `OnUpdate`, remain exact; Effect's sixteen exact siblings remain exact.
  Effect AddedCallback still has eight ordinary byte differences; never
  promote it as exact without new independent evidence.

The **old** Knowledge Base note near ANM resident callback history describes
the now-obsolete eight-byte `AnmLoadedResidentView.hpp` as the single current
view. That statement was accurate for its earlier checkpoint only. The live
class is now the independently verified 0x1C native resource record; do not
restore the obsolete view because of historical text.

The large `AnmManager::ExecuteScript` source-cold original extent is still
9776 bytes/368 relocations, **exact**, and its four independent variable
helpers remain exact. Focused `--owner anm` protects 17 exact siblings and
the 622-byte camera projection; `--owner runecl` preserves 96 exact
siblings and the unsplit 15564-byte/598-field interpreter, still with 30
ordinary mismatch bytes (opcode 4: seven; opcode 155: 23). Etama OnDraw
still has 268 differences in its 600 bytes. Source-level type recovery does
not add to the **1005 exact functions** in tracking.

Compiler-private `$L` names can change when shared headers alter compile
order. They were reviewed against **actual** current COFF section-relative
definitions and each immutable original PE target; only proven label
spellings were changed, not relocation targets/kinds/offsets or ordinary
original code. Re-run only the touched focused original-bound owners:
`--owner effect`, `--owner etama`, `--owner anm`, and any directly
affected individual match units. Never run `--owner all` merely as a check.

## 2026-11-02 continued checkpoint — native Player and shot callbacks

**Verify live source, original TH09 1.50a PE, fresh COFF and Git state rather
than trusting an earlier handoff.** Source commits are local checkpoints, not
proof of remote push or accepted whole-game replay. No all-owner cold replay
was performed for this work.

Five source commits, in order, after the earlier Enemy/ECL work:

- `6b90c045` recovers real Player +0x364/+0x368 focus/secondary Effect
  pointers. The full 1,900-byte movement owner still preserves all 66 fields,
  23 calls and the 129 known mismatch positions. Exact 572-byte
  PlayerAddedState and 243-byte Player constructor remain unchanged.
- `953720eb` drops the separate PlayerPatternOffsetView and directly reads
  PlayerLifecycleView movement multipliers +0x1CDC..+0x1CE8 and primary SHT at
  +0x30338. Original 412 physical bytes/18 relocation fields of the resolver
  remain exact; the separate 3,238-byte/136-field selector boss is also exact.
  All 18 compiler-private switch-label renames were proved using actual COFF
  section-relative addresses and unchanged original target destinations.
- `0635fc93` unifies two original-exact Player shot callbacks with native
  Player movement state, retaining their real callback and rectangle-member
  ABI identities.
- `84086f55` deletes the independent Type4 shot storage overlay and uses
  PlayerShotCtorView position +0x2A4, velocity +0x43C, timer +0x454 and state
  +0x462. A natural local velocity snapshot fixes an initial four-byte x87
  operand-order mismatch: the full Type4 callback is **249/249 bytes exact**
  with 9 original relocation fields. Twelve source-cold expression trials
  independently identified the equivalent codegen choice.
- `448cf6ba` replaces Type3's private shot velocity view with native
  PlayerShotCtorView::velocity43C. Type3 stays **86/86 bytes exact**, with
  its genuine one relocation. Its real InitializeShot callee remains
  **309/309 exact**, with 7 original fields.

**Current reviewed owners**: focused --owner movement: 1,900 physical bytes,
1,835 code bytes, 66 fields, 23 calls, 129 mismatching ordinary physical
positions and 2 exact siblings. Focused --owner added: 572-byte exact
PlayerAddedState, 25 fields, 4 exact siblings. Player constructor:
243 bytes/21 fields exact. Focused Enemy: 3,900 bytes/97 fields/69 calls,
13 mismatches. RunEcl continues at 15,564 physical bytes/598 fields/30
ordinary mismatches, all confined to opcode 4 (7) and 155 (23), with its
96 exact sibling checks previously replayed in this continuation. The
authored-function exact ledger remains **1,005**; no new exactness credit
follows merely from recovered data structures or diagnostic similarity.

Discarded source-cold probes are only ignored diagnostics in
`.analysis/current-session-runecl/`: twelve opcode 155 bit-form spellings
left 30 RunEcl bytes (or regressed), ten movement config-read spellings left
129 bytes (or regressed), and fifteen Effect AddedCallback register-order
spellings did not close its eight residual bytes. Do not misinterpret
these experiments as established semantic facts or promote them as exact.

Next machine-code targets include the lower-clamp EAX/EDX transport around
Player movement physical +0x533, RunEcl opcode 4/155 register/wire scheduling,
Enemy OnUpdate's 13 residual bytes, and SaveReplay's three residual bytes.
Keep reviewing both big functions and exact leaf callers, not only the latter.

Use narrowly scoped source-cold verification: `python3 -B
scripts/check-boss-frontiers.py --owner movement` (or `--owner added`,
`--owner enemy`, `--owner runecl`), and the affected individual
`build-match-unit.py --unit ...` then `compare-coff-function.py --unit ...
--json`. Never use --owner all as routine validation. Keep original frozen
offsets/kinds/destinations unchanged; renaming a private COFF label requires
proving its actual source location still targets the exact original VA.

## 2026-11-02 source checkpoint — shared Enemy/ECL storage

The latest focused work made several **source/ABI representation** corrections
across the unsplit 15,564-physical-byte RunEcl boss and 3,900-byte Enemy
OnUpdate boss. The 0x2AC388 manager prefix now contains nine typed Enemy boss
slots instead of a 0x24-byte anonymous field. ECL state and late handlers use
the actual Background members at +0x18, +0x81C, +0xCD8, +0xCDC and +0xCE0
rather than duplicate private layouts. The side-state owner pointer at +0x14
and its two state dwords at owner +0xA4/+0xA8 share one source view.
The ECL late manager now also uses EnemyManagerCoreView directly instead of
a separate multi-megabyte mock layout; the actual protocol value at
+0x2AC3DC and the spawn-suppression dword at +0x2AC440 are bound by
offset assertions at their observed adjacent fields.

A new bounded native EnemyCorePlayerView.hpp is used by both ECL late
instructions and Enemy OnUpdate. It retains the separately observed
Player fields at +0x364 (effect), +0x1B88 (position), +0x30364/+0x3037C
(tracking/homing), +0x303C8 (state timer) and +0x303F4 (late side-table
value). ECL late instructions now use the same 0x38-byte
EnemyCoreSideStateView and genuine EtamaController* at +0x08 rather
than a second side-state struct. The original nominal C++ owner/lifetime
of unknown Player bytes is not asserted.

After each shared-source change, the original-pinned VC7.1 complete owner
frontiers were rebuilt locally, not from a historical object. The latest
focused --owner runecl replay passed 96 canonical exact sibling
comparisons, including literal/COFF checks, while preserving RunEcl
15,564 bytes/598 fields/30 ordinary differences (opcode 4: 7;
opcode 155: 23). Focused --owner enemy passed 88 exact sibling checks;
Enemy remains 3,900 bytes/97 fields/69 calls/13 ordinary differences.
The private label updates in two exact ECL siblings are backed by their
actual unchanged COFF section positions and original PE destinations;
the relocation offsets, kinds, targets and ordinary code remain fixed.
The five real Etama owner tests and four shared Player/SideState tests
pass. The checked target and tracking inventory remain at 1,005 exact
authored functions; there is no new exact-function or byte credit.

Rejected *source-cold* experiments are not silently adopted: opcode 4/155
RunEcl scheduling variants failed full-owner field/layout checks; six
Enemy draw-index and eight descriptor/source lifetime forms failed to
reduce 13 residuals; fifteen Replay SaveReplay frame/checksum forms
left its three original physical differences; twelve Title DrawResult
entry forms and eight MoveRandomBiased Y-alias forms were neutral or
regressive. A few ScreenEffect timer/envelope changes also regressed the
complete 336-byte owner. Reproducible scratch results, where retained,
are in ignored .analysis/current-session-runecl/. They are *not* canonical
receipts or a reason to relax the documented original-bound guards.

These source commits are local checkpoints: 39fd5e8b (typed boss slots),
b4c35604 (native Background fields), d1db50d1 (side-owner state),
f8d297ce (shared Player and side storage), and 333198b5 (native late
manager protocol). They do not imply remote publication or executable
runtime acceptance.

For continued work, run only the relevant focused commands,
python3 -B scripts/check-boss-frontiers.py --owner runecl and/or
--owner enemy, plus matching target-independent regression tests; never
run all-owner cold replay for routine work. The large-boss opcode 4/155,
Enemy register/copy scheduling, and all other nonexact owners remain open.

## 2026-10-31 source and proof snapshot (historical)

Gameplay setup uses all eight implemented creator contracts/nine references.
Lifecycle/rendering consumers share one AnmVmBase/AnmVm definition. Cleanup calls
the real Background,CardAttack and FrontSide methods;its425-byte/43-field owner
remains exact. Teardown calls six implemented release contracts at seven sites;
the422-byte DeletedCallback remains exact. Four neutral-named release functions
already have implementations and remain. PlayerRelease's shared declaration is
checked against both caller and defining source. Full product linkage remains open.

Current retained proof:
`.analysis/gpt-61sol-20261031-setup-native-releases/current.py` and current.json.
It strictly compares14 affected units/3480 physical bytes/286 fields and reuses
665 unchanged source/profile/object pins:679 combined units/202200 summed physical
bytes/10472 fields,including excluded units. This is not the1005-authored-unit
ledger or a fresh full cold replay.231 distinct primary proof/diagnostic objects
and nine additional historical/diagnostic objects remain. Current-source RunEcl
stays15564/598/30,Enemy3900/97/13,and Setup1689/171/35/77/904.

## 2026-11-01 handoff refresh — historical non-exact snapshots

The live ledgers remain authoritative: 2,248 candidates, 1,035 authored
mappings, 1,005 exact functions, and 30 source-present non-exact functions /
44,286 bytes. Reviewed authored coverage is 247,597 / 291,883 bytes (84.8275%);
the 98% goal is not met. Whole-product linking, runtime semantics and
portability are not started.

Fresh diagnostic-only owner reports were captured under
.analysis/maintenance-20261101/owner-snapshots/ before build-cache cleanup:

- Title DrawResult: 1,939/1,939 bytes, 577 instructions, 82 fields, 16 calls,
  92 CFG blocks; three residual bytes at +0x1D, +0x23 and +0x39.
- Replay SaveReplay: 1,238/1,238 bytes, 380 instructions, 53 fields, 23 call
  operands and 49 blocks; three residual bytes at +0xB7, +0xBB and +0x3F6.
- EnemyManager::OnUpdate: 3,900/3,900 bytes, 97 relocation fields, 1,026
  instructions, 69 calls and 170 blocks; 13 residual bytes at
  +0x19C, +0x1A3, +0x252, +0x263, +0x26D, +0x272, +0x294, +0x29A,
  +0x29C, +0x2A5, +0x2A9, +0x2B7 and +0x2BD.
- Title OnUpdateOptions: 2,045 target / 2,048 candidate bytes, 551/554
  instructions, 135 fields, 51 calls and 121 blocks; 610 unequal overlapping
  bytes plus three extra candidate bytes. Fields, ordered calls and direct CFG
  agree; this is a large non-exact owner, not a 3-byte residual.
- Title active cursor: 211 target / 215 candidate bytes, 185 unequal overlapping
  bytes plus four extra candidate bytes. Reverse and Inactive are each
  175/175 bytes but have 38 differences and a 4-versus-5 block CFG mismatch;
  equal physical size is not exactness.

The SaveReplay barrier/type/address trials did not improve its three residuals.
A barrier after the replay-data copy or offset initialization preserved the
same three bytes; barriers around allocation, copy loops or checksum setup
changed the owner/CFG and were rejected. Enemy side-state/descriptor lifetime
and draw-index probes did not improve its 13 bytes. Title cursor and options
lifetime/zero-initialization probes were also not adopted. Their disposable raw
source/object/log directories were removed after these results were summarized
in the continuation note; the deletion manifest is
.analysis/maintenance-20261101/scratch-prune-manifest.json.

The owner snapshots are diagnostic only and grant no exactness credit. Build
and compiler caches were removed after capture; rebuild only the relevant
translation unit with scripts/build-match-unit.py before a fresh comparison.
No full cold replay was run.

## Historical 2026-10-31 checkpoint — Title DrawResult narrowed to 3 bytes

At this historical checkpoint, the gpt-web code/test commit was 80313a4f. The 1,939-byte
TitleScreenView::DrawResult big owner is now 1,939/1,939 bytes with 577
instructions, 82 fields, 16 ordered calls and 92 CFG blocks; its physical
residual has narrowed from 22 bytes to exactly +0x1D, +0x23 and +0x39.
A no-code _ReadWriteBarrier() keeps the currentScreen read after the
scoreGroup pointer store. The complete owner gate passed after rebuilding
only TitleScreenResultDraw.cpp; the residual guard now protects only those
three bytes and rejects movement into previously matched code. This is still
non-exact and grants no canonical credit. Target-independent CI passed 1,104
tests (29 optional skips), tracking validation, documentation validation and
generated-progress checks. Details and rejected experiments are in
docs/TH09_GPT_WEB_CONTINUED_20261031.md. No full cold replay was run.

## Earlier code checkpoint — Title replay menu provenance (historical)

At that earlier checkpoint, the newest `gpt-web:` commit was `0332496c`. The separate 1,280-byte
`TitleScreenView::DrawReplayMenu` remains non-exact at 29 physical differences,
but its source oracle now verifies all 16 real initialized read-only COFF
literal definitions behind 30 relocation references, including actual payload
bytes and the original PE values. The focused
`python3 scripts/check-boss-frontiers.py --owner replaymenu --skip-exact-siblings`
rebuilds only `TitleScreenReplayMenuDraw.cpp`; it passes the complete 57-field,
13-call, 72-block owner and the literal provenance gate. This adds no exact
credit. The latest public CI passed 1,104 tests (29 optional skips). Detailed
proof, residual offsets and losing source trials are recorded in
`docs/TH09_GPT_WEB_CONTINUED_20261031.md`. No full cold replay was run.

## Effect34 draw — complete 946-byte canonical match

The complete draw callback at `0x00444AF0` is now exact in the pinned VC7.1
source/profile, with all **53 independently verified original relocations**
bound at their actual instruction offsets. Reconstructing the fan-cursor live
range before the alpha branch recovered 65 bytes (74→9); initializing the
actual `layer` loop counter *before* declaring `lineColor` recovered the final
nine bytes. No code patches, fake source, synthetic targets, or compiler flags
were used. The target is the complete original 946-byte owner, not a matching
substring; all 286 instructions, 32 ordered calls (including 12 COM) and
16 control-flow blocks agree. The canonical `effect-draw-callback-type34`
unit is in `config/match-units.toml` with the original 53 referents.

Re-run `python3 scripts/build-match-unit.py --unit effect-draw-callback-type34`
and `python3 scripts/compare-coff-function.py --unit effect-draw-callback-type34 --json`.
The focused `--owner effect34` boss checker protects the native ECL carrier
and the separate Type11 profile and Effect34 update. A single-function Factory replay at source commit `2d6303de` returned
`pass/accepted` on completion, but the *current* registry later marked that
receipt stale (`invocation-plan-stale`, `toolchain-environment-stale`). Do not
count it as a live Factory-admitted exact fact; see
`docs/TH09_GPT_WEB_20261031.md` for the job and receipt identities.
The older source-scoped
`.analysis/gpt-61sol-20261031-effect34-cursor-lifetimes/` records remain
historical experiments, not current-source proof; fresh tests and receipts are
recorded in `docs/TH09_GPT_WEB_20261031.md`. Whole-product closure, unique
original source/TU ownership, runtime and portability remain unproven.

## Historical research and removed copies

Durable findings and bounded negatives are indexed in KNOWLEDGE_BASE.md;dated
sections retain their original source/epoch scope. In particular,the constructor's
194-row stride1C initialization span at Enemy+3E68 does not establish an original
vertex-array declaration. Current source still exposes192 rows plus an opaque tail.
Old359/357/353 and shared-VM whole verifiers are historical;only hash-matched
individual groups can feed current proof reuse. Full factory/release/producer
contexts do not prove original nominal classes or unique translation units.

Some byte-identical old src/docs/config snapshots are removed from ignored
analysis packets. Every removed copy has a reachable Git blob,original SHA-256,
path and line-ending map in:
`.analysis/maintenance-20261031-handoff-1c371cf7/git-backed-copy-plan.json`.
Historical replay tools may require these copies. Preview recovery for one packet:

```bash
python3 -B scripts/restore-analysis-snapshots.py .analysis/maintenance-20261031-handoff-1c371cf7/git-backed-copy-plan.json --prefix .analysis/PACKET
# Add --apply to restore absent files. Existing different files are never overwritten.
```

## Source and evidence routing

Start with the live manifests and current proof subset,not a dated experiment's
object path. Proof reuse requires matching source,profile and object hashes.

- Canonical inventory: `config/functions.csv`,`config/function-origins.csv`,
  `config/matches.csv` and `config/match-units.toml`.
- Current retained subset:
  `.analysis/gpt-61sol-20261031-setup-native-releases/current.py` checks679 units/
  202200 summed physical bytes/10472 fields. It strictly compares14 affected
  units/3480 bytes/286 fields and reuses665 unchanged source/profile/object pins.
  These679 include excluded units;they are not the1005 authored exact ledger
  entries. Its current.json identifies231 distinct primary proof/diagnostic
  objects,including the two new Setup carriers and Player lifecycle carrier.
  Nine additional historical/diagnostic objects remain.
- The preceding setup-native-context whole verifier is source-stale after the
  teardown/header changes. Its unaffected individual groups feed665 current pins.
  The actual current impact is three carriers,not the historical four-profile
  October22 scope. Use the live manifest and source hashes for replay planning.
- The shared-VM whole verifier is source-stale after later Setup/Player header
  changes. Its unaffected individual groups may feed current pins. Replaced
  objects are removed after complete records and compiler contributions are
  audited;absence after cleanup is expected. Historical cleanup details remain
  in the knowledge-base checkpoint records.
- The preceding native-clear,shared-storage,startup and parameter-field whole
  verifiers are source-stale after the common VM header change. Their unaffected
  individual groups may feed current pins. Older359/357/353/344/323/297/251 bundles are source-scoped
  historical verifiers.
  Their unchanged individual groups may feed current pins,but a historical
  whole verifier is not automatically current. Missing discarded trial objects
  are expected;rebuild a needed candidate from its recipe before comparison.
- `scripts/inspect-ecl-interpolation-table.py --object OBJECT` verifies the32-byte
  callback prefix and actual74/188-byte same-object native functions. The prefix
  proof is separate from code exactness and grants no authored-function credit.
- `src/EclOperands.cpp` uses the actual opposite-angle PlayerAngleView producer
  at41BB70. The four resolvers remain5718 complete physical bytes/374 fields.
  Type7 uses the separate forward producer41BB00. Never rebind one to the other.
- BulletManager uses native RNG/Supervisor declarations. Player movement,
  SHT loading and Charge share PlayerShtFileView's observed speed/charge fields.
  ScoreData,finalization,insertion and result drawing share TitleScoreRecordView. These
  interface repairs preserve the current large-owner residuals below.
- Player shot/Charge declarations use the actual initializer,frame,resource,
  SoundPlayer and SpawnShots producers. Enemy tracking separates the position
  at+30364..3036F from the homing pointer at+3037C;Stage D resets only the
  position. The intervening twelve bytes have a separate reset and an unknown
  role. Consult the source and KB for the bounded layouts and exact ABIs.
- The current679-unit receipt includes a fresh affected Front weighted carrier;
  the former standalone object's source-scoped observations remain historical.
- Historical diagnostics outside the current679 remain source-commit scoped:
  network input131/1 in `network-input-contract/native-int.obj`,playfield116/6
  in `enemy-playfield-context/native-playfield.obj`,and Supervisor's952-byte
  baseline in `supervisor-state-branches/baseline.obj`. Packet names here use
  the prefix `.analysis/gpt-61sol-20261031-`.

Latest completed ECL source-partition records are in
`.analysis/gpt-61sol-20261031-ecl-source-partitions/`. Six actual footer groups
all leave RunEcl15564/598/30;the only Enemy-emitting group keeps3900/97/13.
Core-post/core-vectors each reject an existing movement owner;effect-family
rejects ten existing owners. Other existing units absent from each partition
are explicitly outside its proof scope,not passes. No split is adopted.
All79 immutable Git/EOL inputs,recipes,raw/COFF records and logs were audited
before disposing the twelve trial OBJ/PDB files. Maintenance adds no compiler
jobs or independent cold replay. See the source-partition KB note for the complete table.

Other completed context controls are indexed by findings in
[KNOWLEDGE_BASE.md](KNOWLEDGE_BASE.md). Important bounded negatives include
native timeout-field updates,draw-group/packed-flag views,whole ECL operand and
initializer visibility,Enemy vector/periodic/playfield contexts,closed Effect
ANM types,and SaveReplay algorithm/slot/codec contexts. Failed controls do not
prove all natural source models exhausted. Never adopt their types,profiles or
original TU claims merely because a leaf or local byte range matches.

The previous full handoff is recoverable with
`git show a3e4aaee:docs/RE_HANDOFF.md`. The
[October18 checkpoint archive](RE_HANDOFF_CHECKPOINT_ARCHIVE_20261018.md) and
[accepted receipts](RE_ACCEPTED_RECEIPTS.md) are historical;accepted receipts
retain immutable source-commit bindings. Current exactness does not establish a
complete Windows product.

## Live ledger snapshot


The canonical state is `config/functions.csv`, `config/function-origins.csv`,
`config/matches.csv`, and `config/match-units.toml`, checked by
`python3 scripts/validate-tracking.py --require-target` and
`python3 scripts/validate-docs.py`. The current ledger summary is
**1034 source-present / 1021 exact / 13 non-exact**; earlier operator notes
mentioning 979/876/103 are obsolete for this checkout.

| Measure | Count |
| --- | ---: |
| Function candidates | 2,248 |
| Boundary/origin unreviewed | 0 |
| Reviewed but origin-unresolved | 38 |
| Confirmed authored | 1,034 |
| Classified exclusions | 1,176 |
| Source-present authored mappings | 1,034 |
| Canonical exact functions | 1,021 |
| Source-present non-exact functions | 13 |
| Source-present non-exact bytes | 29,834 |
| Authored without maintained source | 0 |
| Canonical exact authored bytes | 262,034 |

**262,034 / 291,868 reviewed-authored bytes = 89.7783% exact**. To reach
98% of the *currently classified* denominator, another **23,997 bytes**
must become complete original-bound exact owners, reaching at least **286,031
exact authored bytes**. The 38 origin-unresolved functions are excluded and could change the denominator. Similarity fractions
and locally matching subsequences never count as accepted exact.

The authored match ledger governs **1021 exact** functions. The separate15-byte
VectorAngle source recipe remains byte-exact while its origin is under review.

The catalog is not proof of complete executable-code inventory. The formerly
uncatalogued **0x0040D677 /0x00444A78** Timer references now belong to the exact
Effect 1/2 and Effect 34 updates. All **203** captured assignment callsites have
catalogued owners; that bounded census proves neither complete executable
coverage nor a return ABI. The Effect 34 **178-byte initializer at 0x00444960** and **946-byte draw
at 0x00444AF0** are both canonical exact in their pinned source/profile.
The separate Effect29 draw is now canonical exact in its selected /Ob0 profile;
see [the complete-owner review](TH09_EFFECT29_EXACT_20261105.md).
Do not silently treat uncatalogued code as excluded.

## Reconstruction frontiers

These are routing measurements, not exactness credit. The ledgers list all13
nonexact functions. The opcode4 setter/assignment visibility and whole Enemy
storage controls are already measured; consult their actual frozen inputs before
repeating them. Historical assumptions remain hypotheses.

| Owner | Target address | Current comparison | Remaining work |
| --- | --- | --- | --- |
| **RunEcl** | 0x004086C0 | **15,564 physical** (14,792 code + 772 table); 598 bound fields; **30** differing non-field bytes | Opcode4 timer transport and opcode155 flag update. Keep all598 fields/tables and complete15564 bytes; actual Timer ABI and RNG ownership are already corrected. `--owner runecl`. |
| **EnemyManager::OnUpdate** | 0x00410730 | **3,900 physical**,97 fields, 69 calls,170 blocks; **13 differing bytes** (29->13) | Draw index2, side/threshold4, descriptor-position copy7. Keep real20-byte Timer greater co-visible, all97 fields and3887 matching physical bytes. |
| **Player movement boss** | 0x0041C170 | **1,900/1,900 physical**, 66 fields, 23 calls, **129** differing bytes | Lower-bound EAX/EDX transport and encoding cascade. Native framerate/coordinate owners and the shared SHT speed type are corrected;preserve1771 matching bytes and both tables. `--owner movement`. |
| **Player charge update** | 0x0041F810 | **1,210 target /1,197 candidate bytes**, 70 fields, 58 calls, 350/342 instructions; **1,160 overlap differences +13 missing** | Recover source chronology and missing13 bytes; preserve repaired SpawnShots transport and actual call/field identities. |
| **TitleScreen::OnUpdateResult** | 0x004266B5 | **485 / 484**, 147/147 instructions,34 fields,nine calls,40 agreeing ordinal blocks; **218** overlap +one missing | Character snapshot and remaining control-flow/byte deficit. Keep the exact Title setup siblings. |
| **TitleScreen::DrawReplayMenu** | 0x004234F6 | **1,280/1,280 bytes**,376/376 instructions,57 original-bound fields,13 ordered calls,72 matching direct blocks; **29 differences** | Menu bank/record chronology. Preserve all57 fields,13 calls,72 blocks and the full original-bound residual guard. `--owner replaymenu`. |
| **GameManager gameplay setup worker** | 0x0041AF2D | **1,689/1,689**, 172/171 fields, 35 calls, 78/77 blocks; **904 differing bytes** (911→904, no damaged matches) | Side registration and shared limit/rate publication. Keep native creator calls and164 aligned identities; B4 multiplicity reflects branch sharing. |
| **Player CheckBulletCollision** | 0x0041DFF0 | **862 / 849**, 26 fields, 12 calls; **799** overlap + 13 missing | Region producer, activeB100 pool and missing physical bytes. Preserve five same-source exact helpers and separate CalcItemBox/FindCollision proofs. `--owner player`. |
| **Replay SaveReplay** | 0x00420C60 | **1,238/1,238 bytes**, 380 instructions, 53 fields, 23 call operands, 49 matching blocks; **2 differences at +0xB7/+0xBB (152→3→2)** | Frame-destination register remains. Shared full-pipeline index closes checksum SIB+3F6. Preserve1236 matching bytes and all12 exact same-source siblings. `--owner replaysave`. |
| **Replay BeginPlaybackStage** | 0x00420840 | **540 / 538**, 152/151 instructions, **32/32 ordered original PE/COFF fields**, 5 calls, 12/12 matching CFG blocks; **39** overlapping unlike bytes + 2 missing (49→39) | Seed/tail chronology and two missing bytes. All32 ordered referents and first30 physical field positions are already restored. `--owner replay`. |
| **ScreenEffect shake envelope** | 0x004227A0 | **336/336**, 132 instructions, 20 fields, 8 calls, 26 blocks; **26** differences (28→26) | EDI/ESI owner allocation. Preserve310 matching bytes; timer-float visibility regresses physical size and x87 lifetime. |
| **Supervisor network service** | 0x00430AA0 | **1,633 target /1,627 candidate**,106 source fields;51 call-identity multiplicities agree |1381 unequal overlaps plus6 missing after the reviewed unsigned-16 queue API change (previously1376+7). Native RNG and packet lifetimes remain;wait/stack/input transport stay open. `inspect-supervisor-service.py` checks the full compiled owner; `--owner network` checks its queue dependencies. |
| **Effect AddedCallback leaf** | 0x0040D1C0 | **51/51**, 4 fields, 2 calls; **8** differences | Remaining receiver/allocation scheduling. Preserve43 matching bytes and genuine AnmLoaded/Chain interfaces. `--owner effect`. |

## Continuing reconstruction

When the operator explicitly resumes reconstruction,read
[AGENTS.md](../AGENTS.md),[RE_WORKFLOW.md](RE_WORKFLOW.md),
[ORACLES.md](ORACLES.md),the affected ledger rows and relevant KB findings.
Recover tracked and untracked work first. Prioritize complete RunEcl and Enemy,
including shared layouts,dependencies and actual translation-unit boundaries.
Other large functions and small leaves remain in scope;old hypotheses are not
conclusive.

```bash
export PYTHONDONTWRITEBYTECODE=1
git status --short --branch
git diff
git diff --cached
python3 -B scripts/verify-target.py
python3 -B scripts/validate-tracking.py --require-target
python3 -B scripts/report-reconstruction-status.py
python3 -B scripts/ghidra-analysis.py --function 0x004086c0 --function 0x00410730
# Build only the affected canonical source/profile before its comparison.
python3 -B scripts/build-match-unit.py --unit NAME
python3 -B scripts/compare-coff-function.py --unit NAME --json
```

The original ignored input is `resources/th09.exe`,Japanese1.50a,SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
Do not search Windows mounts or set TH09_TARGET_PATH during normal work.
Ghidra12.1.4 uses `.tools/ghidra` and reuses
`.analysis/ghidra-direct/243bffea8dcbe99a/th09.gpr`. The direct runner checks
disk/Program identity,i386 language,base/entry and all685056 file-backed mapped
bytes before every analysis/export. Its names,types and decompilation remain
provisional;Ghidra is not an exactness Oracle. No IDA,REA or Factory MCP routing.

For source changes,protect complete owners,tails/tables,callees and affected
exact siblings under pinned VC7.1 profiles. Compile serially at lowered priority
on one CPU with bounded resources,and reuse unchanged source/profile proofs.
Do not run all1005 cold units for a local edit. Windows product/runtime,dual-
Oracle semantics and portability remain later phases. No new defensive gameplay
behavior or routine fuzz/null tests belong in an exact-source checkpoint.

## Maintenance and validation

Current maintenance receipt:
`.analysis/maintenance-20261031-handoff-1c371cf7/cleanup.json`.
584 files/81954293 logical bytes are removed:141 Git-backed source/config/doc
copies and443 reviewed compiler/debug/bytecode cache files. All36112 protected
file/link fingerprints agree across deletion. Recovery from Git is hash-checked,
and the real recovery writer passes a self-cleaning temporary test. Full public
CI runs1080 tests:1051 pass/29 optional skips. Current679 and saved candidate
verification pass without new compiler jobs. Build/ is absent.
The original target,toolchains,working Ghidra project,current source,research
records,current proof objects and pending nine-difference object are preserved.
No source archive is added. Final deletion totals and checks are recorded in the
receipt and the latest knowledge-base maintenance entry.

`clean-generated-artifacts.py` previews by default and preserves analysis OBJ
files. To remove a reviewed obsolete object,use --reviewed-objects with JSON:
`{"objects":[{"path":".analysis/PACKET/file.obj","sha256":"64 lowercase hex digits"}]}`.
Inspect the plan before --apply and wait for producers to terminate. Changed
files or build-tree contents invalidate the plan. Do not delete the preserved
candidate or current proof cohort merely because the extension is.obj.

Documentation-only housekeeping needs no compiler or Ghidra replay. Run
validate-docs.py,tracking/progress and git diff --check;run relevant tooling tests
and public CI when shared cleanup/recovery tooling changes. Accepted receipts
remain source-commit scoped. Whole Windows i386 compile/link/runtime,dual-Oracle
semantics and portability are later phases and remain unfinished.
