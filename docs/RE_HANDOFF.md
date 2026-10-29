# TH09 reconstruction — active handoff

> Current routing only. Use the live Git tree and ledgers before historical
> numbers or hypotheses. Earlier details remain in
> [the historical checkpoint archive](RE_HANDOFF_CHECKPOINT_ARCHIVE_20261018.md),
> [`KNOWLEDGE_BASE.md`](KNOWLEDGE_BASE.md) and Git history.

## Active local restart — October 29

The operator explicitly resumed reconstruction with direct local tools, focused
affected-owner replays and commits prefixed `gpt-6.1-sol:`. The latest routing is
**direct local Ghidra, without IDA, REA or Factory MCP**. This supersedes the
historical stop and tool routing below. Original Japanese v1.50a disk identity
and tracking pass. Ghidra supplements the original PE and pinned VC7.1 Oracles;
it does not grant source exactness credit.

### Direct Ghidra project

`python3 -B scripts/ghidra-analysis.py` imports the original once, then reuses its
saved project with `-noanalysis -readOnly`. Select functions with repeated
`--function 0x...` arguments; the default selects RunEcl, Enemy OnUpdate and
Type3. Use `--ghidra-dir /path/to/ghidra` or the ignored `.tools/ghidra` symlink.
The current direct Ghidra12.1.4 project and exports are below
`.analysis/ghidra-direct/`; `direct-import/` holds the initial three-function
export and `reused-type3/` proves cached export without another import/analysis.

Every invocation checks disk/Program SHA-256, i386 language, image base and entry,
then compares all685056 file-backed PE bytes in headers and five sections before
analysis and export. Tool and script hashes bind the receipts. Native projects,
decompiler text and logs stay ignored. Ghidra rejects hidden components in project
URLs, so the runner uses a temporary visible symlink to the actual cached project;
the database remains below `.analysis/`. Automatic types/boundaries are provisional:
RunEcl has14777 body-set bytes over14792 code-span bytes, Enemy3881 over3883,
and Type3 has780 contiguous bytes. Keep the original reviewed physical owners,
including padding and tables, for compiler comparison.

ReplayInput now uses the four existing `GameManagerModeView` predicates instead
of undefined private Replay method facades. Its data overlay and original
receiver remain unchanged. The previously unadopted H65 source was recovered;
SafeSEH `.sxdata` entries are compared as actual COFF symbol references, with
handler section/value/type/storage identity and code checked. Both canonical
Replay profiles were rebuilt. All **16 affected exact owners / 4,150 physical
bytes / 232 fields** pass, including the four native mode leaves. All nondebug
sections retain their effective contents; only ten manifest symbol strings
change, with no address, offset, addend or profile changes. SaveReplay remains
**1,238 bytes / 53 fields / three differences**; BeginPlaybackStage remains
**538 versus 540 bytes / 32 fields / 39 overlapping differences**. Exact credit
remains **244917/291883 (83.9093%)**, 1003 functions. Forty focused Replay tests
pass. No whole-product/runtime or original-exact EH claim follows.

Evidence and no-compile replay:
`.analysis/gpt-61sol-20261029-replay-mode-adoption/verify_current.py` and
`current-verification.json`; `check_sxdata.py` rejects four metadata/code
corruptions. The original H65 packet and failed verification remain preserved.
A fresh Enemy main-carrier build independently retains **3900/97/13**, 69 calls
and 170 blocks; its diagnostic is in
`.analysis/gpt-61sol-20261029-enemy-live/baseline-enemy.json`.
RunEcl and Enemy remain the principal unfinished fronts; the October 24 source
and receipt bindings below are historical baselines, not the latest source.

### Type29 constructor lifetime study after 962ddb20

Four bounded main-profile builds compare the current Type29 draw with three
ordinary geometric constructor models. Derived and composed 12-byte centers
inline completely and produce identical 2205-byte/129-field owners, but increase
overlap differences from1170 to1238. Owning the angle as well gives2204/129/1431.
None recovers the original six center receiver reloads; all three fail the
unchanged full-owner guard. No game source or profile is adopted. All75 existing
same-profile exact owners retain22463 physical bytes/990 fields in every case,
with actual literal definitions checked; RunEcl30 and Enemy13 also remain.
The source-bound packet is
`.analysis/gpt-61sol-20261029-effect29-witness/`; `verify.py` rechecks its full
objects, original-bound comparisons and source/provider hashes without compiling.
It retains the tested source recipes and native outputs. This closes these
constructor hypotheses, not the Type29 owner or the reconstruction goal.

### Enemy/Front native indicator connection

Enemy now calls the implemented FrontEnemyIndicatorView member through the
shared g_PlayerSharedRuntime pointer, removing its undefined private Front
method/global declarations and two inspector aliases. The shared header uses
the existing Float3 for the same twelve-byte input/position. Front's complete
213-byte/10-field owner remains exact. Both ECL profiles plus Front cold-replay
all80 existing exact owners:25428 physical bytes/1136 fields. All203 nondebug
main-carrier sections retain50452 raw bytes/2097 effective relocation identities;
five API names and nine private-label references change spelling only. Two
canonical private labels were refreshed after their actual coordinates passed.
RunEcl remains15564/598/30; Enemy3900/97/13; no new exact credit is claimed.

Nine bounded Enemy controls preserve75 exact siblings each. Explicit count
branches/store operations leave opaque13 or manager/periodic41 unchanged. A
current-frame snapshot preserves the exact33-byte periodic callee and caller45;
early-return/local-result alternatives emit31/37-byte callees and are ineligible.
Direct Ghidra independently confirms the native indicator and call paths. Its
missing Effect AddedCallback entry was created only in a read-only session from
the reviewed51-byte hash/extent; every mapped file byte was rechecked afterward.
The packet is .analysis/gpt-61sol-20261029-enemy-periodic-selection/;
verify_adoption.py replays the current complete proofs without compiling, and
verify_cases.py rechecks the archived experiments.148 focused tests pass.

### Replay native RNG connection and representation controls

Replay now uses the implemented RngRuntimeView range generator and shared
global instead of the undefined private int-return facade. Actual seed methods
remain behind their existing layout-checked projection. The unused declaration
is removed. Five affected canonical carriers preserve25 existing exact owners,
4553 physical bytes/257 fields, including the complete41-byte range generator
and39-byte NextU16. Both full Replay objects retain all effective nondebug
contents; only nine API references per profile and private EH labels rename.
SaveReplay remains1238/53/3 and BeginPlaybackStage538/540/39. Shared-header
network coverage retains808/46/105. The current proof/replay is
.analysis/gpt-61sol-20261029-replay-native-rng/verify_current.py;57 focused
tests pass. No new exact credit or full product closure is claimed.

Seventeen bounded serial compiles after a4a3cd13 test SaveReplay address/byte
operations and the213-byte received-frame leaf. Eight Replay source models
repeat its complete1238/53/3 baseline; signed-byte masking gives1246 bytes and
a64-bit accumulator1289, both rejected. All22 existing same-profile exact-owner
comparisons pass. Five queue copy models repeat every213-byte baseline byte
and35 differences. No game source/profile/ledger is adopted. The two packets
are .analysis/gpt-61sol-20261029-replay-byte-addresses/ and
.analysis/gpt-61sol-20261029-queue-representation/; each has verify.py, frozen
inputs and complete COFF. Their verify.py scripts use the preserved a4a3cd13
source/provider epoch, not the newly adopted RNG declarations. The direct
Ghidra exports are under .analysis/ghidra-direct/replay-save-20261029/,
frame-insert-20261029/ and replay-native-rng-20261029/.16 focused tests passed
before adoption; all17 archived observations replay afterward.

## Historical operator stop — October 24, 14:09 UTC

TH09 research is paused at the operator's explicit request. Only cleanup and
a local commit were requested; the old autonomous continuation instructions
are superseded. Do not start new experiments, adoption or monitoring without a
new request. The 98% goal remains unfinished.

The only pending tracked source edit, ReplayInput.cpp, was preserved with its
patch and hashes under .analysis/th09-replay-native-mode-api-20261024-v01/
canonical-adoption/ and restored byte-for-byte to HEAD. No manifest, inspector
or test proposal was applied. Its two canonical captures compiled, but adoption
verification stopped at .sxdata raw symbol indices. A read-only observation
resolved those indices to matching handlers; no corrected acceptance verifier
was completed. This remains an unadopted experiment, not a source checkpoint.

Maintained gameplay/API source remains 1bccbf2a; source and exactness credit are
unchanged. H65/H66 below record the final isolated results. All workers stopped.
Original target/toolchains, unique objects, logs, failed gates and proposal
artifacts are retained; no blanket .analysis cleanup or proof deletion occurred.
Final closeout CI passes 1020 tests / 29 skips in 124.831 seconds, durable
exit 0 (.analysis/gpt-dots-20261024-continuation/ci-final-pause.log and .exit).
Tracking, documentation, open build graph, progress and whitespace checks pass.

## Current verified source checkpoint

The operator resumed reconstruction through Factory MCP on October 24 after
the clean successor checkpoint95300f57. Focus on RunEcl and Enemy, including
whole-function/shared-type/translation-unit hypotheses rather than only local
residuals. Disk target and six-sample mapped IDA attestation passed on this
resumption. The following predecessor measurements remain the starting baseline;
new attempts must be independently compiled and fully compared. Current work
is tracked independently in [the fresh hypothesis checklist](TH09_BOSS_HYPOTHESES_20261024.md),
not inferred from historical conclusions. Use AGENTS.md
for current routing and commit identity. Historical handoff cleanup is complete.

- Live exact credit is **244917/291883 (83.9093%)**, another **41129** whole
  exact bytes short of98%. This API-only source cleanup changes no compiler
  profile or exactness ledger. Native product/runtime and later gates remain open.
- Current maintained source is **1bccbf2a9376ba929cdaa71bc488c5e6b02bfdde**,
  the minimal H53 real Effect vector API checkpoint (local, not pushed);
  Documentation checkpoint **e3f3b9d6** adds no source/profile/ledger change.
  **22baf4e7** is its predecessor. It preserves the selected-ECL-manager ownership repair from **f595786** and adds
  the independently verified SDK resolver and effect-release API corrections.
  Preserve `.analysis/th09-selected-ecl-manager-owner-20261023-v01/`, especially
  its34 canonical objects and sidecars. Its140-unit adoption proof is scoped
  to its source/profile bindings; unchanged proofs may be reused after checking.
- RunEcl remains15564 physical bytes/598 fields/30 differences, split7/23 between
  opcodes4/155. Enemy remains3900/97/13, with69 calls and170 blocks. Preserve all
  existing matching bytes; natural source hypotheses still require full owners.
- Preserved predecessor packet: `.analysis/gpt-61sol-20261024-ecl-math-boundary/`.
  `verify.py` replays its archive without compiling:378 attempts,366 passing
  existing-body comparisons,6 expected moved-definition absences and6 actual
  callback regressions. Hiding all Float3 core definitions damages Type4/8 by55
  bytes each; constructor-only damages36, independently cold-reproduced.
- The opcode155 successor audit supports no invented old-flags use or Timer
  return contract. Real rank-reset visibility removes Enemy's original+CC3
  receiver reload. Read these dated KB observations before repeating models;
  they do not identify unique original source partition or exhaust alternatives.
- Cleanup/recovery instructions and exact retained/deleted scopes are in
  [cleanup history](RE_ARTIFACT_CLEANUP_HISTORY.md). The current audit packet is
  `.analysis/gpt-61sol-20261024-clean-handoff/`; ignored evidence must accompany
  the worktree at transfer. Keep original target, toolchains, SDK/Wine/IDA state
  and unique proofs; build caches may be absent and can be rebuilt.
- Cleanup archives32812 historical snapshot files into five verified tar.xz
  files and removes134 regenerated bytecode caches. Combined source/Git file
  allocation falls by about264MiB. All802 non-documentation inputs,14 existing
  build files,34 adopted objects and original target remain unchanged. CI passes
  1016 tests/29 skips; archive-only integrity and an actual restore both pass.

## Current source-interface correction

The October24 continued campaign replaces the isolated ECL resolver's private
vector Length member view with the actual SDK D3DXVec3Length API and existing
Float3 subtraction. Four resolvers remain original-exact:5718 physical bytes and
374 fields. Canonical-path replay refreshes only6 external symbol identities and
291 proved same-coordinate private labels, never target bytes or destinations.
The synthetic co-visible resolver model is rejected; EclManager.cpp is unchanged.
Evidence and current validation state are in H20 of the independent checklist
and .analysis/th09-ecl-sdk-vector-interface-20261024-v01/live-adoption/.
No exactness gain is claimed; RunEcl30 and Enemy13 remain active frontiers.
Earlier experiment packets are source/provider-scoped historical evidence and
may correctly reject current providers after this narrow manifest update.

The subsequent API-only correction directs Enemy attached-effect cleanup to the
existing64-byte Th09EclRunState::ReleaseAttachedEffects implementation instead
of an undefined Core facade. Definition visibility is unchanged. Two individually
proved private COFF label names are refreshed; target addresses/profiles and
comparison rules remain unchanged. Cold Enemy gate passes87 canonical siblings,
RunEcl30/Enemy13 remain. H30 records the current full-CI/checkpoint state.

## Historical continuation results — paused

Earlier continuation authorization is superseded by the explicit stop above.
Current exactness remains RunEcl 30 / Enemy 13 differences; no new
whole-owner credit was gained by the two API corrections. Latest source CI
passes1020 tests with29 skips in133.873 seconds, durable exit0. Strict H53
current-manifest replay passes53 jobs/208 proofs with zero symbol overrides or
local renames; all139 main owners remain unchanged at RunEcl30/Enemy13. Tracking,
progress, documentation, open build graph and whitespace checks pass. This is
the narrow source checkpoint following22baf4e7 and docs3acea9a8; its durable
verified-source-state receipt will bind the commit after creation.

- H31 common Enemy/operand graph and H32 Replay packed-frame pair are complete,
  independently verified negative results, not active preparations. H31 is
  paired-neutral at RunEcl42 / Enemy13 (live remains30 /13); H32 preserves the
  same three SaveReplay residuals. Full affected-header adoption is not claimed.
- H33 explicit descriptor initialization is neutral. H34 reference flag family
  changes whole-owner shape without repairing the residuals. H35 ordinary member
  flags introduce11 unmatched runtime calls and are rejected. All remain isolated;
  stock guards and current source/profile/test inputs remain unchanged.
- H36 current full-macro pair is independently verified: /Ob1 preserves75 exact
  witnesses but has15496-byte RunEcl/368 direct calls; /Ob0 has15540 bytes and the
 375-call destination multiset but fails15 witnesses. Neither fixes opcode4/155;
  counts/multisets are not full CFG or original-byte proof. No profile is adopted.
- H37 wide-wire/flag-successor audits are complete without a staged model: no
  proven four-byte payload extent, original byte reads, no uniquely recovered
  source type from MOVZX, and no actual later old-flag consumer. No compile or
  exactness result is claimed for these audits.
- H38 native-timeout interaction on frozen H36 /Ob0 is complete: same60/15
  witness result,112/113 emitted owners unchanged, neither residual repaired.
  Its inherited15 failures still block adoption. H36 /Ob0 and H38 spawn93/94
  raw paths also replace original integer MOV copies with FLD m32/shared FSTP.
  All-bit-pattern FP equivalence is unproved: signaling NaNs may be quieted and
  x87 invalid/denormal flags may change. This is instruction-semantic analysis,
  not a runtime test or finite-input guarantee, and independently blocks adoption.
  H39 returned-float-base leaf is
  fully neutral across139 emitted owners/50104 bytes/2097 fields;75 witnesses pass.
- H40 conditional five-state wide-native compilation is rejected: all five
  reads are DWORD instead of original BYTE. H37's missing source-extent proof
  remains true; this was a conditional compiler test, not source-memory
  equivalence or permission to adopt/run widened reads. H41 original metadata
  and Enemy/jump use-chain audits are complete without a new source model.
- H42 whole-TU /Ob0 recovers all15 inherited witnesses to75/75 and genuine
  opcode64/88 private caller transport. Of95 surviving owners,90 match maintained;
  four other nonexact owners remain changed. H43 CRT-retained RunEcl macros /Ob1
  also gives75/75 with all114 surviving non-RunEcl owners unchanged. Both have
  RunEcl15544/598/375 direct calls but remain nonexact; neither proves unique
  original profile choice, and both retain the raw-spawn FP semantic mismatch.
- H44 branch-owned spawn transfer is verified:15572/598, same60/15 witnesses,
  all112 other parent owners neutral. H45 composition on H43 /Ob1 is verified:
 15576/598, all75 witnesses and114 other surviving owners maintained-identical.
  Both restore six raw integer-MOV paths and six original-shaped resolver arms.
  The raw-FP limitation is fixed only in those six spawn transfer paths, not a
  blanket FP-equivalence claim; H42/H43 historical limitations remain. Opcode4
  and opcode155 still differ, strict full-owner guards reject, and no adoption
  or exactness credit follows.
- H46 removes two redundant integer-prvalue casts and is fully neutral across
 115 owners/2089 fields,75 witnesses passing. H47 uniformly replaces three do0
  procedural shells at12 safe sites: RunEcl15560/598, all75 witnesses and114
  other owners protected. Its943 blocks/root indices/structural shapes match
  original, but full bytes do not; opcode4/155 remain wrong and guards reject.
  The16-byte reduction is7+8+1 bytes of shell-related layout, not a padding patch
  or proved compiler-IR mechanism. Remaining slice lengths differ140−3/145+1/
 157−2; equal-size regions also differ. Four bytes short is not four mismatches.
- H48 existing four value-read APIs give15564/598 but include two padding bytes:
  stock identity binding passes, equality fails with4147 shifted differences.
  Independent H47/H48 maps bind all598 fields with zero unknowns,943 blocks and
 375 calls. H48 retains six changed regions4/135/139/140/145/155 versus H47 seven;
  only157 disappears from the instruction map, not newly exact owner credit.
- H49 WriteInt/Float retention is neutral across117 previous owners, plus one
  maintained WriteInt COMDAT (118 total), with no synthetic API calls. H50 native
  timeout changes only RunEcl among117 owners:75 witnesses pass,15564/598 but4139
  shifted differences;44-byte opcode155 still has wrong registers. All byte wire
  reads and six raw spawn transfers stay protected. No candidate is adopted.
- H51 State/Late family closes neutral across117 owners/75 witnesses, with byte
  wire and DWORD flag accesses protected. H52 module closure fixes none of13
  Enemy differences and removes the required+CC3 receiver reload; rejected.
  H54 seven raw copies are neutral across139 owners, with seventh-site abstract
  memcpy nonoverlap still unproved. H55 draw SDK/return audit finds no supported
  new mismatch or candidate; missing fresh-object proofs remain explicit.
- H53 three-arm Effect vector study is neutral. This checkpoint adopts only
  three real Float3 += API sites, unused POD facade declaration cleanup and
  three regression tests, not storage aliases or added header co-visibility.
  Strict post-refresh current-manifest53/208 proofs pass with zero overrides or
  local renames. Actual365 symbol-string changes are362 DIR32 same-owner labels
  plus three REL32 API identities; targets/offsets/addends/profiles stay unchanged.
  Twenty initially misattributed staged labels were corrected as introduced.
  CRLF introduced by git apply was rejected; exact reviewed LF source and original
  binding were restored before canonical compilation, without hash waiver.
  Historical failed guards/staged maps remain recorded, not used as exemptions.
  Two unprofiled TUs, native product/runtime and portability remain outside this
  canonical-profile closure. There is no exactness gain.
- H56 byte-flag census confirms exact ANM438DC9 byteArgs[1] assignment without
  later old-word use. Faithful direct compound preserves75 witnesses/137 other
  owners and exactly reproduces H15a's full raw/598-field emitted class:15564
  bytes,2048 ordinary/242 field shifted differences, no adoption. Removed helper
  is16 bytes, not the initial24-byte report; correction note and separate seal
  are complete, with the pre-correction seal retained.
  Oct17 missing break invalidates only that recipe, not every previous model.
- H57 direct-byte acquisition pair is complete:138 owners/2097 fields pair-neutral,
 75 witnesses per arm, inherited native-timeout differences remain. H58 anonymous
  primary owner is neutral across the same138 owners and75 witnesses. Neither
  grants portable lifetime/source uniqueness or exactness credit.
- H59 direct raw-member pair preserves135 common owners/2097 fields; four
  uncalled5-byte free accessors become uncalled14-byte members, zero references
  to them. Both75-witness passes and full RunEcl30/Enemy13 remain unchanged.
  Five-TU header closure is uncompiled, not needed for an unpromoted neutral model.
- H60 shared signed role is complete: 138 other owners remain neutral and 75
  witnesses pass, but RunEcl regresses to the already verified H18 class. Full
  raw bytes and all 598 effective references agree with H18; opcode 157 changes,
  while opcode 4/155 do not improve. This is not a new emitted solution.
- H61 direct single-use signed consumers is complete and fully neutral across
  139 owners / 50104 bytes / 2097 fields, with 75 canonical witnesses passing.
  The transform index and initial callback test are intentionally direct; later
  fresh callback capture and repeated sampleStride remain. RunEcl 30 / Enemy 13.
- H62 four-State low-bit publication pair is complete. Both arms pass 75
  canonical witnesses and are identical across 138 owners / 50088 bytes / 2097
  effective fields. Both inherit the native-timeout regression: 2140 linked
  differences = 1938 ordinary + 202 field bytes; the 26 table bytes are a subset,
  not additional. The unused 16-byte timeout helper is the only missing owner.
  The boolean RHS family adds no further change; maintained stays 30 / 13.
- H63 guarded shared jump body is fully neutral across all 139 owners and
  effective fields, with 75 canonical witnesses passing. The original op5
  prefix's full 50 bytes and two call fields match; direct case4 bypass, positive
  fallthrough, nonpositive advance and time/call/displacement chronology are
  preserved. No original source-style inference or exactness credit follows.
- H64 complete signed-role jump capture is rejected: 2165 shifted differences
  versus H60's2153 and maintained30, with 75 witnesses and138 other owners neutral.
  Exactly12 bytes differ from H60, all in opcode7; its full75-byte slice/two fields
  equal actual H14 output. All598 H60-relative effective fields agree. Actual
  four-byte copy lowers without escaping scratch address or memcpy call, and
  direct4/positive5 timer/displacement chronology remains. Neither boss improves.
- H65 Replay native-mode API experiment is neutral: all 32 emitted-owner pairs,
  24 canonical comparisons and four native callees pass. SaveReplay retains
  three differences; BeginPlayback retains its full prior mismatch boundary.
  Canonical adoption was not completed; the source edit is restored as above.
- H66 persistent trailing operand-record pair is neutral across 139 owners /
  2097 fields per arm, 75 witnesses each. RunEcl30/Enemy13 remain. The legacy
  extent/lifetime hypothesis and uncompiled five-TU/87-unit closure are not
  promoted. The independent deferred-shot audit was not completed.
- There is no active next task. Resume only on a new explicit operator request.
- Lossless archival of116 compiler PDBs passed full restores;1046 non-PDB files
  stayed unchanged, net allocated saving8548352 bytes. Postarchive strict53/208
  replay passes. See cleanup history; no fresh CI claim for this docs-only batch.
- The fresh checklist records H01-H66 and source-bound packets. Earlier provider
  hashes are intentionally historical; never update them blindly to current HEAD.
  Earlier source-bound CI1017/29 and documentation CI1017/29 (130.450 seconds)
  remain historical receipts, superseded for current validation by H53's1020/29.
- Superseded H31 v01 loose source snapshots are now preserved in a verified
 232628-byte tar.xz archive (522 files, actual full restore tested). v02 and all
  accepted evidence remain untouched; see cleanup history for the exact receipt.

Latest H38-H47 documentation validation: fresh full CI1017/29 passes in123.402
seconds, exit0. Only two documentation files change; maintained source22baf4e7
and exact credit stayed unchanged. That historical checkpoint is3acea9a8.
Current H53 source/API adoption and1020/29 validation supersede its active state;
chronological experiment evidence and unchanged exact credit are preserved.

## Live ledger snapshot


The canonical state is `config/functions.csv`, `config/function-origins.csv`,
`config/matches.csv`, and `config/match-units.toml`, checked by
`python3 scripts/validate-tracking.py --require-target` and
`python3 scripts/validate-docs.py`. The current restart summary is
**1035 source-present / 1003 exact / 32 non-exact**; earlier operator notes
mentioning 979/876/103 are obsolete for this checkout.

| Measure | Count |
| --- | ---: |
| Function candidates | 2,248 |
| Boundary/origin unreviewed | 0 |
| Reviewed but origin-unresolved | 37 |
| Confirmed authored | 1,035 |
| Classified exclusions | 1,176 |
| Source-present authored mappings | 1,035 |
| Canonical exact functions | 1,003 |
| Source-present non-exact functions | 32 |
| Source-present non-exact bytes | 46,966 |
| Authored without maintained source | 0 |
| Canonical exact authored bytes | 244,917 |

**244,917 / 291,883 reviewed-authored bytes = 83.9093% exact**. To reach
98% of the *currently classified* denominator, another **41,129 bytes**
must become complete original-bound exact owners, reaching at least **286,046
exact authored bytes**. The 37 origin-unresolved functions are excluded and could change the denominator. Similarity fractions
and locally matching subsequences never count as accepted exact.

The catalog is not proof of complete executable-code inventory. The formerly
uncatalogued **0x0040D677 /0x00444A78** Timer references now belong to the exact
Effect 1/2 and Effect 34 updates. All **203** captured assignment callsites have
catalogued owners; that bounded census proves neither complete executable
coverage nor a return ABI. The Effect 34 **178-byte initializer at 0x00444960** is now exact; its
**946-byte draw at 0x00444AF0** is a maintained, explicitly nonexact frontier.
Do not silently treat uncatalogued code as excluded.

## Active frontiers

Code entrypoints: `src/EclManager.cpp` assembles RunEcl's lexical fragments and
`src/EnemyManagerCore.inl`; `src/EclRunControl.inl` and `src/EclRunState.inl`
contain the remaining opcode4/155 logic. The other principal entries are `src/TitleScreenResultDraw.cpp` and
`src/ExAttackUpdateType3.cpp`; canonical
source/profile bindings are in `config/match-units.toml`. Inspectors and replay
selection live in `scripts/check-boss-frontiers.py` and `scripts/inspect-*.py`.
Detailed prior table wording is preserved in commit77e0b2c2 and the KB.

Prioritize **key large functions**; small leaves remain in scope. Each row
refers to a maintained, authored, **non-exact** function in `config/functions.csv`.
Comparisons are current-source/original PE cold baselines; lengths are logical
unless explicitly marked physical. The ledger, not this selected table, lists
all32 maintained nonexact functions, including the2,220-byte authored Type29
draw whose maintained candidate remains nonexact.
For the complete <=256-byte cohort see
[`SMALL_FUNCTION_FRONTIER.md`](SMALL_FUNCTION_FRONTIER.md).

The isolated SetCurrent/assignment context lead has now been cold-tested:
setter-only retains30; both real bodies before/after RunEcl shorten the caller
to15560 and fail the complete owner. Do not reopen it as an untested hypothesis.
Earlier related noinline/all-method controls were already recorded in the KB;
these results do not establish unique original translation-unit ownership.

The earlier whole public/State/Core Enemy-storage model was byte-neutral
across 139 emitted functions and remains unadopted. The distinct narrow
selected-ECL-manager correction was adopted in f595786, with all 140 canonical
collateral units verified. RunEcl30 and Enemy13 remain unresolved. Earlier
context/adjacent-game audit links are historical evidence, not pending ownership
work. See the current checkpoint above and the dated knowledge-base entries.

| Owner | Target address | Current comparison | Remaining work |
| --- | --- | --- | --- |
| **RunEcl** | 0x004086C0 | **15,564 physical** (14,792 code + 772 table); 598 bound fields; **30** differing non-field bytes | Opcode4 timer transport and opcode155 flag update. Keep all598 fields/tables and complete15564 bytes; actual Timer ABI and RNG ownership are already corrected. `--owner runecl`. |
| **EnemyManager::OnUpdate** | 0x00410730 | **3,900 physical**,97 fields, 69 calls,170 blocks; **13 differing bytes** (29->13) | Draw index2, side/threshold4, descriptor-position copy7. Keep real20-byte Timer greater co-visible, all97 fields and3887 matching physical bytes. |
| **Effect Type29 draw** | 0x00448750 | **2,220 /2,205 bytes**,129 original referents,72 ordered calls,41/41 physical blocks; **1170 overlap differences +15 missing** | Main/satellite fan chronology, polar receiver reloads and ordinal graph. Bounded emulator fixtures are not whole-runtime or exactness proof. `--owner effect29`. |
| **Effect 34 draw** | 0x00444AF0 | **946/946 physical**,53 original referents,20 direct +12 COM calls,286 instructions,16 blocks; **78 linked differences** | Entry65, store order9 and radius operands4 remain. Preserve868 matching bytes, all53 fields, actual float data and full physical extent. `--owner effect34`. |
| **EtamaController::OnDraw** | 0x00413BE0 | **600/600 physical**, 27 fields, 13 calls, 17 blocks; **268** differences | Laser body-VM position cursor. Preserve332 matching bytes and the genuine final DrawBulletLayerEffects member identity. `--owner etama`. |
| **PauseMenu::OnUpdate** | 0x00434740 | **1,776 original physical / 1,732 candidate**; 46 direct calls plus 2/3 original/candidate timestamp calls; 10-way switch; **1,453** overlapping differences + 44 missing | Share timestamp tail for states3/8/9 and recover branch/VM scheduling. Keep real UnPause and existing exact siblings. `--owner pause`. |
| **Player movement boss** | 0x0041C170 | **1,900/1,900 physical**, 66 fields, 23 calls, **129** differing bytes | Lower-bound EAX/EDX transport and encoding cascade. Native framerate/coordinate owners are corrected; preserve1771 matching bytes and both tables. `--owner movement`. |
| **Player charge update** | 0x0041F810 | **1,210 target /1,197 candidate bytes**, 70 fields, 58 calls, 350/342 instructions; **1,160 overlap differences +13 missing** | Recover source chronology and missing13 bytes; preserve repaired SpawnShots transport and actual call/field identities. |
| **Front weighted message RNG** | 0x004162D0 | **693 target / 681 candidate**, 15 fields, 6 calls, **569** overlap + 12 missing | Primary/opposing table selection and remaining control flow; exact FrontCalc sibling remains protected. `--owner front`. |
| **Front added initialization** | 0x00418480 | **706/706**, 37 original fields, 21 calls, 19 equal blocks; **222 differences** (236→222) | Receiver and panel-store scheduling; preserve484 matching bytes and all native GameManager/ANM identities. `--owner frontadded`. |
| **TitleScreen::OnUpdateResult** | 0x004266B5 | **485 / 484**, 147/147 instructions, 34 fields, 9 calls; **218** overlap + 1 missing | Character snapshot and remaining control-flow/byte deficit. Keep the exact Title setup siblings. |
| **TitleScreen::DrawReplayMenu** | 0x004234F6 | **1,280/1,280 bytes**,376/376 instructions,57 original-bound fields,13 ordered calls,72 matching direct blocks; **29 differences** | Menu bank/record chronology. Preserve all57 fields,13 calls,72 blocks and the full original-bound residual guard. `--owner replaymenu`. |
| **TitleScreen::DrawReplaySave** | 0x004239F6 | **800/800 physical**, 245 instructions, 41 fields, 4 calls, 28/28 matching blocks; **96 linked differences** | VM/bank transport and scheduling. Keep245 instructions,41 fields and28 physical blocks. |
| **TitleScreen::DrawResult** | 0x00423D16 | **1,939/1,939**, 82 fields, 16 calls, 92 blocks; **22** differences | Record/bank/currentScreen lifetimes. Existing source/visibility models are bounded negatives; all1917 matching bytes remain protected. `--owner title`. |
| **TitleScreen::OnUpdateOptions** | 0x004276EB | **2,045 / 2,048**, 135 fields, 51 calls, 121 blocks | Options-menu loop and missing bytes; keep complete original-bound field/call/graph checks. `--owner title`. |
| **Title character cursors** | 0x00424EDD / 0x00424FB0 / 0x0042505F | Active target/candidate **211/215**; Reverse/Inactive **175/175** | Active cursor size and cursor state/transport; all three remain nonexact despite Reverse/Inactive having equal physical lengths. `--owner title`. |
| **ExAttack Type3 update** | 0x00442750 | **780/780**, 38 fields, 24 calls, 17 blocks; **15 maintained differences** | Middle-Z chronology; preserve765 matching bytes. Actual Timer visibility and native vector interfaces do not resolve15. `--owner type3`. |
| **ExAttack Type14/22 draw** | 0x00444130 | **959/959**,53 original referents,20 direct +12 COM calls,290/291 instructions; **525 linked differences** | Frame20 versus1C, polar/store scheduling and physical jumps9/10. Preserve434 matching bytes,53 fields and actual scalar/helper payloads. `--owner exattack`. |
| **GameManager gameplay setup worker** | 0x0041AF2D | **1,689/1,689**, 172/171 fields, 35 calls, 78/77 blocks; **904 differing bytes** (911→904, no damaged matches) | Side registration and shared limit/rate publication. Keep native creator calls and164 aligned identities; B4 multiplicity reflects branch sharing. |
| **Player CheckBulletCollision** | 0x0041DFF0 | **862 / 849**, 26 fields, 12 calls; **804** overlap + 13 missing | Region producer, activeB100 pool and missing physical bytes. Preserve five same-source exact helpers and separate CalcItemBox/FindCollision proofs. `--owner player`. |
| **Replay SaveReplay** | 0x00420C60 | **1,238/1,238 bytes**, 380 instructions, 53 fields, 23 call operands, 49 matching blocks; **3 differences** (152→3) | Frame-destination register at+B7/+BB and checksum SIB at+3F6. Preserve1235 matching bytes and all12 exact same-source siblings. `--owner replaysave`. |
| **Replay BeginPlaybackStage** | 0x00420840 | **540 / 538**, 152/151 instructions, **32/32 ordered original PE/COFF fields**, 5 calls, 12/12 matching CFG blocks; **39** overlapping unlike bytes + 2 missing (49→39) | Seed/tail chronology and two missing bytes. All32 ordered referents and first30 physical field positions are already restored. `--owner replay`. |
| **ScreenEffect shake envelope** | 0x004227A0 | **336/336**, 132 instructions, 20 fields, 8 calls, 26 blocks; **26** differences (28→26) | EDI/ESI owner allocation. Preserve310 matching bytes; timer-float visibility regresses physical size and x87 lifetime. |
| **Controller input** | 0x0042B410 | **808 / 796**, 32 fields; **102** overlap differences /12 missing bytes | WinMM midpoint/range CSE and register chronology after the674-byte matching prefix. Preserve all32 fields and native global identities. |
| **Supervisor network HandleMessage** | 0x00432B70 | **768 authored /808 physical bytes**, 46 fields, 17 direct +2 virtual calls, 34 matching blocks; **105 differences** (125→111→105) | Packed-input/frame scheduling. Keep all six switch targets, map bytes and703 matching physical bytes. `--owner netmessage`. |
| **Supervisor network service** | 0x00430AA0 | **1,633 / 1,632**, 108 source fields | Queue ownership and1395 overlapping differences. Keep original referents and exact queue siblings independent. `--owner network`. |
| **Supervisor::OnUpdate** | 0x00431110 | **844 authored / 996 target physical**, 952 candidate physical | Whole-owner physical deficit, startup chronology and remaining GameManager registration alias debt. Keep real Title/Ending symbols and version data. `--owner supervisor`. |
| **ECL MoveRandomBiased leaf** | 0x00407E30 | **516 / 517**, 28 fields, 14 calls, 24 blocks; 148 overlap + 1 extra | Y-pointer LEA versus MOV alias after actual Float3 conversion. Keep complete owner, fields, branches and exact ECL cohort. `--owner runecl`. |
| **Effect AddedCallback leaf** | 0x0040D1C0 | **51/51**, 4 fields, 2 calls; **8** differences | Remaining receiver/allocation scheduling. Preserve43 matching bytes and genuine AnmLoaded/Chain interfaces. `--owner effect`. |
| **Supervisor received-frame leaf** | 0x0042E9E0 | **213/213**, 67 instructions, 9 blocks; **35** differences | Packed input/seed16-bit store order. Preserve178 matching physical bytes and both exact queue members. `--owner queue`. |


## Exact landmarks and evidence routing

The manifests govern **1003 exact** functions. Historical Factory receipts
remain in [the accepted-receipts archive](RE_ACCEPTED_RECEIPTS.md), with
immutable source-commit bindings; they do not accept a later HEAD.

- ExAttack Type19 update is **821/821 exact**, with35 original fields, all five
  actual float definitions and the full79-unit ECL closure protected.
- Network-config parser is **243/243 exact**, with its complete **679-byte**
  caller protected by `--owner parser` from one shared source/profile carrier.
- The native Timer integer conversion has a complete four-byte source alias
  at 0x435F00; existing TextHelper::GetHeight keeps canonical authored credit.
- Two pointer conversion interfaces now have complete three-byte source aliases
  for physical 0x4343D0. Its origin and unique class ownership stay unknown;
  alias-byte units carry no authored exact credit.
- GameManager CleanupGameplayState is **425/425 exact**, with all 43 fields
  and both final limit publications. Original high-level return intent remains
  unknown; the separate setup worker remains nonexact.
- Etama `OnUpdate` is2,195 authored/2,216 physical bytes exact. Its distinct
 600-byte `OnDraw` remains nonexact. TitleSetup and MusicRoom update/draw,
 ExAttack Type19/21 initializers and Player initialization are exact.
- Effect/Etama added callbacks share the target-proven `AnmLoaded` pointer at
 +0x04. The shared eight-byte view does not make Effect's51-byte callback exact.
- `audit-float3-conversion-calls.py --json` bounds 27 physical callsites;
  folded helper identity does not prove unique C++ authorship or full inventory.
- `probe-player-position-visibility.py` demonstrates real ECL 249/249 exact
  versus standalone 250 bytes. Preserve actual include routing in a replay.
- Historical63-file ECL staging preserved bytes but renumbered two private labels.
  Frozen replay stages the original eight files and hashes the other 55;
  all 598 literal COFF rows reproduce. The KB retains that routing evidence.

## Restart and verification

Read [`AGENTS.md`](../AGENTS.md), [`RE_WORKFLOW.md`](RE_WORKFLOW.md), and
[`ORACLES.md`](ORACLES.md). Inspect staged, unstaged and untracked work first.
When reconstruction resumes, use one affected owner at a time and reuse
unchanged proofs. Historical countermodels are bounded
negative evidence, not a reason to assume an entire source family is exhausted.

```bash
export PYTHONDONTWRITEBYTECODE=1
git status --short --branch
git diff
git diff --cached
python3 -B scripts/verify-target.py
python3 -B scripts/validate-tracking.py --require-target
python3 -B scripts/report-reconstruction-status.py
# Direct local Ghidra: verify/import once or reuse the target-bound project.
python3 -B scripts/ghidra-analysis.py --function 0x004086c0 --function 0x00410730
# Choose one affected owner and reuse unchanged source/profile-bound proofs:
nice -n 10 python3 -B scripts/check-boss-frontiers.py --owner runecl
nice -n 10 python3 -B scripts/check-boss-frontiers.py --owner enemy
# Timer only: the shared alias plus its 16 authored exact siblings:
python3 -B scripts/check-timer-current-view.py
# Final checkpoint gates; CI precedes reviewed artifact cleanup:
python3 -B scripts/validate-tracking.py
python3 -B scripts/validate-docs.py
python3 -B scripts/progress.py --check
python3 -B scripts/ci.py
git diff --check
python3 -B scripts/clean-generated-artifacts.py  # Preview only; do not apply wholesale.
```

Metadata hashes alone do not establish an unmodified Ghidra mapping. Compare
all file-backed mapped bytes with the verified disk target before analysis and
each export; target identity is not exactness credit. Build a canonical unit
afresh before comparing its COFF; missing OBJ/PDB files are expected after cleanup.

For other large owners use the frontier table; for small leaves use
[SMALL_FUNCTION_FRONTIER.md](SMALL_FUNCTION_FRONTIER.md). Historical experiment
recipes and their exclusions remain in the knowledge base and Git history.
A diagnostic command may exit zero with a nonexact result: inspect full-byte
results, fields and deficits before making an exactness claim.

## Evidence and cleanup boundaries

Use [cleanup history](RE_ARTIFACT_CLEANUP_HISTORY.md) for exact scopes, hashes
and recovery instructions. Original-bound accepted receipts are archived in
[RE_ACCEPTED_RECEIPTS.md](RE_ACCEPTED_RECEIPTS.md) with immutable source commits.
Replay, Enemy and setup-dataflow packets retain their source-bound comparisons,
cleanup/adoption proofs and bounded Oracles. Their identities and storage scopes
are indexed in cleanup history and the knowledge base.

Earlier source-copy archives remain recoverable: read the original packet's
ARCHIVE_README before replay, especially for the October13 RunEcl archive and
October23 command-consumer snapshots. Do not rerun producers over frozen inputs.
Ignored packets must accompany the worktree when handing local evidence over;
tracked source, scripts and KB observations provide the committed restart path.

Keep resources/th09.exe, toolchains, SDKs, Wine prefixes, IDA databases, adopted
source-bound proof objects and unclassified experiments. Build caches may be
absent; rebuild the configured unit/profile before comparison. The generic
cleaner previews a broader scope than the reviewed cleanup: do not apply its
extension-wide .analysis deletion or run git clean -X. No cleanup implies that
a nonexact owner became exact or that the98% milestone was achieved.
