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
- No further owner became exact. The pinned `c2.dll` register-choice path is
  now partially mapped: `FUN_107833f2` scores candidates from weighted
  interference costs and preserves the first candidate on ties. See the
  knowledge-base sections "VC7.1 allocator: weighted register-conflict
  selection" and "VC7.1 C2 internal IDs and color-pass traversal". Enemy
  OnUpdate's two-byte EAX/ECX residual remains open. Moving the draw-head
  computation before the no-sprite test changes the group-byte register to
  EDX and expands the owner to 27 differences; the current question remains
  the exact C2 live-set/weighted-conflict cost, not another spelling sweep.
- A narrow `SupervisorNetworkState *initialNetwork` alias after the critical
  section did not change the 1,629/1,633-byte, 1,038-difference network
  carrier. Volatile result-home probes expanded the frame to 0x18 and were
  reverted; the target `[ebp-4]` home and EDI network pointer remain open.
- A `ScreenEffect::CalcShakeEnvelope` duration-reference probe regressed to
  340/336 bytes / 252 differences and was reverted; its 336-byte/26-offset
  baseline remains intact.
- **Player CheckBulletCollision** has a protected 853/862-byte, 692-difference
  carrier. The best diagnostic remains **840/862 bytes / 632 differences**
  with the retail circle-first branch order, but it has five backedges versus
  the target's four and a short reward tail. New loop-latch probes bracketed
  the topology: a two-level shared-latch form reached 827/862 / 691 differences
  with three backedges (geometry latches tail-merged); a deeper form reached
  837/862 / 660 differences with five backedges (two inner scan latches).
  A one-loop form rotated the null test to the bottom. None was promoted.
  The 840-byte candidate's five exact same-TU siblings passed scoped replay
  (1,220 bytes / 32 fields). The maintained source and frontier metadata
  remain at 853/862. See the 2026-11-15 collision-latch convergence entry.

## Remaining source-present nonexact owners

Measured 2026-11-14 with each owner's pinned profile. "Masked" counts
exclude relocation fields; dedicated inspectors may report slightly higher
numbers.

| Owner | Target address | Current comparison | Remaining work |
| --- | --- | --- | --- |
| **Enemy OnUpdate** | 0x00410730 | 3,900/3,900, **2 bytes** (+0x19A/+0x1A1) (EclManager `/O2 /Ob1`) | Early draw-group index ECX vs EAX. Helper parameter width, direct address arithmetic, function-scope `drawHead`, switch and early-goto shapes retain the two-byte residual. `check-frontier.py enemy-on-update`. |
| **Replay SaveReplay** | 0x00420C60 | 1,238/1,238, **2 bytes** (+0xB7/+0xBB) (`/O2 /Ob1`) | Header-copy buffer load ECX vs EDI; source is now plain. 128 spelling combinations are byte-neutral. `replay-save`. |
| **Player movement** | 0x0041C170 | 1,900/1,900, 8 bytes (4 masked) at +0x127 (`/O2 /Ob1`) | `g_GameConfiguration`/`sideIndex` load order; every subscript spelling compiles side-first. `player-movement`. |
| **Effect AddedCallback** | 0x0040D1C0 | 51/51, **4 bytes** in two windows (`/O2 /Ob1`, no `/EHsc`) | Retail uses EDI for the global ANM manager and ESI for callback context; current source reverses the setup order. Direct-global-for-both and local-snapshot-for-both probes regressed to 25 and 7 differences respectively. `effect-added-callback`. |
| **Title OnUpdateResult** | 0x004266B5 | **484/485**, one site at +0xFD (Title `/O1 /Ob1`, no `/Oi`) | Retail keeps the selected character in CL and sign-extends separately; VC7.1 folds ours (only a real intervening store unfolds it). That single site causes the 1-byte shift behind ~166 masked differences. `title-update-result`. |
| **ScreenEffect shake** | 0x004227A0 | 336/336, 26 bytes (`/O2 /Ob1`) | ESI/EDI `screenEffect`/`&timer` swap. `screen-shake-envelope`. |
| **Title DrawReplayMenu** | 0x004234F6 | 1,280/1,280, 29 bytes (Title `/O1 /Ob1`) | Selected-record EDX/ECX. `title-draw-replay-menu`. |
| **Replay BeginPlaybackStage** | 0x00420840 | **540/540**, 13 masked / 19 raw at +0x14B–0x15F (`/O2 /Ob1`) | Plain C++ (no `volatile` alias). VC7.1 hoists the `sides[0].runtime` load above the +0x37C store; retail issues it after the +0x1E8 load. Statement moves/swaps, array and side-pointer forms do not move it. `replay-begin-playback`. |
| **Player ChargeAttack** | 0x0041F810 | 1,210/1,210, 183 masked (`/O2 /Ob1`) | Retail: int side in EDI, input in EBX, `sideMode` spilled to `[ebp-1]`. Current `short`/`volatile` forms are compensation. `player-charge-attack`. |
| **Player CheckBulletCollision** | 0x0041DFF0 | **853/862**, 692 differing offsets / 26 relocation fields (`/O2 /Ob1`) | Protected carrier. Best unpromoted diagnostic is 840/862 / 632 differences with circle-first layout, 279 instructions, 32 decoded blocks and five backedges (target four). It breaks protected matches and leaves the reward tail short; do not update frontier metadata. Five exact siblings pass. `player-bullet-collision`. |
| **Supervisor network service** | 0x00430AA0 | 1,629/1,633, **1,038 differing bytes** (`/O2 /Ob1`) | Retail keeps the network pointer in EDI and result in `[ebp-4]`; ours CSEs constant `1` into EDI and folds the result home. Snapshot/predicate/return-shape probes did not improve it. A scratch-union plus cached-pointer candidate regressed to 1,540/1,633 and 1,155 differences and was reverted. `supervisor-network-service`. |

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

1. **Compiler model (the only open lead).** Continue from the mapped
   `c2.dll` scoring path: identify the virtual value for Enemy's draw-group
   byte and compare its candidate-register availability/weighted conflict
   costs. Internal node IDs and bitset traversal are already mapped; the
   remaining work is the source-to-IR interference difference. The ignored
   scratch project is reusable; rebuild from the hash-checked pinned backend
   outside any `.`-prefixed path only if it is absent.
2. Continue the Player CheckBulletCollision boss from the 840/862
   diagnostic geometry layout while keeping the 853/862 carrier protected.
   The target has four backedges: one shared invalid/circle-miss latch, distinct
   rotated-square and AABB latches, and a cancel-invalid latch. The two-level
   probe reached three because geometry latches tail-merged; the deeper probe
   reached five because invalid/delay and circle miss remained separate.
   Find a source shape with exactly four while retaining the top-tested header
   and circle-first branch, then attack the 22-byte shortfall and reward tail.
   Preserve its five exact siblings with scoped replay; never weaken the
   protected-byte guard.
3. Continue Enemy OnUpdate's two-byte EAX/ECX allocation question through the
   mapped C2 weighted-conflict path, not another local spelling sweep. For
   smaller owners, AddedCallback is currently four bytes (not the historical
   one-byte diagnostic); only reopen it with a real context/global-pointer
   lifetime hypothesis. Do not repeat the failed ScreenEffect aliases,
   TitleResult table-index forms, or the documented BeginPlaybackStage and
   SaveReplay variants.

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
- `c2-analysis/` is a local scratch bundle containing the pinned C2 backend,
  Ghidra project/dumps and source probes. Keep it available for the weighted
  allocator investigation, but do not stage its DLL, analyzer database or
  decompiler dumps in source commits.
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
