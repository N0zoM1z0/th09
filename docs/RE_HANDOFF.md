# TH09 reconstruction — live handoff (2026-11-08)

**Start with the [November 8 next-agent guide](TH09_NEXT_AGENT_HANDOFF_20261108.md).**
It covers the current native code, targeted compiler loop, important
corrected assumptions and safe cache cleanup. Also read
[AGENTS.md](../AGENTS.md), the [original PE/VC7.1 Oracles](ORACLES.md),
and the [workflow](RE_WORKFLOW.md).
The older [November 7 guide](TH09_NEXT_AGENT_HANDOFF_20261107.md) is
**historical**, not a current checkout snapshot. The cumulative 971-line
pre-cleanup live journal is preserved in
[the November 8 history archive](RE_HANDOFF_HISTORY_20261108.md);
do not use its individual “latest” or “still pending” notes as live facts.

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

## Current implementation and corrected assumptions

- The most recent source checkpoint in this handoff is `e861abb0`:
  **309-byte `PlayerLifecycleView::InitializeShot`** is now truly on the
  native Player, with native `PlayerShotCtorView *` and
  `PlayerShtDescriptorView *`. Its **seven independently exact callers**
  still target `0x0041F350`. Whole eight-owner original PE/COFF proof:
  **981 physical bytes / 21 fields**, including seven actual callee-name
  migrations. Evidence:
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
| **Player ChargeAttack** | 0x0041F810 | 1,210 original / 1,197 candidate, 70 fields, 58 calls, **13 missing** | Recover source chronology, register lifetime and missing code; old volatile length-only variants were not exact. `--owner charge`. |
| **Title OnUpdateResult** | 0x004266B5 | 485 original / 484 candidate, 34 fields, 9 calls; 218 unlike overlaps + 1 missing | Character/score snapshot and result branch transport. `--owner resultdraw`. |
| **Title DrawReplayMenu** | 0x004234F6 | 1,280/1,280, 57 fields, 13 calls, **29** unlike | Menu record/bank chronology. `--owner replaymenu`. |
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

## Local cache state and history

A reviewed cache cleanup deleted **1,967 disposable generated files /
117,570,122 logical bytes (about 112.12 MiB)**, including all of the
current `build/` cache and SHA-256-reviewed experiment `.obj` objects.
A second cleaner dry run selected **zero** files. A missing `build/matching`
object after handoff is expected: regenerate only the chosen matching
unit. The ignored audit is saved in
`.analysis/handoff-cleanup-20261013/generated-cache-manifest.json`; the
559 explicitly reviewed `.obj` digests are under
`.analysis/handoff-cleanup-20261108/reviewed-analysis-objects.json`.

**Not removed:** original target `resources/th09.exe`, `.tools/wine/`
(pinned compiler), `.analysis/ghidra-direct/` (Ghidra projects),
`.analysis/handoff-archive-*/` (historic compressed evidence), ignored
source experiments and original PE/COFF proof reports. A plain Git
clone lacks these private/local inputs. For further cleanup use
`python3 -B scripts/clean-generated-artifacts.py` to preview first;
apply only a reviewed plan. The [November 8 history archive](RE_HANDOFF_HISTORY_20261108.md)
and [historical knowledge journal](KNOWLEDGE_BASE.md) retain older trial
narratives without cluttering the live restart.
