# TH09 reconstruction — current handoff

## Operator status — maintenance

The operator cleared the active reconstruction goal on October30 and requested
code/documentation cleanup. The98% milestone remains unfinished;it is not an
active autonomous task. Resume reconstruction when requested. Preserve all
documentation and research records;delete unused code/build copies rather than
creating another archive. Use direct local Ghidra only,no IDA,REA or Factory MCP.

Latest game-source checkpoint: **712468128d5d9c8510c3cfe43786811901db4832**.
Maintenance changes no game behavior,compiler profile,extent,origin or exact
credit. There is no unfinished game-source patch to recover at this checkpoint.
The earlier restart atcc19b484 and subsequent experiments remain historical.

## Source and evidence routing

- Current game code is in `src/`;the authoritative state is in
  `config/functions.csv`,`config/function-origins.csv`,`config/matches.csv` and
  `config/match-units.toml`. Use the live report rather than dated measurements.
- Original input is ignored `resources/th09.exe`,Japanese1.50a,SHA-256
  `10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
  Target/toolchains and the working Ghidra project are preserved.
- The latest source repair shares native RNG,sound,ANM and enemy-spawn interfaces.
  Spawn arguments830/10000 mean life/score,not sprite/lifetime. The31 affected
  caller/callee comparisons preserved5891 physical bytes/352 fields. ECL source
  stayed unchanged;75 strict proofs retained22463 bytes/990 fields and complete
  RunEcl30/Enemy13. These are existing comparisons,not new exact credit.
- `.analysis/gpt-61sol-20261030-game-manager-native-entrypoints/` contains current
  source/profile/object bindings,proofs and recipes. Its `verify.py` rechecks106
  units/28354 bytes/1342 fields. Ten optional current objects are retained:
  eight native-interface objects,the active ECL object and standalone Type3.
- `.analysis/gpt-61sol-20261030-type3-whole-context/current-type3.json` binds the
  Type3 cache to current source/profile. The complete inspector remains
  `scripts/inspect-exattack-type3-schedule.py`;Type3 stays780/38/15.
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
| **Player movement boss** | 0x0041C170 | **1,900/1,900 physical**, 66 fields, 23 calls, **129** differing bytes | Lower-bound EAX/EDX transport and encoding cascade. Native framerate/coordinate owners are corrected; preserve1771 matching bytes and both tables. `--owner movement`. |
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
| **Supervisor network service** | 0x00430AA0 | **1,633 / 1,632**, 108 source fields | Queue ownership and1395 overlapping differences. Keep original referents and exact queue siblings independent. `--owner network`. |
| **Supervisor::OnUpdate** | 0x00431110 | **844 authored / 996 target physical**, 952 candidate physical | Whole-owner physical deficit and startup chronology. Keep native GameManager/Title/Ending symbols and actual version data. `--owner supervisor`. |
| **ECL MoveRandomBiased leaf** | 0x00407E30 | **516 / 517**, 28 fields, 14 calls, 24 blocks; 148 overlap + 1 extra | Y-pointer LEA versus MOV alias after actual Float3 conversion. Keep complete owner, fields, branches and exact ECL cohort. `--owner runecl`. |
| **Effect AddedCallback leaf** | 0x0040D1C0 | **51/51**, 4 fields, 2 calls; **8** differences | Remaining receiver/allocation scheduling. Preserve43 matching bytes and genuine AnmLoaded/Chain interfaces. `--owner effect`. |
| **Supervisor received-frame leaf** | 0x0042E9E0 | **213/213**, 67 instructions, 9 blocks; **35** differences | Packed input/seed16-bit store order. Preserve178 matching physical bytes and both exact queue members. `--owner queue`. |

## Restart when reconstruction is requested

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
publication. The current task is maintenance,not a resumed reconstruction goal.
