# TH09 exact reconstruction — agent handoff (verified 2026-11-15)

**Start:** [AGENTS.md](../AGENTS.md) · [workflow](RE_WORKFLOW.md) ·
[original/VC7.1 Oracles](ORACLES.md) ·
[knowledge base](KNOWLEDGE_BASE.md).
This is the **current restart checklist**, not a session log; older session
notes live in Git history and the knowledge base. **Start with `git status`,
recover any dirty changes, then regenerate ledger counts and only the
compiler objects you need.**

## Verified target, counts and evidence boundaries

- **Target:** original Japanese TH09 **v1.50a**, Windows i386, PE image base
  `0x00400000`, entry `0x0047D45F`. Ignored, protected
  `resources/th09.exe` SHA-256:
  `10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
  Do not patch, rename, commit or delete the target.
- **Ledger (2026-11-14):** 2,248 candidates, **1,037 source-present
  authored mappings**, **1,026 exact**, **11 nonexact / 13,353 logical
  bytes**, 35 reviewed unresolved origins. Exact authored bytes
  **278,578 / 291,931 = 95.4260%**. Complete original-code inventory is
  **not** proven. Use `scripts/report-reconstruction-status.py` and the
  generated [progress](PROGRESS.md) rather than copying totals.
- **Phase:** exact reconstruction is unfinished. A whole Windows i386
  product, dual-Oracle semantic tests and port are **not** proven.
- **Proof hierarchy:** original PE + attested Ghidra for analysis; pinned
  VC7.1 COFF for candidates; `config/matches.csv`, `config/functions.csv`
  and `config/match-units.toml` for exact claims. Partial similarity is
  NONEXACT. Rebind private `$L…`/`$name$…` labels only after checking the
  same-section COFF definition, manifest target and original PE operand.

## What changed in the 2026-11-14/15 sessions

- **RunEcl is exact** (unit `ecl-run-ecl`, 14,792 logical / 15,564
  physical bytes). Plain statements replaced mutually compensating
  `goto` merges and snapshot locals; see the knowledge base section
  "VC7.1 register choice is coupled to neighbouring source".
- **BeginPlaybackStage** is plain C++ and now 540/540 bytes (was 538/540).
- **SaveReplay** and **Player movement** cast/address scaffolding was
  replaced by plain indexing with byte-identical codegen.
- No further owner became exact. Every residual below is a VC7.1
  register-choice or scheduling tie; source spelling, statement order,
  profiles (`/O1`, `/O2`, `/G5`–`/G7`), undocumented `c2.dll` options and a
  different compiler build were all ruled out as levers. The knowledge
  base section "allocator and folding observations" records what was
  tried so it is not repeated.

## Remaining source-present nonexact owners

Measured 2026-11-14 with each owner's pinned profile. "Masked" counts
exclude relocation fields; dedicated inspectors may report slightly higher
numbers.

| Owner | Target address | Current comparison | Remaining work |
| --- | --- | --- | --- |
| **Enemy OnUpdate** | 0x00410730 | 3,900/3,900, **2 bytes** (+0x19C/+0x1A3) (EclManager `/O2 /Ob1`) | Early draw-group index ECX vs EAX. Removing the `volatile sideState` read brings back 13 bytes. `check-frontier.py enemy-on-update`. |
| **Replay SaveReplay** | 0x00420C60 | 1,238/1,238, **2 bytes** (+0xB7/+0xBB) (`/O2 /Ob1`) | Header-copy buffer load ECX vs EDI; source is now plain. 128 spelling combinations are byte-neutral. `replay-save`. |
| **Player movement** | 0x0041C170 | 1,900/1,900, 8 bytes (4 masked) at +0x127 (`/O2 /Ob1`) | `g_GameConfiguration`/`sideIndex` load order; every subscript spelling compiles side-first. `player-movement`. |
| **Effect AddedCallback** | 0x0040D1C0 | 51/51, 8 bytes (4 masked) (`/O2 /Ob1`, no `/EHsc`) | ESI/EDI swap; exhaustive sweeps show retail is unreachable from the body alone. `effect-added-callback`. |
| **Title OnUpdateResult** | 0x004266B5 | **484/485**, one site at +0xFD (Title `/O1 /Ob1`, no `/Oi`) | Retail keeps the selected character in CL and sign-extends separately; VC7.1 folds ours (only a real intervening store unfolds it). That single site causes the 1-byte shift behind ~166 masked differences. `title-update-result`. |
| **ScreenEffect shake** | 0x004227A0 | 336/336, 26 bytes (`/O2 /Ob1`) | ESI/EDI `screenEffect`/`&timer` swap. `screen-shake-envelope`. |
| **Title DrawReplayMenu** | 0x004234F6 | 1,280/1,280, 29 bytes (Title `/O1 /Ob1`) | Selected-record EDX/ECX. `title-draw-replay-menu`. |
| **Replay BeginPlaybackStage** | 0x00420840 | **540/540**, 13 masked / 19 raw at +0x14B–0x15F (`/O2 /Ob1`) | Plain C++ (no `volatile` alias). VC7.1 hoists the `sides[0].runtime` load above the +0x37C store; retail issues it after the +0x1E8 load. Statement moves/swaps, array and side-pointer forms do not move it. `replay-begin-playback`. |
| **Player ChargeAttack** | 0x0041F810 | 1,210/1,210, 183 masked (`/O2 /Ob1`) | Retail: int side in EDI, input in EBX, `sideMode` spilled to `[ebp-1]`. Current `short`/`volatile` forms are compensation. `player-charge-attack`. |
| **Player CheckBulletCollision** | 0x0041DFF0 | **849**/862, ~149/269 aligned instructions (`/O2 /Ob1`) | Retail loop is unrotated with a duplicated latch. Retail also enregisters `bullet` and leaves `position` in memory. `player-bullet-collision`. |
| **Supervisor network service** | 0x00430AA0 | 1,629/1,633, ~266/417 aligned instructions (`/O2 /Ob1`) | First divergence: retail keeps the network pointer in EDI; ours CSEs constant `1` into EDI. `supervisor-network-service`. |

## Reproduce *only* the changed compiler carriers

`build/` objects, PDBs and comparison JSON are **ignored disposable build
caches**, removed in the storage cleanup. Missing objects are expected, not
missing sources or original target data; each selected command regenerates
only the necessary carriers.
Do **not** start with `--owner all`, a project-wide cold replay, or
`ci.py --full` just to repopulate caches.

```bash
git status --short --branch             # inspect AND recover dirty work
git diff --check
git log -7 --oneline
python3 -B scripts/verify-target.py
python3 -B scripts/validate-tracking.py --require-target
python3 -B scripts/report-reconstruction-status.py

# Nonexact functions: rebuild and fail if any matching byte regresses
# (diagnostic only; names and residuals live in config/frontiers.toml).
nice -n 12 python3 -B scripts/check-frontier.py                 # all, ~80 s
nice -n 12 python3 -B scripts/check-frontier.py replay-save --show
python3 -B scripts/check-frontier.py replay-save --update       # record an improvement

# Preserve all genuinely affected exact siblings after modifying a source:
python3 -B scripts/replay-scoped-exact.py --changed-file src/EclRunState.inl --dry-run
nice -n 12 python3 -B scripts/replay-scoped-exact.py --changed-file src/EclRunState.inl
# Or use --source src/ReplayInput.cpp for a single source-owned cohort.

# One exact function when needed; builds the selected original-bound carrier:
nice -n 12 python3 -B scripts/build-match-unit.py --unit ecl-pop-context
python3 -B scripts/compare-coff-function.py --unit ecl-pop-context --json

# For documentation/ledger edits (no game compiler necessary):
python3 -B scripts/validate-docs.py
python3 -B scripts/ci.py
```

`replay-scoped-exact.py` only checks previously **exact** canonical functions,
not the nonexact boss in the same TU. Editing a shared `.inl` can change
private symbol names in several VC7.1 carriers. Compile and check every
affected physical owner *before* changing manifest symbol spellings. Avoid
compiler register-forcing hints, artificial padding and target-patching.

For new target analysis use attested `scripts/ghidra-analysis.py`, for example
`python3 -B scripts/ghidra-analysis.py --function 0x004086C0`. This validates
the imported Program and original mapped PE bytes. The earlier Ghidra
pseudocode under ignored `.analysis/ghidra-direct/` is a **hypothesis**, not
a byte-exact source receipt; a fresh import can be necessary after a Ghidra
tool upgrade.

## Next steps (ranked)

1. **Compiler model (the only open lead).** Map `c2.dll`'s candidate order
   and tie-break: colouring driver `0x107821a7`, colour pass `0x107844a4`,
   and where internal symbol IDs are assigned. Entry points are in the
   knowledge base; rebuild the scratch Ghidra project from a hash-checked
   copy of the pinned `c2.dll` outside any `.`-prefixed path.
2. With a tie-break rule in hand, start with the smallest owners:
   BeginPlaybackStage (one load placement), Effect AddedCallback (the
   context needs one more pre-store reference), SaveReplay, Enemy OnUpdate.

## Practical pitfalls

- Compile experiment copies **inside the repository** (e.g. a directory
  under `.analysis/`); compiling from `/tmp` under Wine is ~8x slower.
  Ghidra refuses project paths containing a `.`-prefixed element, so put
  Ghidra scratch projects outside `.analysis/`.
- Title code uses `/O1` without `/Oi`; EffectManager omits `/EHsc`. Check
  `config/match-units.toml` for the owner's unit profile before measuring.
- An automated source-variant search (the Cirno-Labs permuter's rewrite
  functions driven by a local compile/compare harness) was used locally
  and is **not** committed (GPL-3.0 reference code); its results are
  recorded in the knowledge base.
- Repository `.cpp/.hpp` files are CRLF in the working tree
  (`.gitattributes`); edit them with CRLF-aware tools.
- A quoted `#include` resolves next to the including file first. To test an
  edited shared `.inl`/`.hpp`, copy the including `.cpp` next to it; `/I`
  order alone does not override `src/`.
- Comparing private `$L` labels is part of exactness: byte-identical edits to
  `EnemyManagerCore.inl` still renumber labels pinned by exact RunEcl and
  CompareOperands units.
- Parallel Wine compiles can leave `_CL_*` compiler scratch files in the
  repository root; delete them.
- `scripts/ci.py --full` runs the 82 target-independent tooling tests in a
  few seconds and must stay green. Source-spelling snapshot tests and
  per-owner inspectors were removed on 2026-11-15; exactness evidence is the
  canonical comparison, and nonexact progress is `check-frontier.py`.

## Storage / protected inputs / commit discipline

Keep `resources/th09.exe`, the `.tools/` Wine/VC7.1 toolchain selectors,
`.analysis/ghidra-direct/`, `_reference/`, and `.analysis/gpt-web/`.
`build/` and Python caches are disposable. Do not commit executables,
analyzer databases, toolchains or decompiler output. Use the commit prefix
the operator requests (`claude-opus-5.5-high:` in the last session,
`gpt-web:` per AGENTS.md otherwise), commit coherent checkpoints, and never
push silently.
