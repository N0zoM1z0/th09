# TH09 reconstruction — next-agent restart (2026-11-03)

**Start here, not with a dated "current" statement deep in an old log.**
This is a navigation/checkpoint document, **not** an exactness receipt. The
live Git worktree, tracking manifests, target bytes and freshly compiled
COFF always overrule it. The previous
[2026-11-02 handoff](TH09_NEXT_AGENT_HANDOFF_20261102.md),
[historical RE handoff](RE_HANDOFF.md) and
[knowledge archive](KNOWLEDGE_BASE.md) preserve research history, including
superseded intermediate hypotheses. Do not restore old private layouts just
because a dated note calls them current.

## Immediate restart

```bash
git status --short --branch
git log -8 --oneline
python3 -B scripts/verify-target.py
python3 -B scripts/validate-tracking.py --require-target
python3 -B scripts/report-reconstruction-status.py
```

Inspect and **continue any dirty changes**, including ignored experiment
scratch; the operator says this is one worktree, not a competing-agent
checkout. Commit substantive progress in multiple focused commits using
`gpt-web: <English summary>`. Do not push without an explicit request.
**Never run `check-boss-frontiers.py --owner all` or a full all-owner cold
replay** for routine reconstruction: build/replay only changed owners,
source-sharing exact siblings and true dependencies. `--skip-exact-siblings`
is useful for *exploration*, but is **not** a regression certificate.

The only supported original is Japanese TH09 **v1.50a**, ignored and never
committed at `resources/th09.exe`: SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
Use **direct local Ghidra** (`scripts/ghidra-analysis.py`) for new
original-binary analysis, attesting its actual mapped PE bytes; use the pinned
VC7.1 compiler and PE/COFF comparisons for exactness. A connector's
repository shell is an execution interface, **not** a replacement for the
original-binary Oracle. Do not infer TH09 facts from other Touhou versions.

## Current ledger and stage (locally checked 2026-11-03)

| Measure | Verified value |
| --- | ---: |
| Tracked function candidates (inventory incomplete) | 2,248 |
| Authored/source-present mappings | 1,035 |
| Canonical exact functions | **1,005** |
| Source-present non-exact owners | **30** |
| Non-exact authored bytes | 44,286 |
| Exact authored bytes / authored bytes | 247,597 / 291,883 (84.8275%) |
| Authored mappings without source | 0 |

This is **not 84.8% of the game**. The source inventory/boundaries and
whole-program linker/resource graph are not completely closed; Windows i386
product build and runtime verification remain open. Semantic and portability
stages have **not** started. Exact source and local Git commits do not prove
runtime behavior, remote publication or a new accepted Factory receipt.

## Verified big-function boundaries and priorities

| Scope | Original / candidate | Current diagnostic, not exactness credit |
| --- | --- | --- |
| `RunEcl` (`--owner runecl`) | 15,564 / 15,564 physical bytes | **30 unlike bytes**: opcode 4 has 7, opcode 155 has 23; 598 fields, 96 exact source siblings on last complete scoped replay |
| `EnemyManager::OnUpdate` (`--owner enemy`) | 3,900 / 3,900 | **13 unlike bytes**; 97 bound fields, 69 ordered calls, 88 exact source siblings |
| Player `UpdateMovementAndOptions` (`--owner movement`) | 1,900 / 1,900 | **122 unlike bytes**, 66 fields, 23 calls, 435 aligned instructions and 2 exact siblings; **not** the old 129-byte frontier |
| `ReplayManagerView::SaveReplay` (`--owner replaysave`) | 1,238 / 1,238 | **3** differences at `+0xB7`, `+0xBB`, `+0x3F6`, 53 fields, 23 calls; still non-exact |
| `Supervisor::OnUpdate` (`--owner supervisor`) | **996 target-owned / 952 candidate-owned** | 44 absent original bytes and 791 different overlapping bytes; do not equate its 844 authored *code* bytes with its 996-byte owned code-plus-tables extent |
| `EffectManager::AddedCallback` (`--owner effect`) | 51 / 51 | 8 unlike bytes; no exact credit |
| `TitleScreenView::DrawResult` (`--owner resultdraw`) | 1,939 / 1,939 | 3 unlike bytes; no exact credit |

At subsequent 2026-11-03 source checkpoints, **full scoped** `runecl`,
`enemy` and `movement` owner checks were repeated with their exact siblings;
`resultdraw`, `replaysave` and `supervisor` have owner-scoped diagnostics at
their stated source epochs, not fresh complete sibling certificates. The
`Effect` cohort was also checked with its exact siblings. None of these
non-exact owners has been promoted. Rebuild the affected scope before any new
exactness claim.

## Recent source changes worth preserving

- `bc26b958`: the **3,238/3,238 exact** Player selector's 34 playfield
  references now use the real `g_GameManager` base +0x360..+0x36C.
  The same-TU pattern offset resolver is **412/412 physical bytes exact**
  (**346** canonical function bytes). Its local `$L` labels were rebound
  only after actual COFF destinations were checked against original VAs.
- `3cd23f37`: the **1,900-byte non-exact** Player movement owner directly
  uses the native focus/secondary Effect fields and a typed first replay
  input entry. The *second* auxiliary input read remains separate for
  original-bound codegen; do not deduplicate it blindly.
- `a5dbf3b9`: native Player shot/movement/initial-state fields replace
  duplicated `PlayerAddedState` storage. `InitializeAddedState` remains
  **572/572 physical bytes exact**, consisting of **552** canonical bytes
  plus a 20-byte reviewed tail. Its 23/29/41-byte same-TU leaves stay exact.
- Recent Enemy/ECL commits moved ANM VMs, context/call-stack, side flags,
  trails, bullets and callback state into original-bound native classes.
  They **did not** reduce the 30/13 big-boss residuals or increase 1,005.
- The currently observed pre-handoff source commit was `a5dbf3b9`;
  **do not use this line to identify HEAD** after documentation/cleanup commits.

## Subsequent 2026-11-03 source/ABI checkpoints (newer than the historical list)

The following are **local Git source checkpoints**, not Factory acceptance or
proof of a runnable native TH09 game. `git log` and current manifests are still
more authoritative than this dated routing note.

- `81f8b75b` and `ff2acddc`: the native `Effect` now overlays separate
  12-byte enemy-reward and bullet-cancellation payloads at `+0xA0..+0xAB`.
  `HandleDeathRewards` stays **1,142/1,142 exact**. The still-open 862-byte
  Player bullet-collision owner retains its 849-byte natural candidate,
  26 source/target fields and 12 calls; its exact siblings were verified.
- `4fa96ff2`: the **3,900-byte** Enemy `OnUpdate` uses the real
  `EnemyManagerView` rather than a parallel manager object. Its full scoped
  check is still **13** differing bytes / 97 bound fields / 69 calls;
  **88** related canonical exact replays pass. Twenty re-numbered private
  `$L` references were independently validated using actual COFF definitions
  and the original PE destination words before updating only their spellings.
- `13374255` and `8797af99`: Player owner-reward and
  `UpdateBeforeState` store typed `Effect` payloads (including +0x98 delay),
  not parallel private reward-effect objects. The large **1,519 logical /
  1,540 physical byte** reward owner and **814 logical / 852 physical byte**
  Player pre-state owner remain complete exact. Both have target-bound label
  renaming evidence; 17 Player runtime same-carrier exact owners were replayed.
- `b40efda2`: the **392-byte** exact `KillAllNonBossEnemies` scans the
  real native `EnemyManagerView::enemies5758[129]` and native `EnemyView`
  layout, with six actual relocation fields. The method's original source
  provenance remains provisional.
- `53d75b8b`: the **15,564-byte** `RunEcl` opcode-155 handler uses the
  real native `EnemyView::primaryFlags337C`; the full 598-field owner remains
  at 30 ordinary differing bytes (opcode 4: 7; opcode 155: 23), and **96**
  exact source/dependency siblings pass. The 20 compiler-private label names
  changed only after same-section definition/target-address verification.
  Native Enemy `OnUpdate` and the 392-byte clear leaf remain unchanged.
- `d84a4644`: Player `header24.anmFile98` is a native `AnmLoaded *` and
  its runtime draws / movement use the real `AnmManager` service instead of
  duplicated void-pointer interfaces. Seventeen exact Player runtime owners
  replayed **4,368 physical bytes / 243 COFF fields**. The four other
  Player runtime compiler profiles and the independent exact 20-byte
  `AnmManager::GetAnm` leaf pass. The 1,900-byte movement boss stays at
  **122** differences, 66 fields and 23 calls; both movement siblings pass.

**Newly probed, not adopted:** fourteen natural score-bank/screen-read
source models for the 1,939-byte `DrawResult` kept its three register-choice
mismatches at `+0x1D`, `+0x23`, `+0x39` or regressed size/control flow.
The original full owner still has 82 independent fields and sixteen ordered
calls. Likewise, six native timeout-flag variants (plus the baseline) left `RunEcl` at 30
mismatches. The independently recompiled 1,238-byte `SaveReplay` still has
three differences (`+0xB7`, `+0xBB`, `+0x3F6`), with all 53 fields and 23
calls accounted for. All rejected source probes were restored. Their local
ignored observations remain under `.analysis/gpt-web-resultdraw-*`,
`.analysis/gpt-web-runecl-native-timeout-*` and
`.analysis/gpt-web-replay-save-*`; these files are not source acceptance.

**Ledger unchanged at these checkpoints:** 1,005 canonical exact of 1,035
authored functions; 30 source-present non-exact. Source-normalization and
successful local commits are useful preservation, but are not new exact
credit, whole-product build closure, runtime equivalence, or remote push.

## Efficient next work / replay commands

```bash
# A fast diagnostic while iterating a big owner; no sibling protection yet.
python3 -B scripts/check-boss-frontiers.py --owner movement --skip-exact-siblings
# Final scoped original-bound checks with only related exact siblings.
python3 -B scripts/check-boss-frontiers.py --owner movement
python3 -B scripts/check-boss-frontiers.py --owner enemy
python3 -B scripts/check-boss-frontiers.py --owner runecl

# A leaf: build its recorded source/profile before comparing.
python3 -B scripts/build-match-unit.py --unit player-initialize-added-state
python3 -B scripts/compare-coff-function.py --unit player-initialize-added-state --json
# Always distinguish canonical 552 from physical compare_size 572.
```

For the particular change, run only its applicable subset; **do not** run
all three bosses reflexively. On shared `EclManager.cpp` changes, the
compiler-private `$L` names may change. Verify every new label's actual
COFF section-relative definition reaches the **unchanged original VA**
before editing a manifest *symbol spelling*. Never alter target VAs,
relocation types/offsets or mask different bytes to obtain a passing check.
An `inspect-*` diagnostic's overlap similarity does not award exactness.

High-value next investigations: Player movement's register allocation / ANM
transition instructions and lower-clamp block (122 residuals); RunEcl
opcode-4 / 155 register/wire scheduling; Enemy update's draw-chain and
special-attack copy registers (13); Replay SaveReplay's remaining 3;
Supervisor OnUpdate's genuinely missing 44 bytes. Prior natural-C++
probe matrices are hypotheses only, not proof that a different approach
cannot work. Attack **both** complete big functions and smaller leaf/ABI
owners, as the operator requires.

## Storage / analysis artifacts

`build/` is an ignored, reproducible cache; its absence is normal after a
reviewed clean. Original `resources/th09.exe`, tracked source/manifests,
`.tools/` compiler/Ghidra symlinks and `.analysis/ghidra-direct/` projects
are **not** disposable. Current analyses/source experiments under
`.analysis/20261102-*` and all historical directories directly referenced
by tracked source/docs have intentionally been left accessible.

On 2026-11-03, **1,038** historical `gpt-*` / `th09-*` directories
with no direct references in tracked source/docs were first archived to
`.analysis/handoff-archive-20261103/legacy-unreferenced-experiments.tar.zst`
and **all 18,452 files reverified by SHA-256** before unpacked copies were
removed. The 608.35 MiB of loose files compress to 28.94 MiB. The archive
SHA-256 is `1c287d8f1190692966b568a6ebfa62d6f3995dd6873b5782248dbdd2353a2223`.
This archive and the full path/hash manifest are **local-only, not pushed**.
See [cleanup history](RE_ARTIFACT_CLEANUP_HISTORY.md) for a per-directory
restoration command and the precise **590.5 MiB combined net logical savings**.

The built-in reviewed cleaner additionally removed 605 build-cache files
(16,093,238 bytes); its dry run now returns zero. Therefore `build/` is
absent, **not** a source/Oracle loss. Rebuild an affected original-bound
unit before comparing. Do not delete remaining `.analysis/*.obj` without a
separate path/SHA-256 review; original, Ghidra and operator-managed
databases remain protected. If the archive is absent in another checkout,
redo old experiments from maintained sources and the original PE; an
archive is not canonical acceptance evidence.

Read [AGENTS.md](../AGENTS.md), [RE_WORKFLOW.md](RE_WORKFLOW.md),
[ORACLES.md](ORACLES.md), and affected source + manifest rows for policy.
The dated [knowledge base](KNOWLEDGE_BASE.md) supplies leads, not
current-source authority. Use [PROGRESS.md](PROGRESS.md) for ledger-derived
counts and [TOOLS.md](TOOLS.md) for the supported command surface.
