# TH09 next-agent handoff — 2026-11-02

**Read this first**, then inspect the actual Git worktree. The authoritative
project is the original Japanese TH09 **v1.50a**, with ignored local
`resources/th09.exe` SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
This is a **local source checkpoint**, not a pushed branch, a Factory-accepted
current-source receipt, a completed Windows executable, or a runtime proof.

## Git and ledger at the handoff snapshot

The last **source-changing** commit before this handoff was `e4ea703b`
(`gpt-web: unify both Supervisor ThreadClose receivers with native runtime`).
The local `main` was clean and **52 commits ahead of `origin/main`** at that
instant. Subsequent handoff/cleanup commits advance HEAD; **always check live
`git log` and `git status`**, never hard-code these hashes as latest.

Authoritative ledgers: `config/functions.csv`, `config/matches.csv`,
`config/function-origins.csv` and `config/match-units.toml`. The original-bound
checks at this snapshot report **2,248** candidate functions,
**1,035** authored/source-present, **1,005 canonical exact**,
**30 source-present nonexact** (44,286 bytes), and **37 origin-unresolved**.
Exact authored coverage is **247,597 / 291,883 = 84.8275%**. The
98% milestone, native product link, runtime semantics and ports remain **open**.

## Real current source ownership (historical overlays are obsolete)

- **ANM**: One shared `AnmLoaded` **0x1C** resource record, at offsets
  +0x04/+0x08/+0x0C/+0x10/+0x14/+0x18; the former
  `AnmLoadedResidentView.hpp` 8-byte-only view has **been deleted**.
  `AnmManager::ExecuteScript` is **exact for the full 9,776-byte compared
  extent with 368 original relocations**, not just its 9,358 authored code
  bytes. Resource/load/runtime commits include `182071b3`, `707e9616`,
  `1bcbf9d1` and `4e9b0ca3`.
- **GameManager**: One native `AsciiGameManagerView` with two **0x38** side
  records, each sharing the **0xA0** score/phase allocation and seven
  gameplay subsystem pointers. Replay/Setup use the same +0xB0..+0xE8
  fields. The **1,230-byte/87-field `OnUpdate` is exact**; the **1,689-byte
  `GameplaySetupThread` remains nonexact**, with 172 target vs. 171
  candidate fields, 35 ordered calls, 78/77 direct blocks and **904
  differing bytes**. Important checkpoints include `de83c0d4`, `74145f9e`,
  `ced1017f`, `eeb035e6`, `adeac064`, `a835cef3` and `5dd8be98`.
- **Supervisor**: The actual **0x7B0** owner now includes startup, DInput,
  MIDI, 0xF0 ctor-array frames, queues, deletion, and thread state.
  `StartupThread` **882 bytes /106 fields exact**, `DeletedCallback`
  **405/44 exact**, `ThreadClose` **21/0 exact**, DInput enum callback sizes
  **93/94/94 exact**; frame queue pop **119** and predicted insert
  **191** exact. **`Supervisor::OnUpdate` remains NONEXACT:** target physical
  extent **996**, candidate **952**, 92 fields/27 calls, 791 overlapping
  differing bytes and 44 absent bytes. Newer commits include `f359c201`,
  `e4ea703b`, `0b84231f`, `79612f34`, `f60b1f61`, `2fa230b7`,
  `22cf89a8`, `5235f666` and `73bf0ed2`. Do not resurrect
  `SupervisorUpdateLayout` or any old full-size startup/deletion overlays.
- **Other unsolved major owners**: `RunEcl` **15,564 bytes /598 fields,
  30 ordinary differences** (opcode 4 has 7; opcode 155 has 23);
  `EnemyManager::OnUpdate` **3,900 bytes /97 fields/69 calls/13 different
  bytes**; `ReplayManager::SaveReplay` **1,238 bytes/53 fields/23 calls,
  three differing bytes** at +0xB7/+0xBB/+0x3F6; Etama `OnDraw`
  **600 bytes/268 differences**; Title `DrawResult` **1,939 bytes/three
  differences**. Similarity, unchanged exact sibling replays and authored
  local source files **are not new exact matches**.

## Where to attack, and what already failed

The complete `RunEcl` function has valid original-bound calls, relocations
and switch tables; the remaining two instruction families are register/codegen
problems. Natural opcode-4 raw-int direct-access variants widened differences
from 30 to 42, and opcode-155 byte/word lifetime rewrites disrupted subsequent
switch tables. All failed trial source changes were reverted.

`EnemyManager::OnUpdate` has all 1,026 instructions, 69 call identities,
97 relocations and 170 CFG blocks aligned; 13 nonfield byte differences
are two draw-group register choices plus 11 differences in the effect/stack
descriptor path. Reordering side-state or descriptor lifetimes either made no
change or moved original literal fields. Failed trials were reverted.

`GameplaySetupThread` has a real register-base mismatch: the target indexes
side records using `+0x34` (flags) while the candidate prefers `+0x28`
(selector); dozens of pointer declarations/register/flags-anchor rewrites
failed to improve its 904-byte residual. Preserve the authentic 35 calls,
not a forced target-byte patch. The historical Title barrier is a narrow
source-specific no-op ordering constraint, **not** a generic compiler trick.

## Fast safe restart and scoped verification

From the registered live `th09` repository, with no pending other agent:

```sh
git status --short --branch
git log -5 --oneline
python3 -B scripts/verify-target.py
python3 -B scripts/validate-tracking.py --require-target
python3 -B scripts/report-reconstruction-status.py
python3 -B scripts/validate-docs.py
```

Build **only the touched source/carrier** before comparison. For example:

```sh
python3 -B scripts/build-match-unit.py --unit supervisor-startup-thread
python3 -B scripts/compare-coff-function.py --unit supervisor-startup-thread --json
python3 -B scripts/check-boss-frontiers.py --owner supervisor
python3 -B scripts/check-boss-frontiers.py --owner enemy --skip-exact-siblings
python3 -B scripts/check-boss-frontiers.py --owner runecl --skip-exact-siblings
```

The `--skip-exact-siblings` option is for quick boss exploration, **not**
verification of a source change's affected exact siblings. Check them locally
before commits; `docs/TH09_SCOPED_EXACT_REPLAY.md` documents selective
carrier/profile replays. Never run a full `--owner all` cold replay for
ordinary iterations. After artifact cleanup `build/matching/*.obj` can be
absent: **regenerate them**; historical .analysis objects are not a fresh
current-source proof. Treat VC7.1 compiler-private `$L` names as generated:
only rename after proving their COFF-local destinations match the unchanged
original PE addresses, never alter original destination/relocation kinds to
make a test pass.

The exactness ledger remains **1,005** until a genuinely new complete,
original-bound proof is reviewed. Use `gpt-web: ...` for coherent local Git
commits, **preserve incoming dirty work**, and do not push without explicit
instruction. Historical KB and 2026-10-31 reports are evidence for their
specific snapshots, not current-source specifications.

## Storage / preserved proof

`.analysis/` holds ignored experiment scripts, candidate sources and
original-bound JSON results. `build/` and analysis .obj/.pdb/.pyc are
generated compiler caches; see `scripts/clean-generated-artifacts.py` for
preview, SHA256 review and apply. The dated cleanup summary is in
`docs/RE_ARTIFACT_CLEANUP_HISTORY.md`, and the actual reviewed-path audit
is under ignored `.analysis/handoff-cleanup-20261013/`. The original target,
Git-tracked source/ledgers, unreproducible analysis evidence, native Ghidra
projects and the installed toolchain were preserved. Do **not** delete the
remaining JSON source/provenance packets or `.analysis/` en masse merely
because earlier diagnostics were nonexact. Some historical paths named by
old reports were already cleaned and must be rebuilt or recovered via their
documented source/hash inputs.
