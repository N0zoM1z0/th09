# TH09 reconstruction — live handoff (2026-11-10)

**Start with the [November 10 next-agent guide](TH09_NEXT_AGENT_HANDOFF_20261110.md).**
It documents the current reconstructed C++ owner relationships, next focused
compiler checks and safe storage recovery. Read [AGENTS.md](../AGENTS.md),
[the original PE/VC7.1 Oracles](ORACLES.md) and [workflow](RE_WORKFLOW.md).
The [November 9 handoff](TH09_NEXT_AGENT_HANDOFF_20261109.md) and earlier
versions are **historical source/disk snapshots**, not the current checkout.
The November 8 cumulative journal remains in
[the history archive](RE_HANDOFF_HISTORY_20261108.md); its dated “latest”
and “pending” entries are not live facts.

## Verified ledger state

| Measure | Count |
| --- | ---: |
| Function candidates | 2,248 |
| Boundary/origin unreviewed | 0 |
| Reviewed but origin-unresolved | 35 |
| Confirmed authored | 1,037 |
| Classified exclusions | 1,176 |
| Source-present authored mappings | 1,037 |
| Canonical exact functions | 1,024 |
| Source-present non-exact functions | 13 |
| Source-present non-exact bytes | 29,834 |
| Authored without maintained source | 0 |
| Canonical exact authored bytes | 262,097 |

**1037 source-present / 1024 exact / 13 non-exact**.
**262,097 / 291,931 reviewed-authored bytes = 89.7805% exact**.
To reach 98% of the reviewed-authored byte denominator, another **23,996 bytes** must become **whole original-bound exact functions**.
An original 1:1 match for a small patch of a larger nonexact function
does not earn partial credit. The authored inventory is not proven
complete. Whole native Windows i386 product closure, runtime semantics,
and portability are separate unfinished phases.

Private target: `resources/th09.exe`, original Japanese TH09 **v1.50a**,
SHA-256 `10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
Pinned compiler: original-profile VC7.1. The original game, saved Ghidra
project, Wine compiler runtime and `.analysis` experiment receipts are
ignored local inputs, not versioned Git files.

## Current source checkpoint and corrected ownership

The [source-bound accepted-receipt archive](RE_ACCEPTED_RECEIPTS.md) is
**historical**: it keeps the old `EnemyAppendCollisionView` name for the
originally accepted 82-byte wrapper, while the live source and ledger now
use `EnemyView::AppendPlayerCollisionBox` at the same original address.
Do not change the original receipt hash or interpret its old 940-exact
milestone as today's count.


The latest source checkpoint before this documentation pass is
**`746b0e6d`**, not the old November 9 `e7964962` constructor checkpoint.
The November 10 Title work completed four coherent changes:

- [`f32ca36b`](TH09_TITLE_DRAW_FULL_NATIVE_OWNER_20261110.md):
  `TitleDrawCallback` (329 bytes/20 fields) uses the real 0x1B388-byte
  `TitleScreenView` and native ANM texture cache. Four original draw-method
  REL32s remain unchanged; 63 affected Title/ANM owners stayed exact.
- [`096077ac`](TH09_TITLE_CALC_COMPLETE_NATIVE_OWNER_20261110.md):
  TitleCalc's 328-byte physical body including its switch tables uses that
  same Title owner; 36/36 fields remain exact.
- [`6bba2cc7`](TH09_TITLE_REPLAY_MENU_NATIVE_FLOAT3_TEXT_VIEW_20261110.md):
  `DrawReplayMenu` now borrows one checked `Float3` text view. It is **still
  nonexact**, 1,280/1,280 with 57 fields and 29 unlike bytes.
- [`746b0e6d`](TH09_TITLE_MUSIC_ROOM_COMPLETE_NATIVE_OWNER_20261110.md):
  the 2,258-byte MusicRoom update and 209-byte draw use the real Title,
  32 POD music tracks and ANM owner. Both are exact; 49 affected owners
  reproduced 26,914 bytes/1,350 relocation fields. Eleven original
  indexed-global COFF addends were recovered from immutable PE operands.

The recent native Player death/reward/CardAttack/region/shot/damage, Enemy
append-collision, RunEcl background spell and AsciiManager boss-marker
source changes also **preserve previously exact functions**. These are C++
owner/layout advances, not additional ledger exact-credit. See the
[November 10 guide](TH09_NEXT_AGENT_HANDOFF_20261110.md) and their dated
`TH09_*_20261109.md` / `TH09_*_20261110.md` notes for original-bound evidence.

## Previously verified native Front blocked gate (2026-11-09)

The exact 24-byte signed `IsBlocked` at 0x404980 is now a real
`FrontFactoryConstructView` method reading constructed message state at
Front+0xE94C. Player pattern selector (3,238 bytes), Player OnUpdate
(522), GameManager OnUpdate (1,230), Replay frame advance (59) and
Enemy timeline Run (1,048 physical bytes) call the same native interface.
Whole-source scoped replay covers 53 complete exact owners / 24,540 bytes
/ 1,418 bound fields, plus five constructor-source equivalents. The Enemy
timeline owner **was previously exact**; canonical exact remains 1,024.
See [native Front blocked gate](TH09_NATIVE_FRONT_SIGNED_BLOCK_GATE_20261109.md).

## Earlier proven findings and corrected assumptions (dated evidence, not current HEAD)

- **Five native Enemy/ECL special members now have byte-perfect
  source-equivalent C++ constructors** at `0x40FCA0` (28),
  `0x412090` (15), `0x4120C0` (15), `0x4120D0` (47) and
  `0x412100` (207). The 28-byte descriptor constructor
  `0x40D500` also admits a new logical C++ alias. All six are independently
  bound in `scripts/inspect-enemy-ecl-constructors.py`. The five special
  members **stay ledger-unclassified** (explicit vs implicit authorship
  unknown), so the 1,024 authored exact count is unchanged. At Enemy +0x3E68
  **194 x 0x1C vertices are constructed**, while the known renderer consumes
  only 192. The older supposed opaque +0x5368–+0x53A0 gap is disproven;
  purpose of the extra two constructed records remains unknown.
- **Complete ChargeAttack source/CFG recovery:** the source-cold candidate
  now fills all 1,210 target bytes, preserving all 70 fields, 58 calls,
  350 instructions and 53 direct blocks, with **235 ordinary differences**.
  The previous 1,195/1,210 and 1,197/1,210 candidates are historical.
- **Title DrawResult** at `0x00423D16` is strictly exact (1,939 bytes,
  82 fields). The older three-byte frontier allowance was stale and
  removed; do not confuse this exact draw function with the nonexact
  485-byte `TitleScreenView::OnUpdateResult` callback.
- **Replay SaveReplay** remains 1,238/1,238 with two unlike bytes at
  +0xB7/+0xBB. Both frame copy and serialized cursor step now use
  `sizeof(*frameHeader)`. **BeginPlaybackStage** remains 538/540 with
  39 unlike overlap bytes; side-0 value1E8 is explicitly snapshotted
  before reading its runtime owner. Source chronology is not emitted
  register equivalence.
- The **309-byte `PlayerLifecycleView::InitializeShot`** remains on the native
  Player, with native `PlayerShotCtorView *` and `PlayerShtDescriptorView *`.
  Its **seven independently exact callers** still target `0x0041F350`.
  Whole eight-owner original PE/COFF proof: **981 physical bytes / 21 fields**,
  including seven actual callee-name migrations. Evidence:
  `.analysis/gpt-web-native-initialize-shot-owner-20261108/original-proof.json`.
  See [native shot field recovery](TH09_SHOT_INITIALIZE_NATIVE_COMPONENTS_20261108.md).
- **Three previously unclassified constructors are already exact:**
  Player Header `0x41EE00` (28 bytes, three ZunTimers), Player owner
  state `0x41EE30` (23 bytes, two timers), Player shot-state
  `0x41EE20` (12 bytes, one timer). Their original code is exact,
  although explicit versus implicit historic C++ authorship is unknown.
  See [Header](TH09_PLAYER_HEADER_CONSTRUCTOR_EXACT_20261108.md),
  [owner](TH09_PLAYER_OWNER_STATE_CONSTRUCTOR_EXACT_20261108.md),
  [shot state](TH09_PLAYER_STATE3031C_CONSTRUCTOR_EXACT_20261108.md).
- **Do not restore the old handle-release interpretation:** original
  `0x40F860` is the **33-byte `AsciiGameManagerSideView::AddScore`**.
  PlayerOwnerState `Reset @0x41D7E0` publishes the actual reward counter
  through that exact method, then clears counters/timer. See
  [corrected Player reset](TH09_PLAYER_NATIVE_OWNER_RESET_IS_SCORE_20261108.md).
- Player's **512 real 0x44-byte collision records** start at +0x28BC;
  its embedded fallback is at +0xB0BC, followed by real pointer tables.
  The exact 572-byte physical initializer and region allocator share
  the real Reset. See [native pool](TH09_PLAYER_NATIVE_FULL_COLLISION_POOL_20261108.md).
- Enemy `OnUpdate` uses real `AsciiGameManagerSideView` +0x08 Etama /
  +0x20 character, rather than an Enemy-only side. The 3,900-byte boss
  **remains nonexact** with 13 byte differences. See
  [Enemy native side](TH09_ENEMY_ONUPDATE_NATIVE_GAMEMANAGER_SIDE_20261108.md).
- Native `AnmManager` frame counters are +0x0C **started**, +0x10
  **executed**, +0x14 **render-state changes**, +0x18 **flushes**.
  Earlier swapped-name interpretations were superseded by the complete
  9,776-physical-byte exact script interpreter and independent leaves.
  See [original-bound ANM counter witnesses](TH09_NEXT_AGENT_HANDOFF_20261107.md#2-corrected-facts-to-carry-forward).

- **FrontSide native special members** at 0x41A470 (28 bytes) and
  0x41A490 (307 bytes) now reproduce both complete original bodies under
  an independent /Ob0 constructor carrier, with **28 matching original
  relocation fields**. All 15 exact FrontSide.cpp siblings survive the
  separation. Their explicit-versus-implicit origin remains unresolved; no
  authored ledger credit is added. See
  [FrontSide constructor proof](TH09_FRONT_SIDE_CTOR_ORIGINAL_20261109.md).

- **Front gameplay HUD construction** now has matching native C++ constructor
  bodies at 0x4187B0 (143/143, 12 relocated fields) and 0x418750
  (85/85, 7 fields), plus the shared three-byte vertex constructor alias
  at 0x4343D0. The typed 0x1293C Front allocation and 0x1D70 embedded
  message runtime are independently checked; the existing factory caller
  and three affected FrontMessage exact consumers still replay exactly.
  These are origin-unclassified source-equivalent bodies, not newly
  authored exact credit. See [native Front constructor chain](TH09_FRONT_FACTORY_NATIVE_CTORS_20261109.md).

- **Native global AsciiManager constructor chain:** the original CRT
  initialization thunk at `0x48D950` sets `ECX=0x4CE458` (`g_AsciiManager`)
  and jumps to the 227-byte constructor `0x435DC0`. Its typed 0xE0AC-byte
  allocation now uses genuine `AsciiManagerString`, `AsciiManagerPopup`,
  `PauseMenu`, and `AsciiMenuState5` member constructors. The parent 227-byte
  body and all six original constructor addresses retain complete exact
  byte/field proofs under VC7.1. All 28 directly affected Ascii exact owners
  replay, and ECL/Enemy boss residuals remain 30/13. Explicit vs implicit
  constructor origins remain unknown. See
  [Ascii native global construction](TH09_ASCII_NATIVE_CONSTRUCTORS_20261109.md).

- **Front message native runtime fully shared:** original-exact
  `FrontMessageRuntimeView::Update` 2,968 physical bytes/167 fields and
  `Setup` 357/10 now compile against the same constructed 0x1D70-byte
  runtime. The 1,491-byte Front calc and 133-byte message initializer call
  their actual native method symbols, not fake C++ receiver classes. All
  11 affected Front exact functions/9,713 bytes and 11 GameManager exact
  siblings retain their full original-bound source proofs. See
  [native Front Update](TH09_FRONT_MESSAGE_UPDATE_NATIVE_OWNER_20261109.md)
  and [native Setup/callers](TH09_FRONT_MESSAGE_NATIVE_SETUP_OWNER_20261109.md).

- **Etama native Bullet/Laser constructor chain:** three previously
  origin-unclassified constructors now have maintained native C++ source:
  `BulletTypeSprites` 56/5, `Bullet` 58/5, and `Laser` 34/3, all
  exact against original PE bytes/fields. `Bullet` also emits the actual
  12-byte `BulletExState` default constructor sharing target code at
  0x41EE20. The 121-byte Etama owner and 283-byte Register caller both
  remain exact. These source-equivalent special members retain unknown
  authored/implicit origin. See
  [Etama native construction](TH09_NATIVE_BULLET_LASER_CONSTRUCTORS_20261109.md).

## Reconstruction frontiers

All 13 entries below are **nonexact** complete physical owners, not exact
credit. Offsets, calls, fields and residuals are starting evidence from
focused original-bound checks; refresh the chosen owner with the actual
pinned compiler when you make a source change. Use only its focused
`--owner` diagnostic, **never** `--owner all` as routine work.

| Owner | Target address | Current comparison | Remaining work |
| --- | --- | --- | --- |
| **RunEcl** | 0x004086C0 | 15,564 physical bytes, 598 original fields, **30** ordinary unlike | Opcodes 4 (timer transport, 7 bytes) and 155 (flag write, 23 bytes); preserve complete owned jump tables. `--owner runecl`. Source `src/EclManager.cpp`. |
| **Enemy OnUpdate** | 0x00410730 | 3,900/3,900 physical, 97 fields, 69 calls, **13** unlike | Draw-index register and descriptor position/threshold transport. Current source uses actual GameManager Etama side. `--owner enemy`. Source `src/EnemyManagerCore.inl`. |
| **Player movement** | 0x0041C170 | 1,900/1,900, 66 fields, 23 calls, **122** unlike | Loop/index register scheduling; retain authentic native option/position arrays. `--owner movement`. Source `src/PlayerMovement.cpp`. |
| **Player ChargeAttack** | 0x0041F810 | **1,210/1,210**, 70 fields, 58 calls, **235** unlike, 0 missing; 350 instructions, 53 direct blocks, matching CFG | Mode byte [EBP-1] and input receiver register allocation; complete extent has already been recovered. `--owner charge`. |
| **Title OnUpdateResult** | 0x004266B5 | 485 original / 484 candidate, 34 fields, 9 calls; 218 unlike overlaps + 1 missing | Character/score snapshot and result branch transport. `--owner resultdraw`. |
| **Title DrawReplayMenu** | 0x004234F6 | 1,280/1,280, 57 fields, 13 calls, **29** unlike | Selected ReplayData register chronology; pure Float3 math and early index snapshots regressed. `--owner replaymenu`. |
| **GameplaySetupThread** | 0x0041AF2D | 1,689 original / 1,682 candidate, 172 fields, 35 calls, 694 unlike overlaps + 7 missing | Gameplay rate and side/rank ordering. `--owner gamemgr`. |
| **Player CheckBulletCollision** | 0x0041DFF0 | 862 original / 849 candidate, 26 fields, 12 calls, 799 unlike overlaps + 13 missing | Actual collision region allocation and branch chronology. `--owner player`. |
| **Replay SaveReplay** | 0x00420C60 | 1,238/1,238, 53 fields, 23 calls, **2** unlike bytes | Original EDI/ECX destination-register selection at +0xB7/+0xBB. `--owner replaysave`. |
| **Replay BeginPlaybackStage** | 0x00420840 | 540 original / 538 candidate, 32 fields, 5 calls, 39 unlike overlaps + 2 missing | Seed/tail chronology. `--owner replay`. |
| **ScreenEffect shake** | 0x004227A0 | 336/336, 20 fields, 8 calls, **26** unlike | ESI/EDI receiver lifetime and fade transport. `--owner screen`. |
| **Supervisor network** | 0x00430AA0 | 1,633 original / 1,627 candidate, 106 fields; 1,381 unlike overlaps + 6 missing | Queue/input transport and stack lifetime. `--owner network`. |
| **Effect AddedCallback** | 0x0040D1C0 | 51/51, 4 fields, 2 calls, **8** unlike | Receiver register choice. `--owner effect`. |

## Restart and scoped comparison

```bash
git status --short --branch
git log -8 --oneline
python3 -B scripts/verify-target.py
python3 -B scripts/validate-tracking.py --require-target
python3 -B scripts/report-reconstruction-status.py

# Compile and compare only the changed owner (example):
nice -n 12 python3 -B scripts/build-match-unit.py --unit player-initialize-shot
python3 -B scripts/compare-coff-function.py --unit player-initialize-shot --json

# A still-open large boss, not an exactness credit assertion:
nice -n 12 python3 -B scripts/check-boss-frontiers.py --owner enemy --skip-exact-siblings
```

Original PE/COFF **whole code, original owned physical tails/tables,
relocation offsets/types/targets/addends and call identities** are required
for exactness. VC7.1 may renumber compiler-private `$L` labels after a
source change: verify same-section COFF definitions, original PE operands
and owner-relative addresses before rebinding the spelling, then recheck
all affected exact siblings. Preserve dirty/untracked work and do not
reset it to evade failing original-bound comparisons. Checkpoints use
`gpt-web: <English subject>`.

## Local storage, cleanup and recovery

The **November 10 reviewed cleanup** made two SHA-256-verified ignored
archives at `.analysis/handoff-archive-20261110/`: 17,894 original files
(161,902,770 loose bytes) became two 11,376,682-byte compressed bundles.
After removing only reviewed old directories and generated build/object/bytecode
caches, the total net logical saving was **163,162,263 bytes (155.6 MiB)**
including manifests. The complete packages were streamed and individually
hash-verified before deleting loose source experiments; selective recovery
was separately tested. See [dated cleanup history](RE_ARTIFACT_CLEANUP_HISTORY.md)
for both bundle hashes, exact paths and restoration commands. The current
`build/` is intentionally absent; it is a generated cache.

The November 9 cleanup was historical: it archived 140 experiment folders
(24,862 files / 383,162,789 loose bytes) into the verified ignored
`.analysis/handoff-archive-20261109/source-probes.tar.zst` bundle. It also
removed a then-current set of 158 generated build files. Those figures
**must not** be presented as the current disk inventory or added to new
cleanup results. The [dated cleanup history](RE_ARTIFACT_CLEANUP_HISTORY.md)
contains verified paths, checksums and extraction instructions.

The repository may have no `build/` after maintenance. Recompile
only the requested source unit; the ignored original, Wine toolchain,
Ghidra/IDA projects, archival bundles and useful source experiments must be
preserved. Old `.analysis/` evidence may now be archived rather than loose.
Git clones do not include these ignored local inputs. The current recovery
and protection rules are in the [November 10 next-agent guide]
(TH09_NEXT_AGENT_HANDOFF_20261110.md). **Do not delete `.analysis/` wholesale.**
