# TH09 reconstruction handoff

## Active objective

The user resumed on **2026-10-09**: reach **95% exact reviewed-authored bytes**,
work on large functions, use direct IDA Pro MCP and local Bash/compiler Oracles
without Factory MCP, commit locally as `gpt-6.1-sol: ...`, and do not push.
Batch coherent trials before cold replay; reuse unchanged baselines only after
checking their source/backend/object bindings. The goal remains active-incomplete.
The October 7 stop at 7347cb3 is historical.

This checkpoint starts clean atb20fcab. Original target and direct IDA mapped
bytes pass. Five GameManager OnUpdate callback-phase/capture contexts precede one
batch-end cold. Supervisor pointer/reference lifetimes and existing enum returns
retain all105 complete differences; scalar captures widen reads and regress.
All eight existing exact siblings pass1122 bytes/96 fields in every carrier.
No game source/header/ABI/profile or credit change is integrated. IDA's stale
entry comparison comment is corrected/read back with all instructions unchanged.

## Live ledger snapshot

These totals are checked against the live ledgers by scripts/validate-docs.py.

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

The fixed authored denominator is **275,770 bytes**: **79.2331%** exact.
Another **43,481 bytes** are needed for 95%. Origin-unresolved candidates are
separate. Faithful Windows i386 product/runtime gates remain open;
semantic reconstruction and portability have not started.

## Active frontiers

- RunEcl: 14,792 target code /15,564 physical bytes; candidate has
  **14,791 code + one alignment + 772 compiler-table bytes**. All 598 fields
  are independently checked, with 2,161 complete differences. Handler frontiers
  remain 4/7/86/155/156/157; scope controls recorded in the KB are rejected.
- Enemy OnUpdate: 3,883 authored /3,900 physical bytes, 97 fields, 39 differences;
  early draw index, descriptor/effect scheduling, trail and homing.
- Gameplay setup worker: 1,689 bytes, 171 fields, 921 full differences; reuse-base
  caching, flags cursor and rate/failure-tail topology remain open.
- GameManager update: 1,230 bytes, 87 fields, 105 differences; input capture and
  entry-zero scheduling, plus six shifted early-return branch displacements.
- Title Options: 2,045 target /2,048 candidate bytes, 135 fields, 610 linked
  overlap differences plus three excess; 51 calls and 121 direct blocks agree.
- Result draw: 1,939 bytes/82 fields/22 entry differences. Output-reference and
  array-reference producer contexts are rejected; later keyboard graph is covered.
- Replay menu draw: 1,280 target/1,244 candidate bytes, 57 target/54 candidate
  fields, 1,180 differences plus36 absent; row arguments leave caption merging open.
- Supervisor network service: 1,633 target/1,632 candidate bytes, 106 target/108
  candidate fields, 1,395 complete differences plus one absent. Actual indirect
  operands are covered; sign-test opcode and output-workspace evidence are below.
- Player movement: 1,835 authored /1,900 physical bytes, 66 fields, 144 differences.
- Player charge: 1,210 target /1,197 candidate bytes, 70 fields, 1,160 linked
  overlap differences plus 13 absent. Integer conversion preserves seven regions
  and 58 ordered calls; entry register allocation and byte-mode storage are open.
- Enemy draw: 1,758 bytes, four differences around the second subtraction/Abs.
- Type21 update: 1,074 bytes, 29 fields, 15 ring-preheader differences; actual
  descriptor member storage is maintained. Entry-owner-path controls are neutral.
- Remaining owners, including ExAttack18/24, PauseMenu and DirectPlay, are routed
  by config/functions.csv and docs/KNOWLEDGE_BASE.md. Read prior controls first.

## Latest GameManager callback-phase and capture batch

Packet: `.analysis/gpt-6.1-sol-setup-callback-phase-20261009/`.
Actual canonical remains `build/gpt-dots-setup-continuations-20261006/baseline.obj`,
SHA256 `c181cf0b14561687ddf6ba9975ebce492bdfb25d055ea5269183fba608f7e186`.
All123 current source/include/backend/Oracle bindings agree before reuse and after.
Target has1230 bytes/337 instructions/87 fields/16 calls/86 direct blocks;
canonical has1230/338/87/16/86, frame0x10 and105 full differences.

Delaying Supervisor pointer acquisition until after setup exits, then using a
reference, preserves every main byte/field. Existing ChainCallbackResult return
and its delayed-pointer combination are likewise neutral. Actual enum mangling
and the registrar's one callback DIR32 at+34 are recorded explicitly; no original
return type is inferred. Five separate ushort captures produce1225 bytes/87
fields/338 instructions,1133 overlap differences plus five absent. Reads widen to
DWORD; this control is rejected. Entry-zero/capture/publication scheduling stays
open. All eleven collateral owners and seven noncode sections remain unchanged
after the precise enum spelling adapter; eight exact siblings pass1122/96.

One cold combined enum/pointer compile repeats all nondebug runtime bytes/fields
and numeric records. All91 distinct actual source/include paths are hash-bound.
The939-character IDA comment reads back exactly; all337 address/instruction pairs
remain equal and distributed mapped-byte attestation passes. Cleanup removes23
terminal files/1328112 bytes. Five source recipes restore to original-EOL hashes
and are removed again. Retained complete proof stays below350 KB, reparses the
real canonical COFF and independently rebinds deleted full captures/siblings.

Do not repeat these five precise contexts unchanged. Rotate to the1436-byte
ExAttack type18/type24 callback at4491E0 after fresh full source/target/baseline
review. Read its reflection correction and prior const-value/reference negatives
first; target-backed collision-size/returned-vector homes remain open. Native
runtime and95% remain open.

## Enemy update vector-storage/immutability routing

Packet: `.analysis/gpt-6.1-sol-enemy-vector-storage-20261009/`.
Actual canonical remains `build/matching/EnemyManagerCore.obj`, SHA256
`60a054387f8a7125ec31eb44184e0bd6ed8ec711a949d61b9c29beae366aa1f9`.
All50 current source/header/backend/Oracle bindings agree before reuse and after.
Four controls plus one cold retain3900 physical/3883 code bytes,1026 independently
PE-decoded instructions,97 fields,69 ordered calls,170 complete blocks and frame
0x2A8. Immutable distance values, an ordinary x/y/z storage base and their
combination are raw/effective-field neutral. Const result references reverse the
X/Y square-read order at eight bytes, increasing full differences from39 to47.
No explicit copy special member or original class ownership is invented.

Both parsers agree on all five owners/4060 bytes/99 fields. All seven noncode
sections and four collateral owners remain unchanged; existing151-byte/two-field
attached-effect unit passes in all five actual carriers. Five private labels
rename solely at independently verified unchanged numeric section/value records.
One batch-end cold reference carrier repeats complete nondebug runtime records.
All19 actual source/include paths match preobserved bindings.

IDA's obsolete3191/3512 partial comparison is replaced by a919-character full
39-difference comment and exact readback. All1025 direct IDA line pairs stay
unchanged; independent PE decoding, rather than line count, proves1026 machine
instructions. Final distributed mapped-byte attestation passes. Cleanup removes
23 owned terminal files/620062 bytes; all eight source/header copies restore to
pinned original-EOL hashes and are removed again. Retained proof stays below250 KB
and reparses actual canonical COFF while re-binding deleted complete captures.

Do not repeat these four contexts unchanged. The four39-byte scheduling clusters
remain open; GameManager's subsequent phase/capture controls are recorded above.

## Replay-menu routing

Seven row-argument/lifetime contexts plus one cold leave canonical1244 bytes/54
fields/1180 differences plus36 absent. Direct list arguments only reduce one
full difference; separate caption calls remain merged. No control is promoted.
Full proof/restoration is in replay-row-arguments-20261009; see KB exclusions.

## Supervisor service workspace/call routing

Packet: `.analysis/gpt-6.1-sol-service-workspaces-20261009/`.
The actual canonical baseline is
`build/gpt-dots-service-packet-owner-20261007/baseline.obj`, SHA256
`21b8c89578be6936e82186c35c5c37a2ad628c0a7f6f0cb51597bb55adec4f74`.
Its current source/header/backend bindings agree before reuse. Phase-local times
and both packet-readiness arm forms retain every main byte/field. Flag mask changes
only candidate +0x5C5 from JGE to JNS, matching the corresponding target sign test
at431063 but leaving full-owner differences unchanged. No type/API is promoted.

Real success/shared output workspaces emit1638 bytes/108 fields/417 instructions
with1427 differences plus five excess, using frames0x18/0x0C rather than target
0x14. Shared prefill/success scope changes15 raw stack bytes relative to success
scope. Visibility of the actual unchanged int/int ApplyNetworkInput body preserves
each respective main body and adds the existing exact131-byte/one-field owner.
One cold input-visible carrier repeats all nondebug bytes/fields/numeric records.

Independent full operand census finds106 target versus108 candidate fields:
candidate has three more network-pointer loads and one packet-side load; target
has two more timeGetTime IAT loads. All41 direct calls retain identities and
multiplicities; order remains different. All10 indirect calls have concrete
identities, including all-path cached EBX proof under the preserved-register ABI.
Every branch lands on a fully decoded instruction; graph equivalence stays open.
All85 actual source/include paths have pre/post hash checks. Sole noncode section
is unchanged; no current exact unit compiles through SupervisorNetwork.cpp.

IDA's old1625-byte comment is superseded; final880-character comment reads back
exactly and every instruction is unchanged. Cleanup removes35 owned terminal
files/1213617 bytes. All eight source recipes restore/hash-check after deletion
and are removed again. Complete compressed captures/recipes stay below400 KB;
retained replay reparses the real baseline and independently rebinds deleted
captures, with no deleted-object inspection claim. Do not repeat these eight
precise contexts. Reopen the sign test with a predicate-only probe before changing
its existing field type. The subsequent replay-menu batch is recorded in the KB;
replay rendering is distinct from parked SaveReplay serialization. Native runtime
and95% remain open.

## RunEcl source-ownership/return routing

The four static/free stdcall and checked enum-return contexts plus one cold are
neutral across67 owners/28838 bytes/1215 fields; all23 exact siblings pass.
Current baseline stays at `build/gpt-6.1-sol-type21-preheader-20261009/canonical.obj`,
SHA256 `94875d0fa89059d7decded90e9987f79d42388f12b0980cf163cefd154d4d6b9`.
The receiver-ownership packet retains full proof/restoration and corrected IDA
entry comment. No alternative API is integrated. Do not repeat those contexts
or earlier timer/scope/operand controls; see KB for precise exclusions and the
six open handler frontiers. Complete restart commands remain below.

## DrawResult routing

Four bank-producer contexts plus one cold leave the baseline1939-byte owner at22
entry differences. Output-reference/pointer methods are neutral; full/short-scope
bank references regress keyboard allocation to1941 bytes/319 differences plus two
excess. No hypothesis is integrated. Current actual baseline stays at
`build/gpt-6.1-sol-result-difficulty-20261009/canonical.obj`; complete proof and
recipes are in the result-entry-dependencies packet. Reopen with different
TH09-backed value/alias/TU evidence; see the KB for precise exclusions.

## Supervisor routing

The preceding completion packet tests nine controls plus one cold, without exact
growth. The maintained complete inspector covers the full996-byte target and
owned tables. Prior984-byte hypothesis remains601 differences plus12 absent at
`build/gpt-6.1-sol-supervisor-entry-context-20261009/binary-title-flag-bit.obj`.
Its MOVZX/title-call/footer and target late EDI=2 lifetime remain open. Target
registrar preserves full32-bit ECX; the reviewed IDA int prototype is read back.
Do not repeat those completion, enum-return or return-join models. See the KB.

## Previous replay-save depth batch

Packet: `.analysis/gpt-6.1-sol-replay-depth-workspace-20261009/`.
The inherited source/COFF-bound candidate remains
`build/gpt-dots-replay-renderer-residuals-20261006/inplace-target-depth.obj`, SHA256
`2c480b9a519f26a2d0dbc7ac4031dc7e89bc9a9a52594d37458ed684f8fa23c1`.
A fresh pinned carrier repeats its complete five owners/834 bytes/41 fields.
Middle/final/both workspace depth and weighted-depth value/reference controls
retain the four calls and full28-block graph but stay794/797/797/796/794 bytes
against800. The797-byte models add a field write; the796-byte value model delays
the first multiplication and raises x87 depth to5 versus target4. One cold
value compile agrees. See KB for full unmasked scores and exact exclusions.
All four collateral bodies/40 bytes/zero fields and nineteen other sections
remain unchanged. Existing text Oracles pass130/130 and55/55 after rebuilding
only their absent canonical cache. Twenty-one terminal files/527644 bytes are
removed; all six source recipes recover after cleanup. Full compact evidence
stays below150 KB. Game source and all exactness claims remain unchanged.
Earlier sharing and completion controls are recorded in the KB.

## Other current evidence

- RunEcl jump/timer controls:
  `.analysis/gpt-6.1-sol-ecl-jump-timer-20261009/`; all five contexts are neutral
  and one cold family replay agrees. All23 exact siblings pass; the folded
  canonical timer-current diagnostic binding and repaired ECL caches stay.
  Six handler frontiers remain open; see KB for exact excluded contexts.
- Enemy draw expression/declaration controls:
  `.analysis/gpt-6.1-sol-enemy-draw-expression-20261009/`; four main differences
  remain. Three controls are neutral; borrowed next-angle reference regresses.
  Exact scope, cold replay and cleanup are recorded in the KB.
- Enemy update object copies: `.analysis/gpt-6.1-sol-enemy-object-copies-20261009/`;
  typed Player +0x3037C Enemy pointer stays maintained. Current canonical cache
  `build/matching/EnemyManagerCore.obj` SHA256
  `60a054387f8a7125ec31eb44184e0bd6ed8ec711a949d61b9c29beae366aa1f9`.
  Seven controls and one canonical cold retain 39 differences; see KB exclusions.
- Type14/22 draw: `.analysis/gpt-6.1-sol-draw1422-callee-context-20261009/`;
  canonical math declaration and timer conversion stay maintained. Current cache
  `build/matching/ExAttackDrawType14Type22.obj` SHA256
  `cf6d50537f2768a58a4b480072929db81feaa806bdb760addb9f8d02e5fc724f`.
  Twelve controls and one canonical cold reject normalization-call loss; see KB.

- Player collision batch: `.analysis/gpt-6.1-sol-collision-member-visibility-20261009/`;
  actual difficulty +0x11C and coordinate +0x358 connections remain maintained.
  Eight controls precede one canonical cold replay; six existing siblings pass.
  The 849/862-byte owner has 32/33 blocks, so its frontier includes loop layout.
  Current object SHA remains
  `c9ea2f6617302b613b16c66ec53f512df3a0a9ae07c8a0679fd83cdc886240ee`.

- PauseMenu batch: `.analysis/gpt-6.1-sol-pause-partial-visibility-20261009/`;
  five actual +0x13C member connections remain maintained. Thirteen controls and
  the single canonical cold replay are documented in the KB; eleven exact
  siblings pass. Current object hash remains
  `1145e31c07bebac38e69639a835721fbdcb155761ba0251b492acdc03135f788`.

- Enemy lifetime batch: `.analysis/gpt-6.1-sol-enemy-scratch-lifetime-20261009/`;
  four controls are neutral, hoisting all four vectors regresses. One batch-end
  cold compile agrees; precise scopes are excluded by the KB and retained proof.
- Gameplay helper visibility: `.analysis/gpt-6.1-sol-gameplay-helper-visibility-20261009/`;
  all four complete workers remain 1,689 bytes /171 fields /921 differences.
  Actual declaration/implementation context and omitted helpers are rejected;
  all present exact siblings pass. See KB for exact controls and retained recipes.
- GameManager update arrays: `.analysis/gpt-6.1-sol-setup-input-array-20261009/`;
  explicit publication is neutral, two-side loop regresses; see KB.
- Latest flat ECL scope controls: `.analysis/gpt-6.1-sol-ecl-flat-local-scope-20261009/`;
  all 67 owners and 23 exact siblings are neutral. See KB for precise exclusions.
- Prior ECL mutation controls and code-extent correction:
  `.analysis/gpt-6.1-sol-ecl-mutation-expressions-20261009/`. Its precise direct
  timeout/comma-jump/opcode-snapshot controls are neutral. Decoder tests remain.
- Type21 canonical-only proof checks all 23 ECL exact siblings and the actual
  67-owner carrier. No need to rebuild unchanged historical cohorts.
- Charge conversion: `.analysis/gpt-6.1-sol-charge-timer-conversion-20261009/`;
  current carrier `build/gpt-6.1-sol-charge-timer-conversion-20261009/canonical.obj`,
  SHA256 `4744bb7c52cce4a50b55aa7162578ce79f9cf613b56e9e454f0cc3bb1df4971d`.
  Existing timer conversion replaces the private getter overlay; complete code
  and effective fields remain unchanged. Original folded spelling is unknown.
- Options: `.analysis/gpt-6.1-sol-options-index-dispatch-20261009/`; unsigned
  selectors, index reuse and real right-input switch are neutral. Current cache
  `build/matching/TitleScreenOptions.obj` remains hash-bound by its retained audit.

## Restart commands

```bash
git status --short --branch
git diff
python3 scripts/verify-target.py
python3 scripts/check-ida-mcp.py
python3 scripts/validate-tracking.py --require-target
python3 scripts/report-reconstruction-status.py
python3 -B .analysis/gpt-6.1-sol-setup-callback-phase-20261009/retained.py
python3 -B .analysis/gpt-6.1-sol-enemy-vector-storage-20261009/retained.py
python3 -B .analysis/gpt-6.1-sol-replay-row-arguments-20261009/retained.py
python3 -B .analysis/gpt-6.1-sol-service-workspaces-20261009/audit.py --retained-only
python3 scripts/inspect-supervisor-service.py build/gpt-dots-service-packet-owner-20261007/baseline.obj
python3 -B .analysis/gpt-6.1-sol-ecl-receiver-ownership-20261009/audit.py --retained-only
python3 -B .analysis/gpt-6.1-sol-result-entry-dependencies-20261009/closure.py
python3 scripts/inspect-title-result-draw.py build/gpt-6.1-sol-result-difficulty-20261009/canonical.obj
python3 scripts/inspect-supervisor-update.py build/gpt-6.1-sol-supervisor-entry-context-20261009/binary-title-flag-bit.obj
python3 -B .analysis/gpt-6.1-sol-replay-depth-workspace-20261009/audit.py --retained-only
python3 -B .analysis/gpt-6.1-sol-ecl-jump-timer-20261009/audit.py --retained-only
python3 -B .analysis/gpt-6.1-sol-enemy-draw-expression-20261009/audit.py --retained-only
python3 -B .analysis/gpt-6.1-sol-draw1422-callee-context-20261009/audit.py --retained-only
python3 -B .analysis/gpt-6.1-sol-pause-partial-visibility-20261009/audit.py --retained-only
python3 scripts/inspect-pause-menu.py
python3 -B .analysis/gpt-6.1-sol-enemy-object-copies-20261009/audit.py --retained-only
python3 -B .analysis/gpt-6.1-sol-enemy-object-copies-20261009/replay.py --retained-only
python3 -B .analysis/gpt-6.1-sol-gameplay-helper-visibility-20261009/audit.py --retained-only
python3 -B .analysis/gpt-6.1-sol-setup-input-array-20261009/audit.py --retained-only
python3 -B .analysis/gpt-6.1-sol-ecl-flat-local-scope-20261009/audit.py --retained-only
python3 scripts/report-ecl-codegen.py build/gpt-6.1-sol-type21-preheader-20261009/canonical.obj
python3 scripts/inspect-ecl-complete.py build/gpt-6.1-sol-type21-preheader-20261009/canonical.obj
python3 -B .analysis/gpt-6.1-sol-type21-preheader-20261009/audit.py --canonical-only
python3 -B .analysis/gpt-6.1-sol-charge-timer-conversion-20261009/audit.py --retained-only
python3 scripts/inspect-player-charge-text.py build/gpt-6.1-sol-charge-timer-conversion-20261009/canonical.obj
```

Call direct IDA get_metadata with exactly {} during entry attestation. Complete
RunEcl and Supervisor inspections return 1 for expected differences. Full candidate
replay requires rebuilt objects and a fresh trial manifest; retained-only replay verifies captured
proof. Earlier compact evidence and source corrections remain in the KB and Git.

Validation: complete carrier/target proof, strict affected siblings, tracking,
progress, isolated CI and whitespace pass. CI runs 83 tests: 72 pass and eleven
optional Capstone checks skip. Local checkpoints are not pushed. The 95% objective
and native product/runtime/later phase gates remain open.
