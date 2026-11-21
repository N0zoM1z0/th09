# TH09 reconstruction handoff (2026-11-21)

Resume in the existing checkout. Commit `587008ef` closed
`TitleScreenView::DrawReplayMenu` and added the title caption section to its
canonical comparison. Check the local branch before using a fresh checkout.
Use the generated ledger for the exact count and remaining authored bytes.
[Progress](PROGRESS.md)

Earlier probe history is available with
`git show 587008ef:docs/RE_HANDOFF.md`.

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
Both REGRESSED owners still have strict baselines in the manifest.
[Frontier manifest](../config/frontiers.toml)

| Owner | Target address | Current comparison | Remaining work |
| --- | --- | --- | --- |
| PlayerLifecycleView::UpdateMovementAndOptions | 0x0041C170 | 1,900/1,900 bytes, 4 structural differences, 8 after relocation replay (OK) | `player-movement`. Paused by the operator. Retain EAX for the side index and ECX for the configuration address while recovering the configuration-first order and indexed CMP. [Focus-load controls](KNOWLEDGE_BASE.md#2026-11-20-player-movement-focus-loads) |
| PlayerLifecycleView::UpdateChargeAttack | 0x0041F810 | 1,210/1,210 bytes, 183 differences (OK) | `player-charge-attack`. The entry needs EDI for the full DWORD side index and EBX for the input-row pointer. Both registers are reused by later timer receivers. A side local live through sound pan conflicts with that reuse. [Entry and timer controls](KNOWLEDGE_BASE.md#chargeattack-entry-register-and-timer-reuse-probes-2026-11-19) |
| PlayerLifecycleView::CheckBulletCollision | 0x0041DFF0 | 849/862 bytes, 539 differences (REGRESSED against the 842-byte baseline) | `player-bullet-collision`. Preserve the 0x34 frame and EDI=Player, ESI=slot, EBX=Bullet. Compare the X87 radius edge and the circle/shared-dispatch/rectangle order. [CFG and register controls](KNOWLEDGE_BASE.md#2026-11-20-checkbulletcollision-boss-cfg-and-register-frontier) |
| TitleScreenView::OnUpdateResult | 0x004266B5 | 484/485 bytes, 167 differences (OK) | `title-update-result`. Align instruction boundaries and find the first real encoding or CFG divergence before changing the character array or sign extension. The exact `OnUpdateResultNameEntry` sibling shares `src/TitleScreen.cpp`. [Existing Title character-table proof](KNOWLEDGE_BASE.md#2026-11-11-native-title-character-order-unlock-and-screen16-tables) |
| Supervisor::UpdateNetworkService | 0x00430AA0 | 1,633/1,633 bytes, 307 differences (REGRESSED against the 1,629-byte baseline) | `supervisor-network-service`. Keep the 0x14 frame, EBX frame/prefill lifetime and sender reloads. The target branches from the +0x3F8 packet-ready test to +0x422 input handling. Prior source labels and split wait returns left the emitted return block out of line. [Frame and packet recovery](KNOWLEDGE_BASE.md#updatenetworkservice-native-frameprediction-and-packet-flow-recovery-2026-11-19) |

`title-update-result` is a bounded starting point if Player movement
remains paused. Begin with the complete Title carrier and keep its exact
siblings as controls. For `CheckBulletCollision`, the target's shared hit
dispatcher is at `+0x7E`, the zero-radius branch at `+0xAB`, the rotated
rectangle at `+0xBD`, and the AABB path at `+0x13B`. Geometry hits return
to the dispatcher, misses advance the slot at `+0x19E`, and cancellation
exits at `+0x1A6`. The `etama-fsincos` diagnostic remains open for origin
review.

## Recent exact checkpoints

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
