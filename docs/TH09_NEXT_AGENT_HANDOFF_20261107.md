# TH09 handoff to the next agent — 2026-11-07

**This is the current entry point.** Read [AGENTS.md](../AGENTS.md),
[the live ledger and all 13 frontiers](RE_HANDOFF.md#reconstruction-frontiers),
[the reconstruction workflow](RE_WORKFLOW.md), and [Oracle contracts](ORACLES.md)
before editing. The [November 6 handoff](TH09_NEXT_AGENT_HANDOFF_20261106.md)
records the *earlier cleanup state*, not the current source or working tree.
Earlier result notes and negative experiments are **dated observations**, not
permission to fit targets or assume the original author used those expressions.

## Most recent native source continuation — 2026-11-07

The newest source-only checkpoint through `2798a84b` builds on the earlier
four-commit report linked below. New work actively revised both the 3,900-byte
Enemy boss and the exact 1,561-byte D3D initialization and 2,652-byte
Background stage-script bosses. See the [fresh full-byte evidence and
reproduction commands](TH09_GPT_WEB_NATIVE_BOSSES_20261107.md#follow-up-true-supervisor-camera-owners-and-enemy-positional-transport).
The source commits are `126bc837`, `52d2ede5`, `e6d42e9c` and `2798a84b`;
all have the requested `gpt-web:` prefix. The original Japanese executable,
compiled code extents, fields and affected exact siblings were rechecked
locally. The **1,021 exact / 13 nonexact** authored status is *unchanged*;
those commits do not establish runtime/product/Factory acceptance. Do not
throw away dirty source or run full `--owner all` when continuing.

## Additional live-session source checkpoint — 2026-11-07

After this handoff was written, the single active operator committed four
additional `gpt-web:` source changes (`a815c6c0`, `9b905a85`, `3cab7f1e`,
`ca415d42`). The [new complete boss/leaf source and negative-model report](TH09_GPT_WEB_NATIVE_BOSSES_20261107.md)
records original-bound sizes, the native layouts adopted, the rejected
Effect/Replay experiments, and the narrow reproduction commands. The authored
ledger is still **1,034 present / 1,021 exact / 13 nonexact**. The older
"most recent" list below describes the *earlier* November 7 handoff source
advances; consult the real Git HEAD before every new experiment.

## Current verified state and limitations

The authoritative counts are the *live* `config/functions.csv`,
`config/function-origins.csv`, `config/matches.csv`, and
`config/match-units.toml`. At this handoff (checked with the real local
Japanese v1.50a original on November 7):

| Measure | Verified value |
| --- | ---: |
| Provisional function candidates | 2,248 |
| Reviewed but origin-unresolved | 38 |
| Authored functions / source-present mappings | 1,034 / 1,034 |
| Canonical exact original-bound authored functions | **1,021** |
| Source-present, still nonexact authored functions | **13** |
| Reviewed-authored exact bytes | **262,034 / 291,868 (89.7783%)** |
| Bytes needed for the current 98% authored-byte target | **23,997** |

This is not executable-wide coverage, a full product link, a runtime pass, a
semantic reconstruction, or Factory acceptance. Do not upgrade a near match
merely because its calls, fields or blocks agree. The 38 origin-unresolved
functions, including the separately byte-exact VectorAngle recipe, have **no
authored-exact credit**. The candidate inventory is still not proven complete.

The original input is the *ignored*, operator-provided `resources/th09.exe`,
Japanese TH09 1.50a, SHA-256:
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
Do not substitute patched/English/other releases. The compiler is pinned
VC7.1. Direct Ghidra and the fixed compiler/full original byte comparator are
**independent**; Ghidra decompiler labels and provisional types are not proofs.

Run at the start of the *new* session:

```bash
git status --short --branch
git diff --check
git log -8 --oneline
python3 -B scripts/verify-target.py
python3 -B scripts/validate-tracking.py --require-target
python3 -B scripts/report-reconstruction-status.py
python3 -B scripts/validate-docs.py
```

**Recover any dirty/staged/untracked work; do not discard it.** Do not assume
the HEAD or `ahead` value saved in a historical note remains current. The
last reconstructed-source HEAD *before this maintenance handoff* was
`7ec8e316`; maintenance commits deliberately advance the local HEAD.
Only the actually returned `git status`/`git log` define the current branch.
The original branch may be synced with `origin/main` locally; remote
publication or Factory acceptance must not be inferred from commit history.

## Most recent actual source advances

The November 7 source work made **six reviewable `gpt-web:` commits**, from
`494a0e2b` through `7ec8e316`, with substantial work in both large bosses
and small leaf functions. Highlights:

- [ANM Draw3D](TH09_ANM_DRAW3D_NO_SHADOW_MATRIX_20261107.md): its 811-byte
  original-exact owner uses the actual `AnmVm` fields and SDK matrix views;
  the redundant `AnmVmDraw3DView` is gone.
- [ANM common cache](TH09_ANM_COMMON_RENDER_CACHE_20261107.md): real one-
  allocation render-cache layout now shared by PrimitiveDraw, Draw3D,
  Background and Title; 19 complete exact owners / 2,999 bytes / 138 fields.
- [ANM Render core](TH09_ANM_RENDER_CORE_NATIVE_STATE_20261107.md) and
  [DrawInner](TH09_ANM_RENDER_NATIVE_INNER_VM_20261107.md): shared cache and
  real VM color/alpha/flags/sprite ownership now cover **35 exact affected
  owners / 8,813 original physical bytes / 533 bound fields**, including
  both 1,067-byte DrawInner bosses. These were **already exact functions**;
  source fidelity improved, the exact numerator did not.
- [Player CheckBulletCollision](TH09_PLAYER_COLLISION_NATIVE_POOL_TYPE_20261107.md):
  one real pool reference and typed region snapshot, after **12 natural C++
  compile tests**; the entire original-bound 862/849 boss remains **nonexact**.
  Seven exact dependent owners passed the focused `--owner player` check.

November 6 updates to Player runtime, Title replay-menu, RunEcl argument
layout, and gameplay setup also made real C++ source improvements with
**no new matching-credit claim**. They are routed through the dated entries
in [RE_HANDOFF.md](RE_HANDOFF.md).

## Still-open complete-function frontiers

The original and current candidate extents are distinct from a similarity
score. Each owner below has a fixed source/profile and *noncrediting* complete
PE/COFF diagnostic. Use the listed focus owner, **not** `--owner all`.

| Owner | Original / candidate status | Local focus |
| --- | --- | --- |
| RunEcl `0x4086C0` | 15,564 physical (includes 772-byte switch table), 598 fields, **30** unlike ordinary bytes | `--owner runecl` |
| Enemy OnUpdate `0x410730` | 3,900 physical, 97 fields, **13** unlike bytes | `--owner enemy --skip-exact-siblings` |
| Player movement `0x41C170` | 1,900 physical, 66 fields, **122** unlike bytes | `--owner movement` |
| ChargeAttack `0x41F810` | 1,210 original / 1,197 candidate, 70 fields, 13 missing | `--owner charge` |
| Player CheckBulletCollision `0x41DFF0` | 862 / 849, 26 fields, 799 unequal overlaps + 13 missing | `--owner player` |
| Replay SaveReplay `0x420C60` | 1,238 / 1,238, 53 fields, only **2** unlike bytes at `+0xB7/+0xBB` | `--owner replaysave` |
| Replay BeginPlaybackStage `0x420840` | 540 / 538, 32 fields, 39 overlaps + 2 missing | `--owner replay` |
| ShakeEnvelope `0x4227A0` | 336 / 336, 20 fields, 26 unlike bytes | `--owner screen` |
| Title DrawReplayMenu `0x4234F6` | 1,280 / 1,280, 57 fields, **29** unlike bytes | `--owner replaymenu` |
| Title OnUpdateResult `0x4266B5` | 485 / 484, 34 fields, 218 overlaps + 1 missing | `--owner title` |
| Gameplay setup worker `0x41AF2D` | 1,689 / 1,682, 172 fields, 694 overlaps + 7 missing | `--owner gamemgr` |
| Supervisor network `0x430AA0` | 1,633 / 1,627, 106 fields, 1,381 overlaps + 6 missing | `--owner network` |
| Effect AddedCallback `0x40D1C0` | 51 / 51, 4 fields, 8 unlike bytes | `--owner effect` |

The detailed sources, boundaries, residual guard requirements and exception
cases are in [RE_HANDOFF.md](RE_HANDOFF.md#reconstruction-frontiers).
**Promising starting points:** SaveReplay (two register-encoding bytes),
Enemy OnUpdate (13 scattered bytes), RunEcl opcodes 4/155 (30 bytes), and
Title replay-menu (29 bytes). These are not known easy fixes. Prior negative
trials are not exhaustive; always verify current PE and compiler artifacts.

## Efficient focused workflow

Build the affected exact unit using
`python3 -B scripts/build-match-unit.py --unit NAME`, then require
`python3 -B scripts/compare-coff-function.py --unit NAME` for every affected
complete physical owner, including data tails and COFF fields. For bosses,
use `python3 -B scripts/check-boss-frontiers.py --owner OWNER` with the
specific owner name above; its diagnostics **never grant exactness credit**.
Protect already-equal bytes, original field identities, literal definitions,
branch graphs and exact same-carrier siblings. Explore several *natural*
C/C++ source models with the pinned profile, retain useful failed-model
measurements privately, and commit actual source progress as
`gpt-web: <concise English summary>`. No register-forcing hacks, assembly,
barriers, fake returns, padding, profile roulette or full all-target cold
replay. No source-only “cleanup” just to manufacture exact-match credit.

The real repo is the registered live worktree. Do not switch to another
checkout, assume another session is modifying dirty files, or treat old
analysis output as current code. Original nominal class/TU/ownership claims
remain explicitly provisional until TH09 evidence verifies them.

## Storage: current cleanup and artifact recovery

The local cache trees `.analysis/`, `build/`, and toolchain links are **ignored
and absent in a plain Git clone**. Historical source-proof scripts sometimes
reference ignored inputs that may have been archived in November 3/4/6
bundles. Do **not** delete those bundles, their manifests, the two direct
Ghidra projects, unique trial sources, SHA-pinned target, or toolchain.
Do not mistake an absent `build/matching/*.obj` for lost source; rebuild the
specific pinned unit rather than running all-target replay.

See [the artifact cleanup history](RE_ARTIFACT_CLEANUP_HISTORY.md) for the
current cleanup receipt, byte accounting and exclusions. The guarded cache
cleaner is `python3 -B scripts/clean-generated-artifacts.py` (preview), then
repeat with `--apply` to remove **regenerable** `build/` objects/PDBs and
`.analysis/` debug/bytecode caches. `.analysis/*.obj` is preserved unless a
specific file is explicitly SHA-reviewed for deletion. To restore a known
historical archived member, preview:

```bash
python3 -B scripts/restore-analysis-archive.py \
  --bundle .analysis/handoff-archive-20261106 \
  --prefix build/analysis/runecl-timeout-storage-20261106
# Add --apply only after reviewing the manifest and exact requested paths.
```

The November 7 cleanup removed **183 regenerated cache files**
(15,827,383 bytes) and safely repacked Git objects from 224,861,877 to
25,522,760 bytes (`--prune=never` with reflog expiry disabled). The total
reclaimed is **215,166,500 logical bytes (205.20 MiB)**. The original input,
all three historical archives, both Ghidra projects, and 507 existing
analysis OBJ files were retained. All live refs, reachable Git objects,
reflogs and HEAD were hash-checked unchanged; see
[the detailed 2026-11-07 receipt](RE_ARTIFACT_CLEANUP_HISTORY.md#2026-11-07--current-handoff-refresh-reviewed-caches-and-no-prune-git-repack).
`build/` is absent again after this reviewed cleanup; only focused builds
should regenerate it.

The **2026-11-06 handoff's statement that `build/` was absent** was correct
only at that historical checkpoint. Later compiler sessions recreated the
cache, and the current maintenance can clean it again. Do not report the
2026-11-06 reclaimed bytes as newly recovered in a later session. Keep local
Git checkpoints distinct from Oracle receipts and remote publication.
