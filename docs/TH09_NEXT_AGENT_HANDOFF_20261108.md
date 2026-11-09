# TH09 — next agent: clean restart (2026-11-08)

> **Historical snapshot; superseded 2026-11-09.** Start with the
> [November 9 next-agent guide](TH09_NEXT_AGENT_HANDOFF_20261109.md)
> and [live handoff](RE_HANDOFF.md) instead. This note's HEAD,
> generated-object availability, and ChargeAttack candidate size are
> accurate only for its original November 8 checkpoint.

**Use the actual Git worktree, not a remembered checkpoint.** Read
[AGENTS.md](../AGENTS.md), the [live ledger/frontiers](RE_HANDOFF.md),
[original-byte Oracles](ORACLES.md), and [workflow](RE_WORKFLOW.md).
The [November 7 guide](TH09_NEXT_AGENT_HANDOFF_20261107.md) and the
[pre-cleanup November 8 handoff](RE_HANDOFF_HISTORY_20261108.md) are
historical. Their “latest”, total and pending-action claims have been
superseded. This guide describes the state after the November 8 source
checkpoint and artifact cleanup; check Git and the ledgers again at restart.

## 1. Exact target and actual status

The only allowed target is the ignored `resources/th09.exe`: **original
Japanese TH09 v1.50a**, SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
The target is private and is **not in Git**. The pinned VC7.1 compiler and
game-specific Wine environment are under ignored `.tools/wine/`; original
analysis is under ignored `.analysis/` (including the saved Ghidra projects).
Do not replace any of those inputs with a translated or patched executable.

At this handoff the canonical ledgers report **2,248 reviewed candidates,
1,037 game-specific authored source mappings, 1,024 complete byte-exact
functions and 13 still-nonexact functions** (29,834 original physical
bytes). Exact authored coverage is **262,097 / 291,931 bytes = 89.7805%**.
Origin review still has **35 unresolved** cases. The game is **not** a
completed, linked or gameplay-tested native Windows product. The three
additional exact 28-, 23- and 12-byte Player constructors are already in
the match ledger; do **not** call them unclassified or add them twice.

## Current Git/source checkpoint

The source baseline for this refresh is **`6d3a3618`**; `main` was clean
and in sync with `origin/main` before the handoff-only documentation commits.
The latest source-affecting commit is **`f40b2755`**; `6d3a3618` only adjusts
the boss-frontier diagnostic and its regression test.
The intervening source history includes native Enemy/GameManager side
projection work (`4d7b9e6e`, `7e29e4e8`, `34f4016d`, `cf584baf`) and the
RunEcl opcode-155/opcode-4 refinements (`a99405bd`, `76c61837`, `f40b2755`).
The current source files, not old experiment copies, are authoritative.

`PlayerLifecycleView::InitializeShot` still owns the exact **309-byte**
function at `0x0041F350`, takes genuine `PlayerShotCtorView *` and
`PlayerShtDescriptorView *`, and has **seven full-byte-exact shot-spawn
callers** targeting the same original address. The eight complete owners
were independently checked against **981 original physical bytes / 21
PE-bound fields**, including the seven C++ method-name changes; nothing
was credited as a new exact function for this ownership migration.
The affected original-bound proof packet remains at
`.analysis/gpt-web-native-initialize-shot-owner-20261108/`.

## 2. Important corrected semantics and object ownership

- **The former `ReleaseHandle` call at `0x40F860` was wrong.** The exact
  native method is `AsciiGameManagerSideView::AddScore`. Its caller at
  `0x41D7E0` is the 41-byte native Player owner `Reset`, and the alleged
  handles at +0x0C/+0x10 are reward counters. Read
  [the corrected reset](TH09_PLAYER_NATIVE_OWNER_RESET_IS_SCORE_20261108.md).
- The real Player owns **512 native 0x44-byte collision regions** at
  +0x28BC, the embedded fallback at +0xB0BC, and real active/released
  tables. See [the physical pool](TH09_PLAYER_NATIVE_FULL_COLLISION_POOL_20261108.md).
- Native `PlayerHeaderStateView` constructs ZunTimers at +0x00/+0x5C/
  +0x68. Independently exact `0x41EE00` (28 bytes), native owner state
  `0x41EE30` (23 bytes), and state `0x41EE20` (12 bytes) are **tracked
  exact leaves**. Explicit versus implicit historical C++ constructor
  spelling is unknown; compiled identity is not. See
  [Header ctor](TH09_PLAYER_HEADER_CONSTRUCTOR_EXACT_20261108.md),
  [Owner ctor](TH09_PLAYER_OWNER_STATE_CONSTRUCTOR_EXACT_20261108.md),
  and [shot state ctor](TH09_PLAYER_STATE3031C_CONSTRUCTOR_EXACT_20261108.md).
- Type1 shot spawn and 309-byte `InitializeShot` use the real
  `PlayerLifecycleView`, 0x484-byte `PlayerShotCtorView`, and the SHT
  loader's 0x38-byte `PlayerShtDescriptorView`. Some *other* exact shot
  callback source declarations retain legacy `PlayerShotInitShotView`
  and `PlayerShotInitDescriptorView` adapters: do not confuse them with
  an independent runtime allocation.
- In the real `AnmManager`, frame counters are +0x0C **started**, +0x10
  **executed**, +0x14 **render-state changes**, +0x18 **flushes**.
  Earlier historical names had +0x0C and +0x10 swapped. See
  [live ANM evidence](RE_HANDOFF.md).
- Enemy `OnUpdate` is still **3,900 bytes with 13 unlike bytes**; its
  special Etama path now borrows the genuine `AsciiGameManagerSideView`
  from the Enemy manager. See [Enemy native side](TH09_ENEMY_ONUPDATE_NATIVE_GAMEMANAGER_SIDE_20261108.md).

## 3. Good next tasks: both large bosses and leaves

Consult the [13-owner live frontier table](RE_HANDOFF.md#reconstruction-frontiers)
before choosing a scope. Suggested independent attacks:

- `ReplayManagerView::SaveReplay` at **0x420C60**: 1,238-byte original /
  candidate, just **two** different ordinary bytes at +0xB7/+0xBB.
  Source: `src/ReplayInput.cpp`. Keep all 53 original fields/23 calls.
- `EnemyManagerView::OnUpdate` at **0x410730**: 3,900-byte boss, **13**
  differences after extensive natural source trials, all 97 fields and
  69 calls preserved. Source: `src/EnemyManagerCore.inl` within
  `src/EclManager.cpp`. The prior register-allocation explanation is an
  observed difference, not a proof all C++ alternatives are exhausted.
- `EclManager::RunEcl` at **0x4086C0**: 15,564 physical bytes, **30**
  differences in opcode 4 timer transport and opcode 155 flag update.
  All 598 original fields and owned jump tables must remain unchanged.
- `PlayerLifecycleView::CheckBulletCollision` at **0x41DFF0**: original
  862 / candidate 849, **13 missing bytes**, 26 original fields and
  12 calls. Native full 512-record collision pool is now typed.
- A smaller linked exact leaf can follow a boss discovery, but a
  near-exact candidate does **not** enter `matches.csv` until the **entire
  original physical code, tails and all relocation operands** match.

Do not adopt older speculative `volatile`, `_ReadWriteBarrier`, inline
assembly, register-clobber or fabricated return tricks to force a match.
Compile natural C/C++ under the original unit's pinned profile.

## 4. Follow-up boss probes that were not adopted

The current source was challenged with further natural VC7.1 C/C++ variants;
none changed the maintained source or improved the two large-owner frontiers.
The temporary variant trees and object files were removed after their results
were summarized here and in the dated boss investigation note.

- **Enemy `OnUpdate`:** additional local-index widths, pointer arithmetic,
  inline helper returns, copied/reference/bitfield flag snapshots, POD and
  `D3DXVECTOR3` descriptor-position overlays, `Float3` copy operators,
  descriptor evaluation-order changes, and split speed temporaries were tried.
  The baseline remains **3,900 physical bytes / 97 bound fields / 69 calls / 13
  unlike bytes**. Hoisting the side alias across the branch changed the owner
  extent to 3,852 bytes; custom descriptor assignment and split-speed forms
  worsened codegen. Reject those shapes rather than changing the physical
  extent or hiding differences.
- **`RunEcl`:** 90 combinations of natural opcode-4 timer operand forms and
  opcode-155 timeout-bit expressions compiled. None improved the current
  **15,564-byte / 598-field / 30-difference** owner. The opcode-155 semantic
  expression is the masked XOR update in `Th09EclRunState::AssignTimeoutSpellBit`;
  bitfield, union, local-result and alternate operand-lifetime forms changed
  register allocation or shifted later code. Keep the maintained wire snapshot
  and helper until a new complete-owner comparison proves a better form.

No variant is accepted because it compiles or improves a local block. Recheck
all original physical bytes, relocations, jump tables and affected siblings.

## 5. Restart commands and scoped checks

```bash
git status --short --branch
git log -8 --oneline
python3 -B scripts/verify-target.py
python3 -B scripts/validate-tracking.py --require-target
python3 -B scripts/report-reconstruction-status.py

# Example focused exact source + all physical relocations:
nice -n 12 python3 -B scripts/build-match-unit.py --unit player-initialize-shot
python3 -B scripts/compare-coff-function.py --unit player-initialize-shot --json

# Example noncrediting full Enemy boss diagnostic:
nice -n 12 python3 -B scripts/check-boss-frontiers.py --owner enemy --skip-exact-siblings

# Only after editing affected sources, for the known changed carriers:
nice -n 12 python3 -B scripts/replay-scoped-exact.py --source src/PlayerInitializeShot.cpp
```

Compiler profile and object comparisons may renumber VC7.1 `$L` symbols.
Do not simply change the manifest names to “make tests pass”: prove each
new COFF definition is in the same real owner section, has the same
owner-relative address and resolves to the **same immutable original PE
operand**, then recheck the *complete* owners. The source cold replay
can be used narrowly with multiple `--source` arguments; **never**
routinely invoke full-project/all-owner cold replay.

## 6. Storage cleanup and what is deliberately retained

This handoff cleanup removed only reviewed, regenerable or superseded scratch:

- **13 failed experiment directories / 24,356 files / 189,165,842 logical
  bytes** were removed. They contained copied source trees and non-adopted
  Enemy/RunEcl compiler variants. The per-file SHA-256 inventory is the ignored
  local manifest `.analysis/handoff-cleanup-20261108-agent/removed-scratch.json`;
  the findings worth carrying forward are summarized in section 4 and
  `TH09_GPT_WEB_BOSS_FRONTIERS_20261108.md`.
- The guarded `scripts/clean-generated-artifacts.py --apply` removed **1,357
  cache files / 279,337,498 logical bytes**: 32 `build/` files / 3,197,466
  bytes and 1,325 `.analysis/` debug/bytecode cache files / 276,140,032 bytes.
  It did not remove `.analysis/*.obj` files because no object-review list was
  supplied. Objects inside the explicitly removed failed experiment trees were
  removed with those trees.

The combined logical reduction was **468,503,340 bytes (about 446.9 MiB)**.
`build/` is absent and will be recreated by the next focused build. The
remaining `.analysis/` tree is **1,231,904,579 logical bytes** (about 1.4 GiB
allocated); it still contains historical evidence and saved Ghidra projects.
The pinned `.tools/wine/` runtime remains **604,127,593 bytes** and is not a
cache-cleanup target.

**Preserved, do not bulk-delete:** `resources/th09.exe`, `.tools/wine/`
(VC7.1, runtime state), `.analysis/ghidra-direct/` (saved analyzer
projects), `.analysis/handoff-archive-*/` (compressed historical source
and experiment backups), the retained `gpt-web-th09-boss-leaf-20261108`
baseline evidence and `runecl-probes/`, plus original PE/COFF comparison
reports. The cleanup audit is in
`.analysis/handoff-cleanup-20261013/generated-cache-manifest.json`; the
scratch deletion inventory is in
`.analysis/handoff-cleanup-20261108-agent/removed-scratch.json`. Both are
ignored local records, not files in a plain Git clone.

For further cleanup, run `python3 -B scripts/clean-generated-artifacts.py`
without flags first. Its default policy preserves analysis `.obj` files unless
an explicit reviewed path/SHA-256 list authorizes deletion. Never delete
`.analysis/` wholesale or mistake the Wine runtime, Ghidra projects or
historical archives for disposable caches. A fresh Git clone does not contain
the private target, toolchain, analyzer projects or local evidence; continue in
the same live worktree or restore the required inputs deliberately.

## 7. Handoff conventions

Keep user dirty/untracked work and finish it rather than discarding it.
Small, coherent commits use `gpt-web: <English summary>`. Git HEAD and
`config/matches.csv` are separate from Factory acceptance, Git remote
publication, native game linking and runtime semantic validation.
Current knowledge is in the [live handoff](RE_HANDOFF.md); longer
November 8 work notes are preserved in
[the dated snapshot](RE_HANDOFF_HISTORY_20261108.md) and
[historical evidence index](KNOWLEDGE_BASE.md).
