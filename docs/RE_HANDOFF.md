# TH09 reconstruction handoff (2026-11-21)

Player movement remains paused by the operator.
[Progress](PROGRESS.md)

## Start the session

Read [AGENTS.md](../AGENTS.md). Inspect staged, dirty and untracked work
before changing it. Run these commands before target-dependent work.

```bash
git status --short --branch
python3 -B scripts/verify-target.py
python3 -B scripts/validate-tracking.py --require-target
python3 -B scripts/report-reconstruction-status.py
```

Pick one source-present owner and rebuild its full frontier with the
profile in `config/frontiers.toml`. Keep that owner as the focus through
strict acceptance. Follow its real callees, same-carrier siblings and shared
data when the evidence points beyond its body. Use the direct Ghidra runner
for new analysis. It checks the disk target and the saved Program's
architecture, entry and every file-backed mapped byte before export.
[Comparing compiler contexts](RE_WORKFLOW.md#trace-a-difference-through-its-compiler-context)

```bash
python3 -B scripts/check-frontier.py NAME --show
python3 -B scripts/ghidra-analysis.py --function ADDRESS
```

## Current nonexact owners

The table records the last measured physical extents. Frontier counts
mask candidate COFF fields. Compare relocation identities and addends
separately. An OK result means the recorded frontier was preserved.
[Frontier manifest](../config/frontiers.toml)

| Owner | Target address | Current comparison | Remaining work |
| --- | --- | --- | --- |
| PlayerLifecycleView::UpdateMovementAndOptions | 0x0041C170 | 1,900/1,900 bytes, 4 structural differences, 8 after relocation replay (OK) | `player-movement`. Paused by the operator. Retain EAX for the side index and ECX for the configuration address while recovering the configuration-first order and indexed CMP. [Focus-load controls](KNOWLEDGE_BASE.md#2026-11-20-player-movement-focus-loads) |

The active `etama-fsincos` diagnostic at 0x00412780 compares 36/33 bytes
with 25 structural differences. The caller remains exact with either
declaration or definition visible. The outstanding evidence is a source
or library provider for the paired x87 operation.
[Compiler and origin controls](KNOWLEDGE_BASE.md#2026-11-21-etama-paired-trigonometric-helper)

## Recent exact checkpoints

- `Supervisor::UpdateNetworkService` matches 1,633 bytes and 106 relocations.
  Block-local seed and prediction outputs let VC7.1 reuse the original
  stack slots and keep the input loads in EDI and EBX.
  [Network service proof](KNOWLEDGE_BASE.md#2026-11-21-network-service-retry-and-input-context)
- `PlayerLifecycleView::CheckBulletCollision` matches 862 bytes and 26
  relocations. The inner scan and common hit-count exit recover the original
  dispatcher layout and cursor lifetime.
  [Collision proof](KNOWLEDGE_BASE.md#2026-11-21-checkbulletcollision-reward-storage-and-ordered-bounds)
- `PlayerLifecycleView::UpdateChargeAttack` matches 1,210 bytes and 70
  relocations. Two row-end lookups recover the early base/index encodings.
  [Charge input proof](KNOWLEDGE_BASE.md#2026-11-21-chargeattack-input-and-timer-allocation)
- `TitleScreenView::OnUpdateResult` reproduces the original byte load and
  sign extension when VC7.1 merges the two cursor-direction calls.
  [Result update proof](KNOWLEDGE_BASE.md#2026-11-21-title-result-cursor-direction)
- `RunEcl` recovered its final register choices by restoring natural source
  in other handlers. [RunEcl context](KNOWLEDGE_BASE.md#runecl-register-coupling-across-handlers)
- `EnemyManagerView::OnUpdate` needed the exact timer definition visible in
  its carrier and the authentic draw-list control flow.
  [Enemy update proof](KNOWLEDGE_BASE.md#2026-11-21-enemy-update-exact)
- `ReplayManagerView::BeginPlaybackStage` needed the visible, byte-identical
  `RngRuntimeView::SetSeed` body to recover scheduling across the restore
  block. [Replay seed proof](KNOWLEDGE_BASE.md#2026-11-21-co-visible-replay-seed-and-exact-playback)
- `DrawReplayMenu` needed separate caption-table ownership and a shared
  selected-slot reference. `SaveReplay` retains its own Shift-JIS table
  proof. [Title replay proof](KNOWLEDGE_BASE.md#2026-11-21-title-replay-caption-tables)
  [SaveReplay proof](KNOWLEDGE_BASE.md#2026-11-20-savereplay-payload-copy-and-user-tables)

## Finish the selected owner

Require a cold pinned VC7.1 replay of the complete physical extent and all
fields, plus checked tables, literal data and associated unwind contributions
where applicable. Rebuild the changed carrier and its affected exact
consumers. Use `--changed-file` with `--dry-run` first for a shared header or
inline file to check the dependent scope. Track the chosen owner's promotion
in the exact ledger and regenerate progress. Use basic CI and review the
diff before the local checkpoint. [Oracle policy](ORACLES.md)

```bash
python3 -B scripts/replay-scoped-exact.py --source src/FILE.cpp
python3 -B scripts/replay-scoped-exact.py --changed-file src/FILE.hpp --dry-run
python3 -B scripts/progress.py
python3 -B scripts/ci.py
git diff --check
```

## Private inputs and storage

Preserve the original `resources/th09.exe` and game data, `.tools/`, the
`_reference/` checkout, the active Ghidra project
`.analysis/ghidra-direct/243bffea8dcbe99a/`, and `.analysis/factory-runtime/`.
These include this game's toolchain, runtime selectors and Wine state.

Keep the existing C2 project roots `c2-analysis/project/` and
`c2-analysis/allocator-project/`, `c2-analysis/c2.dll`, its scripts and
retained top-level allocator reports. The small compiler context comprises
`.analysis/c2-heap.bin`, `.analysis/FUN_107844a4.decomp.txt`, the four
`.analysis/gdb-c2-*.cmd` recipes, and the
`SupervisorNetwork.natural-ebx-reload.cpp`,
`PlayerCollisionHelpers.natural-preheader-842.cpp`, and
`PlayerCollisionHelpers.nested-three-loop-849.cpp` controls under
`.analysis/`. They retain allocator traces and CFG controls for unresolved
work. Rebuild selected objects when `build/` is absent. Keep cleanup scoped
to identified disposable outputs. [Shared tools](SHARED_TOOLS.md)
