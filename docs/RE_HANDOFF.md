# TH09 reconstruction — current handoff (2026-11-06)

Start with [the November 6 restart guide](TH09_NEXT_AGENT_HANDOFF_20261106.md).
The operator resumed exact-source TH09 reconstruction after the November 6
maintenance checkpoint. Both large bosses and leaves are in scope; verify
original bytes and compiler evidence instead of trusting archival hypotheses.
The 98% reviewed-authored-byte goal remains unfinished. Before this restart,
`39511403` committed the cleanup handoff and recovered evidence without new
exact gameplay credit. New source checkpoints must use `gpt-web: ...`, protect
complete owners and affected exact siblings, and avoid full cold replay.
Local commits do not establish Factory acceptance or remote publication.

Historical session prose was moved byte-for-byte into
[the November 6 history archive](RE_HANDOFF_HISTORY_20261106.md).
Immutable source-bound [accepted receipts](RE_ACCEPTED_RECEIPTS.md) remain
separate. Historical "current" and "goal active" assertions are not session
instructions.

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

## 2026-11-06 resumed RunEcl ABI recovery

The recovered `EnemyView&` RunEcl object was finally compared against all
15,564 original physical bytes/598 fields and the actual 3,900-byte Enemy
caller; 77 exact same-source units also passed. Its remaining 30 RunEcl and
13 Enemy differences provide no exact credit. The prior private `KeyError`
was a bad diagnostic alias lookup, not a compile/byte-matching result.
Public exactness inspectors now support a strictly whitelisted diagnostic-only
reference projection without changing canonical pointer builds. See
[reference ABI recovery](TH09_RUNECL_REFERENCE_ABI_RECOVERY_20261106.md).

## 2026-11-06 exact-source Player selector owner cleanup

The native Player selector (`PlayerGameplayState.cpp`) uses the typed Enemy
manager/Enemy objects rather than raw `void*` member arithmetic in its target
and reward selection. The pinned original-bound compiler reproduces the
complete already-exact 3,238-byte selector and 412-byte physical resolver,
including all18 resolver table fields. Follow-on native conversions also
cover the opponent Player +0x3044C frame counter and the local +0xC110 collision
count. Only two compiler-private resolver label names change in the canonical
manifest at each source checkpoint; target destinations and authored totals
are unchanged. See [the focused Player source proof](TH09_PLAYER_SELECTOR_NATIVE_ENEMY_OWNER_20261106.md).

## Reconstruction frontiers

Counts below are complete-owner diagnostics, not partial acceptance.

| Owner | Target address | Current comparison | Remaining work |
| --- | --- | --- | --- |
| **RunEcl** | 0x004086C0 | **15,564 physical** (14,792 code + 772 table); 598 bound fields; **30** differing non-field bytes | Opcode4 timer transport and opcode155 flag update. Keep all598 fields/tables and complete15564 bytes; actual Timer ABI and RNG ownership are already corrected. `--owner runecl`. |
| **EnemyManager::OnUpdate** | 0x00410730 | **3,900 physical**,97 fields, 69 calls,170 blocks; **13 differing bytes** (29->13) | Draw index2, side/threshold4, descriptor-position copy7. Keep real20-byte Timer greater co-visible, all97 fields and3887 matching physical bytes. |
| **Player movement boss** | 0x0041C170 | **1,900/1,900 physical**, 66 fields, 23 calls, **122** differing bytes | Lower-bound EAX/EDX transport and encoding cascade. Native framerate/coordinate owners and the shared SHT speed type are corrected;preserve1778 matching bytes and both tables. `--owner movement`. |
| **Player charge update** | 0x0041F810 | **1,210 target /1,197 candidate bytes**, 70 fields, 58 calls, 350/342 instructions; **1,160 overlap differences +13 missing** | Recover source chronology and missing13 bytes; preserve repaired SpawnShots transport and actual call/field identities. |
| **TitleScreen::OnUpdateResult** | 0x004266B5 | **485 / 484**, 147/147 instructions,34 fields,nine calls,40 agreeing ordinal blocks; **218** overlap +one missing | Character snapshot and remaining control-flow/byte deficit. Keep the exact Title setup siblings. |
| **TitleScreen::DrawReplayMenu** | 0x004234F6 | **1,280/1,280 bytes**,376/376 instructions,57 original-bound fields,13 ordered calls,72 matching direct blocks; **29 differences** | Menu bank/record chronology. Preserve all57 fields,13 calls,72 blocks and the full original-bound residual guard. `--owner replaymenu`. |
| **GameManager gameplay setup worker** | 0x0041AF2D | **1,689 target /1,682 candidate**, 172 bound fields, 35 calls; **694 unequal overlaps +7 missing** | Shared limit/rate publication and side/rank chronology. All172 referent multiplicities now agree; old1689/171/904 is historical. Preserve native creators and the paired int Player producer. See [shared-rate review](TH09_SETUP_SHARED_RATE_CONTEXT_20261106.md). |
| **Player CheckBulletCollision** | 0x0041DFF0 | **862 / 849**, 26 fields, 12 calls; **799** overlap + 13 missing | Region producer, activeB100 pool and missing physical bytes. Preserve five same-source exact helpers and separate CalcItemBox/FindCollision proofs. `--owner player`. |
| **Replay SaveReplay** | 0x00420C60 | **1,238/1,238 bytes**, 380 instructions, 53 fields, 23 call operands, 49 matching blocks; **2 differences at +0xB7/+0xBB (152→3→2)** | Frame-destination register remains. Shared full-pipeline index closes checksum SIB+3F6. Preserve1236 matching bytes and all12 exact same-source siblings. `--owner replaysave`. |
| **Replay BeginPlaybackStage** | 0x00420840 | **540 / 538**, 152/151 instructions, **32/32 ordered original PE/COFF fields**, 5 calls, 12/12 matching CFG blocks; **39** overlapping unlike bytes + 2 missing (49→39) | Seed/tail chronology and two missing bytes. All32 ordered referents and first30 physical field positions are already restored. `--owner replay`. |
| **ScreenEffect shake envelope** | 0x004227A0 | **336/336**, 132 instructions, 20 fields, 8 calls, 26 blocks; **26** differences (28→26) | EDI/ESI owner allocation. Preserve310 matching bytes; timer-float visibility regresses physical size and x87 lifetime. |
| **Supervisor network service** | 0x00430AA0 | **1,633 target /1,627 candidate**,106 source fields;51 call-identity multiplicities agree |1381 unequal overlaps plus6 missing after the reviewed unsigned-16 queue API change (previously1376+7). Native RNG and packet lifetimes remain;wait/stack/input transport stay open. `inspect-supervisor-service.py` checks the full compiled owner; `--owner network` checks its queue dependencies. |
| **Effect AddedCallback leaf** | 0x0040D1C0 | **51/51**, 4 fields, 2 calls; **8** differences | Remaining receiver/allocation scheduling. Preserve43 matching bytes and genuine AnmLoaded/Chain interfaces. `--owner effect`. |

## 2026-11-06 Player selector typed Enemy owner

The exact 3,238-byte Player pattern selector now reads the one native
EnemyManager/Enemy allocation rather than duplicated raw numeric offsets.
Its 346-byte/412-physical-byte direction resolver also remains exact after
all 18 compiler-local table labels were verified against unchanged original
destinations and refreshed. These are meaningful typed gameplay-source changes
but **zero new exact bytes**. See
[the native Enemy owner review](TH09_PLAYER_SELECTOR_NATIVE_ENEMY_20261106.md).

## Latest source and evidence

- RunEcl: one native Enemy owner and one switch, current30 residuals; see
  [scope/setter review](TH09_RUNECL_SCOPE_AND_SETTER_CONTEXT_20261106.md).
  Source proof: `.analysis/runecl-schedule-source-20261106/source-proof.json`.
- Player collision: structured loop and shared geometry result, native reward
  calls; complete849/862 frontier unchanged. Source checkpoint `0df06fa5`, seal
  `3d1ad1c9`; [vector-context review](TH09_PLAYER_COLLISION_VECTOR_CONTEXT_20261106.md).
- Replay Save: nine destination controls and a coherent instance-method witness
  did not close the two residuals. Current static declaration stays; original
  source method shape remains unknown. Source checkpoint `1be9fee4`, seal
  `89058d69`; [stage-flow review](TH09_REPLAY_SAVE_STAGE_FLOW_20261106.md).

The last interrupted RunEcl packet is **unaccepted**:
`.analysis/runecl-timeout-storage-20261106/RECOVERY.json`. Seventeen completed
attempt receipts cover16 compiled comparisons plus one failed compile. A
seventeenth object (`borrowed-enemy`) was recovered after compilation, but its
postcompile alias lookup raised KeyError before a complete owner comparison.
`borrowed-native-enemy` never started. No model was adopted. The private
prototype adapter is unvalidated; do not report it as an Oracle result. Its
source, logs, frozen inputs and recovery inventory remain loose; build objects
are losslessly archived as described below.

## Gates and restart scope

Boundary/origin review, source presence, exact codegen, product closure, runtime
semantics and portability remain separate. The Windows i386 product is not
closed or runtime-validated; semantic dual-Oracle work and portability have not
started. The15-byte VectorAngle recipe has byte evidence but unresolved origin;
it does not add authored exact credit. No catalog is proof of complete code
inventory.

On a reconstruction request, follow [RE_WORKFLOW.md](RE_WORKFLOW.md) and
[ORACLES.md](ORACLES.md), verify the canonical target, and attest every mapped
file-backed byte before direct Ghidra work. Rebuild/replay affected owners and
dependencies only; reuse unchanged source/profile-bound proofs. Learn natural
source structure from references, then investigate the TH09 nonexact source;
reference implementations and adjacent games are hypothesis material. No
assembly, barriers, fake returns, inert padding or register-order hints.

## Reviewed storage cleanup (2026-11-06)

Net logical regular-file bytes under `.analysis/` and `build/` fell by
**422,418,351 bytes (402.85 MiB)**, including archive/audit overhead up to the
final measurement. The small final audit itself was written after measurement;
filesystem allocated blocks may differ. `build/` is intentionally absent.

| Treatment | Files | Original bytes |
| --- | ---: | ---: |
| Build OBJ, losslessly archived | 1208 | 70,558,367 |
| Large analysis JSON, losslessly archived | 209 | 94,922,787 |
| Large private TOML manifests, losslessly archived | 96 | 175,470,846 |
| Cache cleanup: build plus analysis debug/bytecode | 2450 | 164,100,962 |

The cache row includes the1208 archived objects; do not add it to the archive
rows as a disjoint total. All1513 archive members passed streamed size/SHA-256
checks and all originals were rechecked before removal. The10,300,825-byte
archive and its manifest/receipt remain locally ignored at
`.analysis/handoff-archive-20261106/`. Archive SHA-256:
`c2ef291c49e913a5b44d5b68e9dc568c924732976d27cc3545e2945ca04a7da0`;
manifest SHA-256:
`191d2dc6f3395c31b0d43f43abe74c8a24ab68b2365661f4a0b8e40bdf14c976`.
These local packages do not travel in a Git clone.

The canonical target, all tracked source/config files at cleanup inspection and
1467 Ghidra working-state files were preserved and rehashed (1996 protected
files total). Analysis OBJ files remain loose; older verified archives,
toolchains, frozen input sources, scripts and unresolved RunEcl evidence remain.
PDB/bytecode caches were discarded. Large JSON/TOML evidence was archived, not
lost: a verifier may need selective restoration of its named inputs first.

Preview and then restore the interrupted packet's objects:

```bash
python3 -B scripts/restore-analysis-archive.py --bundle .analysis/handoff-archive-20261106 --prefix build/analysis/runecl-timeout-storage-20261106
# Repeat with --apply to restore absent, hash-verified members.
```

Use `--path` for a specific archived JSON, TOML or OBJ; use `--prefix` for an
archived analysis packet. Without `--bundle`, the helper still selects the
older November4 JSON archive. Existing different files are never overwritten.
A real restore of17 objects plus one JSON and one TOML (19 files/7,049,630 bytes)
passed SHA checks; those restored duplicates were then removed by hash review.
Rebuild only the affected canonical unit when starting a new comparison; do
not interpret absent build cache as lost canonical source or run a full cold
replay to refill it.

Local audit entrypoints are
`.analysis/handoff-cleanup-20261106/final-audit.json`, `archive-plan.json`,
`archive-verification.json`, `generated-cache-manifest.json`,
`selective-restore-check.json`, and `cache-resume.json`. The cache-cleaner call
initially rejected an absolute audit path after archive-backed analysis removal;
no build cache was deleted by that rejected call. A relative audit path, fresh
fingerprint checks and protected-input checks completed cleanup.

Validation:1389 target-independent tests ran with29 optional skips; test,
ledger, documentation and generated-progress stages passed. The first CI command
ended at its final whitespace stage on one trailing blank line, then
`git diff --check` passed after correction. Target verification, target-required
tracking, status, documentation and progress checks passed after cleanup. No
new compiler/Ghidra replay or exactness claim is part of this maintenance work.
