> **Historical November 7 snapshot.** This guide was superseded by the
> [November 8 next-agent handoff](TH09_NEXT_AGENT_HANDOFF_20261108.md).
> The figures and “latest/current” notes below describe the November 7
> checkpoint, not the present worktree or cache state.

# TH09 — next-agent handoff, current source and storage (2026-11-07)

This is the **operational restart guide**, refreshed after the November 7 ANM
counter/cache work and safe storage cleanup. Read [AGENTS.md](../AGENTS.md),
[live ledger/frontiers](RE_HANDOFF.md), [workflow](RE_WORKFLOW.md),
[Oracle evidence rules](ORACLES.md) and [tool routing](TOOLS.md).
Previous entries titled "latest" in older handoffs are **historical**. The
former 560-line live log is retained in
[RE_HANDOFF_HISTORY_20261107.md](RE_HANDOFF_HISTORY_20261107.md); dated source
experiments remain in [the original-bound source log](TH09_GPT_WEB_NATIVE_BOSSES_20261107.md).

## 1. Establish actual state; do not assume a saved HEAD

```bash
git status --short --branch
git diff --check
git log -8 --oneline
python3 -B scripts/verify-target.py
python3 -B scripts/validate-tracking.py --require-target
python3 -B scripts/report-reconstruction-status.py
python3 -B scripts/validate-docs.py
```

The only valid original is **Japanese TH09 v1.50a** at the ignored operator
path `resources/th09.exe`, SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
Do not substitute localized/patched files or old adjacent-game binaries.
The VC7.1 compiler profiles/objects are pinned by `config/match-units.toml`.

At the last ledger validation: **2,248 function candidates**, **1,034
source-present mapped**, **1,021 exact**, **13 nonexact** spanning **29,834
original bytes**. A further **38** reviewed function origins remain
unresolved. Authored exact bytes **262,034 / 291,868 (89.7783%)**; the
current 98% target still needs **23,997** exact bytes. This is not proven
complete inventory, executable-wide byte parity, Windows i386 product closure,
runtime behavior, porting, or Factory acceptance.

**Latest source checkpoint preceding this documentation refresh:**
`9c17b496` (ANM shared vertex-buffer release). The preceding new native ANM
counter commits were `03cf4b33`, `d3c44fcf`, and regression `910f653a`;
`78776e7e` corrected the counter identity/order. Subsequent documentation
commits advance HEAD, so the live `git log` is authoritative. All commits are
local until an explicit remote publication; never infer a push from a hash.

## 2. Corrected facts to carry forward

The earlier wrong ANM counter names were corrected **against exact original
code and independent call sites**. In the same physical `AnmManager`:

| Offset | Correct original-bound source-facing member |
| --- | --- |
| `+0x0C` | `scriptsStartedThisFrame0C` — incremented by `AnmLoaded::SetAndExecuteScript` |
| `+0x10` | `scriptsExecutedThisFrame10` — incremented by `AnmManager::ExecuteScript` |
| `+0x14` | `renderStateChangesThisFrame14` — incremented by both render-state methods |
| `+0x18` | `flushesThisFrame18` — shared with ANM rendering |

`ResetFrameDebugInfo` is an exact **15-byte leaf**, with original machine
stores ordered **+0x10, +0x14, +0x0C, +0x18** (not declaration order).
Related exact witnesses are `AnmManager::ExecuteScript` **9,358 code /
9,776 compared bytes**, seven runtime methods **507 physical bytes**,
16 ANM Render functions **5,814 bytes**, and the **31-byte** shared
`ReleaseVertexBuffer` leaf. The shared `AnmManagerQuadCacheView` has a
`quadVertexBuffer` pointer at +0x12890 and software vertices at +0x12894.
The native render cache world matrix is at +0x12024. All source-facing
projection/derived type names remain *reconstruction views*, not proven
original C++ class or translation-unit names.

The exact **3,238-byte** Player selector uses the real input global
`g_ReplayInputStates` at `0x4ACE18` (three **0x8E**-byte slots), not the old
private `g_PlayerSideProtocols`; its counter at `0x4A7E4C` is shared with
`Player::EnterDeathState`. Its independent original-bound direction resolver
is **346 code / 412 compared bytes**. Authentic FrontSide timing, native
ZunTimer, RNG, shared runtime and lifecycle methods are now directly bound.

**Do not misclassify experiments as accepted source:** ChargeAttack's
`volatile sideMode` variant can reach the original **1,210-byte length** and
350 instructions, but still differs in registers, branches and other bytes.
The maintained source remains **1,197 bytes, nonexact**, until the complete
PE/COFF + graph acceptance passes. Replay SaveReplay remains 1,238/1,238
with two unlike bytes; BeginPlaybackStage remains 540/538. See
[all 13 exact open frontiers](RE_HANDOFF.md#reconstruction-frontiers).

## 3. What to investigate next — large bosses and real leaves

Start from the fixed original target, challenge legacy assumptions, try
**natural C/C++ source**, and protect complete physical code/table extents.
Reasonable open priorities (all **nonexact**, no acceptance implied):

- **ChargeAttack:** 1,210 target / 1,197 maintained candidate; 70 fields,
  13 bytes missing. `volatile` source tests recovered length but **not**
  original EBX/EDI input-index ownership or control-flow graph. Evidence in
  ignored `.analysis/gpt-web-charge-mode-stack-20261107/` and
  `.analysis/gpt-web-charge-input-regalloc-20261107/`.
- **Replay SaveReplay / BeginPlaybackStage:** 1,238 bytes with 2 unlike;
  540/538 bytes with 39 overlap differences and 2 missing. Source-side
  receiver and seed-move variants were compiled and rejected when they did
  not improve the actual original-bound result. Replay's existing 12 exact
  siblings must remain exact.
- **RunEcl / Enemy update:** 15,564 physical bytes / 30 differences, and
  3,900 / 13. Preserve all reviewed original operands, table addresses and
  the target byte-read widths. Some natural rewrites had deceptively equal
  byte counts but altered an original-bound memory-read width.
- **Player movement / Title menu / Effect and Shake:** 1,900/122, 1,280/29,
  51/8 and 336/26 residuals; keep the focused diagnostic contracts and
  compare the original instruction sequence, not only result length.

For a changed exact function:

```bash
nice -n 12 python3 -B scripts/build-match-unit.py --unit anm-manager-execute-script
python3 -B scripts/compare-coff-function.py --unit anm-manager-execute-script
# Example separate leaf profile:
nice -n 12 python3 -B scripts/build-match-unit.py --unit anm-manager-release-vertex-buffer
python3 -B scripts/compare-coff-function.py --unit anm-manager-release-vertex-buffer
# Nonexact scope example, never all owners:
python3 -B scripts/check-boss-frontiers.py --owner charge --skip-exact-siblings
```

Run changed-owner tests rather than the forbidden all-owner cold replay. Exact
comparators must pass **complete original-bound bytes, fields, call targets,
addends, and independent table tails**. Nonexact inspectors make no exactness
credit claim. Preserve all dirty, staged or untracked changes; investigate
and finish them instead of resetting/ignoring them. Save each meaningful
checkpoint with a `gpt-web: <English description>` subject; make several
reviewable commits rather than combining unrelated source experiments.

## 4. Storage, evidence and cleanup guardrails

**The latest reviewed cache cleanup removed 312 disposable files
(25,372,141 logical bytes, about 24.20 MiB).** The guarded cleaner's
repeat dry-run returned zero files. It is normal for `build/` directories
to remain while **all cached compiler objects have been removed**; the next
agent must rebuild only the selected original-bound unit. A missing OBJ is
not a missing authored source file. This current cleanup is **separate**
from the previously documented November 7 cache/Git repack (205.20 MiB)
and older November 6 archives; do not double-count historical savings.

Safe workflow:

```bash
python3 -B scripts/clean-generated-artifacts.py            # preview
python3 -B scripts/clean-generated-artifacts.py --apply    # reviewed rebuildable caches only
python3 -B scripts/clean-generated-artifacts.py            # should be zero
# Selective hash-verified archive restore, only when actually needed:
python3 -B scripts/restore-analysis-archive.py \
  --bundle .analysis/handoff-archive-20261106 \
  --prefix build/analysis/runecl-timeout-storage-20261106
```

**Never delete** the operator's ignored `resources/th09.exe`, toolchain,
three `.analysis/handoff-archive-2026110{3,4,6}` bundles/manifests,
`.analysis/ghidra-direct/` projects, unique source trials or otherwise
unreviewed `.analysis/*.obj`. Before/after hashes were compared for the
original and all three historical archives; **507 loose `.analysis/*.obj`**
and **1,467 Ghidra project files** were checked unchanged. The newest local
cleanup receipt is
`.analysis/gpt-web-handoff-refresh-20261107/verified-cleanup-receipt.json`.
Those ignored bytes and their private proofs are **not included in ordinary
Git clones**; selective restoration may be required. See
[cleanup history](RE_ARTIFACT_CLEANUP_HISTORY.md) for archived receipts and
[tools](TOOLS.md) for Ghidra/VC7.1 setup.

**Hand-off state:** the exact ledger is still **1,021 exact / 13 nonexact**.
This is a tested and committed local C++ reconstruction, **not** a complete
Windows executable, runtime pass, or Factory acceptance.
