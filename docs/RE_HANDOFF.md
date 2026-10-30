# TH09 reconstruction — current handoff

## Operator status — reconstruction resumed

The operator explicitly resumed reconstruction on October31. Continue the98%
reviewed-authored-byte milestone,prioritizing complete RunEcl and Enemy while
keeping big functions and small leaves in scope. Use direct local Ghidra only,
without IDA,REA or Factory MCP;reuse unchanged proofs and cold-build only affected
sources/profiles. Preserve research records;delete unused old code/build copies.

Pre-resumption game-source checkpoint: **712468128d5d9c8510c3cfe43786811901db4832**.
Current game-source checkpoint: **4502c056**;the previous checkpoint is
**f424b1ac**. ScoreData,GameManager finalization,insertion and result drawing
now share TitleScoreRecordView and the same native table/current-record names.
The known chapter prefix and date fields join the existing0x2C view;unknown
bytes remain unknown and the renderer's signed final byte is preserved. All56
affected existing exact owners pass the strict Oracle;240 unchanged units are
reused,for296 current units. DrawResult remains1939/82/22;no new exact credit.
The previous ScoreData checkpoint replaced its undefined private RNG methods
with the actual RngRuntimeView declaration and unsigned-short range result.
The preceding SHT checkpoint unified the loader,pattern-offset resolver and both
movement switches through PlayerShtFileView's four observed speed fields. It
removed two private SHT classes and one cast helper,without changing SHT
representations,ABIs or compiler profiles. Its200 affected and51 reused units pass the strict
Oracle;48 compiler-private label names refresh at unchanged actual coordinates.
There is no new exactness credit. Movement1900/66/23/129,RunEcl30,Enemy13,
Charge1197 and Type3-15 remain unchanged. The prior network-dataflow,shot-chain,
Charge and tracking repairs remain;network service stays1626/1633 bytes with
all106 referent and51 call-identity multiplicities,1376 unequal overlaps+7 absent.

## Source and evidence routing

- `.analysis/gpt-61sol-20261031-shared-score-record/current.py` is the current
  source-bound subset:296 existing units/87451 physical bytes/4177 fields.
  It strictly compares56 affected owners/23974 bytes/1355 fields across five
  configured source/profile groups,and reuses240 unchanged source/profile/
  object-bound owners. A sixth retained carrier checks DrawResult22. All123
  paired emitted regular bodies/effective fields and complete compiler
  contributions remain equal. Twenty-nine global reference names now identify
  the shared declarations;nine StartMenu private labels refresh only at proven
  unchanged coordinates. Profiles,extents,field targets and exact credit stay.
  All12 baseline/proposal input sets recover from immutable Git/EOL inputs;
  independent Title-O1 and GameManager-Os cold replays pass. Six source-cold
  proposal objects are current after their consumed input hashes equal source.
- Old ScoreData258-unit and SHT251-unit whole verifiers are historical after
  this change:ScoreData's seven and GameManagerSetup's eleven source pins
  changed. Only the unaffected240 SHT units are reused by the current verifier.
  Their records and recipes remain;three superseded current objects are removed
  after complete before/after recording and immutable input reconstruction.
- Current game code is in `src/`;the authoritative state is in
  `config/functions.csv`,`config/function-origins.csv`,`config/matches.csv` and
  `config/match-units.toml`. Use the live report rather than dated measurements.
- Original input is ignored `resources/th09.exe`,Japanese1.50a,SHA-256
  `10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
  Target/toolchains and the working Ghidra project are preserved.
- `src/PlayerEnemyTrackingStateView.hpp` represents Player+30364..3037F:
  nearest-position storage and the separate +3037C homing pointer. Stage D resets
  only the position;it does not reset that pointer. The intervening12 bytes have
  a separate vector reset,their role remains unknown. Type7 calls the existing
  `PlayerAngleView::AngleToPoint` at41BB00.
- `.analysis/gpt-61sol-20261031-player-sht-speed-contract/` retains the
  preceding SHT dependency proof:251 units/66468 physical bytes/3065 fields
  at2b990254. Its original scope was47 configured source/profile groups(200
  units),plus Movement,Charge and Type3 diagnostics,with51 unchanged reuses.
  Fifty carriers were cold-built then;366 paired regular bodies/effective
  fields and an independent Runtime replay passed. Its current GameManager
  objects are now superseded;use the296-unit verifier above for current pins.
- Loader41BBE0 scales normal/focused axis speeds+14/+18 into diagonal+1C/+20;
  exact resolver404710 and movement41C170 read the same loaded SHT pointer at
  Player+30338. The shared field names/offset checks describe these TH09 facts,
  without claiming original class names,full file format or TU ownership.
- Previous network153-unit and shot-chain141-unit combined verifiers are
  historical at43f491a7 because this shared header changes their source pins.
  Their exact owners remain covered by the current296-unit verifier. Network's
  service object itself remains source-unchanged and is checked there. Charge
  stays1197/70/58 with1160 overlaps+13 missing;RunEcl15564/598/30 and Enemy3900/
  97/13 stay. The native Init309,Type7-139 and Sound140/153 owners remain exact.
- Player OnUpdate's field+1BE now references the defined int-returning
  `PlayerChargeAttackView::UpdateChargeAttack`,through its shared header.
  PlayerRuntime uses the actual SoundPlayer API. Both current Runtime profiles
  and Charge retain all46 regular raw bodies/effective fields. Genuine joined
  caller/Charge controls in both orders remain nonexact;no TU claim is adopted.
- Charge and the actual CardAttackStart/ResourceSubtract implementations now
  consume identical complete class definitions. SpawnShots uses its existing
  shared header. All1197 Charge raw bytes/fields remain identical. Actual
  resource-callee visibility shortens Charge to1195/1162+15,while SpawnShots
  visibility is raw-neutral;neither context is adopted as an original TU claim.
- `PlayerShotInitializeView.hpp` and `PlayerSpawnFrameView.hpp` replace seven
  undefined initializer references and the generic dispatcher's undefined frame
  helper reference. All13 changed fields retain their original offsets,kinds,
  addends and targets;the frame owner's symbol also follows its shared types.
  Init309 and Type7-139 keep their distinct canonical /Ob0 profiles. Default
  chain visibility leaves Charge1197;full visibility under Charge's /Ob1 makes
  Init301/5 instead of309/7 and is rejected as original callee closure.
- `.analysis/gpt-61sol-20261031-ecl-word-results/` retains thirteen complete
  ECL controls and all75 exact siblings per control. POD word-result/native
  bits/wide-intermediate controls remain30;constructed/by-value-input/wide-
  return models add an unbound helper and are rejected. Existing integer
  workspace reuse gives42/2290/2302 differences;draw-index reuse is neutral.
  Enemy's own3900 raw bytes/effective97 fields stay unchanged in all thirteen;
  a rejected RunEcl callee still prevents a complete Enemy diagnostic score.
- A control-family rhsInt time capture in `ecl-control-workspace/` retains
 15564/598 but regresses30 to42 ordinary differences,with no field/table drift.
  All75 exact siblings and Enemy13 remain;the source model is rejected.
- `.analysis/gpt-61sol-20261031-ecl-update-objects/` retains seven further
  complete controls. Four full-operation object/proxy models add a genuine
  unbound helper call and fail the whole owner. Small value accessors and a
  real word conversion inline but preserve30. Tiny reference accessors inline,
  leave opcode155 unchanged and shorten opcode157 from160 to158,giving2153 total
  differences. All75 exact siblings pass per model;none is adopted. Immutable
  Git/EOL reproduction and complete raw/COFF records remain after trial cleanup.
- `.analysis/gpt-61sol-20261031-ecl-native-vectors/` bounds nine complete
  vector/workspace/head-reset models,with675 existing exact sibling comparisons.
  Movement/trail native vectors and real word workspaces leave all138 regular
  bodies unchanged. A genuine inline reset adds an unused27-byte body but keeps
  the caller unchanged;ascending reset adds four differences(Enemy17),and memset
  shortens the whole owner to3892,so is rejected. RunEcl stays30 in every model.
  One invalid cross-family RHS scope fails compilation;the corrected shared
  function-scope model compiles and stays30/13. Immutable Git/EOL reconstruction
  and one independent cold whole-object replay pass. No source/type/ABI adoption.
- Three new representation packets remain negative at2b990254. Replay plain
  payload/compressed/both lifetime records retain1238 bytes but give31/63/75
  differences;all old3 remain. Complete queue-owner/base/direct-field views
  preserve every213-byte/35-difference insert and both exact queue siblings.
  A coherent Win32-long counter model updates both fields and the actual
  TickTimer pointer API;all19 native bodies compare original-equal. Whole ECL
  remains30/13 with periodic opaque and30/45 with the real body visible. All75
  exact ECL siblings pass in both models;no type/ABI/source adoption follows.
  Full15-input Git/EOL reconstruction and an independent cold visible-ECL
  replay pass. The then-current251-unit proof pins remained unchanged. Routing:
  replay-buffer-lifetimes/,queue-complete-owner/,timer-counter-contract/.
- Supervisor's fresh current baseline retains952/996 physical bytes and every
  prior raw byte/COFF field. Seven complete state-branch controls show that
  genuinely repeated registration bodies restore target EDI=2 and its delayed
  save. A combined volatile close-bit model reproduces MOV AL/TEST AL/JNS and
  the first197 bytes, but remains1000/996 with493 unequal overlaps:375 old
  positions recover while33 old matches break, plus4 excess bytes. No source,
  qualifier or exactness credit is adopted. All seven immutable input sets and
  one independent cold whole-object replay pass;the then-current251 pins remained.
  Recipe/full observations:supervisor-state-branches/ below the October31
  analysis prefix. Its audit.py checks stored records after trial cleanup;
  baseline.obj is the retained source-bound current Supervisor diagnostic.
- Actual ECL InitializeSubroutine visibility is also bounded at310510f6:
  before all callers,before RunEcl or after the whole carrier,its73 bytes/two fields
  remain original-equal and all138 common regular bodies retain raw/effective
  fields. Four caller references become genuinely same-object-defined. Two
  returned-XOR-mask/compound-assignment contexts retain137 common bodies;
  only the unbound16-byte helper is replaced by an unreferenced14/13-byte
  helper. All five keep75 exact siblings and RunEcl30/Enemy13. Eighty-input
  Git/EOL recovery and one independent cold whole-object replay pass.
  Records/recipes:ecl-native-initializer/;no source/TU/ABI adoption or credit.
- A separately compiled const periodic predicate remains original33/0. Its
  opaque declaration preserves all138 regular raw bodies and Enemy13;the same
  real definition visible gives Enemy45,with only Enemy raw code changed.
  Five native names use private reviewed interface views;production identities
  reject both alternatives. All75 sibling payloads pass per context,including
  four reviewed interface views. Independent whole-object replay passes.
  A real timeout function-pointer call instead emits15560/599 with an unbound
  16-byte helper reference;RunEcl and dependent Enemy coverage reject it.
  All137 other raw bodies and75 siblings remain. Its independent replay repeats
  the rejection. Research routes:enemy-const-periodic/ and ecl-timeout-callable/;
  no production source,qualifier,manifest,profile or exactness credit changes.
- Adjacent flag-pair controls are bounded in ecl-flagpair-storage/ below the
  October31 analysis prefix. A continuous eight-byte group of two unsigned-int
  allocation units preserves all138 regular bodies and RunEcl30/Enemy13.
  Native timeout assignment repeats2140/13 whether its group is four or eight
  bytes. All75 siblings per successful model and one independent replay pass.
  A genuine unsigned-__int64 group fails original layout assertions;compiler
  data confirms its default alignment moves primary337C to3380. No packing,
  source/type/profile adoption or exact credit follows;packed alternatives and
  unique original grouping remain unknown. Current296 pins remain unchanged.
- Two whole-ECL native-side-type controls retain75 exact siblings each.
  Native shared side storage alone retains Enemy13;adding the genuine periodic
  Timer body gives45. Neither candidate is adopted. `probe.py` and `reproduce.py`
  retain bounded recipes/full observations,without old source trees.
- The previous native RNG/sound/ANM/spawn repairs remain. Spawn arguments830/
  10000 mean life/score. The older31/106-unit combined packets are historical
  source epochs;use the296-unit verifier above for the current closure. Reuse
  individual older proofs only when their actual source/profile/object pins
  remain current;this shared-header change invalidates several old bundles.
- The standalone Type3 source also includes the changed header;its old
  current-type3.json binding is historical. Use this packet's maintained-path
  current-ExAttackUpdateType3-0.obj with inspect-exattack-type3-schedule.py.
  Fresh complete validation retains780/38/15 and the unresolved middle-Z order.
- Current proof objects are the six shared-score proposal carriers,the68
  unchanged objects in its current.json,and the unchanged service object.
  Cleanup removes15 ignored baseline/superseded/debug files only after complete
  controls,reconstruction and1130 protected-hash checks. Records,source recipes,
  original target/toolchains and working Ghidra remain;build/ may be absent.
- Other dated packet verifiers are historical research tools. Old compiler
  objects have been cleared;rebuild from current source and the configured
  profile when needed. Their absence is expected,not lost canonical evidence.

The complete previous handoff is preserved in
[the checkpoint archive](RE_HANDOFF_CHECKPOINT_ARCHIVE_20261018.md).
[KNOWLEDGE_BASE.md](KNOWLEDGE_BASE.md) retains all findings and failed models;
[accepted receipts](RE_ACCEPTED_RECEIPTS.md) remain immutable source-commit-bound
history. Current exactness does not establish a complete Windows product.

## Live ledger snapshot


The canonical state is `config/functions.csv`, `config/function-origins.csv`,
`config/matches.csv`, and `config/match-units.toml`, checked by
`python3 scripts/validate-tracking.py --require-target` and
`python3 scripts/validate-docs.py`. The current ledger summary is
**1035 source-present / 1004 exact / 31 non-exact**; earlier operator notes
mentioning 979/876/103 are obsolete for this checkout.

| Measure | Count |
| --- | ---: |
| Function candidates | 2,248 |
| Boundary/origin unreviewed | 0 |
| Reviewed but origin-unresolved | 37 |
| Confirmed authored | 1,035 |
| Classified exclusions | 1,176 |
| Source-present authored mappings | 1,035 |
| Canonical exact functions | 1,004 |
| Source-present non-exact functions | 31 |
| Source-present non-exact bytes | 45,232 |
| Authored without maintained source | 0 |
| Canonical exact authored bytes | 246,651 |

**246,651 / 291,883 reviewed-authored bytes = 84.5034% exact**. To reach
98% of the *currently classified* denominator, another **39,395 bytes**
must become complete original-bound exact owners, reaching at least **286,046
exact authored bytes**. The 37 origin-unresolved functions are excluded and could change the denominator. Similarity fractions
and locally matching subsequences never count as accepted exact.

The manifests govern **1004 exact** functions.

The catalog is not proof of complete executable-code inventory. The formerly
uncatalogued **0x0040D677 /0x00444A78** Timer references now belong to the exact
Effect 1/2 and Effect 34 updates. All **203** captured assignment callsites have
catalogued owners; that bounded census proves neither complete executable
coverage nor a return ABI. The Effect 34 **178-byte initializer at 0x00444960** is now exact; its
**946-byte draw at 0x00444AF0** is a maintained, explicitly nonexact frontier.
Do not silently treat uncatalogued code as excluded.

## Reconstruction frontiers

These are routing measurements, not exactness credit. The ledgers list all31
nonexact functions. The opcode4 setter/assignment visibility and whole Enemy
storage controls are already measured; consult their actual frozen inputs before
repeating them. Historical assumptions remain hypotheses.

| Owner | Target address | Current comparison | Remaining work |
| --- | --- | --- | --- |
| **RunEcl** | 0x004086C0 | **15,564 physical** (14,792 code + 772 table); 598 bound fields; **30** differing non-field bytes | Opcode4 timer transport and opcode155 flag update. Keep all598 fields/tables and complete15564 bytes; actual Timer ABI and RNG ownership are already corrected. `--owner runecl`. |
| **EnemyManager::OnUpdate** | 0x00410730 | **3,900 physical**,97 fields, 69 calls,170 blocks; **13 differing bytes** (29->13) | Draw index2, side/threshold4, descriptor-position copy7. Keep real20-byte Timer greater co-visible, all97 fields and3887 matching physical bytes. |
| **Effect Type29 draw** | 0x00448750 | **2,220 /2,205 bytes**,129 original referents,72 ordered calls,41/41 physical blocks; **1170 overlap differences +15 missing** | Main/satellite fan chronology, polar receiver reloads and ordinal graph. Bounded emulator fixtures are not whole-runtime or exactness proof. `--owner effect29`. |
| **Effect 34 draw** | 0x00444AF0 | **946/946 physical**,53 original referents,20 direct +12 COM calls,286 instructions,16 blocks; **74 linked differences** | Entry65 and store order9 remain; radius operands now match. Preserve872 matching bytes, all53 fields, actual float data and full physical extent. `--owner effect34`. |
| **EtamaController::OnDraw** | 0x00413BE0 | **600/600 physical**, 27 fields, 13 calls, 17 blocks; **268** differences | Laser body-VM position cursor. Preserve332 matching bytes and the genuine final DrawBulletLayerEffects member identity. `--owner etama`. |
| **Player movement boss** | 0x0041C170 | **1,900/1,900 physical**, 66 fields, 23 calls, **129** differing bytes | Lower-bound EAX/EDX transport and encoding cascade. Native framerate/coordinate owners and the shared SHT speed type are corrected;preserve1771 matching bytes and both tables. `--owner movement`. |
| **Player charge update** | 0x0041F810 | **1,210 target /1,197 candidate bytes**, 70 fields, 58 calls, 350/342 instructions; **1,160 overlap differences +13 missing** | Recover source chronology and missing13 bytes; preserve repaired SpawnShots transport and actual call/field identities. |
| **Front weighted message RNG** | 0x004162D0 | **693 target / 693 candidate**, 15 fields, 6 calls, **351** differences, zero missing/excess | Remaining source lifetime and CFG(41/40); four exact Front family owners remain protected. `--owner front`. |
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
| **Player CheckBulletCollision** | 0x0041DFF0 | **862 / 849**, 26 fields, 12 calls; **799** overlap + 13 missing | Region producer, activeB100 pool and missing physical bytes. Preserve five same-source exact helpers and separate CalcItemBox/FindCollision proofs. `--owner player`. |
| **Replay SaveReplay** | 0x00420C60 | **1,238/1,238 bytes**, 380 instructions, 53 fields, 23 call operands, 49 matching blocks; **3 differences** (152→3) | Frame-destination register at+B7/+BB and checksum SIB at+3F6. Preserve1235 matching bytes and all12 exact same-source siblings. `--owner replaysave`. |
| **Replay BeginPlaybackStage** | 0x00420840 | **540 / 538**, 152/151 instructions, **32/32 ordered original PE/COFF fields**, 5 calls, 12/12 matching CFG blocks; **39** overlapping unlike bytes + 2 missing (49→39) | Seed/tail chronology and two missing bytes. All32 ordered referents and first30 physical field positions are already restored. `--owner replay`. |
| **ScreenEffect shake envelope** | 0x004227A0 | **336/336**, 132 instructions, 20 fields, 8 calls, 26 blocks; **26** differences (28→26) | EDI/ESI owner allocation. Preserve310 matching bytes; timer-float visibility regresses physical size and x87 lifetime. |
| **Controller input** | 0x0042B410 | **808 / 810**, 32 fields; **84** overlap differences /2 excess bytes | Both midpoint evaluations per axis are restored; register scheduling and two excess bytes remain. Preserve all694 old matches plus30 restored positions,32 fields and native globals. |
| **Supervisor network HandleMessage** | 0x00432B70 | **768 authored /808 physical bytes**, 46 fields, 17 direct +2 virtual calls, 34 matching blocks; **105 differences** (125→111→105) | Packed-input/frame scheduling. Keep all six switch targets, map bytes and703 matching physical bytes. `--owner netmessage`. |
| **Supervisor network service** | 0x00430AA0 | **1,633 target /1,626 candidate**,106 source fields;51 call-identity multiplicities agree |1376 unequal overlaps plus7 missing. Native RNG and observed packet/session lifetimes are repaired;wait/stack/input transport remain open. `--owner network`. |
| **Supervisor::OnUpdate** | 0x00431110 | **844 authored / 996 target physical**, 952 candidate physical | Whole-owner physical deficit and startup chronology. Keep native GameManager/Title/Ending symbols and actual version data. `--owner supervisor`. |
| **ECL MoveRandomBiased leaf** | 0x00407E30 | **516 / 517**, 28 fields, 14 calls, 24 blocks; 148 overlap + 1 extra | Y-pointer LEA versus MOV alias after actual Float3 conversion. Keep complete owner, fields, branches and exact ECL cohort. `--owner runecl`. |
| **Effect AddedCallback leaf** | 0x0040D1C0 | **51/51**, 4 fields, 2 calls; **8** differences | Remaining receiver/allocation scheduling. Preserve43 matching bytes and genuine AnmLoaded/Chain interfaces. `--owner effect`. |
| **Supervisor received-frame leaf** | 0x0042E9E0 | **213/213**, 67 instructions, 9 blocks; **35** differences | Packed input/seed16-bit store order. Preserve178 matching physical bytes and both exact queue members. `--owner queue`. |

## Continuing reconstruction

Read [AGENTS.md](../AGENTS.md),[RE_WORKFLOW.md](RE_WORKFLOW.md),
[ORACLES.md](ORACLES.md),the affected rows and relevant KB findings. Recover all
tracked/untracked work first. Prioritize complete RunEcl and Enemy,including
shared layouts,dependencies and translation-unit boundaries;old hypotheses are
not conclusive. The other big functions and small leaves remain in scope.

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

Ghidra12.1.4 uses `.tools/ghidra` and reuses
`.analysis/ghidra-direct/243bffea8dcbe99a/th09.gpr`. The runner checks disk/Program
identity,i386 language,base/entry and all685056 mapped file-backed bytes on every
export. Ghidra names,types and decompilation remain provisional. It is not an
exactness Oracle. The obsolete IDA/MCP client chain and its bootstrap are removed.

For source changes,protect complete owners,tails/tables,callees and affected
exact siblings under pinned VC7.1 profiles. Compile serially at lowered priority
on one CPU with bounded resources,and reuse unchanged source/profile proofs.
Do not run all1004 cold units for a local edit. Windows product/runtime,dual-
Oracle semantics and portability remain later phases. No new defensive gameplay
behavior or routine fuzz/null tests belong in an exact-source checkpoint.

## Maintenance and validation

This cleanup preserves current source,ten optional proof objects,all records,
original/tools and working Ghidra. It removes unused compiler outputs and the
unreferenced five-file IDA/MCP implementation chain. Mixed historical archives
contain research records and are retained intact;no new source archive is made.
Per-file inventories are below `.analysis/maintenance-20261030-final/`;see
[cleanup history](RE_ARTIFACT_CLEANUP_HISTORY.md) for final counts and checks.
The larger space occupied by JSON/log/configuration evidence is intentional.

Final public CI passes1055 tests/29 skips in110.109s;all832 tracked input hashes
were stable during the run. The old exact-link-label assertion is now a semantic
link-target check;26 focused documentation tests pass. In total905 compiler/CI
artifacts,62926041 logical bytes,and five obsolete tools were removed. All32413
original retained analysis files are byte-identical after final CI/cleanup.
The106 current comparisons and complete Type3 check passed;build/ is absent.

```bash
python3 -B scripts/validate-tracking.py --require-target
python3 -B scripts/validate-docs.py
python3 -B scripts/progress.py --check
git diff --check
```

Use full public CI for shared validation/tooling changes;documentation alone
needs no compiler or Ghidra replay. English local commits use `gpt-6.1-sol:` and
the identity/trailer in AGENTS.md. A local checkpoint does not prove remote
publication. The October30 maintenance report above is historical;reconstruction is now active.
