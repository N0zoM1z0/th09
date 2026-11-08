# TH09 — next agent: clean restart (2026-11-08)

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

The most recent source recovery in this handoff is `e861abb0`:
`PlayerLifecycleView::InitializeShot` now owns the exact **309-byte**
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

## 4. Restart commands and scoped checks

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

## 5. Storage cleanup and what is deliberately retained

On this cleanup, the existing hash-guarded
`scripts/clean-generated-artifacts.py` removed **1,967 generated files
(117,570,122 logical bytes, about 112.12 MiB)**:

- **1,354 files / 79,860,933 bytes** of disposable `build/` outputs.
- **613 files / 37,709,189 bytes** of `.analysis` compiler objects,
  debug symbols and bytecode. This included **559 `.obj` files**, each
  approved by a SHA-256 path/file review at
  `.analysis/handoff-cleanup-20261108/reviewed-analysis-objects.json`.

The dry-run after deletion selected **zero** remaining cache files.
`build/` was removed, so a future compiler test must first compile its
selected match unit. This is normal and not a broken source checkout.
The ignored cleanup audit is at
`.analysis/handoff-cleanup-20261013/generated-cache-manifest.json`.

**Preserved, do not bulk-delete:** `resources/th09.exe`, `.tools/wine/`
(VC7.1, runtime state), `.analysis/ghidra-direct/` (saved analyzer
projects), `.analysis/handoff-archive-*/` (compressed historical source
and experiment backups), all ignored scripts/candidate sources, and
original target/COFF comparison reports. Those ignored inputs and local
receipts are **not included in a plain Git clone**. The new agent should
use the same live registered repository/worktree or restore those inputs
separately; Git alone cannot supply the private game binary/toolchain.

Preview additional safe caches with
`python3 -B scripts/clean-generated-artifacts.py`; use `--apply` only
for a reviewed nonempty plan. Avoid deleting `.analysis` wholesale or
mistaking the ~579 MiB pinned Wine environment for disposable cache.

## 6. Handoff conventions

Keep user dirty/untracked work and finish it rather than discarding it.
Small, coherent commits use `gpt-web: <English summary>`. Git HEAD and
`config/matches.csv` are separate from Factory acceptance, Git remote
publication, native game linking and runtime semantic validation.
Current knowledge is in the [live handoff](RE_HANDOFF.md); longer
November 8 work notes are preserved in
[the dated snapshot](RE_HANDOFF_HISTORY_20261108.md) and
[historical evidence index](KNOWLEDGE_BASE.md).
