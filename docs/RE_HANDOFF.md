# TH09 exact reconstruction — agent handoff (verified 2026-11-14)

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

## What changed in the 2026-11-14 session

- **RunEcl is exact** (unit `ecl-run-ecl`, 14,792 logical / 15,564
  physical bytes, 598 fields, both switch tables). The old 7+23-byte
  residual was VC7.1 register choice coupled to *neighbouring* handlers;
  earlier hand-written `goto` merges and snapshot locals were compensating
  for each other. Plain statements fixed both sites. See the knowledge
  base section "VC7.1 register choice is coupled to neighbouring source".
- `ApplyNetworkInput` uses its original `u16 (int, u16)` signature and
  stays exact; the network service tail now has retail's shape.
- Guards and tests that hard-coded "RunEcl is nonexact" (30-byte residual,
  83 EclManager units, 49 angle callsites) were updated; obsolete RunEcl
  residual probes and source-text snapshot tests were removed.

## Remaining source-present nonexact owners

Measured 2026-11-14 with each owner's pinned profile. "Masked" counts
exclude relocation fields; dedicated inspectors may report slightly higher
numbers.

| Owner | Target address | Current comparison | Remaining work |
| --- | --- | --- | --- |
| **Enemy OnUpdate** | 0x00410730 | 3,900/3,900, **2 bytes** (+0x19C/+0x1A3) (EclManager `/O2 /Ob1`) | Early draw-group index ECX vs EAX. Removing the `volatile sideState` read brings back 13 bytes. `--owner enemy`. |
| **Replay SaveReplay** | 0x00420C60 | 1,238/1,238, **2 bytes** (+0xB7/+0xBB) (`/O2 /Ob1`) | Header-copy buffer load ECX vs EDI. All address arithmetic can be written plainly with identical bytes. `--owner replaysave`. |
| **Player movement** | 0x0041C170 | 1,900/1,900, 8 bytes (4 masked) at +0x127 (`/O2 /Ob1`) | `g_GameConfiguration`/`sideIndex` load order. `--owner movement`. |
| **Effect AddedCallback** | 0x0040D1C0 | 51/51, 8 bytes (4 masked) (`/O2 /Ob1`, no `/EHsc`) | ESI/EDI swap; exhaustive sweeps show retail is unreachable from the body alone. `--owner effect`. |
| **Title OnUpdateResult** | 0x004266B5 | **484/485**, one site at +0xFD (Title `/O1 /Ob1`, no `/Oi`) | Retail keeps the selected character in CL and sign-extends separately; VC7.1 folds ours. That single site causes the 1-byte shift behind all ~166 masked differences. Best near-term target. `inspect-title-update-result.py` after building `title-screen-setup-thread`. |
| **ScreenEffect shake** | 0x004227A0 | 336/336, 26 bytes (`/O2 /Ob1`) | ESI/EDI `screenEffect`/`&timer` swap. `--owner screen`. |
| **Title DrawReplayMenu** | 0x004234F6 | 1,280/1,280, 29 bytes (Title `/O1 /Ob1`) | Selected-record EDX/ECX. `--owner replaymenu`. |
| **Replay BeginPlaybackStage** | 0x00420840 | 538/540, 31 masked (`/O2 /Ob1`) | Side-runtime ESI/EDX and RNG seed `xor/mov dx` vs `movzx`. `--owner replay`. |
| **Player ChargeAttack** | 0x0041F810 | 1,210/1,210, 183 masked (`/O2 /Ob1`) | Retail: int side in EDI, input in EBX, `sideMode` spilled to `[ebp-1]`. Current `short`/`volatile` forms are compensation. `--owner charge`. |
| **Player CheckBulletCollision** | 0x0041DFF0 | **849**/862, ~149/269 aligned instructions (`/O2 /Ob1`) | Retail loop is unrotated with a duplicated latch. The former `__asm`/`_emit` shim was removed in 77e6dddd (AGENTS.md still mentions it). `--owner player`. |
| **Supervisor network service** | 0x00430AA0 | 1,629/1,633, ~266/417 aligned instructions (`/O2 /Ob1`) | First divergence: retail keeps the network pointer in EDI; ours CSEs constant `1` into EDI. Compile `src/SupervisorNetwork.cpp` and run `inspect-supervisor-service.py`. |

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

# Choose ONE open boss. Diagnostic-only: it cannot claim exactness.
nice -n 12 python3 -B scripts/check-boss-frontiers.py --owner enemy --skip-exact-siblings

# Preserve all genuinely affected exact siblings after modifying the owner:
python3 -B scripts/replay-scoped-exact.py --changed-file src/EclRunState.inl --dry-run
nice -n 12 python3 -B scripts/replay-scoped-exact.py --changed-file src/EclRunState.inl
# Or use --source src/ReplayInput.cpp for a single source-owned cohort.

# One exact function when needed; builds the selected original-bound carrier:
nice -n 12 python3 -B scripts/build-match-unit.py --unit ecl-pop-context
python3 -B scripts/compare-coff-function.py --unit ecl-pop-context --json

# A separate Title owner (do not substitute exact DrawResult):
nice -n 12 python3 -B scripts/build-match-unit.py --unit title-screen-setup-thread
python3 -B scripts/inspect-title-update-result.py build/matching/TitleScreen.obj

# The standalone, nonexact network service has NO canonical exact sibling:
mkdir -p build/matching
nice -n 12 scripts/compile-probe.sh src/SupervisorNetwork.cpp \
  build/matching/SupervisorNetwork.obj \
  /MT /EHsc /Gs /DNDEBUG /Zi /Gy /GF /Oi /Gr /O2 /Ob1 /Oy- /I src
python3 -B scripts/inspect-supervisor-service.py build/matching/SupervisorNetwork.obj

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

1. **Title OnUpdateResult** — a single-site, 1-byte problem. Find the
   source shape that keeps a real `char` in CL (see the knowledge base
   entry; everything plain folds into `movsx`).
2. **Enemy OnUpdate / SaveReplay** (2 bytes each). Ablation (blank one
   statement or `if`, recompile, read the register) is the best locator;
   the steering sites for Enemy are listed in the knowledge base.
3. **Compiler model.** All residuals are VC7.1 allocation ties. The
   allocator lives in `c2.dll` (`P2\color.c`); entry points and the
   `id & 0x3FF` hash structure are in the knowledge base. Reverse
   engineering its tie-break is the only lead not yet exhausted.

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
- The optional full unit suite (`scripts/ci.py --full`) already had about
  120 failures before this session, mostly source-text and inventory
  snapshots; check whether a failure predates your change before chasing it.

## Storage / protected inputs / commit discipline

Keep `resources/th09.exe`, the `.tools/` Wine/VC7.1 toolchain selectors,
`.analysis/ghidra-direct/`, `_reference/`, and `.analysis/gpt-web/`.
`build/` and Python caches are disposable. Do not commit executables,
analyzer databases, toolchains or decompiler output. Use the commit prefix
the operator requests (`claude-opus-5.5:` in the last session,
`gpt-web:` per AGENTS.md otherwise), commit coherent checkpoints, and never
push silently.
