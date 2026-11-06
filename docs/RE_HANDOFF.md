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

## 2026-11-06 exact ANM loader slot path

The exact 390-byte ANM ReadAnmEntries now addresses the existing native
slot path at +0x20 instead of casting beyond the 0x1C resource. All 16
same-profile ANM functions and the separate 341-byte constructor remain
complete original-bound exact. See
[ANM native reader path](TH09_ANM_READER_NATIVE_SLOT_PATH_20261106.md).
This is source ownership progress, not new exactness or Factory acceptance.

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

## 2026-11-06 ECL resolver native GameManager storage

The four existing exact operand resolvers now read/write the native typed
`GameManager.valueB4` instead of a casted `+0xB4` raw storage alias. Fixed
VC7.1 replays cover 5,718 original physical bytes and 374 fields across all
four complete owners. 291 compiler-private labels changed spelling but their
original addresses, field coordinates, types and linked bytes did not change.
The exact authored count is unchanged. See
[the original-bound ECL resolver proof](TH09_ECL_OPERAND_NATIVE_GAMEMANAGER_20261106.md).

## 2026-11-06 RunEcl native wire argument source checkpoint

The complete 15,564-byte RunEcl now uses `instruction->arguments` for wire
bytes, shorts and trail parameters rather than raw `+0x0C` address arithmetic.
All 598 fields and previously exact 15,534 physical positions remain
unchanged, with seven opcode4 and23 opcode155 mismatches still open. The
focused RunEcl replay passes99 complete canonical comparisons, including
both affected exact sibling label refreshes. Enemy OnUpdate remains3900/97/13.
See [native wire argument source evidence](TH09_RUNECL_NATIVE_WIRE_ARGUMENTS_20261106.md).

## 2026-11-06 ANM VM native color/sprite ownership

The already exact 811-byte Draw3D owner reads the native `color1Script.a`
field at VM+0x1F3 rather than a raw byte offset. The 45-byte VM constructor
sets `activeSpriteIndex` at +0x214 by name; the 62-byte base constructor also
remains exact. See [native ANM VM member proof](TH09_ANM_VM_NATIVE_COLOR_AND_SPRITE_20261106.md).
These are source-quality advances, not new exact authored bytes.

## 2026-11-06 shared ANM file-slot source

`AnmManager::GetAnm` now uses the same typed `+0x24/0x120` file-slot record
as `ReadAnmEntries`. Focused current-source VC7.1 replay passes all 22 exact
Background, Load and constructor units in their existing profiles; no authored
exact total changes. See [ANM shared slot evidence](TH09_ANM_SHARED_FILE_SLOT_20261106.md).

## 2026-11-06 FrontSide native ANM position cursor

The entire 3,094-byte FrontSide OnDraw remains original-exact while its
cursor now uses the native `AnmVm::pos +0x208` and `pos2 +0x288` members.
Direct unshifted VM iteration regressed to 3,127 bytes and was rejected.
See [FrontSide native VM cursor](TH09_FRONTSIDE_NATIVE_VM_CURSOR_20261106.md).

## 2026-11-06 ANM constructor native render fields

`AnmManager::AnmManager` now directly initializes the typed native render
state at +0x1287C..+0x12890 and its existing +0x2B28F0 tail field. The
341-byte constructor and the other 16 exact same-source functions still match
all 3,258 original physical bytes. See
[the ANM native-constructor proof](TH09_ANM_CONSTRUCTOR_NATIVE_RENDER_STATE_20261106.md).
No new authored exact bytes were added.

## 2026-11-06 ANM constructor native device state

The large native ANM manager constructor now initializes verified device
state, vertex-buffer and tail words through direct C++ members instead of
raw `this+offset` writes; offset assertions guard the layout. All 17 exact
same-source loader units replay: 3,258 bytes / 133 relocations across their
two pinned compiler profiles. See
[the constructor member audit](TH09_ANM_CONSTRUCTOR_NATIVE_DEVICE_STATE_20261106.md).
This is source-quality progress, not newly matched authored bytes.

## 2026-11-06 Etama/Player native collision query

Etama's complete 2,216-byte OnUpdate now uses the native Player collision
query at `+0x36C`, instead of four raw byte-offset casts. All 18 same-source
BulletManager units (including 1,932-byte SpawnSingleBullet) remain exact;
16 compiler-local labels were re-bound from independently verified COFF
positions to their **unchanged** original addresses. The focused Bullet
replay passes 30 complete comparisons. See
[the Etama collision receiver proof](TH09_BULLET_NATIVE_COLLISION_SUBOBJECT_20261106.md).

## 2026-11-06 native ECL bullet wire and Enemy descriptor copy

RunEcl's bullet/laser and independent shot-post paths now use the real
`instruction->arguments +0x0C` trailing wire array. Enemy OnUpdate copies
its XYZ into the actual descriptor through C++ `memcpy`, avoiding unrelated
POD lvalue aliases. The focused complete 15,564/598/30 RunEcl and
3,900/97/13 Enemy audits pass, together with 99 exact related ECL
comparisons. Only two compiler-local original-identity labels changed names.
See [the ECL and Enemy source evidence](TH09_ECL_BULLET_NATIVE_WIRE_AND_ENEMY_COPY_20261106.md).
No new exact bytes are claimed.

## 2026-11-06 Title draw native VM rotation

The exact 329-byte TitleDrawCallback uses the native animation VM
`rotation.z` instead of a raw `vm + 0x08` cast; all 20 original-bound
fields remain unchanged. See
[the title native VM member check](TH09_TITLE_DRAW_NATIVE_VM_ROTATION_20261106.md).

## 2026-11-06 native Player/ExAttack members

The exact Player register (328 bytes), Player added-state initializer (572
physical bytes) and ExAttack Type19/21 draw (318 bytes) now use native Player
Timer/header and AnmVm alpha members instead of naked byte offsets. Eight
affected original-bound units remain exact, including six original-local
initializer table fields rebound from observed COFF definitions. See
[the native Player/ExAttack source proof](TH09_PLAYER_EXATTACK_NATIVE_MEMBERS_20261106.md).

## 2026-11-06 native replay codec payload/source offsets

Replay `LoadReplayData` now traverses its real serialized header/payload
members. A signed `i32` cast on `offsetof` preserves the original loop
bound semantics; unsigned `offsetof` without that cast was rejected by actual
codegen. All 12 same-source exact units match across 4,093 bytes/232 fields;
SaveReplay remains 1,238 bytes/53 fields/two unlike bytes. See
[the native Replay codec proof](TH09_REPLAY_NATIVE_CODEC_WIRE_20261106.md).

## 2026-11-06 Player movement native reference

The still-nonexact 1,900-byte Player movement boss now uses a genuine
`PlayerMovementFields&` over the native +0x1CCC subobject. Ten complete
fixed-profile C++ source variants were tried; the adopted reference preserves
all 66 original fields, 23 calls and 122 existing unlike bytes, without any
new mismatch. The focused movement replay passes the nearby exact owners.
See [the movement source trial and evidence](TH09_PLAYER_MOVEMENT_NATIVE_REFERENCE_20261106.md).

## 2026-11-06 ScreenEffect native side flags

The exact 316-byte `CalcShake` now uses the shared side flags word rather
than an opaque byte cast. All 18 exact same-source ScreenEffect units remain
original-bound exact (2,562 physical bytes / 164 fields); nine private
RegisterChain labels were rebound by actual COFF identity. The 336-byte
shake envelope remains nonexact by 26 ordinary bytes. See
[the native side flags evidence](TH09_SCREEN_EFFECT_NATIVE_SIDE_FLAGS_20261106.md).

## 2026-11-06 PlayerRuntime native movement and VM

The current native PlayerRuntime source now uses typed Player movement
fields, ANM VM color/scale, selector header and tail-state storage instead
of repeated raw offsets. The **complete 21-owner, five-profile source cohort**
replays exact: 5,391 physical bytes / 288 bound fields, including the
522-byte OnUpdate and 852-byte UpdateBeforeState. Eleven private compiler
label names were verified against unchanged original destinations.
See [native PlayerRuntime source proof](TH09_PLAYER_RUNTIME_NATIVE_MOVEMENT_VM_20261106.md).
These were previously exact functions; no new match credit.

## 2026-11-06 ExAttack native VM draw loops

OnDraw0 (286 bytes/9 fields) and OnDraw1 (193/5) now use actual
`AnmVm[vmIndex]` subobjects and `AnmVmBase::type` instead of manually adding
VM stride/0x1FC offsets. Both remained fully target-exact under the pinned
compiler; the independent 318-byte Type19/21 draw is also exact. This is
real C++ source ownership progress, not new exact function count.
See [ExAttack native VM draw loops](TH09_EXATTACK_NATIVE_VM_ARRAY_DRAWS_20261106.md).

## 2026-11-06 Player native transition/effect ownership

The native Player layout now exposes its transition state word at +0x3032C,
transition countdown at +0x30334, and effect pointer at +0x303E0. The exact
Player transition/common update uses those direct members rather than raw
pointer casts. 41 selected exact units spanning Player/Etama/ExAttack
validate all 18,121 original physical bytes/846 fields. The nonexact Enemy
and ChargeAttack original-bound diagnostics remain unchanged. See
[the native Player transition proof](TH09_PLAYER_TRANSITION_NATIVE_STATE_20261106.md).

## 2026-11-06 ANM native texture cache shared by Title

The original TitleDrawCallback now writes the actual `AnmManager` cached
texture pointer at +0x12880 through the same typed native prefix as the
five original-bound ANM background units. Six complete functions (425
physical bytes/22 fields) remain exact under their pinned compiler profiles.
See [shared ANM/Title texture owner](TH09_ANM_TITLE_TEXTURE_CACHE_SHARED_20261106.md).
No new authored exactness or product/runtime acceptance is claimed.

## 2026-11-06 Player tracking shot-local reference source

The exact Player shot update1/update3/spawn7 callbacks now borrow the actual
tracking state at Player +0x30364 once, instead of repeating the same opaque
cast for each tracked vector component. Their complete 471/248/139 byte
owners all remain original-exact (858 physical bytes total), with unchanged
call/referent identities. A common inline accessor and Enemy cleanup local
alias were separately compiled and rejected on complete-owner extent.
See [borrowed native Player tracking references](TH09_PLAYER_TRACKING_BORROWED_REFERENCES_20261106.md).

## 2026-11-06 global Player shared runtime native signed gate

The global Player runtime's signed block-state dword +0xE94C now belongs to
the same native C++ view as update-block +0x1095C and draw counter +0x10960.
All 23 affected PlayerRuntime/Replay/IsBlocked functions replay exact:
5,474 original physical bytes / 291 fields. The anonymous-namespace selector
view was deliberately kept separate after a real source/COFF experiment
proved that merging it changes required original C++ symbol identities.
See [the shared Player gate audit](TH09_PLAYER_SHARED_RUNTIME_NATIVE_GATE_20261106.md).

## 2026-11-06 shared Player runtime native blocked state

The exact IsBlocked leaf now reads its real `+0xE94C` field through the
native shared-runtime layout used by PlayerRuntime and Replay control. The
anonymous-selector local type is kept separate to preserve its original
mangled symbol identity. 25 affected complete unit comparisons remain exact
(9,124 bytes/445 fields). See
[the shared runtime owner proof](TH09_SHARED_RUNTIME_NATIVE_BLOCK_STATE_20261106.md).

## 2026-11-06 RunEcl native laser color member

The still-nonexact 15,564-byte RunEcl boss now reads its 16-bit laser color
through the genuine `LaserInstructionArgs::color02` wire member instead of
manual byte addition/reinterpret-cast. All 598 fields are preserved, with
the same 30 unlike bytes (opcode 4/155). A focused 99-comparison cohort
preserves 30,461 exact related bytes/1,404 fields; only 20 verified
compiler-private label names changed. See
[laser color original-bound source proof](TH09_RUNECL_NATIVE_LASER_COLOR_WIRE_20261106.md).

## 2026-11-06 native per-side setup score buffers

The original gameplay setup thread now allocates and clears its real
0xA0-byte `SetupStateBuffer` using `sizeof`, rather than three hard-coded
byte counts. Eight actual VC7.1 source contexts were compared. The full
1,682/1,689-byte nonexact thread keeps all 172 fields, 35 calls and its
7 missing bytes/694 differing overlaps, with no new unequal positions;
16 related exact unit replays pass. See
[native setup score-buffer proof](TH09_GAMEPLAY_SETUP_NATIVE_SCORE_BUFFER_20261106.md).

## 2026-11-06 native Player stage-D tracking reset

The already exact 121-byte Player stage-D leaf now resets its two positions
and adjacent word through the actual Player +0x30364 tracking storage rather
than three absolute byte-address casts. All 21 same-source original-bound
PlayerRuntime units remain exact across 5,391 bytes/288 fields; only 11
verified compiler-private names were refreshed. See
[native stage-D original-byte proof](TH09_PLAYER_STAGE_D_NATIVE_TRACKING_RESET_20261106.md).

## 2026-11-06 native opponent Player metric receiver

The 522-byte exact Player OnUpdate now derives its opponent timing/metric
receiver through the actual native Player +0x24 header at inner +0x5C,
not a generic +0x80 byte pointer on an opaque side slot. All 21 complete
same-source units still match 5,391 original bytes / 288 fields. See
[native opponent Player metric receiver](TH09_PLAYER_OPPONENT_METRIC_NATIVE_HEADER_20261106.md).

## 2026-11-06 Title replay-menu record ownership

The still-nonexact 1,280-byte ReplayMenu draw boss now borrows the actual
native `ReplayDataView` record as a const reference before snapshotting the
selected index. Twenty-three original-profile C++ models were compiled;
this adopted record model retains all 57 fields, 13 calls, 72 blocks and
29 existing unequal bytes, with no new mismatch. See
[Title native replay record proof](TH09_TITLE_REPLAY_MENU_NATIVE_RECORD_SNAPSHOT_20261106.md).

## 2026-11-06 native ANM PrimitiveDraw VM source

PrimitiveDraw no longer maintains an independent fake VM prefix. Its
DrawVertices/QueueSpriteQuad/DrawTexturedTriangleFan paths now use actual
`AnmVm` color, flags and sprite members, with native offset assertions.
All 12 same-source exact units match the original 1,763 physical bytes/90
fields. Five natural C++ models were compiled. See
[native ANM primitive VM evidence](TH09_ANM_PRIMITIVE_NATIVE_VM_20261106.md).

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
